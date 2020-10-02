#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Windows resource types

Requires packages: pefile

WARNING: This module is deprecated, broken, and will be removed eventually.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

__all__ = ['ResourceName', 'ResourceId']

#######################################################################################

import abc, collections, datetime, pefile
from typing import Any, Dict, Iterable, List, Optional, Set, Text, Tuple, Union


## PREDECLARE TYPES ##

ResName = Union[int,str]
ResNameArg = Union[int,str,pefile.UnicodeStringWrapperPostProcessor,pefile.ResourceDirEntryData]
OptionalResName = Optional[ResName]
OptionalResNameArg = Optional[ResNameArg]


class ResourceName(object):
  """
  A single name or id for a resource. This can be the type, name, or language id.
  """
  __slots__ = ('value',)
  def __new__(cls, resname:Union[int,str,'ResourceName',None]):
    if resname is None:
      return None
    elif cls is ResourceName: # and resname is not None:
      if resname.__class__ is ResourceName:
        return resname # Return existing instance
    return object.__new__(cls)
  def __init__(self, resname:Union[int,str,None]=None):
    """Convert argument to a valid resource name for use with ResourceId"""
    if isinstance(resname, ResourceName):
      resname = resname.value
    elif not isinstance(resname, (int,str)):
      raise TypeError('ResourceName() expected int, str or ResourceName, not {0}'.format(type(resname).__name__))
    self.value:Union[int,str] = resname
  def __setattr__(self, key:str, value):
    if not hasattr(self, 'value'):
      # Very lazy method to check for initialization
      object.__setattr__(self, key, value)
    else:
      # Very lazy method to throw a different error on attribute existance
      val = object.__getattribute__(self, key)
      raise AttributeError('ResourceName object attribute {0!r} is read-only'.format(key))
  @staticmethod
  def cast(other:Union[int,str,'ResourceName',None]) -> Union[int,str,None]:
    if other is None:
      return None
    elif isinstance(other, (int,str)):
      return other
    elif isinstance(other, ResourceName):
      return other.value
    else:
      raise TypeError('ResourceName cast() expected int, str, ResourceName or None, not {0}'.format(type(other).__name__))
  @staticmethod
  def cast_entry(entry:Union[pefile.ResourceDataEntryData,pefile.UnicodeStringWrapperPostProcessor,None]) -> Union[int,str,None]:
    """Convert pefile module types to resource names."""
    if entry is None:
      return None
    elif isinstance(entry, pefile.ResourceDirEntryData):
      return entry.name.decode() if entry.name is not None else entry.id
    elif isinstance(entry, pefile.UnicodeStringWrapperPostProcessor):
      return entry.decode()
    else:
      raise TypeError('ResourceName read_name() expected ResourceDirEntryData, UnicodeStringWrapperPostProcessor or None, not {0}'.format(type(entry).__name__))
  @staticmethod
  def from_entry(entry:Union[pefile.ResourceDataEntryData,pefile.UnicodeStringWrapperPostProcessor,None]) -> 'ResourceName':
    """Convert pefile module types to resource names."""
    if entry is None:
      return ResourceName(None)
    elif isinstance(entry, pefile.ResourceDirEntryData):
      return ResourceName(entry.name.decode() if entry.name is not None else entry.id)
    elif isinstance(entry, pefile.UnicodeStringWrapperPostProcessor):
      return ResourceName(entry.decode())
    else:
      raise TypeError('ResourceName from_entry() expected ResourceDirEntryData, UnicodeStringWrapperPostProcessor or None, not {0}'.format(type(entry).__name__))
  def __bool__(self) -> bool:
    #TODO: How to handle bool?
    #return self.value is not None
    return bool(self.value)
  def __int__(self) -> int:
    if isinstance(self.value, int):
      return self.value
    raise TypeError('ResourceName() cannot cast {0} to int'.format(type(self.value).__name__))
  def __str__(self) -> str:
    return str(self.value)
  def __repr__(self) -> str:
    return 'ResourceName({0!r})'.format(self.value)
    #return repr(self.value)
  def __hash__(self) -> int:
    return hash(self.value)
  # int/str equality
  def __eq__(self, other) -> bool:
    return self.value == ResourceName._cast(other)
  def __ne__(self, other) -> bool:
    return self.value != ResourceName._cast(other)
  def __lt__(self, other) -> bool:
    return self.value < ResourceName._cast(other)
  def __gt__(self, other) -> bool:
    return self.value > ResourceName._cast(other)
  def __le__(self, other) -> bool:
    return self.value <= ResourceName._cast(other)
  def __ge__(self, other) -> bool:
    return self.value >= ResourceName._cast(other)
  @staticmethod
  def from_entry(entry:Union[pefile.ResourceDataEntryData,pefile.UnicodeStringWrapperPostProcessor,None]) -> 'ResourceName':
    """Convert pefile module types to resource names."""
    if entry is None:
      return ResourceName(None)
    elif isinstance(entry, pefile.ResourceDirEntryData):
      return ResourceName(entry.name.decode() if entry.name is not None else entry.id)
    elif isinstance(entry, pefile.UnicodeStringWrapperPostProcessor):
      return ResourceName(entry.decode())
    else:
      raise TypeError('ResourceName from_entry() expected ResourceDirEntryData, UnicodeStringWrapperPostProcessor or None, not {0}'.format(type(entry).__name__))
  @property
  def id(self) -> Union[int,None]:
    """Returns the resource id when an int, otherwise None."""
    return self.value if isinstance(self.value, int) else None
  @property
  def name(self) -> Union[str,None]:
    """Returns the resource name when a str, otherwise None."""
    return self.value if isinstance(self.value, str) else None
  @staticmethod
  def _cast(other:OptionalResNameArg) -> Union[str,int,None]:
    if isinstance(other, ResourceName):
      return other.value
    elif isinstance(other, pefile.ResourceDirEntryData):
      return other.name.decode() if other.name is not None else other.id
    elif isinstance(other, pefile.UnicodeStringWrapperPostProcessor):
      return other.decode()
    return other


ResourceName(None) # Instantiate global



_ResourceIdNamedTuple = collections.namedtuple('_ResourceIdNamedTuple', ('type','name','lang'))

class ResourceId(_ResourceIdNamedTuple):
  """ResourceId(type, name, lang)"""
  type:ResourceName
  name:ResourceName
  lang:ResourceName
  def __new__(cls, type:ResourceName=None, name:ResourceName=None, lang:ResourceName=None):
    if cls is ResourceId and type is not None and name is None and lang is None:
      if type.__class__ is ResourceId:
        return type # Return existing instance
    if isinstance(type, tuple) and name is None and lang is None:
      type, name, lang = type
    if type is not None and 
    return tuple.__new__(cls, (ResourceName(type), ResourceName(name), ResourceName(lang)))
  def __repr__(self) -> str:
    return 'ResourceId(type={0.value!r}, name={1.value!r}, lang={2.value!r})'.format(*self)
  def __str__(self) -> str:
    return 'ResourceId(type={0.value!r}, name={1.value!r}, lang={2.value!r})'.format(*self)

del _ResourceIdNamedTuple

def read_struct(pe:pefile.PE, struct:pefile.Structure) -> bytes:
  return pe.get_string_at_rva(struct.OffsetToData, struct.Size)

class Resource(object):
  def __init__(self, resid:ResourceId, resdata:Union[pefile.ResourceDirEntryData,pefile.ResourceDataEntryData], pe:Optional[pefile.PE]=None):
    if pe is not None and not isinstance(pe, pefile.PE):
      raise TypeError('Resource() argument pe must be pefile.PE or None, not {0}'.format(type(pe).__name__))
    if resid is None or not isinstance(resid, (ResourceId, tuple)):
      raise TypeError('Resource() argument resid must be ResourceId or tuple, not {0}'.format(type(resid).__name__))
    if resdata is None or not isinstance(resdata, (pefile.ResourceDirEntryData,pefile.ResourceDataEntryData)):
      raise TypeError('Resource() argument resdata must be pefile.ResourceDirEntryData or pefile.ResourceDataEntryData, not {0}'.format(type(resdata).__name__))
    if not isinstance(resdata, pefile.ResourceDataEntryData):
      if not hasattr(resdata, 'data'):
        raise ValueError('Resource() argument resdata must have a data attribute when pefile.ResourceDirEntryData')
      resdata = resdata.data
    self._resid:ResourceId = resid if isinstance(resid, ResourceId) else ResourceId(*resid)
    self._resdata:pefile.ResourceDataEntryData = resdata
    self._data:bytes = self.read(pe) if pe is not None else None #(pe, resdata.struct) #self.read(pe)
  def read(self, pe:pefile.PE) -> bytes:
    if self._data is None:
      self._data = read_struct(pe, self._resdata.struct)
    return self._data
  @property
  def data(self) -> bytes:
    return self._data
  @property
  def offset(self) -> int:
    return self._resdata.struct.OffsetToData
  @property
  def size(self) -> int:
    return self._resdata.struct.Size
  @property
  def struct(self):
    return self._resdata.struct
  # def read(pe:pefile.PE=...) -> bytes:
  #   if self._data is not None:
  #     return self._data
  #   #elif pe is None:
  #   #  raise TypeError('Data has')
  #   return pe.get_string_at_rva(self.offset, self.size)
  def __repr__(self) -> str:
    return 'Resource(type={0.value!r}, name={1.value!r}, lang={2.value!r}, size={3!r})'.format(*self._resid, self.size)
  def __str__(self) -> str:
    return 'Resource(type={0.value!r}, name={1.value!r}, lang={2.value!r}, size={3!r})'.format(*self._resid, self.size)

def _match_entry(entry:pefile.ResourceDirEntryData, search_set:Set[ResourceName]) -> Union[ResourceName,None]:
  entry_match = ResourceName.cast_entry(entry)
  if entry_match in search_set:

    return ResourceName(entry)
    search_set.remove(entry_match)
    return 

def find_resource_match(entry:pefile.ResourceDirEntryData, resname:ResourceName) -> Optional[ResourceName]:
  name:Union[int,str] = ResourceName.from_entry(entry)
  if resname is None:
    return ResourceName(name)
  elif name == resname:
    return resname
  return None

def find_resources(pe:pefile.PE, search_id:ResourceId, read_data:bool=False) -> List[Resource]:
  if isinstance(pe, pefile.PE):
    if not hasattr(pe, 'DIRECTORY_ENTRY_RESOURCE'):
      raise TypeError('iter_resource*() argument pe (PE) does not have {0!r}'.format('DIRECTORY_ENTRY_RESOURCE'))
    directory = pe.DIRECTORY_ENTRY_RESOURCE
  search_id = search_id if isinstance(search_id, ResourceId) else ResourceId(*search_id)
  results:List[Resource] = []
  for entry_t in directory.entries:
    t = find_resource_match(entry_t, search_id.type)
    if t is None: continue
    for entry_n in entry_t.directory.entries:
      n = find_resource_match(entry_n, search_id.name)
      if n is None: continue
      for entry_l in entry_n.directory.entries:
        l = find_resource_match(entry_l, search_id.lang)
        if l is not None:
          results.append(Resource(ResourceId(t,n,l), entry_l.data, pe=pe if read_data else None))
  return results

# def iter_resources(pe:pefile.PE)

#   results:List[Resource]=[]
#   if isinstance(types, tuple) and not isinstance(types, ResourceId):
#     search_types = set(ResourceName(t) for t in types)
#     for entry_t in directory.entries:
#       entry_type = ResourceName.cast_entry(entry_t)
#       if search_id.type is not None and entry_type != search_id.type:
#         continue
#       entry_type = search_id.type or ResourceName(entry_type)
#       for entry_n in entry_t.directory.entries:
#         entry_type = ResourceName.cast_entry(entry_n)
#         if search_id.type is not None and entry_type != search_id.type:
#           continue
#         entry_type = search_id.type or ResourceName(entry_type)
#         for entry_l in entry_n.directory.entries:
#           l = ResourceName.cast_entry(entry_l)
#           if search_id.lang is None:
#             l = ResourceName(l)
#           elif l == search_id.lang:
#             l = search_id.lang
#             continue
#           l = search_id.lang or 
#       if entry_type in search_types:
#         search_types.remove(entry_type)


# def _iter_resource_names(entries):
  
#     search_types = set(ResourceName(t) for t in types)
#     for entry in directory.entries:
#       entry_type = ResourceName.cast_entry(entry)
#       if entry_type in search_types:
#         search_types.remove(entry_type)

# def _iter_resource_types(entries)

# def iter_resource_types(pe:pefile.PE) -> Iterable[ResourceName]:
#   if 'DIRECTORY_ENTRY_RESOURCE' not in pe:
#     raise ValueError('iter_resource*() argument pe does not have {0!r}'.format('DIRECTORY_ENTRY_RESOURCE'))
#   entries = pe.DIRECTORY_ENTRY_RESOURCE.entries
#   for entry in entries:
#     yield ResourceName.from_entry(entry)



# def iter_resource_names(pe:Union[pefile.PE,pefile.ResourceDirData,pefile.ResourceDirEntryData], types:Union[ResourceName,Tuple[ResourceName]]=...) -> Iterable[ResourceName]:
#   if isinstance(pe, pefile.PE):
#     if 'DIRECTORY_ENTRY_RESOURCE' not in pe:
#       raise TypeError('iter_resource*() argument pe (PE) does not have {0!r}'.format('DIRECTORY_ENTRY_RESOURCE'))
#     directory = pe.DIRECTORY_ENTRY_RESOURCE
#   # elif isinstance(pe, pefile.ResourceDirData):
#   #   # if not hasattr(pe, 'directory'):
#   #   #   raise TypeError('iter_resource*() argument pe (ResourceDirData) does not have {0!r}'.format('entries'))
#   #   directory = pe
#   # elif isinstance(pe, pefile.ResourceDirEntryData):
#   #   if not hasattr(pe, 'directory'):
#   #     raise TypeError('iter_resource*() argument pe (ResourceDirEntryData) does not have {0!r}'.format('directory'))
#   #   directory = pe.directory
#   #entries = pe.DIRECTORY_ENTRY_RESOURCE.entries
#   entries = directory.entries
#   if isinstance(types, tuple) and not isinstance(types, ResourceId):
#     search_types = set(ResourceName(t) for t in types)
#     for entry in directory.entries:
#       entry_type = ResourceName.cast_entry(entry)
#       if entry_type in search_types:
#         search_types.remove(entry_type)

#   for entry in directory.entries:
#     yield ResourceName.from_entry(entry)

# def 

# pe2.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_IMPORT']])

# def iter_resource_names(pe:Union[pefile.PE,pefile.ResourceDirEntryData], type:Union[ResourceName,Tuple[ResourceName]]) -> Iterable[ResourceName]:
#   if isinstance(pe, pefile.ResourceDirEntryData):
#     if not hasattr(pe, 'directory'):
#       raise TypeError('iter_resource*() argument pe (as directory) does not have {0!r}'.format('directory'))
#     directory = 
#   elif isinstance(pe, pefile.PE):
#     if 'DIRECTORY_ENTRY_RESOURCE' not in pe:
#       raise TypeError('iter_resource*() argument pe does not have {0!r}'.format('DIRECTORY_ENTRY_RESOURCE'))
#     directory = pe.DIRECTORY_ENTRY_RESOURCE
#   if 'DIRECTORY_ENTRY_RESOURCE' not in pe:
#     raise ValueError('iter_resource*() argument pe does not have {0!r}'.format('DIRECTORY_ENTRY_RESOURCE'))
#   if 'DIRECTORY_ENTRY_RESOURCE' not in pe:
#     return
#   entries = pe.DIRECTORY_ENTRY_RESOURCE.entries
#   for entry in entries:
#     yield ResourceName.from_entry(entry)

# def iter_resource_ids(pe:pefile.PE, )

# def iter_resource_entries(directory:pefile.ResourceDirData) -> Iterable[ResourceName]:
#   entries = directory.entries
#   for entry in entries:
#     yield ResourceName.from_entry(entry)
