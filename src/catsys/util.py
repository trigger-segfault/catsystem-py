#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Temporary utility functions file

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-09-28'
__author__  = 'Robert Jordan'

#######################################################################################

import io, struct


def _read_bytes(file) -> bytes:
    """input: file|bytes|str, output: bytes"""
    if file is None:
        return None
    elif isinstance(file, bytes):
        return file
    elif isinstance(file, str):
        with open(file, mode='rb') as f:
            return f.read()
    elif hasattr(file, 'read'):
        file_data = file.read()
        if not isinstance(file_data, bytes):
            raise ValueError('File-like object must return binary data')
        return file_data
    else:
        raise ValueError('Expected file-like object or bytes')

def _write_stream(file):
    """input: file|byets|str, output: file"""
    if file is None:
        return None
    elif isinstance(file, bytes):
        return io.BytesIO(file)
    elif isinstance(file, str):
        return open(file, mode='wb+')
        # with open(file, mode='wb+') as f:
        #     return f
    elif hasattr(file, 'write'):
        return file
    else:
        raise ValueError('Expected file-like object or bytes')

def _read_uint8(data:bytes, offset:int) -> (int, int):
    """returns: (value:int, new_offset:int)"""
    #if isinstance(data, bytes):
    lastbyte:int = data[offset]
    value:int = lastbyte
    offset += 1
    while lastbyte == 0xFF:
        lastbyte = data[offset]
        value += lastbyte
        offset += 1
    return (value, offset)

def _read_uint8_string(data:bytes, offset:int) -> (str, int):
    """returns: (value:str, new_offset:int)"""
    strlen, offset = _read_uint8(data, offset)
    strdata:bytes = data[offset:offset+strlen]
    offset += strlen
    try:
        strvalue:str = strdata.decode('utf-8')
        return (strvalue, offset)
    except Exception as ex:
        return (strdata, offset)

def _write_uint8(stream, value:int):
    if value < 0:
        raise ValueError('Cannot write FF-encoded int with negative value')
    count = 1
    while value >= 0xFF:
        value -= 0xFF
        stream.write(struct.pack('<B', 0xFF))
        count += 1
    stream.write(struct.pack('<B', value))
    return count

def _write_uint8_string(stream, text:str):
    """Also supports bytes (already encoded as utf-8)"""
    if hasattr(text, 'encode'):
        text = text.encode('utf-8')
    count = _write_uint8(stream, len(text))
    stream.write(text)
    return count + len(text)

# def is_msgline(kind) -> bool:
#     return kind == SceneLineType.MESSAGE or kind == SceneLineType.NAME

# def is_inputline(kind) -> bool:
#     return kind == SceneLineType.INPUT or kind == SceneLineType.PAGE

# class SceneMessageTranslation(object):
#     pass

# class SceneTranslation(object):
#     def __init__(self, cst, cstl):
#         self.cst = cst
#         self.cstl = cstl
#         self.cst_msglines
#         self.translations:list = []
#         self.cst_lines:list = []
#         for i, (input, message) in enumerate(zip(cst.inputs,cstl.messages)):
#             index = input[0]
#             start = index-1
#             end = index-1
#             name:str = ''
#             content:str = ''
#             while is_msgline(cst.lines[start-1].kind):
#                 start -= 1
#                 line = cst.lines[start]
#                 if line.kind == SceneLineType.NAME:
#                     name += line.content + name
#                 elif line.kind == SceneLineType.MESSAGE:
#                     content = line.content + content
#             lines = lines[start:end]
#             SceneMessage

#             # for line in lines:
#             #     if line.kind == SceneLineType.NAME:
#             #         name += line.content
#             #     elif line.kind == SceneLineType.MESSAGE:


#         self.cst_messages:list = []
#         self.cstl_messages:list = []

#     def __len__(self) -> int:
#         return len(self.translations)
#     def __nonzero__(self) -> bool:
#         return bool(self.translations)
#     def __contains__(self, item) -> bool:
#         return item in self.translations
#     def __getitem__(self, item):
#         return self.translations[item]
#     def __iter__(self):
#         return iter(self.translations)
#     def __repr__(self):
#         return repr(self.translations)
#     def __str__(self):
#         return str(self.translations)

        



# def _get_tr(lines, input, message, *args, **kwargs):
#     index = input[1]
#     start = index-1
#     end = index-1
#     while is_msgline(cst.lines[start-1].kind):
#         start -= 1
#     lines = lines[start:end]
#     return 

# def get_tr(cst, cstl, item, *args, **kwargs):
#     if isinstance(item, slice):
#         inputs = cst.inputs[item]
#         messages = cstl.messages[item]
#         return tuple((_get_tr(cst.lines, inputi, messagei, *args, **kwargs) for inputi, messagei in zip(inputs, messages)))
#     return get_tr(cst.lines, cst.inputs[item], cstl.messages[item], *args, **kwargs)

