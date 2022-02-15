#!/usr/bin/env python3
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

