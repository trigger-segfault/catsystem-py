#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Reversing engineering interactive shell helper functions
"""

__version__ = '0.0.1'
__date__    = '2020-09-01'
__author__  = 'Robert Jordan'

#######################################################################################

import io, os, struct, zlib
from importlib import reload


def cstring(data, encoding=None, *, terminator=b'\x00'):
    term = data.find(terminator) # find null terminator
    text = data[:term if term != -1 else len(data)]
    return text if encoding is None else text.decode(encoding)

def cstring_from(data, offset=0, count=None, encoding=None, *, terminator=b'\x00'):
    if count is None: count = len(data) - offset
    start, end = offset, min(len(data), offset + count)
    #
    term = data.find(terminator, start, end) # find null terminator
    text = data[start:term if term != -1 else end]
    return text if encoding is None else text.decode(encoding)

def findall(items, item):
    results = []
    idx = items.find(item)
    while idx != -1:
        results.append(idx)
        idx = items.find(item, idx + 1)
    return results

def readfile(filename, mode='b', encoding=None):
    """mode: 'b'=binary data (default), 't'=text data
    """
    if mode != 't' and mode != 'b':
        raise ValueError('Unexpected mode {0!r}, expected \'b\' or \'t\''.format(mode))
    args = () if encoding is None else (encoding,)
    with open(filename, 'r{0}'.format(mode), *args) as f:
        return f.read()

def writefile(filename, data, mode=None, encoding=None):
    """mode: None=datatype (default), 'b'=binary data, 't'=text data
    """
    if mode is None:
        mode = 't' if isinstance(data, str) else 'b'
    if mode != 't' and mode != 'b':
        raise ValueError('Unexpected mode {0!r}, expected \'b\' or \'t\''.format(mode))
    args = () if encoding is None else (encoding,)
    with open(filename, 'w{0}+'.format(mode), *args) as f:
        count = f.write(data)
        f.flush()
        return count

def readdata(filename):
    with open(filename, 'rb') as f:
        return f.read()

def readtext(filename, encoding=None):
    args = () if encoding is None else (encoding,)
    with open(filename, 'rt', *args) as f:
        return f.read()

def writedata(filename, data):
    with open(filename, 'wb+') as f:
        count = f.write(data)
        f.flush()
        return count

def writetext(filename, text, encoding=None):
    args = () if encoding is None else (encoding,)
    with open(filename, 'wt+', *args) as f:
        count = f.write(text)
        f.flush()
        return count

def printlog(file, *names):
    with open(file, 'rt') as f:
        values = []
        names = list(names)
        line = f.readline()
        while line:
            for i in range(len(names)):
                if line.lower().startswith(names[i].lower()+':'):
                    names.remove(names[i])
                    values.append(line)
                    print(line.rstrip('\n'))
                    break
            line = f.readline()
    # print()
    # return ''.join(values).rstrip('\n')

