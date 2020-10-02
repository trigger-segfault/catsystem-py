#!/usr/bin/python
# -*- coding: utf-8 -*-
"""CsPack (*.dat) archive type

Provides the CsPackArchive class for CatSystem1 "CsPack1"/"CsPack2" (*.dat) archives.
"""

__version__ = '1.0.0'
__date__    = '2020-09-17'
__author__  = 'Robert Jordan'

__all__ = ['CsPackArchive']

#######################################################################################

import io, os, struct
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations

# local imports
from ._basearc import Archive, ArchiveEntry


## PREDECLARE TYPES ##

# no special entry features
CsPackEntry = ArchiveEntry
CsPackArchive = Type['CsPackArchive']


## ARCHIVE TYPE: CsPack1, CsPack2 (*.dat, *.css, *.csp) ##

class CsPackArchive(Archive, signatures=(b'CsPack1\x00', b'CsPack2\x00'),
                             extensions=('.dat', '.css', '.csp')):
    """CsPackArchive(filename) -> CsPack archive from file
    CsPackArchive(stream) -> CsPack archive from file-like
    CsPackArchive(bytes) -> CsPack archive from bytes-like
    CsPackArchive() -> empty CsPack archive
    """
    SIGNATURES:List[bytes] = (b'CsPack1\x00', b'CsPack2\x00')
    EXTENSIONS:List[str] = ('.dat', '.css', '.csp')
    CHARSET:str = "\x000123456789abcdefghijklmnopqrstuvwxyz___"
    RADIX:int = 40
    #
    # OVERRIDE METHODS:
    #
    def _read(self, file:io.BufferedReader, **kwargs):
        # if isinstance(file, str):
        #     with open(file, 'rb') as f:
        #       return self._read(f)
        # elif isinstance(file, (bytes,bytearray)):
        #     return self._read(io.BytesIO(f))
        #
        self.signature, self.data_offset = struct.unpack('<8s I', file.read(12))
        #
        try:
            version = self.SIGNATURES.index(self.signature) + 1
            if version == 1:
                entry_len = 12
                entry_fmt = '<2I I'
            elif version == 2:
                entry_len = 24
                entry_fmt = '<5I I'
        except:
            raise ValueError('Not a CsPack archive, signature={0!r}'.format(self.signature.decode('latin-1')))
        #
        self.entries = []
        #
        entry_num = (self.data_offset - 12) // entry_len
        offset = self.data_offset
        for i in range(entry_num):
            blocks = struct.unpack(entry_fmt, file.read(entry_len))
            #
            name = self._decode_name(blocks, version)
            offset_next = self._crypt_offset_next(blocks, blocks[-1])
            length = offset_next - offset
            #
            self.entries.append(CsPackEntry(name, offset, length, archive=self))
            #
            offset = offset_next  # not an increment
        #
    #
    # PROPERTIES:
    #
    @property
    def version(self) -> int:
        return self.SIGNATURES.index(self.signature) #1 if self.signature == 'CsPack1\x00' else 2
    #
    @property
    def _signaturedefault(self) -> bytes:
        return self.SIGNATURES[-1] # latest version
    #
    # CRYPT METHODS:
    #
    @classmethod
    def _crypt_offset_next(cls, blocks:List[int], offset_next:int) -> int:
        return blocks[0] ^ blocks[1] ^ offset_next #blocks[-1]
    #
    #
    @classmethod
    def _decode_name(cls, blocks:List[int], version:int) -> str:
        if version == 1:
            str_len, ext_pos, block_num = 12,  8, 2
        elif version == 2:
            str_len, ext_pos, block_num = 30, 16, 5
        #
        str_buf = bytearray(str_len)
        #
        for i in range(block_num):
            num = blocks[i]
            for j in range(5, -1, -1):
                code, num = num % cls.RADIX, num // cls.RADIX
                # code = num % cls.RADIX
                # num //= cls.RADIX
                str_buf[i*6 + j] = ord(cls.CHARSET[code])
        #
        # handle extension position
        #
        name_end = str_buf.find(0, 0, str_len - 4)
        if name_end == -1: name_end = str_len - 4
        #
        ext_end = str_buf.find(0, ext_pos, ext_pos + 3)
        if ext_end == -1: ext_end = ext_pos + 3
        ext_len = ext_end - ext_pos
        if ext_len != 0:
            str_buf[name_end] = ord('.')
            str_buf[name_end + 1:name_end + 1 + ext_len] = str_buf[ext_pos:ext_end]
            name_end += 1 + ext_len
        #
        return str_buf[:name_end].decode('ascii')
    #
    @classmethod
    def _encode_name(cls, name:str, version:int) -> List[int]:
        if version == 1:
            str_len, ext_pos, block_num = 12,  8, 2
        elif version == 2:
            str_len, ext_pos, block_num = 30, 16, 5
        #
        blocks = [0] * (block_num) # + 1) # +1 for offset_next field
        name_buf = name.encode('cp932')
        str_buf = bytearray(str_len)
        #
        ext_start = srt_buf.find(ord('.'))
        if ext_start == -1:
            ext_len = 0
            ext_start = len(name_buf)
            str_buf[:len(name_buf)] = name_buf
        else:
            ext_len = len(name_buf) - ext_start - 1
            str_buf[:ext_start] = name_buf[:ext_start]
            str_buf[ext_pos:ext_pos + ext_len] = name_buf[ext_start:len(name_buf)]
        #
        for i in range(block_num):
            num = 0
            for j in range(6):
                c = str_buf[i*6 + j]
                code = cls.CHARSET.find(c)
                if code == -1:
                    if code <= ord(' '): # Yes, space is changed to null-terminator
                        code = 0
                    else:
                        code = cls.CHARSET.index('_')
                num = num * cls.RADIX + code
            blocks[i] = num
        #
        return blocks


## CLEANUP ##

del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
