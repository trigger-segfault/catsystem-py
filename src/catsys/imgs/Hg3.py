#!/usr/bin/python
# -*- coding: utf-8 -*-
"""HG-3 (*.hg3) image type

Provides the Hg3Image and Hg3Frame class for CatSystem2 "HG-3" (*.hg3) images.
"""

__version__ = '1.0.0'
__date__    = '2020-09-19'
__author__  = 'Robert Jordan'

__all__ = ['Hg3Image', 'Hg3Frame']

#######################################################################################

import io, os, struct
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations

# local imports
from ._baseimg import ImageContainer, ImageFrame

## PREDECLARE TYPES ##

Hg3Frame = Type['Hg3Frame']
Hg3Image = Type['Hg3Image']

## IMAGE FRAME TYPE: HG-3 (*.hg3) ##

class Hg3Frame(ImageFrame):
    """Hg3Frame() -> empty HG-3 image frame
    """
    #
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

## IMAGE CONTAINER TYPE: HG-3 (*.hg3) ##

class Hg3Image(ImageContainer, signatures=(b'HG-3',),
                               extensions=('.hg3',)):
    """Hg3Image(filename) -> HG-3 image container from file
    Hg3Image(stream) -> HG-3 image container from file-like
    Hg3Image(bytes) -> HG-3 image container from bytes-like
    Hg3Image() -> empty HG-3 image container
    """
    SIGNATURES:List[bytes] = (b'HG-3',)
    EXTENSIONS:List[str] = ('.hg3',)
    KEYDAT_ENTRYNAME:bytes = b'__key__.dat\x00'
    #
    def __init__(self, file:Union[io.BufferedReader,str,bytes]=None, filename:str=None, **kwargs):
        self.password = password
        self._tocseed = None
        self._cipher = None
        self._keyentry = None
        self._version = 2 # 64-byte entries
        super().__init__(file, filename, **kwargs)
    #
    # OVERRIDE METHODS:
    #
    def _read(self, file:io.BufferedReader, *, password:Union[str,bytes,int]=..., **kwargs):
        from .. import crypt
        if password is Ellipsis:
            password = self.password
        # else:
        #     self.password = password
        tocseed = None
        if isinstance(password, str):
            tocseed = crypt.vcode_seed(password.encode('cp932'))
        elif isinstance(password, (bytes,bytearray)):
            tocseed = crypt.vcode_seed(password)
        elif isinstance(password, int):
            tocseed = password
        #
        self.password = password
        self._tocseed = tocseed
        self._version = version = kwargs.get('version', None)
        self._cipher = None
        self._keyentry = None
        #
        self.signature, entry_num = struct.unpack('<4s I', file.read(8))
        self.entries = []
        #
        if entry_num == 0:
            self._version = version = 2
            return
        
        
        ## Special handling to determine file version
        # entry_buf = file.peek(32 + 8)
        # # Check entry filename size
        # self._version = version = 1
        # if entry_buf[31] != 0 or entry_buf[32:36] == b'\x00\x00\x00\x00':
        #     # Either name > 31 bytes (no 32-byte null terminator)
        #     #  or expected data offset is zero (invalid?)
        #     #  assume 64-byte filenames
        #     self._version = version = 2
        
        if version is None:
            self._version = version = self._guess_version(file)
        #
        if version == 1:
            entry_len = 32 + 8
            entry_fmt = '<32s II'
        elif version == 2:
            entry_len = 64 + 8
            entry_fmt = '<64s II'

        # name, offset, length = struct.unpack_from(entry_fmt, file.peek(entry_len))
        # if name.lower().startswith(self.KEYDAT_ENTRYNAME):
        #     self._keyentry = keyentry = self._decode_entry(name, offset, length)
        #     self._keyentry = KifEntry(name, offset, length, archive=self)
        #     self._cipher = crypt.Blowfish(struct.pack('<I', length))
        #     i = 1
        # else:
        #     self._tocseed = tocseed = None
        #     i = 0

        #name, offset, length = struct.unpack_from(entry_fmt, file.peek(entry_len))
        name, offset, length = struct.unpack_from(entry_fmt, file.read(entry_len))
        if name.lower().startswith(self.KEYDAT_ENTRYNAME):
            cipher_key = crypt.mt_genrand(length)
            self._cipher = cipher = crypt.Blowfish(struct.pack('<I', cipher_key))
            #i = 1
            name, offset, length = self._decode_entry(name, offset, length)
            self._keyentry = KifEntry(name, offset, length)
        else:
            self._cipher = cipher = None
            self._tocseed = tocseed = None
            #i = 0
            name, offset, length = self._decode_entry(name, offset, length)
            self.entries.append(KifEntry(name, offset, length, archive=self))

        for i in range(1, entry_num):
            name, offset, length = struct.unpack_from(entry_fmt, file.read(entry_len))
            name, offset, length = self._decode_entry(name, offset, length, cipher, tocseed, i)
            self.entries.append(KifEntry(name, offset, length, archive=self))
    #
    def _extract(self, entry:ArchiveEntry, dstfile:io.BufferedWriter, arcfile:io.BufferedReader, *, buffersize:int=0x4000, **kwargs):
        if self._cipher is None or entry.length == 0:
            return super()._extract(entry, dstfile, arcfile)
        #
        arcfile.seek(entry.offset)
        #
        cipher = self._cipher
        length = entry.length
        #
        tmp_len = (buffersize & ~0x7) if buffersize else 0x4000
        if tmp_len <= length:
            tmp_buf = bytearray(length)
            read_len = arcfile.readinto(tmp_buf)
            if read_len < length:
                raise ValueError('Unexpected end of entry data {0.name!r}'.format(entry))
            self._cipher.decrypt_buffer(tmp_buf, 0, length & ~0x7)
            dstfile.write(tmp_buf)
        else:
            tmp_buf = bytearray(tmp_len)
            read_len = arcfile.readinto(tmp_buf)
            while read_len == tmp_len and length >= tmp_len:
                self._cipher.decrypt_buffer(tmp_buf, 0, min(read_len, length) & ~0x7)
                dstfile.write(tmp_buf)
                length -= read_len
                read_len = arcfile.readinto(tmp_buf)
            if read_len < length:
                raise ValueError('Unexpected end of entry data {0.name!r}'.format(entry))
            if length > 0:
                self._cipher.decrypt_buffer(tmp_buf, 0, length & ~0x7)
                dstfile.write(tmp_buf[:length])
        # if length > 0:
        #     if length >= 8:
        # if length >= 8:
        # while min(read_len, length) >= 8:
        #     if self._cipher is not None:
        #         self._cipher.decrypt_buffer(tmp_buf, 0, min(read_len, length) & ~0x7)
    #
    @property
    def version(self) -> int:
        return self._version
    #
    @classmethod
    def _guess_version(cls, file:io.BufferedReader) -> int:
        ## Special handling to determine file version
        name, offset, length = struct.unpack_from('<32s II', file.peek(32 + 8))
        # Check entry filename size
        if name[-1] != 0 or offset == 0:
            # Either name > 31 bytes (no 32-byte null terminator)
            #  or expected data offset is zero (invalid?)
            #  assume 64-byte filenames
            return 2
        else:
            return 1
    #
    @classmethod
    def _decode_entry(cls, name:bytes, offset:int, length:int, cipher:Blowfish=None, tocseed:int=None, index:int=None) -> bytes:
        if cipher is not None:
            offset, length = cipher.decrypt_lr(offset + index, length)
        if tocseed is not None:
            from .. import crypt
            name = crypt.beaufort_cipher(name, crypt.beaufort_key(tocseed, index))
        #
        name_end = name.find(0)
        if name_end == -1: name_end = len(name)
        return name[:name_end].decode('cp932'), offset, length


## CLEANUP ##

del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
del Blowfish  # only used during declarations
