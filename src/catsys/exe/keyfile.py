#!/usr/bin/python
# -*- coding: utf-8 -*-
"""CatSystem2 key file types (.dat)

WARNING: This module is deprecated, and will be removed soon,
         in favor of catsys.crypt.keyfile_* functions.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

#######################################################################################

import enum, os, struct
from struct import pack
from typing import Optional, Tuple, Union


#ERROR: removed parent submodule imports
from catsys.utils.blowfish2 import Cipher
from catsys.utils.mt import MT19937
from catsys.utils.crc32 import Crc32Normal


class KeyFileType(enum.Enum):
    DIRECT = 'direct.dat'
    CDKEY = 'key.dat'
    DEBUG = 'cs2_debug_key.dat'

CS2_DEBUG_KEY_CONST:str = '@@--cs2-debug-key--@@'
CS2_DEBUG_KEY_CONSTB:bytes = CS2_DEBUG_KEY_CONST.encode('shift_jis')
CS2_OPEN_VCODE:str = 'open_cs2'
CS2_OPEN_VCODEB:bytes = CS2_OPEN_VCODE.encode('shift_jis')


def get_windowsroot() -> str:
    windowsPath = os.path.expandvars('%WINDIR%')
    return os.path.splitdrive(windowsPath)[0] + '\\'

def get_volumeserialnum(volumePath:Union[str,bytes]=...) -> int:
    from ctypes import byref, WinDLL
    from ctypes.wintypes import DWORD
    if volumePath is Ellipsis:
        volumePath = get_windowsroot()
    kernel32 = WinDLL('kernel32', use_last_error=True)
    serialNum = DWORD(0)
    maxCompLen = DWORD(0)
    fileSysFlags = DWORD(0)
    if isinstance(volumePath, bytes):
        GetVolumeInformation = kernel32.GetVolumeInformationA
    elif isinstance(volumePath, str):
        GetVolumeInformation = kernel32.GetVolumeInformationW
    else:
        raise TypeError('get_volumeserialnum() argument \'volumePath\' must be bytes or str, not {0}'.format(type(volumePath).__name__))
    res = GetVolumeInformation(
      volumePath, # <in>
      None, DWORD(0),  # (volumeNameBuf, volumeNameSize)
      byref(serialNum), # <out>
      byref(maxCompLen),   # (unused)
      byref(fileSysFlags), # (unused)
      None, DWORD(0))  # (fileSysNameBuf, fileSysNameSize)
    if not res:
        raise WindowsError('get_volumeserialnum() error argument \'volumePath\' may be invalid, {0!r}'.format(volumePath))
    return serialNum.value

# get_volumeserialnum()

def compute_seed(vcode1:Union[str,bytes]) -> int:
    if hasattr(vcode1, 'encode'): # Always work at the byte-level
        vcode1 = vcode1.encode('shift_jis')
    if isinstance(vcode1, str):
        vcode1 = vcode1.encode('shift_jis')
    seed:int = Crc32Normal.INITIAL
    for c in vcode1:
        if c == 0:
            break
        seed = Crc32Normal.update_octet(seed, c) ^ Crc32Normal.XOROUT
    return seed & 0xffffffff
    return Crc32Normal.compute_seed(vcode1)

def compute_direct_seed(vcode1:Union[str,bytes]) -> int:
    return compute_seed(vcode1)

def compute_debug_seed(vcode1:Union[str,bytes]) -> int:
    if isinstance(vcode1, str):
        vcode1 = vcode1 + CS2_DEBUG_KEY_CONST
    else:
        vcode1 = bytes(vcode1) + CS2_DEBUG_KEY_CONSTB
    return compute_seed(vcode1)

# Requires the "VOLUME" serial number of the Volume containing /Windows. This is not a Drive serial number
def compute_cdkey_seed(vcode1:Union[str,bytes], volumeSerialNum:int=...) -> int:
    if volumeSerialNum is Ellipsis:
        volumeSerialNum = get_volumeserialnum()
    elif isinstance(volumeSerialNum, (str,bytes)):
        volumeSerialNum = get_volumeserialnum(volumeSerialNum) # Is volume path
    return (compute_seed(vcode1) + volumeSerialNum) & 0xffffffff

PACK_FMT:str = '<'+'I'*16

# def compute_debugkey(vcode1=):
#     return keyfile.gen_keydata(keyfile.compute_debug_seed(vcode1))


# class Cs2KeyFile(object):
#     def __init__(self, vcode1:str):
#         if isinstance(vcode1, str):
#             vcode1 = vcode1.encode('shift_jis')
#         self.vcode1:bytes = bytes(vcode1)
#         self.

def gen_keydata(keyseed:int):
    mt:MT19937 = MT19937(keyseed)
    # mt:MT19937 = MT19937()
    # mt.sgenrand(keyseed)
    #words:List[int] = [mt.genrand() for _ in range(32)]
    #bfKey:bytes = pack(PACK_FMT, *words[0:16])
    bfKey:bytes = pack(PACK_FMT, *[mt.genrand() for _ in range(0, 16)])
    bfData:bytes = pack(PACK_FMT, *[mt.genrand() for _ in range(16, 32)])
    bf:Cipher = Cipher(bfKey)
    keyData:bytes = b''.join(bf.encrypt_block(bfData[i:i+8]) for i in range(0, 64, 8))
    return keyData

def gen_direct_keydata(vcode1:Union[str,bytes]) -> bytes:
    return gen_keydata(compute_direct_seed(vcode1))

def gen_debug_keydata(vcode1:Union[str,bytes]) -> bytes:
    return gen_keydata(compute_debug_seed(vcode1))

# Requires the "VOLUME" serial number of the Volume containing /Windows. This is not a Drive serial number
def gen_cdkey_keydata(vcode1:Union[str,bytes], volumeSerialNum:int=...) -> bytes:
    return gen_keydata(compute_cdkey_seed(vcode1, volumeSerialNum))

# keyData = pack(PACK_FMT, *[bf.encrypt_lr(mt.genrand(), mt.genrand()) for i in range(16, 32, 2)])
# bfData = pack(PACK_FMT, *words[16:32])

# keyData = pack(PACK_FMT, *[bf.encrypt_lr(L,R) for mt.genrand()]


# bf:Cipher = Cipher(bfKey)
# b''.join([bf.encrypt_lr()])
# bf.encrypt_block)
# bfkey:bytearray 

def cmp_keyfile(keydata:bytes, keyfilepath:str):
    if len(keydata) != 64:
        raise ValueError('cmp_keyfile() argument \'keydata\' is not a valid key file, must be 64 bytes in length')
    with open(keyfilepath, 'rb') as f:
        keyfiledata:bytes = f.read()
    if len(keyfiledata) != 64:
        raise ValueError('cmp_keyfile() file at argument \'keyfilepath\' is not a valid key file, must be 64 bytes in length')
    return keydata == keyfiledata

