#!/usr/bin/python
# -*- coding: utf-8 -*-
"""HG image encoding utility

Provides the encoding and decoding methods for HG image encodings (HG-2, HG-3).
"""

__version__ = '1.0.0'
__date__    = '2020-09-19'
__author__  = 'Robert Jordan'

__all__ = []

#######################################################################################

import io, os, struct
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations

# local imports
from ._baseimg import ImageContainer, ImageFrame


## PREDECLARE TYPES ##

HgSlice = collections.namedtuple('HgSlice', ('index', 'length')) #, 'data', 'cmd'))
HgData  = collections.namedtuple('HgData',  ('data', 'cmd'))

Point   = collections.namedtuple('Point', ('x, y')):
Size    = collections.namedtuple('Size', ('width', 'height'))

class StandardInfo(object):
    __slots__ = ('width', 'height', 'bpp', 'offsetx', 'offsety', 'fullwidth', 'fullheight', 'transparent', 'originx', 'originy')
    #
    def __init__(self, width:int=0, height:int=0, bpp:int=0, offsetx:int=0, offsety:int=0, fullwidth:int=0, fullheight:int=0, transparent:bool=False, originx:int=0, originy:int=0):
        self.width  = 0
        self.height = 0
        self.bpp    = 0
        self.offsetx    = 0
        self.offsety    = 0
        self.fullwidth  = 0
        self.fullheight = 0
        self.transparent = False
        self.originx = 0
        self.originy = 0
    @property
    def size(self) -> Tuple[int, int]:
        return Size(self.width, self.height)
    @size.setter
    def size(self, size:Tuple[int, int]):
        self.width, self.height = size
    #
    @property
    def fullsize(self) -> Tuple[int, int]:
        return Size(self.fullwidth, self.fullheight)
    @fullsize.setter
    def fullsize(self, fullsize:Tuple[int, int]):
        self.fullwidth, self.fullheight = fullsize
    #
    @property
    def offset(self) -> Tuple[int, int]:
        return Point(self.offsetx, self.offsety)
    @offset.setter
    def offset(self, offset:Tuple[int, int]):
        self.offsetx, self.offsety = offset
    #
    @property
    def origin(self) -> Tuple[int, int]:
        return Point(self.originx, self.originy)
    @origin.setter
    def origin(self, origin:Tuple[int, int]):
        self.originx, self.originy = origin
    #
    @property
    def bytedepth(self) -> int:
        return (self.bpp + 7) // 8
    # @bytedepth.setter
    # def bytedepth(self, bytedepth:int):
    #     self.bpp = bytedepth * 8
    #
    @property
    def stride(self) -> int:
        return (self.width * self.bpp + 7) // 8
    #
    @property
    def buffersize(self) -> int:
        return (self.stride * self.height)
    #
    @property
    def hasalpha(self) -> bool:
        return self.bpp == 32


## INITIALIZATION TABLES ##

def make_weight_tables():
    """make_weight_tables() -> weight_tables[4][256]

    NOTE: These tables contain unsigned integers

    >>> make_weight_tables()
    ([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, ...0xc0c0c080, 0xc0c0c0c0],
     [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, ...0x30303020, 0x30303030],
     [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, ...0x0c0c0c08, 0x0c0c0c0c],
     [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, ...0x03030302, 0x03030303])
    """
    weighttables = tuple([0] * 0x100 for _ in range(4)) # weighttables[4][256]
    for i in range(0x100):
        val = (((i & 0xc0) << 6 | (i & 0x30)) << 6 | (i & 0xc)) << 6 | (i & 0x3)
        #
        weighttables[3][i] = val
        weighttables[2][i] = val << 2
        weighttables[1][i] = val << 4
        weighttables[0][i] = val << 6
    #
    # convert to tuples for performance
    return tuple(tuple(t) for t in weighttables)

#TODO: Would returning tables as (bytes, bytes) be better for efficiency?
def make_abs_tables():
    """make_abs_tables() -> (abstable, inv_abstable)

    NOTE: These tables contain unsigned integers
    
    >>> make_abs_tables()
    # signed representation (however result is signed)
    ([0, 2, 4, 6, 8, 10, 11, ...5, 3, 1],
     [0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128])
    """
    abstable     = [0] * 0x100
    inv_abstable = [0] * 0x100
    #
    for i, j in zip(range(0x80), range(0, 0x100, 2)): # for(i=0,j=0; i<128; i++,j+=2)
        abstable[     i] = j
        abstable[0xff-i] = j + 1 # ~i
        #
        inv_abstable[j    ] =      i
        inv_abstable[j + 1] = 0xff-i # ~i
    #
    # convert to tuples for performance
    return (tuple(abstable), tuple(inv_abstable))

## BIT BUFFER / ELIAS GAMMA CODING ##


class BitBuffer(object):
    """BitBuffer(bytes) -> readable bitbuffer
    BitBuffer(bytearray) -> writable bitbuffer

    Returns a bitbuffer class for reading and writing individual bits and
    positive integers in elias gamma coding.

    NOTE: no bounds checking is performed
    ---
    attr            : b:bytes, i:next, k:next
    order           : check k, inc k, read k-1
    get_bit         : return int [0,1]
    get_elias_gamma : inline get_bit()
    
    conditional value |= bit
    no __repr__
    short local var names
    check bit >= 8
    local attrs : i, k, b
    """
    __slots__ = ('b', 'i', 'k')  # buffer, byte index, bit index
    def __init__(self, b:bytes):
        self.b = self.i = self.k = 0  # buffer, byte index, bit index
        # this._bit = 8; // set to check for EOF on next bit
        # this._index = index - 1; // Incremented on next bit
        # this._remaining = length + 1; // Decremented on next bit
    #
    # READ BITBUFFER:
    #
    def read_flag(self) -> bool:
        """Bb.read_flag() -> bool

        Reads the next boolean from the bitbuffer as a single bit.
        """
        k = self.k  # bit index
        if k >= 8:
            k = 0
            self.i += 1

        self.k = k + 1
        return bool((self.b[self.i] >> k) & 0x1)
        #return (self.b[self.i] >> (k    )) & 0x1
    #
    def read_eliasgamma(self) -> int:
        """Bb.read_eliasgamma() -> positive integer

        Reads the next positive integer from the bitbuffer in elias gamma coding.
        """
        b = self.b  # buffer
        i = self.i  # byte index
        k = self.k  # bit index
        if k >= 8: # incr bit [0:1]
            k = 0
            i += 1
        k += 1

        d = 0  # digits
        while not ((b[i] >> (k - 1)) & 0x1): # read bit [0:d+1]
            d += 1
            if k >= 8: # incr bit [1:d+1]
                k = 0
                i += 1
            k += 1
        
        v = 1 << d  # value
        while d:
            d -= 1
            if k >= 8: # incr bit [d+1:d*2+1]
                k = 0
                i += 1
            k += 1
            if (b[i] >> (k - 1)) & 0x1: # read bit [d+1:d*2+1]
                v |= (1 << d)

        self.i = i
        self.k = k
        return v
    #
    # WRITE BITBUFFER:
    #
    def write_flag(self, f:bool) -> int:
        """Bb.write_flag(flag) -> integer bits written (always 1)

        Writes a boolean to the bitbuffer as a single bit.
        """
        k = self.k  # bit index
        if k >= 8:
            k = 0
            self.i += 1
            # assume buffer is initialized with zero-bytes
        
        self.k = k + 1
        if f:
            self.b[self.i] |= (1 << k)
    #
    def write_eliasgamma(self, v:int) -> int:
        """Bb.write_eliasgamma(integer) -> integer bits written

        Writes a positive integer to the bitbuffer in elias gamma coding.
        """
        if v <= 0:
            raise ValueError('Elias gamma coded integer must be positive, not {0!r}'.format(v))
        b = self.b  # buffer
        i = self.i  # byte index
        k = self.k  # bit index

        d = 0  # digits
        while v >> (d + 1):
            d += 1
            if k >= 8: # incr bit [0:d]
                k = 0
                i += 1
            k += 1
            #b[i] |= (0 << (k - 1) # skip bit [0:d] (false)

        if k >= 8: # incr bit [d:d+1]
            k = 0
            i += 1
        k += 1
        b[i] |= (1 << (k - 1)) # write bit [d:d+1] (true)

        v = 1 << d  # value
        while d:
            d -= 1
            if k >= 8: # incr bit [d+1:d*2+1]
                k = 0
                i += 1
            k += 1
            if (v >> d) & 0x1: # write bit [d+1:d*2+1] (if true)
                b[i] |= 1 << (k - 1):

        self.i = i
        self.k = k
        return v


def sizeof_eliasgamma(v:int) -> int:
    """sizeof_eliasgamma(value) -> integer bit length

    Measures the bit length of a positive integer in elias gamma coding.
    """
    if v <= 0:
        raise ValueError('Elias gamma coded integer must be positive, not {0!r}'.format(v))

    d = 0  # digits
    while v >> (d + 1):
        d += 1

    # 1 bit minimum plus 2 bits per digit
    return d * 2 + 1


## ZERO RUN-LENGTH CODING ##

# --- format ---
# data bytes:
#  [<non-zero run 0:bytes>, <non-zero run 1:bytes>, ...<non-zero run m-1:bytes>]
# cmd bits:
#  <copyflag:bit>, <buffer length:eliasgamma>,
#  [<run length 0:eliasgamma>, <run length 1:eliasgamma>, ...<run length n-1:eliasgamma>]

def encode_zrle(hgslice:HgSlice, buffer:bytes) -> HgSlice:
    """encode_zrle(hgslice, data bytes) -> HgSlice(index, length, data bytearray, cmd bytearray)
    """
    buflen = len(buffer)
    ## STEP 1 MEASURE: measure length and offset of all runs ##
    #  (to allocate correct buffer sizes the first time)
    datalen = 0
    cmdbitlen = 1 # 1 bit for consumed copyflag
    cmdbitlen += sizeof_eliasgamma(buflen)

    runs = [] # includes zero and non-zero runs
    copyflag = bool(buffer[0]) # is first run non-zero data?

    off = 0
    while off < buflen:
        runlen = 1  # starts with the first non-conforming byte reached last run
        if copyflag:
            # length of non-zero run
            while off + runlen < buflen and buffer[off + runlen]:
                runlen += 1
            datalen += runlen
        else:
            # length of zero run
            while off + runlen < buflen and not buffer[off + runlen]:
                runlen += 1
        #
        runs.append(runlen)
        cmdbitlen += sizeof_eliasgamma(runlen)
        off += runlen
        copyflag = not copyflag

    ## STEP 2 BUILD: non-zero data runs buffer, cmd bits buffer ##
    data = bytearray(datalen) # already filled with zero-bytes
    cmd = bytearray((cmdbitlen + 7) // 8)

    copyflag = bool(buffer[0])

    bitbuf = BitBuffer(cmd, len(cmd))
    bitbuf.write_flag(copyflag)
    bitbuf.write_eliasgamma(length)

    off = dataoff = 0
    for runlen in runs:
        if copyflag:
            data[dataoff:dataoff + runlen] = buffer[off:off + runlen]
            dataoff += runlen

        bitbuf.write_eliasgamma(runlen)
        off += runlen
        copyflag = not copyflag

    return (data, cmd)


    databuf, cmdbuf = bytearray(), bytearray()
    deltalen = len(deltabuf) 
    copyflag = (deltabuf[0] != 0)
    #
    # cmd = BitBuffer(cmdbuf)
    # #
    # cmd.set_bit(copyflag)
    # cmd.set_elias_gamma(deltalen)
    firstcopy = copyflag
    runs = []
    cmdlen = 0
    datalen = 0
    # 
    i = 0
    while i < deltalen:
        n = 1
        if copyflag: # copy fill
            while i+n < deltalen and deltabuf[i+n] != 0:
                n += 1
            #
            #databuf.extend(deltabuf[i:i+n])
            datalen += n
            #
        else: # zero fill
            while i+n < deltalen and deltabuf[i+n] == 0:
                n += 1
        #
        #cmd.set_elias_gamma(n)
        cmdlen += elias_gamma_size(n)
        runs.append(n)
        #
        i += n
        copyflag = not copyflag
    #
    cmdlen += elias_gamma_size(datalen)
    cmdlen += 1 # copyflag bit
    #
    return ((datalen, cmdlen // 8), copyflag, runs)
    return databuf, cmdbuf

def decode_zrle(hgslice:HgSlice, data:bytes, cmd:bytes) -> bytearray:
    """decode_zrle(hgslice, data bytes, cmd bytes) -> buffer bytearray
    """
    bifbuf = BitBuffer(cmd, len(cmd))

    copyflag = bitbuf.read_flag() # is first run non-zero data?
    buflen = bitbuf.read_eliasgamma() # length of output buffer (usually height x stride)
    buffer = bytearray(buflen) # already filled with zero-bytes

    off = dataoff = 0
    while off < buflen:
        runlen = bitbuf.read_eliasgamma()
        #
        if copyflag:
            # Copy non-zero data into ouput buffer
            buffer[off:off + runlen] = data[dataoff:dataoff + runlen]
            dataoff += runlen
        #else skip zero bytes, buffer already filled with zero-bytes
        
        off += runlen
        copyflag = not copyflag
    
    return buffer

## ENCODE/DECODE BLOCKS ##

def encode_blocks(hgslice:HgSlice, delta:bytes, width:int, height:int, bpp:int) -> bytearray:
    """encode_blocks(hgslice, delta bytes, width, height, bpp) -> blocks bytearray
    """
    abstable = make_abs_tables()[0]  # fwd abstable

    stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    bufstart = hgslice.index * stride
    bufend = (hgslice.index + hgslice.length) * stride

    sectlen = hgslice.length * stride // 4
    blocks = bytearray(sectlen * 4)

    # first loop through the entire delta slice and perform absolute transform
    for i in range(bufstart, bufend, 1):
        delta[i] = abstable[delta[i]]

    # Iterate through each section one at a time, each pass
    # through delta encodes a different mask (section/block) of bytes
    dst = 0
    # Section masks: [0xc0c0c0c0, 0x30303030, 0x0c0c0c0c, 0x03030303]
    for k in range(6, -1, -2): # current section
        src = bufstart
        for i in range(sectlen): # section iteration
            val = 0
            for j in range(0, 8, 2): # section mask to byte
                val |= (((delta[src] >> k) & 0x3) << j)
                src += 1
            blocks[dst] = val & 0xff
            dst += 1
    
    return blocks

def decode_blocks(hgslice:HgSlice, blocks:bytes, width:int, height:int, bpp:int) -> bytearray:
    """decode_blocks(hgslice, blocks bytes, width, height, bpp) -> delta bytearray
    """
    table0, table1, table2, table3 = make_weight_tables()
    invtable = make_abs_tables()[1]  # inv abstable

    stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    bufstart = hgslice.index * stride
    #bufend = (hgslice.index + hgslice.length) * stride

    sectlen = hgslice.length * stride // 4
    sect0, sect1, sect2, sect3 = range(bufstart, bufstart + sectlen, sectlen)
    delta = bytearray(sectlen * 4)

    for i, j in zip(range(sectlen), range(0, sectlen * 4, 4)):
        val = (table0[blocks[sect0 + i]] | table1[blocks[sect1 + i]] |
               table2[blocks[sect2 + i]] | table3[blocks[sect3 + i]])

        delta[j    ] = invtable[(val      ) & 0xff]
        delta[j + 1] = invtable[(val >>  8) & 0xff]
        delta[j + 2] = invtable[(val >> 16) & 0xff]
        delta[j + 3] = invtable[(val >> 24)       ]
    
    return delta

## ENCODE/DECODE DELTA ##

def encode_delta(hgslice:HgSlice, pixels:bytes, width:int, height:int, bpp:int) -> bytearray:
    """encode_delta(hgslice, pixels bytes, width, height, bpp) -> delta bytearray
    """
    bytedepth = (bpp + 7) // 8
    stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    bufstart = hgslice.index * stride
    bufend = (hgslice.index + hgslice.length) * stride
    delta = bytearray(pixels)

    # delta RGB(A) channels of each previous stride in all but first row
    for yi, yj in zip(range(bufend - 1, bufstart + stride - 1, -1),
                      range(bufend - stride - 1, bufstart - 1, -1)):
        delta[yi] = (delta[yi] - delta[yj]) & 0xff

    # delta RGB(A) channels of each previous pixel in first row
    for xi, xj in zip(range(bufstart + stride - 1, bufstart + bytedepth - 1, -1),
                      range(bufstart + stride - bytedepth - 1, bufstart - 1, -1)):
        delta[xi] = (delta[xi] - delta[xj]) & 0xff
    
    return delta

def decode_delta(hgslice:HgSlice, delta:bytes, width:int, height:int, bpp:int) -> bytearray:
    """decode_delta(hgslice, delta bytes, width, height, bpp) -> pixels bytearray
    """
    bytedepth = (bpp + 7) // 8
    stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    bufstart = hgslice.index * stride
    bufend = (hgslice.index + hgslice.length) * stride

    # undelta RGB(A) channels of each previous pixel in first row
    for xi, xj in zip(range(bufstart + bytedepth, bufstart + stride, 1),
                      range(bufstart, bufstart + stride - bytedepth, 1)):
        delta[xi] = (delta[xi] + delta[xj]) & 0xff
    
    # undelta RGB(A) channels of each previous stride in all but first row
    for yi, yj in zip(range(bufstart + stride, bufstart, 1),
                      range(bufstart, bufstart - stride, 1)):
        delta[yi] = (delta[yi] + delta[yj]) & 0xff

    pixels = bytearray(delta)
    return pixels

## ENCODE/DECODE SLICES ##

def encode_slice(hgslice:HgSlice, pixels:bytes, width:int, height:int, bpp:int) -> HgSlice:
    """encode_slice(hgslice, pixels bytes, width, height, bpp) -> HgData(data bytearray, cmd bytearray)
    """
    delta = encode_delta(hgslice, pixels, width, height, bpp)
    blocks = encode_blocks(hgslice, width, height, bpp)
    data, cmd = encode_zrle(hgslice, blocks)
    return HgData(data, cmd)

def decode_slice(hgslice:HgSlice, hgdata:HgData, width:int, height:int, bpp:int) -> bytearray:
    """decode_slice(hgslice, HgData(data bytes, cmd bytes), width, height, bpp) -> pixels bytearray
    """
    data, cmd = decode_zrle(hgslice, hgdata.data, hgdata.cmd)


## CLEANUP ##

del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
