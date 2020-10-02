
import enum, struct, zlib
from struct import unpack, iter_unpack
from zlib import decompress

from catsys.util import _read_bytes

# from struct import unpack, iter_unpack
# import struct

# from catsys.util import _read_bytes, _read_uint8, _read_uint8_string

class AnimationLineType(enum.IntEnum):
    # [ID] [min] (max)
    ID = 0
    # set [var] [min] (max)
    SET = 1
    # loop [var] [label]
    LOOP = 2,
    # jump [label]
    JUMP = 3
    # if [var] [label]
    IF = 4
    # ife [var] [value] [label]
    IFE = 5
    # ifn [var] [value] [label]
    IFN = 6
    # ifg [var] [value] [label]
    IFG = 7
    # ifs [var] [value] [label]
    IFS = 8
    # ifge [var] [value] [label]
    IFGE = 9
    # ifse [var] [value] [label]
    IFSE = 10
    # max [param]
    MAX = 11
    # blend [param]
    BLEND = 12
    # disp [bool]
    DISP = 13
    # pos [x] [y]
    POS = 14
    # wait [min] [max]
    WAIT = 15
    # add [var] [value]
    ADD = 16
    # sub [var] [value]
    SUB = 17

class AnimationParamType(enum.IntEnum):
    VALUE = 0
    VARIABLE = 1

class AnimationParam(object):
    def __init__(self, kind:AnimationParamType, value:int):
        self.kind:AnimationParamType = kind
        self.value:int = value
    def __repr__(self) -> str:
        return '({0}, {1})'.format(repr(self.kind.name.lower()), repr(self.value))
        # if self.kind == AnimationParamType.VARIABLE:
        #     return '@{0}'.format(self.value)
        # else:
        #     return '{0}'.format(self.value)
    def __str__(self) -> str:
        if self.kind == AnimationParamType.VARIABLE:
            return '@{0}'.format(self.value)
        else:
            return  '{0}'.format(self.value)

class AnimationLine(object):
    def __init__(self, kind:AnimationLineType, params:[AnimationParam]):
        self.kind:AnimationLineType = kind
        self.parameters:[AnimationParam] = list(params)
        while len(self.parameters) < 8:
            self.parameters.append(AnimationParam(AnimationParamType.VALUE, 0))
    def __repr__(self) -> str:
        return '({0}, {1})'.format(repr(self.kind.name.lower()), repr(self.parameters))
    def __str__(self) -> str:
        if self.kind == AnimationLineType.ID:
            #if self.kind.value == 0:
            return ' '.join([str(p) for p in self.parameters])
        else:
            return '{0} {1}'.format(self.kind.name.lower(), ' '.join([str(p) for p in self.parameters]))

class AnimationScript(object):
    SIGNATURE:str = 'ANM'
    EXTENSION:str = '.anm'
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
        self.signature:str = unpack('4s', self.file_data[0:4])[0].decode('ascii')
        self.unknown0x4_4:int = unpack('i', self.file_data[4:8])[0]
        self.line_num:int = unpack('i', self.file_data[8:12])[0]
        self.unknown0xc_20:bytes = self.file_data[12:32]
        self.lines:[AnimationLine] = []
        offset = 0x20
        for i in range(self.line_num):
            #offset = 0x20+i*(0x44)
            line_kind = unpack('i', self.file_data[offset:offset+4])[0]
            params:[AnimationParam] = []
            offset += 4
            for j in range(8):
                param_kind, param_value = unpack('ii', self.file_data[offset:offset+8])
                params.append(AnimationParam(AnimationParamType(param_kind), param_value))
                offset += 8
            self.lines.append(AnimationLine(AnimationLineType(line_kind), params))
    def __init__(self, data:bytes=None, offset:int=...):
        # CatScene Header
        self.file_data:bytes = None
        self.signature:str = ''
        self.unknown0x4_4:int = 0
        self.line_num:int = 0
        self.unknown0xc_20:bytes = bytes(20)
        self.lines:[AnimationLine] = []
        if data is not None:
            self.read(data, offset)
    # @property
    # def entry_num(self) -> int:
    #     return int((self.string_off - self.table_off) / 4)
    def index(self, item) -> int:
        return self.lines.index(item)
    def __len__(self) -> int:
        return len(self.lines)
    def __nonzero__(self) -> bool:
        return bool(self.lines)
    def __contains__(self, item) -> bool:
        return item in self.lines
    def __getitem__(self, item) -> AnimationLine:
        return self.lines[item]
    def __iter__(self):
        return iter(self.lines)
    def __repr__(self) -> str:
        return repr(tuple(iter(self)))
    def __str__(self) -> str:
        return str(tuple(iter(self)))

#print('    '+'\n    '.join([str(l) for l in AnimationScript(r"data\games\cs2_full_v401\system\image\sys_click.anm")]))
