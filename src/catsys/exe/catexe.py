#!/usr/bin/python
# -*- coding: utf-8 -*-
"""CatSystem 2 cs2.exe PE executable information

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

__all__ = ['CatExecutable']

#######################################################################################

#import os, datetime
from typing import Optional, Tuple, Union


# local imports
from .resource import resourceid, Resource
from .baseexe import version, Executable

#ERROR: removed parent submodule imports
from ..utils.blowfish2 import Cipher

#WARNING: deprecated parent imports
from ..util import _read_bytes, _read_uint8, _read_uint8_string, _write_stream, _write_uint8, _write_uint8_string



def decrypt_keycode(keycode:bytes) -> bytes:
    return bytes(b^0xCD for b in keycode)

def create_cipher(keycode_decrypted:bytes) -> Cipher:
    return Cipher(keycode_decrypted, byte_order='little')

def decrypt_vcode(bf:Cipher, vcode:bytes) -> bytes:
    if (len(vcode) & 0x7) != 0:
        # Pad with zeros
        vcode += bytes(8 - (len(vcode) & 0x7))
    vcode_out = bytes()
    for i in range(0, len(vcode), 8):
        vcode_out += bf.decrypt_block(vcode[i:i+8])
    nullchar = vcode_out.find(0)
    if nullchar != -1:
        return vcode_out[0:nullchar]
    return vcode_out


class CatExecutable(Executable):
    def __init__(self, filepath:str, displayname:Optional[str]=None, comment:Optional[str]=None, fast_load:bool=True, keep_open:bool=False):
        super().__init__(filepath, displayname=displayname, comment=comment, fast_load=fast_load)
        self.parse_resources()
        self.parse_fileinfo()
        self._keycoderes:List[Resource] = self.find_resources(resourceid('KEY_CODE', 'KEY', None), read_data=False)
        self._vcode1res:List[Resource]  = self.find_resources(resourceid('V_CODE',  'DATA', None), read_data=False)
        self._vcode2res:List[Resource]  = self.find_resources(resourceid('V_CODE2', 'DATA', None), read_data=False)
        self._keycode:bytes = None
        self._vcode1:bytes = None
        self._vcode2:bytes = None
        self._bf:Cipher = None
        if self._keycoderes:
            self._keycode = decrypt_keycode(self._keycoderes[0].read(self._pe))
            self._bf = create_cipher(self._keycode)
            if self._vcode1res:
                self._vcode1 = decrypt_vcode(self._bf, self._vcode1res[0].read(self._pe))
            if self._vcode2res:
                self._vcode2 = decrypt_vcode(self._bf, self._vcode2res[0].read(self._pe))
        if not keep_open:
            #self._pe.close()
            self.close()
    @property
    def has_vcodes(self) -> int:
        return int(self._vcode1 is not None) + int(self._vcode2 is not None)
    @property
    def keycode(self) -> bytes:
        return self._keycode
    @property
    def vcode1(self) -> bytes:
        return self._vcode1
    @property
    def vcode2(self) -> bytes:
        return self._vcode2
    @property
    def vcodes(self) -> Tuple[bytes, bytes, bytes]:
        """Returns (vcode1, vcode2)"""
        return (self.vcode1, self.vcode2)
    @property
    def codes(self) -> Tuple[bytes, bytes, bytes]:
        """Returns (vcode1, vcode2, keycode)"""
        return (self.vcode1, self.vcode2, self.keycode)
    # @staticmethod
    # def decrypt_keycode(keycode:bytes) -> bytes:
    #     return bytes(b^0xCD for b in keycode)
    # @staticmethod
    # def create_cipher(keycode_decrypted:bytes) -> Cipher:
    #     return Cipher(keycode_decrypted, byte_order='little')
    # @staticmethod
    # def decrypt_vcode(bf:Cipher, vcode:bytes) -> bytes:
    #     if (len(vcode) & 0x7) != 0:
    #         # Pad with zeros
    #         vcode += bytes(8 - (len(vcode) & 0x7))
    #     vcode_out = bytes()
    #     for i in range(0, len(vcode), 8):
    #         vcode_out += bf.decrypt_block(vcode[i:i+8])
    #     nullchar = vcode_out.find(0)
    #     if nullchar != -1:
    #         return vcode_out[0:nullchar]
    #     return vcode_out

#cs2=CatExecutable('cs2.exe',displayname='HAPPINESS2.exe')
