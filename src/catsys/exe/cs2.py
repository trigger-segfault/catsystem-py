#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Windows resource types

Requires packages: pefile

WARNING: This module is deprecated, broken, and will be removed eventually.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

#######################################################################################

import pefile

#ERROR: removed parent submodule imports
from catsys.utils.blowfish2 import Cipher
#from catsys.utils.latin40 import *


def readres(pe, type, name):
    for restype in pe.DIRECTORY_ENTRY_RESOURCE.entries:
        if restype.name.decode() == type:
            for resname in restype.directory.entries:
                if resname.name.decode() == name:
                    data = resname.directory.entries[0].data
                    return pe.get_string_at_rva(data.struct.OffsetToData, data.struct.Size)

def readvcodes(pe, should_close:bool=False) -> (bytes,bytes,bytes):
    if isinstance(pe, str):
        should_close = True
        pe=pefile.PE(pe)
    try:
        vcode1_r:bytes  = readres(pe, 'V_CODE',  'DATA')
        vcode2_r:bytes  = readres(pe, 'V_CODE2', 'DATA')
        keycode_r:bytes = readres(pe, 'KEY_CODE', 'KEY')
    finally:
        if should_close:
            pe.close()
    return (vcode1_r, vcode2_r, keycode_r)

def decodekeycode(keycode_r:bytes) -> (str,bytes,'Cipher'):
    keycode:bytes = bytes(b^0xCD for b in keycode_r)
    keycode_s:str = keycode.decode('shiftjis')
    bf = Cipher(keycode, byte_order='little')
    return (keycode_s, keycode, bf)

def decodevcode(bf, vcode_r:bytes) -> (str,bytes):
    #vcode:bytes = bytes(0)
    if (len(vcode_r) & 0x7) != 0:
        print(vcode_r)
        vcode_r += bytes(8 - (len(vcode_r) & 0x7))
        print(vcode_r)
    vcode:bytes = b''.join(bf.decrypt_block(vcode_r[i:i+8]) for i in range(0, len(vcode_r), 8))
    #nullchar:int = len(vcode)
    try:
        nullchar = vcode.index(0)
        vcode_s:str = vcode[0:nullchar].decode('shiftjis')
    except:
        vcode_s:str = vcode.decode('shiftjis')
        #pass
    #vcode_s:str = vcode.decode('shiftjis')
    return (vcode_s, vcode)
    # for i in range(0, len(vcode_r), 8):
    #     if len(vcode_r) - i < 8:
    #         block = vcode_r[i:8] + bytes(len(vcode_r) - 8)
    #         vcode_de.append()
    # bf.decrypt_block(

def decodevcodes(vcode1_r:bytes, vcode2_r:bytes, keycode_r:bytes) -> ((str,str,str), (bytes,bytes,bytes)):
    keycode_s, keycode, bf = decodekeycode(keycode_r)
    vcode1_s, vcode1 = decodevcode(bf, vcode1_r)
    vcode2_s, vcode2 = decodevcode(bf, vcode2_r)
    return ((vcode1_s, vcode2_s, keycode_s), (vcode1, vcode2, keycode))

def getvcodes(pe) -> ((str,str,str), (bytes,bytes,bytes), (bytes,bytes,bytes)):
    vcodes_r = readvcodes(pe)
    vcodes_s, vcodes = decodevcodes(*vcodes_r)
    return (vcodes_s, vcodes, vcodes_r)

SETLOWER:str = "\x000123456789abcdefghijklmnopqrstuvwxyz_?*"
SETUPPER:str = "\x000123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_?*"
ALTSETUPPER:str = "\x000123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-?*"

def encode_chars(text:str, letterset:str=SETLOWER, altletterset:str=ALTSETUPPER, size:int=40, ratio:(int,int)=(4,6), default:int=0):
    codesize = size
    blocksize, charsize = ratio
    buffer:bytearray = bytearray() #len(data) * charsize // blocksize)
    #chars:[str] = []
    #codes:[int] = []
    #blocki:int = 0
    chari:int = 0
    #blockbuf:bytearray = bytearray()
    blockval:int = 0
    # def flush(chari:int):
    #     if chari > 0:
    #         for i in range(chari, charsize):
    #             blockval *= codesize
    #         #buffer.append(blockval.to_bytes(blocksize, byteorder='little'))
    #         buffer += blockval.to_bytes(blocksize, byteorder='little')
    #         chari = 0
    #         blockval = 0
    #     return chari, blockval
    # def append_block(block:bytes):
    #     if len(block) > blocksize:
    #         raise RuntimeError('encode_chars() cannot end block larger than blocksize')
    #     buffer.append(block)
    #     if len(block) < blocksize:
    #         remain:int = blocksize - len(block)
    #         buffer.append(bytes(blocksize - len(block)))
    # def end_block(clip:bool=False):
    #     if len(blockbuf) > blocksize:
    #         if not clip:
    #             raise RuntimeError('encode_chars() cannot end block larger than blocksize')
    #     #blockbuf = blockbuf[0:blocksize]
    #         append_block(blockbuf[0:blocksize])
    #         chari = 0
    #         blocki += 1
    #     else:
    #         for i in range(0, len(blockbuf), blocksize):
    #             append_block(blockbuf[i:i+blocksize])
    #             chari = 0
    #             blocki += 1
    #     blockbuf.clear()
    for i,c in enumerate(text):
        # chari = i % codesize
        # blocki = i // blocksize
        # if chari == 0 and i != 0:
        #     end_block(clip=False)
        codeval:int = default
        try:
            codeval = letterset.index(c)
        except:
            try:
                codeval = altletterset.index(c)
            except:
                pass
          if codeval >= codesize:
                raise RuntimeError('encode_chars() code is larger than code size multiplier')
        blockval *= codesize
        blockval += codeval
        chari += 1
        if chari == charsize or i+1 == len(text):
            # FLUSH BLOCK:
            for i in range(chari, charsize):
                blockval *= codesize
            #buffer.append(blockval.to_bytes(blocksize, byteorder='little'))
            buffer += blockval.to_bytes(blocksize, byteorder='little')
            chari = 0
            blockval = 0
            #chari = flush(chari)
    #chari = flush(chari)
    return bytes(buffer)
    #         end_block(clip=False)
    #         end_block()
    #         chari = 0
    #         blocki += 1
    #     end_block(clip=False)
    # if chari != 0:
    #     for i in range(chari, codesize):
    #         blockval *= codesize
    #     buffer.append(blockval.to_bytes(blocksize, byteorder='little'))
    # while chari < codesize:
    #     chari 

def decode_chars(data:bytes, letterset:str=SETLOWER, size:int=40, ratio:(int,int)=(4,6), default:str='\xff') -> bytes: # -> str:
    #letterset = "\x000123456789abcdefghijklmnopqrstuvwxyz_?*"
    codesize = size
    blocksize, charsize = ratio
    buffer:bytearray = bytearray() #len(data) * charsize // blocksize)
    chars:[str] = []
    codes:[int] = []
    #chars:str = ''
    # m_name = []
    # m_name_length:int = 0x1E
    # m_buffer = bytearray(m_name_length)
    # length = int(m_name_length / 6) << 2
    # dst = 0
    # m_lit:[str] = ['']*5
    #for i, (num,) in enumerate(struct.iter_unpack('<I', data[0:length])):
    #workbuf:bytearray = bytearray(charsize)
    for i in range(0, len(data), blocksize):
        #num:int = struct.unpack('<I', data[i:i+4])[0]
        num:int = int.from_bytes(data[i:i+blocksize], byteorder='little', signed=False)
        workbuf:bytearray = bytearray(charsize)
        charbuf:[str] = ['\x00']*charsize
        codebuf:[int] = [0]*charsize
        for j in range(charsize-1,-1,-1):
            val:int = num % codesize
            num //= codesize
            codeval:int = val
            charval:str = letterset[val] if val < len(letterset) else default
            workbuf[j] = val
            charbuf[j] = charval
            codebuf[j] = codeval
            #buffer.append(val)
            #chars.append(charval)
            #m_buffer[dst+j] = val
        # buffer.append(workbuf)
        # chars.append(charbuf)
        # codes.append(codebuf)
        buffer += workbuf
        chars += charbuf
        codes += codebuf
    return ''.join(chars)

vc=getvcodes(r"path\to\cs2.exe")
vce = tuple(encode_chars(v) for v in vc[0])
vc2e=encode_chars(vcode2s)

pe=pefile.PE(r"path\to\cs2.exe")
keycode:bytes = bytes(b^0xCD for b in keycode_r):
bf = Cipher(keycode, byte_order='little')
for i in range(2):
    vcode_de.append(bf.decrypt_block(vcode[i*8+0:i*8+8]))
    vcode2_de.append(bf.decrypt_block(vcode2[i*8+0:i*8+8]))
vcodes_r = readvcodes(r"path\to\cs2.exe")
vcodes_s = decodevcodes(*vcodes_r)
vcode1_r, vcode2_r, keycode_r = vcodes_r
vcode1_s, vcode2_s, keycode_s = vcodes_s

bf=Cipher(keycode, byte_order='little')
#bf=Cipher(keycode_s, byte_order='little')


keycode_r:bytes = readres(pe, 'KEY_CODE', 'KEY')
vcode1_r:bytes  = readres(pe, 'V_CODE',  'DATA')
vcode2_r:bytes  = readres(pe, 'V_CODE2', 'DATA')
keycode:bytes = bytes(b^0xCD for b in keycode_orig)
keycode_s:bytes = keycode.decode('shiftjis')

print('KEY_CODE={0}'.format(repr(bytes(b^0xCD for b in readres(pe, 'KEY_CODE', 'KEY')).decode('shiftjis'))))
print('V_CODE  ={0}'.format(repr(bytes(readres(pe, 'V_CODE', 'DATA')))))
print('V_CODE2 ={0}'.format(repr(bytes(readres(pe, 'V_CODE2', 'DATA')))))

readres(pe, 'V_CODE', 'DATA')
readres(pe, 'V_CODE2', 'DATA')
