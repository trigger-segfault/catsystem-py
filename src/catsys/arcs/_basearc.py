#!/usr/bin/python
# -*- coding: utf-8 -*-
"""ABC archive type

Provides the Archive abstract base class, ArchiveEntry class, and archive type
registration functions.
"""

__version__ = '1.0.0'
__date__    = '2020-09-17'
__author__  = 'Robert Jordan'

__all__ = ['Archive', 'ArchiveEntry', 'registerarc', 'identifyarc', 'openarc']

#######################################################################################

import abc, io, os, struct
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations


## PREDECLARE TYPES ##

ArchiveEntry = Type['ArchiveEntry']
Archive = Type['Archive']


## ARCHIVE REGISTRATION ##

_registered_arcs:List[type] = []

def registerarc(arctype) -> NoReturn:
    if arctype in _registered_arcs:
        raise ValueError('Archive type {0.__name__!r} already registered'.format(arctype))
    _registered_arcs.append(arctype)

def identifyarc(file:Union[io.BufferedReader,str,bytes], filename:str=None, minscore:int=1) -> Tuple[type, int]:
    if isinstance(file, str):
        filename = file
        with open(file, 'rb') as file:
            return identifyarc(file, filename)
    #
    if isinstance(file, (bytes,bytearray)):
        sig = bytes(file)
        file = io.BytesIO(file)
    else:
        sig = file.peek(16)[:16]
        if not filename:
            if hasattr(file, 'name'):
                filename = file.name
            elif hasattr(file, 'raw') and hasattr(file.raw, 'name'):
                filename = file.raw.name
    #
    if filename:
        name, ext = os.path.splitext(os.path.basename(filename))
    else:
        name, ext = None, None
    #
    #
    besttype, bestscore = None, minscore - 1
    for arctype in _registered_arcs:
        arcscore = 0
        #
        extresult = arctype._identify_extension(ext) if ext is not None else NotImplemented
        if extresult is not NotImplemented:
            arcscore += 1 if extresult else -1
        sigresult = arctype._identify_signature(sig) if sig is not None else NotImplemented
        if sigresult is not NotImplemented:
            arcscore += 3 if sigresult else -3
        #
        if arcscore > bestscore:
            besttype, bestscore = arctype, arcscore
    #
    return (besttype, bestscore)

def openarc(file:Union[io.BufferedReader,str,bytes], filename:str=None, **kwargs) -> Archive:
    if isinstance(file, str):
        if filename is None:
            filename = file
        with open(file, 'rb') as file:
            return openarc(file, filename, **kwargs)
    elif isinstance(file, (bytes,bytearray)):
        return openarc(io.BytesIO(file), filename, **kwargs)
    #
    arctype, arcscore = identifyarc(file, filename)
    if arctype is None:
        raise ValueError('Could not guess Archive type for file {0!r}'.format(file))
    return arctype(file, filename, **kwargs)



## ARCHIVE BASE TYPES ##


class ArchiveEntry(object):
    """ArchiveEntry() -> empty archive entry
    ArchiveEntry(name, offset, length, *, [archive=archive]) -> archive entry
    """
    #
    # INSTANCE METHODS:
    #
    def __init__(self, fullname:str=None, offset:int=0, length:int=0, *, archive:Archive=None):
        self.fullname = fullname
        self.offset = offset
        self.length = length
        self.archive = archive
    #
    # PROPERTIES:
    #
    @property
    def name(self) -> str:
        return os.path.basename(self.fullname) if self.fullname is not None else None
    @name.setter
    def name(self, name:str) -> NoReturn:
        self.fullname = name
    @property
    def start(self) -> int:
        return self.offset
    @property
    def end(self) -> int:
        return self.offset + self.length
    @property
    def span(self) -> Tuple[int, int]:
        """Returns (start, end)"""
        return (self.offset, self.offset + self.length)
    #
    # REPRESENTATION:
    #
    def __repr__(self) -> str:
        return '<{0.__class__.__name__}: {0.name!r} offset={0.offset!r} length={0.length!r}>'.format(self)
    #
    # ARCHIVE ALIAS METHODS:
    #
    def peek(self, length, *, arcfile=None):
        return self.archive.peek(self, length, arcfile=arcfile)
    #
    def extract(self, dst:Union[io.BufferedWriter,str], *, arcfile=None):
        return self.archive.extract(self, dst, arcfile=arcfile)


# Base Archive class
class Archive(abc.ABC):
    """Archive(filename) -> archive from file
    Archive(stream) -> archive from file-like
    Archive(bytes) -> archive from bytes-like
    Archive() -> empty archive
    """
    #
    # SUBCLASSING:
    #
    def __init_subclass__(cls, signatures:Union[tuple,bytes]=None, extensions:Union[tuple,str]=None, **kwargs):
        super().__init_subclass__(**kwargs)
        if signatures is not None:
            cls.SIGNATURES = signatures if isinstance(signatures, tuple) else (signatures,)
        if extensions is not None:
            cls.EXTENSIONS = extensions if isinstance(extensions, tuple) else (extensions,)
        registerarc(cls)
    #
    #@abc.abstractclassmethod
    @classmethod
    def _identify_extension(cls, ext:str) -> bool:
        """override optional"""
        EXTENSIONS = getattr(cls, 'EXTENSIONS', None)
        if EXTENSIONS:
            return ext.lower() in cls.EXTENSIONS
        else:
            return NotImplemented
    #@abc.abstractclassmethod
    @classmethod
    def _identify_signature(cls, sig:bytes) -> bool:
        """override optional"""
        SIGNATURES = getattr(cls, 'SIGNATURES', None)
        if SIGNATURES:
            for SIG in cls.SIGNATURES:
                if sig.startswith(SIG):
                    return True
            return False
        else:
            return NotImplemented
    # #@abc.abstractclassmethod
    # @classmethod
    # def _identify_file(cls, file:io.BufferedReader) -> bool:
    #     return NotImplemented
    # #@abc.abstractclassmethod
    # @classmethod
    # def _identify_arc(cls, file:io.BufferedReader) -> bool:
    #     return NotImplemented
    #
    # INSTANCE METHODS:
    #
    def __init__(self, file:Union[io.BufferedReader,str,bytes]=None, filename:str=None, **kwargs):
        self.signature = self._signaturedefault
        self.entries = []
        #
        self.fullname = os.path.abspath(filename) if filename is not None else None
        #
        if file is not None:
            self.read(file, filename, **kwargs)
    #
    # ABSTRACT METHODS:
    #
    @abc.abstractmethod
    def _read(self, file:io.BufferedReader, **kwargs):
        """abstract required"""
        raise NotImplementedError()
    #
    #@abc.abstractmethod
    def _write(self, file:io.BufferedWriter, **kwargs):
        """abstract optional"""
        raise NotImplementedError()
    #
    #@abc.abstractmethod
    def _extract(self, entry:ArchiveEntry, dstfile:io.BufferedWriter, arcfile:io.BufferedReader, **kwargs):
        """override optional (if no encryption/compression)"""
        arcfile.seek(entry.offset)
        dstdata = arcfile.read(entry.length)
        if len(dstdata) != entry.length:
            raise ValueError('Archive Entry data length does not match, expected {0.length!r} bytes, not {1!r} bytes'.format(entry, len(dstdata)))
        if dstfile is None:
            return dstdata
        else:
            dstfile.write(dstdata)
    #@abc.abstractmethod
    def _peek(self, entry:ArchiveEntry, length:int, arcfile:io.BufferedReader, **kwargs):
        """override optional (if no encryption/compression)"""
        arcfile.seek(entry.offset)
        dstdata = arcfile.read(min(length, entry.length))
        if length > entry.length:
            return dstdata + bytes(length - entry.length)
        return dstdata
    #
    # IO METHODS:
    #
    def read(self, file:Union[io.BufferedReader,str,bytes]=None, filename:str=None, **kwargs):
        if filename is None:
            if isinstance(file, str):
                filename = os.path.abspath(file)
            elif isinstance(file, (bytes,bytearray)):
                filename = None
            elif hasattr(file, 'name'):
                filename = os.path.abspath(file.name)
            elif hasattr(file, 'raw') and hasattr(file.raw, 'name'):
                filename = os.path.abspath(file.raw.name)
        self.fullname = filename
        #
        if isinstance(file, str):
            with self.open() as file:
                return self._read(file, **kwargs)
        elif isinstance(file, (bytes,bytearray)):
            return self._read(io.BytesIO(file), **kwargs)
        return self._read(file, **kwargs)
    #
    def open(self, mode='rb') -> io.BufferedReader:
        return open(self.fullname, mode)
    #
    def peek(self, entry:ArchiveEntry, length:int, *, arcfile=None, **kwargs) -> bytes:
        if arcfile is None:
            with self.open() as arcfile:
                return self.peek(entry, length, arcfile=arcfile, **kwargs)
        #
        return self._peek(entry, length, arcfile, **kwargs)
    # 
    def extract(self, entry:ArchiveEntry, dst:Union[io.BufferedWriter,str]=None, *, arcfile=None, **kwargs):
        if arcfile is None:
            with self.open() as arcfile:
                return self.extract(entry, dst, arcfile=arcfile, **kwargs)
        #
        if isinstance(dst, str):
            with open(dst, 'wb+') as dstfile:
                return self._extract(entry, dstfile, arcfile, **kwargs)
        elif dst is None:
            dstfile = io.BytesIO()
            self._extract(entry, dstfile, arcfile, **kwargs)
            return dstfile.getvalue()
        else:
            return self._extract(entry, dst, arcfile, **kwargs)
    #
    def iter_extractall(self, outdir:str, **kwargs) -> Iterator[ArchiveEntry]:
        """yields each Archive Entry before extraction"""
        with self.open() as arcfile:
            for i, entry in enumerate(self.entries):
                yield entry
                with open(os.path.join(outdir, entry.fullname or str(i))) as dstfile:
                    self._extract(entry, dstfile, arcfile, **kwargs)
    #
    def extractall(self, outdir:str, **kwargs):
        with self.open() as arcfile:
            for i, entry in enumerate(self.entries):
                with open(os.path.join(outdir, entry.fullname or str(i))) as dstfile:
                    self._extract(entry, dstfile, arcfile, **kwargs)
    #
    # PROPERTIES:
    #
    @property
    def version(self) -> int:
        return 0 # default version
    @property
    def name(self) -> str:
        return os.path.basename(self.fullname) if self.fullname is not None else None
    @property
    def _signaturestr(self) -> str:
        return self.signature.decode('latin-1')
    @property
    def _entrycount(self) -> int:
        return len(self.entries)
    #
    @property
    def _signaturedefault(self) -> bytes:
        """override optional"""
        return None
    #
    # REPRESENTATION:
    #
    def __repr__(self) -> str:
        return '<{0.__class__.__name__}: {0._signaturestr!r} version={0.version!r} name={0.name!r} entries={0._entrycount!r}>'.format(self, self.name)
    #
    # ACCESSORS:
    #
    def __len__(self) -> int:
        return len(self.entries)
    def __iter__(self):
        return iter(self.entries)
    def __contains__(self, item) -> bool:
        if isinstance(item, str):
            for entry in self.entries:
                if entry.name == item:
                    return True
            return False
        else:
            return item in self.entries
    def __getitem__(self, item) -> ArchiveEntry:
        if isinstance(item, str):
            for entry in self.entries:
                if entry.name == item:
                    return entry
            raise KeyError('Archive Entry with name {0!r} not in archive'.format(item))
        else:
            return self.entries[item]
    def index(self, item) -> int:
        if isinstance(item, str):
            for i, entry in enumerate(self.entries):
                if entry.name == item:
                    return i
            raise ValueError('Archive Entry with name {0!r} not in archive'.format(item))
        else:
            return self.entries.index(item)
    def find(self, item) -> int:
        if isinstance(item, str):
            for i, entry in enumerate(self.entries):
                if entry.name == item:
                    return i
            return -1
        else:
            try:
                return self.entries.index(item)
            except ValueError:
                return -1


## CLEANUP ##

del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
