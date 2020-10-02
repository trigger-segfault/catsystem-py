import struct

from catsys.utils.structure import NamedStruct as _NamedStruct

def _encode_tagname(tagname:str) -> bytes:
  if hasattr(tagname, 'encode'):
    tagname = tagname.encode('latin1')
  return tagname.ljust(8, b'\x00')

def _decode_tagname(tagname:bytes) -> str:
  if hasattr(tagname, 'decode'):
    tagname = tagname.decode('latin1')
  return tagname.rstrip('\x00')

# class _NamedStruct(object):
#   _struct_fmt_:str = None
#   _struct_slots_:tuple = None
#   _struct_:struct.Struct = None
#   __slots__ = ()
#   @classmethod
#   def __init_subclass__(cls, **kwargs):
#     if not hasattr(cls, '_struct_') or cls._struct_ is None:
#       cls._struct_ = struct.Struct(cls._struct_fmt_)
#     if not hasattr(cls, '_struct_slots_') or cls._struct_slots_ is None:
#       cls._struct_slots_ = cls.__slots__
#   def _pack_tuple(self) -> tuple:
#     return tuple([getattr(self, k) for k in self._struct_slots_])
#     #raise NotImplementedError('_pack_tuple()')
#   def _unpack_tuple(self, tup:tuple):
#     [setattr(self, k,v) for k,v in zip(self._struct_slots_, tup)]
#     return self
#     #raise NotImplementedError('_unpack_tuple()')
#   def pack(self) -> bytes:
#     return self._struct_.pack(*self._pack_tuple())
#   def pack_into(self, buffer:bytearray, offset:int=0):
#     return self._struct_.pack_into(buffer, offset, *self._pack_tuple())
#   def unpack_self(self, buffer:bytes, offset:int=0):
#     return self._unpack_tuple(self._struct_.unpack_from(buffer, offset))
#   @classmethod
#   def unpack(cls, buffer:bytes):
#     return cls()._unpack_tuple(cls._struct_.unpack(buffer))
#   @classmethod
#   def unpack_from(cls, buffer:bytes, offset:int=0):
#     return cls()._unpack_tuple(cls._struct_.unpack_from(buffer, offset))
#   @classmethod
#   def sizeof(cls) -> int:
#     return cls._struct_.size

class Hg3Tag(_NamedStruct):
  _struct_fmt_:str = '<8sII'
  _struct_slots_:tuple = ('name', 'next_offset', 'size')
  __slots__ = _struct_slots_
  def __init__(self, name:str='', next_offset:int=0, size:int=0):
    self.name:str = _decode_tagname(name) if name != '' else name
    self.next_offset:int = next_offset
    self.size:int = size
  def __repr__(self) -> str:
    return '<Hg3Tag: {0!r} next={1:d} size={2:d}>'.format(self.name, self.next_offset, self.size)
  def __str__(self) -> str:
    return '<Hg3Tag: {0!r} next={1:d} size={2:d}>'.format(self.name, self.next_offset, self.size)
  def _unpack_tuple(self, tup:tuple):
    name, self.next_offset, self.size = tup
    self.name = _decode_tagname(name)
    return self
  def _pack_tuple(self) -> tuple:
    return (_encode_tagname(self.name), self.next_offset, self.size)
  @classmethod
  def parse_tagid(cls, tagname:str) -> int:
    if '%' in cls._tag_name_:
      tagname = _decode_tagname(tagname)
      return int(tagname[tagname.index('%'):])
    return None
    # tagname = _decode_tagname(tagname)
    # return int(tagname[3:])
  @classmethod
  def format_tagid(cls, tagid:int) -> str:
    if '%' in cls._tag_name_:
      return cls._tag_name_ % tagid
    return cls._tag_name_

class Hg3Stdinfo(_NamedStruct):
  _tag_name_:str = 'stdinfo'
  _struct_fmt_:str = '<IIIiiIIIii'
  #_struct_types_:tuple = (int,int,int,int,int,int,int,bool,int,int)
  _struct_slots_:tuple = ('width', 'height', 'bitdepth', 'offset_x', 'offset_y', 'total_width', 'total_height', 'transparent', 'base_x', 'base_y')
  __slots__ = _struct_slots_
  def __init__(self, width:int=0, height:int=0, bitdepth:int=0, offset_x:int=0, offset_y:int=0, total_width:int=0, total_height:int=0, transparent:bool=False, base_x:int=0, base_y:int=0):
    self.width:int = width  # uint32[0:4]
    self.height:int = height  # uint32[4:8]
    self.bitdepth:int = bitdepth  # uint32[8:12]
    self.offset_x:int = offset_x  # int32[12:16]
    self.offset_y:int = offset_y  # int32[16:29]
    self.total_width:int = total_width  # uint32[20:24]
    self.total_height:int = total_height  # uint32[24:28]
    self.transparent:bool = bool(transparent)  # bool32[28:32]
    self.base_x:int = base_x  # int32[32:36]
    self.base_y:int = base_y  # int32[36:40]
  def _unpack_tuple(self, tup:tuple):
    super()._unpack_tuple(tup)
    self.transparent = bool(tup[7])
    return self

class Hg3Img(_NamedStruct):
  _tag_name_:str = 'img%04d'
  _struct_fmt_:str = '<IIIIII'
  _struct_slots_:tuple = ('reserved', 'height', 'comp_data_len', 'orig_data_len', 'comp_cmd_len', 'orig_cmd_len')
  __slots__ = _struct_slots_
  def __init__(self, reserved:int=0, height:int=0, comp_data_len:int=0, orig_data_len:int=0, comp_cmd_len:int=0, orig_cmd_len:int=0):
    self.reserved:int = reserved
    self.height:int = height
    self.comp_data_len:int = comp_data_len
    self.orig_data_len:int = orig_data_len
    self.comp_cmd_len:int = comp_cmd_len
    self.orig_cmd_len:int = orig_cmd_len
  @classmethod
  def parse_tagid(cls, tagname:str) -> int:
    tagname = _decode_tagname(tagname)
    return int(tagname[3:])
  @classmethod
  def format_tagid(cls, tagid:int) -> str:
    return cls._tag_name_ % tagid

class Hg3ImgAl(_NamedStruct):
  _tag_name_:str = 'img_al'
  _struct_fmt_:str = '<II'
  _struct_slots_:tuple = ('comp_len', 'orig_len')
  __slots__ = _struct_slots_
  def __init__(self, comp_len:int=0, orig_len:int=0):
    self.comp_len:int = comp_len
    self.orig_len:int = orig_len

class Hg3ImgJpg(_NamedStruct):
  _tag_name_:str = 'img_jpg'
  _struct_fmt_:str = '<'
  _struct_slots_:tuple = ()
  __slots__ = _struct_slots_
  # Dummy tag, special handling of raw JPEG file data

class Hg3ImgWbp(_NamedStruct):
  _tag_name_:str = 'img_wbp'
  _struct_fmt_:str = '<'
  _struct_slots_:tuple = ()
  __slots__ = _struct_slots_
  # Dummy tag, special handling of raw WebP file data

class Hg3Ats(_NamedStruct):
  _tag_name_:str = 'ats%04d'
  _struct_fmt_:str = '<iiIII'
  _struct_slots_:tuple = ('x', 'y', 'width', 'height', 'color')
  __slots__ = _struct_slots_
  def __init__(self, x:int=0, y:int=0, width:int=0, height:int=0, color:int=0):
    self.x:int = x
    self.y:int = y
    self.width:int = width
    self.height:int = height
    self.color:int = color
  @classmethod
  def parse_tagid(cls, tagname:str) -> int:
    tagname = _decode_tagname(tagname)
    return int(tagname[3:])
  @classmethod
  def format_tagid(cls, tagid:int) -> str:
    return cls._tag_name_ % tagid

class Hg3Cptype(_NamedStruct):
  _tag_name_:str = 'cptype'
  _struct_fmt_:str = '<I'
  _struct_slots_:tuple = ('cptype',)
  __slots__ = _struct_slots_
  def __init__(self, cptype:int=0):
    self.cptype:int = cptype

class Hg3Imgmode(_NamedStruct):
  _tag_name_:str = 'imgmode'
  _struct_fmt_:str = '<I'
  _struct_slots_:tuple = ('imgmode',)
  __slots__ = _struct_slots_
  def __init__(self, imgmode:bool=0):
    self.imgmode:bool = bool(imgmode)
  def _unpack_tuple(self, tup:tuple):
    self.imgmode = bool(tup[0])
    return self
