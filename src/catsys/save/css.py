from struct import unpack, iter_unpack
from zlib import decompress
import struct, zlib, enum, os

##########################
# STRUCTURE
#
## XWBHEADER:
## [0:4] "CSS\x00"
## [4:8] Compressed Length
## [8:12] Decompressed Length
## [12:16] Unknown, always seems to be 0x10000 (as uint) (data type, maybe?)
## <ZLIB COMPRESSED> Data
#
### DATA ENTRY:
### [0:4] Entry type
### [4:8] Offset to next entry (or EOF)
##
## [0:4] Length, or 0xffffffff (repeat)
## [4:8] OffsetNext
## 0x00000000 = No more recursion
## 0x01000000 = More items in list
## 0xffffffff
## TYPES:
## 0x00000000 = BLOB
## 0x01000000 = Next array item
## 0x10000000 = <len(item)=48> String table (type 1, movies)/ SCENE TABLE??
## 0x50000000 = <len(item)=76> Event/CG string table (type 2, CG) (one occurance of stdinfo within)
## 0x00400000 = <len(item)=64> meswnd.xml (item len=64)
## 0x00040000 = Variable table (item len=64)

def read_tables(data:bytes):
  class GameSave(object): # Dummy
    def __init__(self, data):
      self.data:bytes = data
      self.tables:['Table'] = []
      offset:int = 0
      while offset < len(data):
        table:'Table' = Table(self, offset)
        self.tables.append(table)
        offset += table.length + 8
      #return table
  return GameSave(data)

class GameSave(object):
  SIGNATURE:str = 'CSS\x00'
  EXTENSION:str = '.dat'
  def read(self, data:bytes):
    self.file_data:bytes = data
    self.signature:str = self.file_data[0:4].decode('latin1')
    self.comp_len:int = struct.unpack('<I', self.file_data[4:8])[0]
    self.orig_len:int = struct.unpack('<I', self.file_data[8:12])[0]
    self.unk_0xc:int = struct.unpack('<I', self.file_data[12:16])[0]
    self.data:bytes = zlib.decompress(self.file_data[16:16+self.comp_len])
    self.leftover:bytes = self.file_data[16+self.comp_len:]
    self.tables:['Table'] = []
    offset:int = 0
    while offset < len(self.data):
      table:'Table' = Table(self, offset)
      self.tables.append(table)
      offset += table.length + 8
  def __init__(self, filepath:str):
    self.filepath:str = filepath
    self.name:str = os.path.split(filepath)[1]
    self.signature:str = None
    self.comp_len:int = 0
    self.orig_len:int = 0
    self.unk_0xc:int = 0
    self.file_data:bytes = None
    self.data:bytes = None
    self.leftover:bytes = None
    self.tables:['Table'] = []
    if filepath is not None:
      with open(filepath, mode='rb') as f:
        self.read(f.read())
  def print_hdr(self):
    print(f'GameSave({self.name}).hdr')
    print(f'  signature={self.signature!r}')
    print(f'  comp_len ={self.comp_len!r}')
    print(f'  orig_len ={self.orig_len!r}')
    print(f'  unknown12={self.unk_0xc!r}')
  def print_data(self, num:int=4):
    print(f'GameSave({self.name}).data')
    print(f'  counts={struct.unpack("<"+("I"*num),self.data[0:4*num])}')
  @property
  def num_breaks(self) -> int:
    return self.data.count(b'\xff\xff\xff\xff')
  #def num_zeros(self) -> int:

class TableType(enum.IntEnum):
  IMGBANK  = 0x00000001 # Length=8850,item=1180 (\x00\x00\x00\x00\x01\x00\x00\x00 at 0, "filename" at 8 if exists, other plane data)
  SNDBANK  = 0x00000002 # Length=5896,item=268 (\x00\x00\x00\x00 at 0 "bgm##" at 4 (item[0]) bits of sound data)
  UNK_0x4  = 0x00000004 # Length=294 Table of strings, includes scene title (STRUCTURE: "[uint16:identifier][\x00\x00]astring\x00" repeat until [\x00\x00] does not match, starts at offset 4)
  SCENE    = 0x00000005 # Length=124 (\x00\x00\x00\x00gri_ani at 0, \r at 36, \xff\xff\xff\xff at 116)
  RES_0x6  = 0x00000006 # Length=3204
  UNK_0x7  = 0x00000007 # Length=LARGE (internal dictionary of same structure offset=16, backup, maybe?, previous save? auto? quick?) [CONTAINS ALL SAVE#### KEYS EXCEPT 0x7 and 0x100 (this table and thumbnail)]
  UNK_0x8  = 0x00000008 # Length=144 some \x14's one \xff\xff\xff\xff, etc
  UNK_0xa  = 0x0000000a # Length=900,item=100 (\x00\x00\x00\x00\xff\xff\xff\xff, c82f470d (bytes) at 29, item[2], otherwise all zero)
  RES_0xb  = 0x0000000b # Length=148
  RES_0xc  = 0x0000000c # Length=67816 (that's a whole lotta nothing right there I tell you hwat)
  CST      = 0x00000010 # Length=100,item=48,offset=4 (starts with name then scattered data)
  UNK_0x20 = 0x00000020 # Length=100,item=48,offset=4 identical structure (visually and with strings to 0x10 CST)
  CG       = 0x00000050 # Length=4864,item=76 (\x01\x00\x00\x00 at 0, "cgname" at 4, scattered cg data?)
  RES_0x60 = 0x00000060 # Length=560
  THUMB    = 0x00000100 # File=<HG-2>, thumbnail
  UNK_0x200= 0x00000200 # Combination message+Speaker with no spacing, last 4 bytes are \x00\x00\x6c\x20
  UNK_0x201= 0x00000201 # Length=11 (contains b'#str155\x00\x00\x00\x00')
  UNK_0x300= 0x00000300 # Length=4, value=1
  VAR      = 0x00000400 # Length=2048  (identical (visually) to 0x500 below)
  UNK_0x500= 0x00000500 # Length=2048  (contents \x01 at 776, 780, 1200 and \xff\xff\xff\xff at 1440)
  WND      = 0x00004000 # Length=64 ("meswnd.xml" at 0, a lot of other scattered data)
  UNK_0x10000000 = 0x10000000 # Length=156  (contains message+speaker like 0x200, everything else is zeros, starts at 12)
  UNK_0x20000000 = 0x20000000 # Length=576 (contains "meswnd" at 0, and \x01 at 264)

class TableItem(object):
  def __init__(self, table, index:int, data:bytes):
    self.table = table
    self.index:int = index
    self.data:bytes = data
  def __len__(self) -> int:
    return self.table.item_length
  @property
  def offset(self) -> int:
    return self.table.offset + self.table.item_offset + self.table.item_length * self.index
  @property
  def kind(self) -> TableType:
    return self.table.kind
  @property
  def parent(self) -> 'GameSave':
    return self.table.parent
  def __repr__(self) -> str:
    name = self.kind
    if isinstance(self.kind, TableType):
      name = self.kind.name
      #name = '{0!r} {1}'.format(self.kind.name, hex(self.kind.value))
    elif isinstance(self.kind, int):
      name = hex(self.kind)
    return '<Item: {0} data={1!r}>'.format(name, self.data)

class Table(object):
  def __init__(self, parent, offset:int):
    self.parent = parent
    self.offset:int = offset
    self.kind:TableType = struct.unpack('<I', parent.data[offset:offset+4])[0]
    if self.kind in TableType.__members__.values():
      self.kind = TableType(self.kind)
    self.length:int = struct.unpack('<I', parent.data[offset+4:offset+8])[0]
    self.data:bytes = parent.data[offset+8:offset+8+self.length]
    self.item_length:int = None
    self.item_offset:int = 0
    if self.kind == TableType.VAR:
      self.item_length = None
      self.item_offset = 0
    if self.kind == TableType.SCENE:
      self.item_length = None
      self.item_offset = 0
    elif self.kind == TableType.CST:
      self.item_length = 48
      self.item_offset = 4
    elif self.kind == TableType.CG:
      self.item_length = 76
      self.item_offset = 0
    elif self.kind == TableType.WND:
      self.item_length = 64
      self.item_offset = 0
    elif self.kind == TableType.WND:
      self.item_length = 64
      self.item_offset = 0
    elif self.kind == TableType.IMGBANK:
      self.item_length = 1180
      self.item_offset = 0
    elif self.kind == TableType.SNDBANK:
      self.item_length = 268
      self.item_offset = 0
    elif self.kind == TableType.THUMB:
      self.item_length = self.length
      self.item_offset = 0
    #self.item_len:int = self.item_length
    self.items:list = []
    self.item_num:int = 0
    if self.item_length is not None:
      offlength = self.length - self.item_offset
      if ((offlength // self.item_length) * self.item_length) != offlength:
        #self.item_len = offlength / self.item_length
        self.item_num = offlength / self.item_length
      else:
        self.item_num = offlength // self.item_length
        offset = offset
        for i,off in enumerate(range(self.item_offset, self.length, self.item_length)):
          self.items.append(TableItem(self, i, self.data[off:off+self.item_length]))
      #self.item_num:int = 
  def __repr__(self) -> str:
    name = self.kind
    if isinstance(self.kind, TableType):
      name = '{0!r} {1}'.format(self.kind.name, hex(self.kind.value))
    elif isinstance(self.kind, int):
      name = hex(self.kind)
    return '<Table: {0} items={1!r}>'.format(name, self.item_num)


# class PlainBank(object):
#   def __init__(self, index, data, offset):

path1=r"%USERPROFILE%\AppData\Roaming\Frontwing\The Fruit of Grisaia -The Leisure of Grisaia-\savegen.dat"
path2=r"%USERPROFILE%\AppData\Roaming\Frontwing\The Fruit of Grisaia -The Leisure of Grisaia-\save0168.dat"
path3=r"%USERPROFILE%\AppData\Roaming\Frontwing\The Labyrinth of Grisaia -The Melody of Grisaia-\savegen.dat"
path4=r"%USERPROFILE%\AppData\Roaming\Frontwing\The Labyrinth of Grisaia -The Melody of Grisaia-\save0002.dat"

s1 = GameSave(path1)
s2 = GameSave(path2)
s3 = GameSave(path3)
s4 = GameSave(path4)


# def entries(data):
#   if type(data).__name__ == 'GameSave':
#     data = data.data
  

def is_ascii(arg:'chr|byte|int') -> bool:
  if isinstance(arg, (bytes,str)):
    if len(arg) != 1:
      raise ValueError('is_ascii() expected str or bytes len() of 1, not {0!r}'.format(len(arg)))
    arg = ord(arg)
  elif not isinstance(arg, int):
    raise TypeError('is_ascii() expected len() 1 str, bytes or int, not {0}'.format(type(arg).__name__))
  #return (arg >= 0x20 and arg < 0x127) or (arg ==)
  return (arg >= 0x20 and arg < 0x7E) or arg == ord('\t') or arg == ord('\n') or arg == ord('\r')

def read_ascii(arg:'str|bytes', start:int=..., terminate:bool=True) -> int:
  if isinstance(arg, (str,bytes)):
    termchar:'str|int' = 0 if isinstance(arg, bytes) else '\x00'
    if start is not Ellipsis:
      arg = arg[start:]
    for i,c in enumerate(arg):
      if c == termchar:
        return i # Terminated
      if not is_ascii(c):
        if terminate: # Terminate not requested
          return i # Ascii length
        else:
          return 0 # Not terminated, unusable
    return len(arg)
  else:
    raise TypeError('read_ascii() expected str or bytes, not {0}'.format(type(arg).__name__))
    #raise TypeError('read_ascii() expected str, bytes or int, not {0}'.format(type(arg).__name__))

def search_strs(data:bytes, start:int=0, step:int=4, terminate:bool=True, thresh:int=4, pretty:bool=False) -> [(int,str)]:
  if isinstance(data, GameSave):
    data = data.data
  elif isinstance(data, str):
    data = GameSave(data).data
  strs:[(int,str)] = []
  index = start
  length = len(data)
  while index < length:
    ascii_len = read_ascii(data, start=index, terminate=terminate)
    if ascii_len >= thresh:
      strs.append((index, data[index:index+ascii_len]))
      # Not step-1 because the end of an ascii string means the next character is non-ascii
      index += ((ascii_len + (step-0)) // step) * step
      #index += ((ascii_len + (step-1)) // step) * step
    else:
      index += step
  return strs

# def section(data, index:int) -> (int,int,int):
#   offset = data.index(b'\x00\x01\x00\x00', index)
#   length = struct.unpack('<I', data[offset+4:offset+8])
#   except:

SECTION_START=b'\x00\x01\x00\x00'
def sections(data, start:int=0) -> [(int,int)]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  index:int = start
  secs:[(int,int)] = []
  data_len = len(data)
  while index < data_len:
    try:
      offset = data.index(b'\x00\x01\x00\x00', index)  
    except ValueError:
      break
    length = struct.unpack('<I', data[offset+4:offset+8])[0]
    secs.append((offset,length))
    index = offset + 1 # + length
  #   #print(secs)
  #   #raise
  #   pass
  # except:
  #   print(index)
  #   print(data_len)
  #   raise
  return secs

def section(data, index:int):
  if type(data).__name__ == 'GameSave':
    data = data.data
  struct.unpack('<I')

def sections2(data, start:int=0, conv=str, block=b'\xff\xff\xff\xff') -> [(int,int,int)]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  index:int = start
  secs:[(int,int)] = []
  data_len = len(data)
  nextsec=None
  while index < data_len:
    try:
      offset = data.index(block, index)
      if nextsec is not None:
        secs.append((nextsec[0], offset-nextsec[0], nextsec[1], nextsec[2]))
    except ValueError:
      break
    head = 0xffffffff
    kind, length = struct.unpack('<II', data[offset+4:offset+12])
    #secs.append((offset,hex(kind),conv(length)))
    nextsec = (offset,data[offset+4:offset+8].hex(),data[offset+8:offset+12].hex())
    index = offset + 4 # + length
  if nextsec is not None:
    secs.append((nextsec[0], len(data)-nextsec[0], nextsec[1], nextsec[2]))
  return secs

def sections3(data, start:int=0, conv=str) -> [(int,int,int)]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  index:int = start
  secs:[(int,int)] = []
  data_len = len(data)
  nextsec=None
  while index < data_len:
    try:
      offset = data.index(b'\xff\xff\xff\xff\xff\xff\xff\xff', index)
      if nextsec is not None:
        secs.append((nextsec[0], offset-nextsec[0], nextsec[1], nextsec[2]))
    except ValueError:
      break
    head = 0xffffffff
    offset += 4
    kind, length = struct.unpack('<II', data[offset+4:offset+12])
    #secs.append((offset,hex(kind),conv(length)))
    nextsec = (offset-4,data[offset+4:offset+8].hex(),data[offset+8:offset+12].hex())
    index = offset + 4 # + length
  if nextsec is not None:
    secs.append((nextsec[0], len(data)-nextsec[0], nextsec[1], nextsec[2]))
  return secs

def unique(iterable, index) -> dict:
  from collections import OrderedDict
  d=OrderedDict()
  for i,v in enumerate(iterable):
    if index is not None:
      key=v[index]
    else:
      key=v
    if key not in d:
      d[key] = []
    d[key].append(v)
  return d

def val(data, index:int, step:int=4, conv=None) -> int:
  if type(data).__name__ == 'GameSave':
    data = data.data
  v = struct.unpack('<I', data[index:index+4])[0]
  return conv(v) if conv else v

def vals(data, slice=(None,None,4), num=..., conv=None) -> [int]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  if type(data).__name__ == 'GameSave':
    data = data.data
  if slice is None:
    slice=(None,None,4)
  elif isinstance(slice, int):
    if num is Ellipsis:
      return val(data, slice, step=4, conv=conv)
    else:
      slice=(slice,slice+4*num,4)
  elif isinstance(slice, (tuple,list)):
    if   len(slice) == 0: slice=(None,None,4)
    elif len(slice) == 1: slice=(slice[0],None,4)
    elif len(slice) == 2: slice=(slice[0],slice[1],4)
    elif len(slice) == 3: slice=(slice[0],slice[1],slice[2])
    else: raise ValueError('blocks() expected slice tuple len() of 0-3, not {0!r}'.format(len(slice)))
  if not isinstance(slice, __builtins__.slice):
    slice=__builtins__.slice(*slice)
  step = slice.step
  ranges = range(*slice.indices(len(data)))
  if num is Ellipsis: num = len(data)
  return [val(data,i, step=step, conv=conv) for n,i in enumerate(ranges) if n < num]

def section_strs(data):
  if type(data).__name__ == 'GameSave':
    data = data.data
  return [(i,l,data[i+8:i+8+read_ascii(data,i+8)]) for i,l,key,s in sections2(data,0) if key == '01000000']

def str_diffs(data, *args, **kwargs):
  if type(data).__name__ == 'GameSave':
    data = data.data
  sss1 = search_strs(data, *args, **kwargs)
  sss1b = sss1[1:] + [(len(data),b'')]
  return [(a,b-a,sa) for (a,sa),(b,sb) in zip(sss1,sss1b)]

def maskzeros(iterable):
  return [s.replace('00000000','........') for s in iterable]

def block(data, index:int, step:int=4) -> str:
  if type(data).__name__ == 'GameSave':
    data = data.data
  return f'({index:4d}, {data[index:index+step].hex()})'

def blocks(data, slice:tuple=(None,None,4), num:int=...) -> [str]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  if slice is None:
    slice=(None,None,4)
  elif isinstance(slice, int):
    if num is Ellipsis:
      return block(data, slice)
    else:
      slice=(slice,slice+4*num,4)
  elif isinstance(slice, (tuple,list)):
    if   len(slice) == 0: slice=(None,None,4)
    elif len(slice) == 1: slice=(slice[0],None,4)
    elif len(slice) == 2: slice=(slice[0],slice[1],4)
    elif len(slice) == 3: slice=(slice[0],slice[1],slice[2])
    else: raise ValueError('blocks() expected slice tuple len() of 0-3, not {0!r}'.format(len(slice)))
  if not isinstance(slice, __builtins__.slice):
    slice=__builtins__.slice(*slice)
  step = slice.step
  ranges = range(*slice.indices(len(data)))
  if num is Ellipsis: num = len(data)
  return [block(data,i,step) for n,i in enumerate(ranges) if n < num]

def blocks(data, slice:tuple=(None,None), num:int=...) -> [str]:
  if type(data).__name__ == 'GameSave':
    data = data.data
  # if isinstance(slice, int):
  #   slice=(slice,slice+4)
  # elif isinstance(slice, tuple):
  #   if len(slice) == 1: slice=(slice[0],None)
  #   elif len(slice) == 0: slice=(None,None)
  # if slice is None or slice == (None,None) or slice == ():
  #   ranges = range(0, len(data), 4)
  # else:
  if slice is None:
    slice=(None,None,4)
  elif isinstance(slice, int):
    slice=(slice,slice+4,4)
  elif isinstance(slice, (tuple,list)):
    if   len(slice) == 0: slice=(None,None,4)
    elif len(slice) == 1: slice=(slice[0],None,4)
    elif len(slice) == 2: slice=(slice[0],slice[1],4)
    elif len(slice) == 3: slice=(slice[0],slice[1],slice[2])
    else: raise ValueError('blocks() expected slice tuple len() of 0-3, not {0!r}'.format(len(slice)))
  if not isinstance(slice, __builtins__.slice):
    slice=__builtins__.slice(*slice)
  step = slice.step
  ranges = range(*slice.indices(len(data)))
  if num is Ellipsis:
    num = len(data)
  return [block(data,i,step) for n,i in enumerate(ranges) if n < num]
  # if n is not Ellipsis:
  #   iterable = enumerate(ranges)
  #   return [block(data,i,step) for n,i in enumerate(ranges) if n < num]
  # else:
  #   return [block(data,i,step) for i in ranges]
  # #ranges = range(*slice.indices(len(data)))
  # return [block(data,i) for n,i in enumerate(range(*slice.indices(len(data)))) if num is Ellipsis or (n < n um)]
  # return [f'({i:4d}, {data[i:i+4].hex()})' for n,i in enumerate(range(*slice.indices(len(data)))) if num is Ellipsis or (n < n um)]

  for i in range(start, len(data), 4):
    ascii_len = read_ascii(data, start)
def find_strs(data, indexes):
  if isinstance(data, GameSave):
    data = data.data
  elif isinstance(data, str):
    data = GameSave(data).data
  strs:[str] = []
  for i in indexes:



def read_ascii(arg:'str|bytes', start:int=..., terminate:bool=True) -> int:
  #i:int = 0
  # if isinstance(arg, int):
  #   if start is not Ellipsis:
  #     raise TypeError('read_ascii() cannot specify start when arg is type int')
  #   return int(is_ascii(arg))
  #     i += 1
  # elif isinstance(arg, (str,bytes)):