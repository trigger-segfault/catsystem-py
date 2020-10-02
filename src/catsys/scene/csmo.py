#!/usr/bin/python
# -*- coding: utf-8 -*-
"""CatSystem 1 CsMO scene type (.cst)

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-09-17'
__author__  = 'Robert Jordan'

#######################################################################################

import collections, enum, io, os, struct, zlib


def extractarc(arc, outdir, arcfmt='{0}-{1}'):
    arcdir = os.path.join(outdir, arcfmt.format(*[n.replace('.','') for n in os.path.splitext(arc.name)]))
    with open(arc.fullname, 'rb') as fa:
        for entry in arc.entries:
            print(arc.name + '/' + entry.name)
            entpath = os.path.join(arcdir, entry.name)
            entdat = fa.read(entry.size)
            if len(entdat) != entry.size:
                raise ValueError('Entry {0.name!r} length too small, got {1!r} instead of {0.size!r}'.format(entry, len(entdat)))
            with open(entpath, 'wb+') as fe:
                fe.write(entdat)
                del entdat
                fe.flush()

# CsMO = collections.namedtuple('CsMO', ('sig', 'offlen', 'orig_offlen', 'offs', 'txtlen', 'orig_txtlen', 'txts'))



def readcsmo(dat):
    offcomp = struct.unpack_from('<II', dat, 8)
    txtcomp = struct.unpack_from('<II', dat, 8+8+offcomp[0])
    csmo = CsMO(*struct.unpack('<8s II {0}s II {1}s'.format(offcomp[0], txtcomp[0]), dat))
    csmo = csmo._replace(
        offs=[v[0] for v in struct.iter_unpack('<I', zlib.decompress(csmo.offs))],
        txts=zlib.decompress(csmo.txts))
    return csmo

import os, sys, enum, struct, zlib
from struct import unpack, iter_unpack
from zlib import decompress

class CsMOLineType(enum.IntEnum):
    NONE      = 0 # end of script, also first byte
    UNK       = 0x01 # no length or content, beginning of script?
    MONOLOGUE = 0x02 # null-terminated
    NAME      = 0x03
    DIALOGUE  = 0x04 # null-terminated
    COMMAND   = 0x08


class CsMOLine(object):
    """line for CsMO class"""
    def __init__(self, kind:int, content:str, offset:int=None):
        self.offset = offset
        self.kind = kind
        self.content = content
    def __repr__(self):
        return '<CsMOLine: {0.kind!r} {0.content!r}>'.format(self)
    def __str__(self):
        return self.content
    def __len__(self):
        return len(self.content)

class CsMO(object):
    """b'CsMO   \\x00' (*.cst)"""
    SIGNATURE:bytes = b'CsMO   \x00'
    EXTENSION:str = '.cst'
    #
    def read(self, file, *, offset=..., length=...):
        if isinstance(file, str):
            with open(file, 'rb') as f:
                return self.read(f, offset=offset, length=length)
        elif isinstance(file, (bytes,bytearray)):
            return self.read(io.BytesIO(file), offset=offset, length=length)
        #
        self.signature = struct.unpack('<8s', file.read(8))[0]
        #probably inputs by scrdat byte offset
        self.offlen, self.origofflen = struct.unpack('<II', file.read(8))
        offraw = struct.unpack('<{0}s'.format(self.offlen), file.read(self.offlen))[0]
        self.scrlen, self.origscrlen = struct.unpack('<II', file.read(8))
        scrraw = struct.unpack('<{0}s'.format(self.scrlen), file.read(self.scrlen))[0]
        self.offdat = zlib.decompress(offraw)
        self.offsets = list(struct.unpack('<{0}'.format(self.origofflen // 4), offraw)) #[v[0] for v in struct.iter_unpack('<I', self.offdat)]
        self.scrdat = zlib.decompress(scrraw)
        scrfile = io.BytesIO(self.scrdat)
        self.scrhdr = self.scrdat[0:2]
        self.script = []
        scrfile.seek(2)
        kind = struct.unpack('<B', scrfile.read(1))[0]
        try:
            while kind:
                lnoff = scrfile.tell() - 1
                lnlen = struct.unpack('<H', scrfile.read(2))[0]
                lnstr = struct.unpack('<{0}s'.format(lnlen), scrfile.read(lnlen))[0].decode('cp932')
                if kind == 0x02 or kind == 0x04: # message content and quoted, also treated as null-terminated?
                    scrfile.seek(1, io.SEEK_CUR)
                self.script.append(CsMOLine(kind, lnstr, lnoff))
                print('{1}: {0.kind!r} {0.content!r}'.format(self.script[-1], len(self.script)-1))
                kind = struct.unpack('<B', scrfile.read(1))[0]
        except BaseException as ex:
            print(ex)
    #
    def __init__(self, file=None, *, offset=..., length=...):
        self.signature = self.SIGNATURE
        self.offlen, self.origofflen = 0, 0
        self.scrlen, self.origscrlen = 0, 0
        self.offsets = []
        self.script = []
        self.offdat = b''
        self.scrdat = b''
        self.scrhdr = b'\x00\x00'
        #
        if file is not None:
            self.read(file, offset=offset, length=length)
    #
    def __len__(self):
        return len(self.script)
    def __iter__(self):
        return iter(self.script)
    def __getitem__(self, item):
        return self.script[item]
    def __contains__(self, item):
        return item in self.script
    def index(self, item, *args, **kwargs):
        """C.index(line, [start, [stop]]) -> integer -- return first index of value."""
        return self.script.index(item, *args, **kwargs)
    def count(self, item, *args, **kwargs):
        return self.script.count(item, *args, **kwargs)

