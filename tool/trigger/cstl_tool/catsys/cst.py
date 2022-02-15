#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CatSystem 1+2 CatScene scene type (.cst)

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-09-29'
__author__  = 'Robert Jordan'

#######################################################################################

import enum, io, struct, zlib
from struct import unpack, pack, iter_unpack
from zlib import decompress, compress

#WARNING: deprecated parent imports
from .util import _read_bytes, _write_stream


class SceneLineType(enum.IntEnum):
    NONE    = 0
    INPUT   = 0x0201
    PAGE    = 0x0301
    MESSAGE = 0x2001
    NAME    = 0x2101
    COMMAND = 0x3001

def is_msgline(kind) -> bool:
  return kind == SceneLineType.MESSAGE or kind == SceneLineType.NAME

def is_inputline(kind) -> bool:
  return kind == SceneLineType.INPUT or kind == SceneLineType.PAGE

class SceneLine(object):
    @classmethod
    def read(cls, index:int, data:bytes=None, offset:int=None) -> 'SceneLine':
        #if data is not None:
        #  data = _read_bytes(data)
        if data is None:
            raise ValueError('No line data to read')
        if offset is None:
            raise ValueError('No line data offset to read at')
        nullchar:int = data.index(0, offset+2)
        line_data:bytes = data[offset:nullchar]
        #string_data:bytes = line_data[2:]
        kind:SceneLineType = SceneLineType(unpack('h', line_data[0:2])[0])
        try:
            content:str = line_data[2:].decode('cp932')
        except Exception as ex:
            content:str = None
        return cls(index, kind, content, data=line_data)
    def write(self, stream):
        stream.write(pack('<H', int(self.kind)))
        strdata = self.content.encode('cp932')
        stream.write(strdata)
        stream.write(pack('<B', 0x00))
        return 2 + len(strdata) + 1
    def __init__(self, index:int, kind:SceneLineType, content:str, data:bytes=None):
        self.index:int = index
        self.line_data:bytes = data
        self.string_data:bytes = data[2:] if data else None
        self.kind:SceneLineType = SceneLineType(kind)
        self.content:str = content
    # def __init__(self, index:int, data:bytes=None, offset:int=None):
    #     self.index:int = index
    #     self.line_data:bytes = None
    #     self.string_data:bytes = None
    #     self.kind:SceneLineType = SceneLineType.NONE
    #     self.content:str = None
    #     if data is not None:
    #         self.read(data, offset)
    @property
    def name(self) -> str:
        return self.kind.name.lower()
    def getname(self):
        if self.name:
            return self.name
        return f'0x{self.kind.value:04x}'
    def getnamerepr(self):
        if self.name:
            return repr(self.name)
        return f'0x{self.kind.value:04x}'
    def getstr(self):
        return self.content or self.string_data
    def __len__(self) -> int:
        return len(self.content)
    def __repr__(self) -> str:
        return f'({self.getnamerepr()}, {repr(self.getstr())})'
    def __str__(self) -> str:
        return f'[{self.getname()}]: {self.getstr()}'


class SceneScript(object):
    EXTENSION:str = '.cst'
    SIGNATURE:str = 'CatScene'
    def read(self, data:bytes=None, offset:int=...):
        if data is not None:
            data = _read_bytes(data)
        if offset is Ellipsis:
            offset = 0
        if data is None:
            raise ValueError('No file data to read')
        if offset is None:
            raise ValueError('No file data offset to read at')
        if offset: self.file_data = data[offset:]
        else:      self.file_data = data
        # CatScene Header
        self.signature:str = unpack('8s', self.file_data[0:8])[0].decode('ascii')
        self.comp_len:int = unpack('i', self.file_data[8:12])[0]
        self.orig_len:int = unpack('i', self.file_data[12:16])[0]
        # CatScene Compressed Script
        self.script_data:bytes = zlib.decompress(self.file_data[16:16+self.comp_len])
        self.script_len:int = unpack('i', self.script_data[0:4])[0]
        self.input_num:int = unpack('i', self.script_data[4:8])[0]
        self.table_off:int = unpack('i', self.script_data[8:12])[0]
        self.string_off:int = unpack('i', self.script_data[12:16])[0]
        self.inputs:[(int,int)] = list(iter_unpack('ii', self.script_data[16:16+self.input_num*8]))
        self.offsets:[int] = [o[0] for o in iter_unpack('i', self.script_data[16+self.table_off:16+self.string_off])]
        self.lines:[SceneLine] = []
        for i, off in enumerate(self.offsets):
            line = SceneLine.read(i, self.script_data, 16+self.string_off+off)
            self.lines.append(line)
    def calc_inputs(self):
        self.inputs = []
        start = 0
        for i in range(len(self.lines)):
            line = self.lines[i]
            if line.kind == SceneLineType.INPUT:
                self.inputs.append((i + 1 - start, start))
                start = i + 1
        self.inputs.append((i + 1 - start, start)) # i == len(self.lines) - 1
        start = i + 1
    def write(self, file, autofill=False):
        if file is not None:
            file = _write_stream(file)
        if autofill:
            self.signature = 'CatScene'
            self.script_len = 0
            self.comp_len = 0
            self.orig_len = 0
            self.input_num = len(self.inputs)
            self.table_off = len(self.inputs) * 8
            self.string_off = self.table_off + len(self.lines) * 4
            self.calc_inputs()
        strstream = io.BytesIO()
        for i in range(len(self.lines)):
            if autofill:
                self.offsets[i] = self.script_len # starts at zero from str table (implementation)
            self.script_len += self.lines[i].write(strstream)
        self.script_len += self.string_off
        stream = io.BytesIO()
        stream.write(pack('<IIII', self.script_len, self.input_num, self.table_off, self.string_off))
        for i in range(self.input_num):
            stream.write(pack('<II', *self.inputs[i]))
        for i in range(len(self.offsets)):
            stream.write(pack('<I', self.offsets[i]))
        self.script_data = stream.getvalue() + strstream.getvalue()
        self.orig_len = len(self.script_data)
        comp_script_data = compress(self.script_data)
        self.comp_len = len(comp_script_data)
        file_hdr = pack('<8sII', self.signature.encode('ascii'), self.comp_len, self.orig_len)
        self.file_data = file_hdr + comp_script_data
        # file.write(file_hdr)
        file.write(pack('<8sII', self.signature.encode('ascii'), self.comp_len, self.orig_len))
        file.write(comp_script_data)
        file.close()
        # self.table_off = stream.tell() - 16
    def __init__(self, data:bytes=None, offset:int=...):
        # CatScene Header
        self.file_data:bytes = _read_bytes(data)
        self.signature:str = ''
        self.comp_len:int = 0
        self.orig_len:int = 0
        # CatScene Compressed Script
        self.script_data:bytes = None
        self.script_len:int = 0
        self.input_num:int = 0
        self.table_off:int = 0
        self.string_off:int = 0
        self.inputs:[(int,int)] = []
        self.offsets:[int] = []
        self.lines:[SceneLine] = []
        if data is not None:
            self.read(data, offset)
    @property
    def entry_num(self) -> int:
        return int((self.string_off - self.table_off) / 4)
    def index(self, item) -> int:
        return self.lines.index(item)
    def __len__(self) -> int:
        return len(self.lines)
    def __nonzero__(self) -> bool:
        return bool(self.lines)
    def __contains__(self, item) -> bool:
        return item in self.lines
    def __getitem__(self, item) -> SceneLine:
        return self.lines[item]
    def __iter__(self):
        return iter(self.lines)
    def __repr__(self) -> str:
        return repr(tuple(iter(self)))
    def __str__(self) -> str:
        return str(tuple(iter(self)))

