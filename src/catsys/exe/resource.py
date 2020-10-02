#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Windows resource types

Requires packages: pefile

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

__all__ = ['resourcename', 'resourceid', 'Resource', 'find_resources']

#######################################################################################

import collections, pefile
from typing import Iterable, List, Optional, Tuple, Union


## PREDECLARE TYPES ##

ResName = Union[int,str,None]


def resourcename(resname:ResName) -> ResName:
  """Ensure resname is a valid resource name type and casts pefile module types."""
  if resname is None or isinstance(resname, (int,str)):
    return resname
  elif isinstance(resname, pefile.ResourceDirEntryData):
    return resname.name.decode() if resname.name is not None else resname.id
  elif isinstance(resname, pefile.UnicodeStringWrapperPostProcessor):
    return resname.decode()
  else:
    raise TypeError('resourcename() expected int, str, pefile.ResourceDirEntryData, pefile.UnicodeStringWrapperPostProcessor or None, not {0}'.format(type(resname).__name__))

# _ResourceIdNamedTuple = collections.namedtuple('resourceid', ('type','name','lang'))
# class resourceid(_ResourceIdNamedTuple):
_ResourceIdNamedTuple = collections.namedtuple('resourceid', ('type','name','lang'))
class resourceid(_ResourceIdNamedTuple):
  """resourceid(type, name, lang)"""
  #"""resourceid(type, name, lang)"""
  type:ResName
  name:ResName
  lang:ResName
  def __new__(cls, type:ResName=None, name:ResName=None, lang:ResName=None):
    if type is not None and name is None and lang is None:
      if isinstance(type, tuple) and name is None and lang is None:
        type, name, lang = type
      elif cls is resourceid and type.__class__ is resourceid:
        return type # Return existing instance
    return tuple.__new__(cls, (resourcename(type), resourcename(name), resourcename(lang)))
  # def __repr__(self) -> str:
  #   return 'resourceid(type={0!r}, name={1!r}, lang={2!r})'.format(*self)
  # def __str__(self) -> str:
  #   return 'resourceid(type={0!r}, name={1!r}, lang={2!r})'.format(*self)
  def __repr__(self) -> str:
    return 'resourceid(type={0!r}, name={1!r}, lang={2!r})'.format(*self)
  def __str__(self) -> str:
    return 'resourceid(type={0!r}, name={1!r}, lang={2!r})'.format(*self)

del _ResourceIdNamedTuple


def read_struct(pe:pefile.PE, struct:pefile.Structure) -> bytes:
  return pe.get_data(struct.OffsetToData, struct.Size)

def read_struct_string(pe:pefile.PE, struct:pefile.Structure) -> bytes:
  return pe.get_string_at_rva(struct.OffsetToData, struct.Size)

class Resource(object):
  def __init__(self, resid:resourceid, resdata:Union[pefile.ResourceDirEntryData,pefile.ResourceDataEntryData], pe:Optional[pefile.PE]=None):
    if resid is None or not isinstance(resid, (resourceid, tuple)):
      raise TypeError('Resource() argument resid must be resourceid or tuple, not {0}'.format(type(resid).__name__))
    if resdata is None or not isinstance(resdata, (pefile.ResourceDirEntryData,pefile.ResourceDataEntryData)):
      raise TypeError('Resource() argument resdata must be pefile.ResourceDirEntryData or pefile.ResourceDataEntryData, not {0}'.format(type(resdata).__name__))
    if not isinstance(resdata, pefile.ResourceDataEntryData):
      if not hasattr(resdata, 'data'):
        raise ValueError('Resource() argument resdata must have a data attribute when pefile.ResourceDirEntryData')
      resdata = resdata.data
    if pe is not None and not isinstance(pe, pefile.PE):
      raise TypeError('Resource() argument pe must be pefile.PE or None, not {0}'.format(type(pe).__name__))
    self._resid:resourceid = resid if isinstance(resid, resourceid) else resourceid(*resid)
    self._resdata:pefile.ResourceDataEntryData = resdata
    self._data:bytes = self.read(pe) if pe is not None else None #(pe, resdata.struct) #self.read(pe)
  def read(self, pe:pefile.PE) -> bytes:
    if not hasattr(self, '_data') or self._data is None:
      self._data = read_struct(pe, self._resdata.struct)
    return self._data
  @property
  def resid(self) -> resourceid:
    return self._resid.type
  @property
  def type(self) -> Union[int,str]:
    return self._resid.type
  @property
  def name(self) -> Union[int,str]:
    return self._resid.name
  @property
  def lang(self) -> int:
    return self._resid.lang
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
  def struct(self) -> pefile.Structure:
    return self._resdata.struct
  def hex(*args, **kwargs) -> str:
    return self._data.hex(*args, **kwargs) if self._data is not None else None
  def __len__(self) -> int:
    return self._resdata.struct.Size
  def __repr__(self) -> str:
    return '<Resource: {0!r} {1!r} {2!r}, len={3!r}>'.format(*self._resid, self.size)
  def __str__(self) -> str:
    return '<Resource: {0!r} {1!r} {2!r}, len={3!r}>'.format(*self._resid, self.size)
    #return 'Resource(type={0!r}, name={1!r}, lang={2!r}, size={3!r})'.format(*self._resid, self.size)

def has_resources(pe:pefile.PE) -> bool:
  return hasattr(pe, 'DIRECTORY_ENTRY_RESOURCE') and bool(pe.DIRECTORY_ENTRY_RESOURCE.entries)

def has_fileinfo(pe:pefile.PE) -> bool:
    return hasattr(pe, 'VS_FIXEDFILEINFO') and bool(pe.VS_FIXEDFILEINFO)

def parse_resources(pe:pefile.PE):
  pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_RESOURCE']])
  return has_resources(pe)

def parse_fileinfo(pe:pefile.PE):
  if has_resources(pe):
    filevers = find_resources(pe, resourceid(16, 1, None), read_data=False) # RT_VERSION, always 1?, any lang
    for filever in filevers:
      pe.parse_version_information(filever.struct)
  return has_fileinfo(pe)

def iter_resources(pe:pefile.PE, read_data:bool=False) -> Iterable[Resource]:
  if not has_resources(pe):
    return
  directory = pe.DIRECTORY_ENTRY_RESOURCE
  search_id = search_id if isinstance(search_id, resourceid) else resourceid(*search_id)
  #results:List[Resource] = []
  for entry_t in directory.entries:
    t = resourcename(entry_t)
    for entry_n in entry_t.directory.entries:
      n = resourcename(entry_n)
      for entry_l in entry_n.directory.entries:
        l = resourcename(entry_l)
        resource = Resource(resourceid(t,n,l), entry_l.data, pe=pe if read_data else None)
        yield resource
        #results.append(resource)
        #results.append(Resource(resourceid(t,n,l), entry_l.data, pe=pe if read_data else None))
  #return results

def find_resource_match(entry:pefile.ResourceDirEntryData, resname:ResName) -> Optional[ResName]:
  name:Union[int,str] = resourcename(entry)
  if resname is None or name == resname:
    return name
  return None

def find_resources(pe:pefile.PE, search_id:resourceid, read_data:bool=False) -> List[Resource]:
  if not has_resources(pe):
    return
  directory = pe.DIRECTORY_ENTRY_RESOURCE
  search_id = search_id if isinstance(search_id, resourceid) else resourceid(*search_id)
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
          results.append(Resource(resourceid(t,n,l), entry_l.data, pe=pe if read_data else None))
  return results
