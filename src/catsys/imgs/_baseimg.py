#!/usr/bin/python
# -*- coding: utf-8 -*-
"""ABC image type

Provides the ImageContainer and ImageFrame abstract base classes, and image type
registration functions.
"""

__version__ = '1.0.0'
__date__    = '2020-10-26'
__author__  = 'Robert Jordan'

__all__ = ['ImageContainer', 'ImageFrame']

#######################################################################################

import abc, io, os, struct
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations


## PREDECLARE TYPES ##

ImageFrame = Type['ImageFrame']
ImageContainer = Type['ImageContainer']


## IMAGE REGISTRATION ##

_registered_imgs:List[type] = []

def registerimg(imgtype) -> NoReturn:
    if imgtype in _registered_imgs:
        raise ValueError('Image type {0.__name__!r} already registered'.format(imgtype))
    _registered_imgs.append(imgtype)

def identifyimg(file:Union[io.BufferedReader,str,bytes], filename:str=None, minscore:int=1) -> Tuple[type, int]:
    if isinstance(file, str):
        filename = file
        with open(file, 'rb') as file:
            return identifyimg(file, filename)
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
    for imgtype in _registered_imgs:
        typescore = 0
        #
        extresult = imgtype._identify_extension(ext) if ext is not None else NotImplemented
        if extresult is not NotImplemented:
            typescore += 1 if extresult else -1
        sigresult = imgtype._identify_signature(sig) if sig is not None else NotImplemented
        if sigresult is not NotImplemented:
            typescore += 3 if sigresult else -3
        #
        if typescore > bestscore:
            besttype, bestscore = imgtype, typescore
    #
    return (besttype, bestscore)

def openimg(file:Union[io.BufferedReader,str,bytes], filename:str=None, **kwargs) -> ImageContainer:
    if isinstance(file, str):
        if filename is None:
            filename = file
        with open(file, 'rb') as file:
            return openimg(file, filename, **kwargs)
    elif isinstance(file, (bytes,bytearray)):
        return openimg(io.BytesIO(file), filename, **kwargs)
    #
    imgtype, typescore = identifyimg(file, filename)
    if imgtype is None:
        raise ValueError('Could not guess Image Container type for file {0!r}'.format(file))
    return imgtype(file, filename, **kwargs)



## IMAGE BASE TYPES ##


class ImageFrame(object):
    """ImageFrame() -> empty image frame
    ImageFrame(name, offset, length, *, [container=container]) -> image frame
    """
    #
    # INSTANCE METHODS:
    #
    def __init__(self, fullname:str=None, width:int=0, height:int=0, offset:int=0, length:int=0, *, container:ImageContainer=None):
        self.fullname = fullname
        self.offset = offset
        self.length = length
        self.container = container
        self.width = width
        self.height = height
        self.originx = 0
        self.originy = 0
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
    @property
    def size(self) -> tuple: return (self.width, self.height)
    @size.setter
    def size(self, size:tuple): self.width, self.height = size
    @property
    def origin(self) -> tuple: return (self.originx, self.originy)
    @origin.setter
    def origin(self, origin:tuple): self.originx, self.originy = origin
    #
    # REPRESENTATION:
    #
    def __repr__(self) -> str:
        return '<{0.__class__.__name__}: {0.name!r} offset={0.offset!r} length={0.length!r}>'.format(self)
    #
    # ARCHIVE ALIAS METHODS:
    #
    def peek(self, length, *, imgfile=None):
        return self.container.peek(self, length, imgfile=imgfile)
    #
    def extract(self, dst:Union[io.BufferedWriter,str], *, imgfile=None):
        return self.container.extract(self, dst, imgfile=imgfile)


# Base ImageContainer class
class ImageContainer(abc.ABC):
    """ImageContainer(filename) -> image container from file
    ImageContainer(stream) -> image container from file-like
    ImageContainer(bytes) -> image container from bytes-like
    ImageContainer() -> empty image container
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
        registerimg(cls)
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
    # def _identify_img(cls, file:io.BufferedReader) -> bool:
    #     return NotImplemented
    #
    # INSTANCE METHODS:
    #
    def __init__(self, file:Union[io.BufferedReader,str,bytes]=None, filename:str=None, **kwargs):
        self.signature = self._signaturedefault
        self.frames = []
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
    def _extract(self, frame:ImageFrame, dstfile:io.BufferedWriter, imgfile:io.BufferedReader, **kwargs):
        """override optional (if no encryption/compression)"""
        imgfile.seek(frame.offset)
        dstdata = imgfile.read(frame.length)
        if len(dstdata) != frame.length:
            raise ValueError('Image Frame data length does not match, expected {0.length!r} bytes, not {1!r} bytes'.format(frame, len(dstdata)))
        if dstfile is None:
            return dstdata
        else:
            dstfile.write(dstdata)
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
    def extract(self, frame:ImageFrame, dst:Union[io.BufferedWriter,str]=None, *, imgfile=None, **kwargs):
        if imgfile is None:
            with self.open() as imgfile:
                return self.extract(frame, dst, imgfile=imgfile, **kwargs)
        #
        if isinstance(dst, str):
            with open(dst, 'wb+') as dstfile:
                return self._extract(frame, dstfile, imgfile, **kwargs)
        elif dst is None:
            dstfile = io.BytesIO()
            self._extract(frame, dstfile, imgfile, **kwargs)
            return dstfile.getvalue()
        else:
            return self._extract(frame, dst, imgfile, **kwargs)
    #
    def iter_extractall(self, outdir:str, **kwargs) -> Iterator[ImageFrame]:
        """yields each Image Frame before extraction"""
        with self.open() as imgfile:
            for i, frame in enumerate(self.frames):
                yield frame
                with open(os.path.join(outdir, frame.fullname or str(i))) as dstfile:
                    self._extract(frame, dstfile, imgfile, **kwargs)
    #
    def extractall(self, outdir:str, **kwargs):
        with self.open() as imgfile:
            for i, frame in enumerate(self.frames):
                with open(os.path.join(outdir, frame.fullname or str(i))) as dstfile:
                    self._extract(frame, dstfile, imgfile, **kwargs)
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
    def _framecount(self) -> int:
        return len(self.frames)
    #
    @property
    def _signaturedefault(self) -> bytes:
        """override optional"""
        return None
    #
    # REPRESENTATION:
    #
    def __repr__(self) -> str:
        return '<{0.__class__.__name__}: {0._signaturestr!r} version={0.version!r} name={0.name!r} frames={0._framecount!r}>'.format(self, self.name)
    #
    # ACCESSORS:
    #
    def __len__(self) -> int:
        return len(self.frames)
    def __iter__(self):
        return iter(self.frames)
    def __contains__(self, item) -> bool:
        if isinstance(item, str):
            for frame in self.frames:
                if frame.name == item:
                    return True
            return False
        else:
            return item in self.frames
    def __getitem__(self, item) -> ImageFrame:
        if isinstance(item, str):
            for frame in self.frames:
                if frame.name == item:
                    return frame
            raise KeyError('Image Frame with name {0!r} not in image container'.format(item))
        else:
            return self.frames[item]
    def index(self, item) -> int:
        if isinstance(item, str):
            for i, frame in enumerate(self.frames):
                if frame.name == item:
                    return i
            raise ValueError('Image Frame with name {0!r} not in image container'.format(item))
        else:
            return self.frames.index(item)
    def find(self, item) -> int:
        if isinstance(item, str):
            for i, frame in enumerate(self.frames):
                if frame.name == item:
                    return i
            return -1
        else:
            try:
                return self.frames.index(item)
            except ValueError:
                return -1


## CLEANUP ##

del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
