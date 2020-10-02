#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Windows PE executable information

Requires packages: pefile

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

__all__ = ['version', 'Executable']

#######################################################################################

import collections, datetime, os, pefile
from typing import List, Optional, Tuple, Union

# local imports
from .resource import resourcename, resourceid, Resource
from . import resource


_VersionNamedTuple = collections.namedtuple('version', ('major', 'minor', 'revision', 'build'))
class version(_VersionNamedTuple):
    @staticmethod
    def cast_part(part:Union[int,str,None]) -> Union[int,str,None]:
        if part is None or part == '' or part == '*':
            return None
        elif isinstance(part, str):
            if part == '' or part == '*':
                return None
            return part
        elif isinstance(part, int):
            if part < 0:
                raise ValueError('version cast_part() expected non-negative int')
            return part
      # elif isinstance(part, (int,str)):
      #     return part
      # if part is None or isinstance(part, (int,str)):
      #     return part
        raise TypeError('version cast_part() expected int, str or None, not {0}'.format(type(part).__name__))
    def __new__(cls, major:int, minor:int=None, revision:int=None, build:int=None):
        return tuple.__new__(cls, (version.cast_part(major), version.cast_part(minor), version.cast_part(revision), version.cast_part(build)))
    def __str__(self) -> str:
        return '{0!s}.{1!s}.{2!s}.{3!s}'.format(*((n if n is not None else '*') for n in self))
    def __repr__(self) -> str:
        #return '({0!r}, {1!r}, {2!r}, {3!r})'.format(*self)
        return 'version({0!r}, {1!r}, {2!r}, {3!r})'.format(*self)
    @classmethod
    def parse_msls(cls, versionms:int, versionls:int=0) -> 'version':
        """Parses VS_FIXEDFILEINFO File/Product -VersionMS and -VersionLS into one version number."""
        return cls(versionms >> 16, versionms & 0xffff, versionls >> 16, versionls & 0xffff)
    @classmethod
    def parse_fileinfo(cls, vs_fixedfileinfo:'pefile.PE.VS_FIXEDFILEINFO[ ]') -> Tuple['version', 'version']:
        """Parses (FileVersion*, ProductVersion) from pefile.PE.VS_FIXEDFILEINFO[ ]"""
        verinfo = vs_fixedfileinfo
        filever = cls.parse_msls(verinfo.FileVersionMS, verinfo.FileVersionLS)
        prodver = cls.parse_msls(verinfo.ProductVersionMS, verinfo.ProductVersionLS)
        return (filever, prodver)
    @classmethod
    def parse_strict(cls, s:str) -> tuple:
        parts = s.split('.')
        if len(parts) != 4:
            raise ValueError('version parse_strict() expected str with 3 points, not {0}'.format(len(parts)-1))
        for i in range(4):
            try:
                parts[i] = int(parts[i])
            except ValueError:
                if parts[i] == '*':
                    parts[i] = None
                pass
        return cls(*parts)
    @classmethod
    def parse_loose(cls, s:str) -> tuple:
        parts = s.split('.')
        if len(parts) > 4:
            raise ValueError('version parse_loose() expected str with up to 3 points, not {0}'.format(len(parts)-1))
        for i in range(len(parts)):
            try:
                parts[i] = int(parts[i])
            except ValueError:
                if parts[i] == '' or parts[i] == '*':
                    parts[i] = None
                pass
        if len(parts) < 4:
            parts += [None]*(4 - len(parts))
        return cls(*parts)

del _VersionNamedTuple

# def hiloword(dword:int) -> (int, int):
#     return ((dword >> 16) & 0xffff, dword & 0xffff)

# def parse_versions(vs_fixedfileinfo:'pefile.PE.VS_FIXEDFILEINFO[ ]') -> Tuple[version, version]:
#     """Returns (file_version, product_version) from pefile.PE.VS_FIXEDFILEINFO[ ]"""
#     verinfo = vs_fixedfileinfo
#     filever = version(verinfo.FileVersionMS >> 16, verinfo.FileVersionMS & 0xffff, verinfo.FileVersionLS >> 16, verinfo.FileVersionLS & 0xffff)
#     prodver = version(verinfo.ProductVersionMS >> 16, verinfo.ProductVersionMS & 0xffff, verinfo.ProductVersionLS >> 16, verinfo.ProductVersionLS & 0xffff)
#     return (filever, prodver)

class Executable(object):
    def __init__(self, filepath:str, displayname:Optional[str]=None, comment:Optional[str]=None, fast_load:bool=True):
        self._fullname:str = os.path.abspath(filepath)
        self._name:str = os.path.split(self._fullname)[1]
        self._size:int = os.path.getsize(self._fullname)
        self._displayname:str = displayname
        self._comment:str = comment
        self._fast_load:bool = fast_load
        self._pe:pefile.PE = pefile.PE(filepath, fast_load=fast_load)
        self._parsed_directories:Set[str] = set() if fast_load else set(pefile.DIRECTORY_ENTRY.keys())
        self._parsed_resources:bool = not fast_load
        self._parsed_fileinfo:bool = False
        self._timestamp:datetime.datetime = datetime.datetime.fromtimestamp(self._pe.FILE_HEADER.TimeDateStamp)
        self._file_version:version = None
        self._product_version:version = None
    @property
    def comment(self) -> int:
        return self._comment
    @property
    def displayname(self) -> int:
        return self._displayname if self._displayname is not None else self._name
    @property
    def name(self) -> str:
        return self._name
    @property
    def fullname(self) -> str:
        return self._fullname
    @property
    def ext(self) -> str:
        return os.path.splitext(self._name)[1].lower()
    @property
    def size(self) -> int:
        return self._size
    # @property
    # def pe(self) -> pefile.PE:
    #     return self._pe
    @property
    def timestamp(self) -> datetime.datetime:
        return self._timestamp
    @property
    def has_resources(self) -> bool:
        return hasattr(self._pe, 'DIRECTORY_ENTRY_RESOURCE')
    @property
    def has_fileinfo(self) -> bool:
        return hasattr(self._pe, 'VS_FIXEDFILEINFO') and bool(self._pe.VS_FIXEDFILEINFO)
    def close(self):
        return self._pe.close()
    def find_resources(self, search_id:resourceid, read_data:bool=False) -> List[Resource]:
        if self.parse_resources():
            return resource.find_resources(self._pe, search_id, read_data=read_data)
        return []
    def parse_resources(self) -> bool:
        if not self._parsed_resources:
            resource.parse_resources(self._pe)
            #self._pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_RESOURCE']])
            self._parsed_resources = True
        return self.has_resources
    def parse_fileinfo(self) -> bool:
        if self.parse_resources() and not self._parsed_fileinfo:
            if self._fast_load:
                resource.parse_fileinfo(self._pe)
                # filevers = self.find_resources(resourceid(16, 1, None), read_data=False) # RT_VERSION, always 1?, any lang
                # for filever in filevers:
                #     self._pe.parse_version_information(filever.struct)
            if self.has_fileinfo:
                verinfo = self._pe.VS_FIXEDFILEINFO[0]
                self._file_version, self._product_version = version.parse_fileinfo(verinfo)
                # filever = version(verinfo.FileVersionMS >> 16, verinfo.FileVersionMS & 0xFFFF, verinfo.FileVersionLS >> 16, verinfo.FileVersionLS & 0xFFFF)
                # prodver = version(verinfo.ProductVersionMS >> 16, verinfo.ProductVersionMS & 0xFFFF, verinfo.ProductVersionLS >> 16, verinfo.ProductVersionLS & 0xFFFF)
                # self._file_version:version = filever
                # self._product_version:version = prodver
            self._parsed_fileinfo = True
        return self.has_fileinfo
    @property
    def version(self) -> version:
        return self.file_version or self.product_version
    @property
    def file_version(self) -> version:
        return self._file_version
    @property
    def product_version(self) -> version:
        return self._product_version
    def __str__(self) -> str:
        anyver = self._file_version or self._product_version
        if anyver:
            return '<{2}: {0!r}, v{1}>'.format(self.displayname, anyver, self.__class__.__name__)
        return '<{1}: {0!r}>'.format(self.displayname, self.__class__.__name__)
    def __repr__(self) -> str:
        anyver = self._file_version or self._product_version
        if anyver:
            return '<{2}: {0!r}, v{1}>'.format(self.displayname, anyver, self.__class__.__name__)
        return '<{1}: {0!r}>'.format(self.displayname, self.__class__.__name__)
