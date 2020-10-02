#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Windows resource FileInfo structure

Requires packages: pefile

WARNING: This module is deprecated, broken, and will be rewritten eventually.
"""

__version__ = '0.0.1'
__date__    = '2020-01-01'
__author__  = 'Robert Jordan'

#######################################################################################

import collections, pefile
from typing import Iterable, List, Optional, Tuple, Union


# local imports
from .resource import *



## SOURCE:
## https://docs.microsoft.com/en-us/windows/win32/menurc/stringfileinfo-block

StringFileInfoCharsets:Dict[int,str] = {
       0: 'ascii',   # 7-bit ASCII
     932: 'cp932',   # Japan (Shift – JIS X-0208)
     949: 'cp949',   # Korea (Shift – KSC 5601)
     950: 'cp950',   # Taiwan (Big5)
    1200: 'utf_16',  # Unicode
    1250: 'cp1250',  # Latin-2 (Eastern European)
    1251: 'cp1251',  # Cyrillic
    1252: 'cp1252',  # Multilingual
    1253: 'cp1253',  # Greek
    1254: 'cp1254',  # Turkish
    1255: 'cp1255',  # Hebrew
    1256: 'cp1256',  # Arabic
}

StringFileInfoLanguages:Dict[int,str] = {
    0x0401: 'Arabic',
    0x0402: 'Bulgarian',
    0x0403: 'Catalan',
    0x0404: 'Traditional Chinese',
    0x0405: 'Czech',
    0x0406: 'Danish',
    0x0407: 'Gernan',
    0x0408: 'Greek',
    0x0409: 'U.S. English',
    0x040A: 'Castilian Spanish',
    0x040B: 'Finnish',
    0x040C: 'French',
    0x040D: 'Hebrew',
    0x040E: 'Hungarian',
    0x040F: 'Icelandic',
    0x0410: 'Italian',
    0x0411: 'Japanese',
    0x0412: 'Korean',
    0x0413: 'Dutch',
    0x0414: 'Norwegian – Bokmal',
    0x0415: 'Polish',
    0x0416: 'Portuguese (Brazil)',
    0x0417: 'Rhaeto-Romanic',
    0x0418: 'Romanian',
    0x0419: 'Russian',
    0x041A: 'Croato-Serbian (Latin)',
    0x041B: 'Slovak',
    0x041C: 'Albanian',
    0x041D: 'Swedish',
    0x041E: 'Thai',
    0x041F: 'Turkish',
    0x0420: 'Urdu',
    0x0421: 'Bahasa',
    0x0804: 'Simplified Chinese',
    0x0807: 'Swiss German',
    0x0809: 'U.K. English',
    0x080A: 'Spanish (Mexico)',
    0x080C: 'Belgian French',
    0x0810: 'Swiss Italian',
    0x0813: 'Belgian Dutch',
    0x0814: 'Norwegian – Nynorsk',
    0x0816: 'Portuguese (Portugal)',
    0x081A: 'Serbo-Croatian (Cyrillic)',
    0x0C0C: 'Canadian French',
    0x100C: 'Swiss French',
}

StringFileInfoNames:Dict[str,str] = {
    'Comments'        : ('comments', ),
    'CompanyName'     : ('company', 'company_name'),
    'FileDescription' : ('description', 'file_description'),
    'FileVersion'     : ('file_version', ),
    'InternalName'    : ('internalname', 'internal_name'),
    'LegalCopyright'  : ('copyright', 'legal_copyright'),
    'LegalTrademarks' : ('trademark', 'legal_trademarks'),
    'OriginalFilename': ('origname', 'original_filename'),
    'PrivateBuild'    : ('private_build', ),
    'ProductName'     : ('product', 'product_name'),
    'ProductVersion'  : ('product_version', ),
    'SpecialBuild'    : ('special_build', ),
}

def read_struct(pe:pefile.PE, struct:pefile.Structure) -> bytes:
    return pe.get_data(struct.OffsetToData, struct.Size)

class _FileInfoSection(object):
    def __init__(self, off:int, size:int, val_len:int, val_type:int, name:str, level:int=0):
        self.offset:int = off
        self.size:int = size
        self.val_len:int = val_len
        self.val_type:int = val_type
        self.name:str = name
        self.level:int = level
    @classmethod
    def parse(data:bytes, offset:int) -> Tuple[int, 'Section']:
        size, val_len, val_type = struct.unpack_from('<III', data, offset)

class VALUE(object):
    def __init__(self, )

class StringFileInfo(object):
    def __init__(self):
        pass

    def parse(self, data:bytes, offset:int) -> Tuple[int, 'StringFileInfo']:


  
    def _add_value()
class StringFileInfoLangCharset(object):


class StringFileInfoValue(object):


class VarFileInfo(object):
    def __init__(self, )

class VarFileInfoValue

class FileInfo(object):
    def __init__(self, data:bytes):
        self.VarFileInfo = []

def readit(data:bytes):
    @property
    def val_typename(self) -> str:
        if   self.val_type == 0:
            return 'INT'
        elif self.val_type == 1:
            return 'STRING'
        else:
            return hex(self.val_type)
    def print(flat:bool=False):

    
    def __str__(self) -> str:
      
    
    def unpack_string(encoding:str, buf:bytes, off:int) -> (int,str):
        nullchar = buf.find(b'\x00\x00', off)
        if (nullchar & 0x1) - (off & 0x1):
            nullchar += 1 # Handle unicode 0x00 on last character
        newoff = nullchar+2
        return (newoff, buf[off:nullchar].decode(encoding))
    def align4(off:int) -> int:
        return off+4 - ((off & 0x3) or 4)
    def align2(off:int) -> int:
        return off+2 - ((off & 0x1) or 2)
    offset =  0
    reslen, vslen = struct.unpack_from('<HI', data, offset)
    offset += 6
    offset, vsname = unpack_string('utf_16', data, offset)
    print([hex(n) for n in struct.unpack_from('<'+('I'*13), data, offset)])
    print((offset, (vsname, reslen, vslen )))
    vsoffset = offset
    #offset += vslen+2
    offset = align4(offset+vslen) #NOTE: Testing
    #offset = align4(offset) #NOTE: Testing
    def unpack_next(encoding:str, buf:bytes, off:int) -> (int,(int,int,int,str)):
        #_ignore, nextlen, nextvallen = struct.unpack_from('<HIH', data, off)
        nextlen, nextvallen, nexttype = struct.unpack_from('<HHH', data, off)
        off += 6
        off, nextname = unpack_string(encoding, buf, off)
        return (off, (nextlen, nextvallen, nexttype, nextname))
    while offset < reslen:
        blockoffset = offset = align4(offset) #NOTE: Testing
        #blockoffset = offset
        offset, (blocklen, blockvallen, blocktype, blockname) = unpack_next('utf_16', data, offset)
        #print((offset, (blocklen, blockvallen, blocktype, blockname)))
        #print('  ',end='')
        print(((blockoffset, blockoffset+blocklen), (blocklen, blockvallen, blocktype)))
        print('BLOCK "{0}"\nBEGIN'.format(blockname))
        while offset < blockoffset+blocklen:
            if blockname == 'StringFileInfo':
                langchroffset = offset = align4(offset) #NOTE: Testing
                offset, (langchrlen, langchrvallen, langchrtype, langchrname) = unpack_next('utf_16', data, offset)
                #print((offset, (langchrlen, langchrvallen, langchrtype, langchrname)))
                langid,chrid, = int(langchrname[0:4],16),int(langchrname[4:8],16)
                langname = StringFileInfoLanguages[langid]
                chrname = StringFileInfoCharsets[chrid]
                #print(((hex(langid), chrid),(langname, chrname)))
                print('  ',end='')
                print(((langchroffset, langchroffset+langchrlen), (langchrlen, langchrvallen, langchrtype)))
                print('  BLOCK "{0}"\nBEGIN'.format(langchrname))
                strlen = -1
                while offset < langchroffset + langchrlen:
                    stroffset = offset = align4(offset)
                    offset, (strlen, strvallen, strtype, strname) = unpack_next('utf_16', data, offset)
                    #print((offset, (strlen, strvallen, strtype, strname)))
                    offset = align4(offset)
                    #if (offset & 0x3) != 0:
                    #    offset += (4 - (offset & 0x3))
                    offset, strval = unpack_string('utf_16', data, offset)
                    offset = align4(offset)
                    #print((offset, strval))
                    print('    ',end='')
                    print(((stroffset, stroffset+strlen), (strlen, strvallen, strtype)))
                    print('    VALUE "{0}",   "{1}"'.format(strname, strval))
                    offset = stroffset+strlen
                print('  END')
                #print(unpack_next('utf_16', data, offset))
                #break
                offset = langchroffset + langchrlen
            elif blockname == "VarFileInfo":
                troffset = offset = align4(offset)
                offset = align4(offset) #NOTE: Testing
                offset, (trlen, trvallen, trtype2, trname) = unpack_next('utf_16', data, offset)
                #print((offset, (trlen, trvallen, trtype2, trname)))
                offset = align4(offset)
                trsets = []
                trsetnames = []
                trvaloffset = offset
                while offset < troffset+trlen:
                    #while offset < trvaloffset+trvallen:
                    #for _ in range(offset, offset+trvallen, 4):
                    trlang, trchr = struct.unpack_from('<HH', data, offset)
                    offset += 4
                    trsets.append((trlang, trchr))
                    trsetnames.append('0x{0:X}, {1:d}'.format(trlang, trchr))
                #offset, strval = unpack_string('utf_16', data, offset)
                #offset = align4(offset)
                #print((offset, trvval))
                print('  ',end='')
                print(((troffset, troffset+trlen), (trlen, trvallen, trtype2)))
                print('  VALUE "{0}",   {1}'.format(trname, ', '.join(trsetnames)))
                #print('  VALUE "{0}",   0x{1:X}, {2:d}'.format(trname, trlang, trchr))
                offset = troffset+trlen
        offset = blockoffset + blocklen
        print('END\n')
    #reslen, vslen = struct.unpack_from('<HIH', data, offset)
    #return blockstart

readit(vd)

def readit(pe:pefile.PE, vsversioninfo_res:pefile.Structure):
    data:bytes = read_struct(pe, vsversioninfo_res)
    reslen, vslen = struct.unpack('<HI')
    def unpack_string(buf, offset) -> (int,str):
        nullchar = buf.find(b'\x00\x00', offset)
        if (nullchar & 0x1) - (offset & 0x1):
            nullchar += 1 # Handle unicode 0x00 on last character
        newoff = nullchar+2
        return (newoff, buf[offset:nullchar])
    reslen, vslen = struct.unpack_from('<HI', data, 0)
    offset += 6
    offset, vsname = offsetunpack_string(data, offset)
    
    blockstart = offset+vslen
    return blockstart

def readit(pe:pefile.PE, vsversioninfo_res:pefile.Structure):
    data:bytes = read_struct(pe, vsversioninfo_res)
    reslen, vslen = struct.unpack('<HI')
    def unpack_string(data, offset) -> (int,str):
        nullchar = data.find(b'\x00\x00', offset)
        if (nullchar & 0x1) - (offset & 0x1):
            nullchar += 1 # Handle unicode 0x00 on last character
        newoff = nullchar+2
        blockoff = nullchar+2 + vslen
    reslen, vslen = struct.unpack_from('<HI', data, 0)
    offset += 6
    offset, vsname = offsetunpack_string(data, offset)
    
    blockstart = offset+vslen
    return blockstart
    def read_block_head()
    nullchar = data.find(b'\x00\x00', 6)
    if nullchar & 0x1:
        nullchar += 1 # Handle unicode 0x00 on last character
    blockoff = nullchar+2 + vslen
    if nullchar == -1:
        return {}
  
def read_StringFileInfo(pe:pefile.PE, versioninfores:pefile.Structure) -> Dict[Tuple[str,int],Dict[str,str]]:
    blocks = {}
    data:bytes = read_struct(pe, versioninfores)
def read_StringFileInfo2(pe:pefile.PE, versioninfores:pefile.Structure) -> Dict[Tuple[str,int],Dict[str,str]]:
    blocks = {}
    data:bytes = read_struct(pe, versioninfores)

# StringFileInfoCharsets:Dict[int,Tuple[str,str]] = {
#      0: ( 'ascii', '7-bit ASCII'),
#    932: ( 'cp932', 'Japan (Shift – JIS X-0208)'),
#    949: ( 'cp949', 'Korea (Shift – KSC 5601)'),
#    950: ( 'cp950', 'Taiwan (Big5)'),
#   1200: ('utf_16', 'Unicode'),
#   1250: ('cp1250', 'Latin-2 (Eastern European)'),
#   1251: ('cp1251', 'Cyrillic'),
#   1252: ('cp1252', 'Multilingual'),
#   1253: ('cp1253', 'Greek'),
#   1254: ('cp1254', 'Turkish'),
#   1255: ('cp1255', 'Hebrew'),
#   1256: ('cp1256', 'Arabic'),
#   #    0: 'ascii',     # 7-bit ASCII
#   #  932: 'shift_jis', # Japan (Shift – JIS X-0208)
#   #  949: 'euc_kr',    # Korea (Shift – KSC 5601)
#   #  950: 'big5',      # Taiwan (Big5)
#   # 1200: 'utf_16',    # Unicode
#   # 1250: 'latin2',    # Latin-2 (Eastern European)
#   # 1251: 'cyrillic',  # Cyrillic
#   # 1252: 'multilingual', # Multilingual
#   # 1253: 'greek',     # Greek
#   # 1254: 'turkish',   # Turkish
#   # 1255: 'hebrew',    #  	Hebrew
#   # 1256: 'arabic',    # Arabic
# }

StringFileInfo

# def parse_fileinfo(pe:pefile.PE):
#     if has_resources(pe):
#         filevers = find_resources(pe, resourceid(16, 1, None), read_data=False) # RT_VERSION, always 1?, any lang
#         for filever in filevers:
#             pe.parse_version_information(filever.struct)
#     return has_fileinfo(pe)
