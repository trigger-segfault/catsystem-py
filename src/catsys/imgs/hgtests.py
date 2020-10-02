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

import io, os, struct, zlib
import enum, collections  # for type declarations only
from typing import Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # for hinting in declarations
import PIL
import PIL.Image
import PIL.ImageOps

PIL_Image = Type['PIL.Image']

# local imports
# from ._baseimg import ImageContainer, ImageFrame


## PREDECLARE TYPES ##

HgSlice = collections.namedtuple('HgSlice', ('index', 'length')) #, 'data', 'cmd'))
HgData  = collections.namedtuple('HgData',  ('data', 'cmd'))

Point   = collections.namedtuple('Point', ('x, y'))
Size    = collections.namedtuple('Size', ('width', 'height'))
Rect    = collections.namedtuple('Rect', ('x', 'y', 'width', 'height'))

#FORMAT: in byte-order (little endian)
#24bit = BGR
#32bit = BGRA

Color   = collections.namedtuple('Color', ('r', 'g', 'b', 'a'))

## TEST FUNCTIONS ##

class NoReprData(object):
    def __init__(self, **kwargs):
        self.__dict__.update(**kwargs)

def run_tests():
    import PIL
    import PIL.Image
    import PIL.ImageOps
    needalpha = False
    orientation = -1
    flip = orientation != 1
    imgpath = r"path\to\testimage.png"
    imgpath_c = r"path\to\testimage_c.png"
    with PIL.Image.open(imgpath) as imgfile:
      with PIL.Image.open(imgpath_c) as imgfile_c:
        image = rgba_image(imgfile, needalpha)
        image_c = rgba_image(imgfile_c, needalpha)
        hasalpha = image.mode == 'RGBA'
        mode = image.mode
        bpp = 32 if image.mode == 'RGBA' else 24
        pixels = rgba_bytes(image_c, needalpha, orientation)
        pixels_0 = rgba_bytes(image, needalpha, orientation)
        # fullwidth, fullheight = Size(image.width, image.height)
        # default identical
        size = Size(image.width, image.height)
        baseoffset = Point(0, 0)
        fullsize = size
        offset = baseoffset

        fullsize = Size(image.width * 3 // 2, image.height * 3 // 2)
        offset = Point(image.width * 3 // 8, image.height * 3 // 8)
        fullstride = fullsize[0] * (bpp // 8)

        # offsetx, offsety = Point(0, 0)
        # fullwidth, fullheight = Size(image.width * 2, image.height * 2 + image.height // 2)
        # # fullstride = fullwidth * (bpp // 8)
        # fullbuffersize = fullstride * fullheight
        # offsetx, offsety = Point(image.width // 2, image.height // 3)
        pixels_a = bytearray(pixels)
        buffer_a = pack_from_pixels(pixels_a, 0, size[1], *size, bpp, 8, *baseoffset, *size, flip)

        buffer_b = bytearray(buffer_a)
        pixels_b = bytearray(fullstride * fullsize[1])
        unpack_into_pixels(buffer_b, pixels_b, 0, size[1], *size, bpp, 8, *offset, *fullsize, flip)
        # buffer_b = bytearray(buffer_a)
        # pixels_b = bytearray(len(pixels))
        # unpack_into_pixels(buffer_b, pixels_b, 0, image.height, image.width, image.height, bpp, 8, offsetx, offsety, not flip)
        rawmode = 'BGRA' if image.mode == 'RGBA' else 'BGR'

        # print('len(pixels)={!r}'.format(len(pixels)))
        print('len(pixels)={!r}\nlen(pixels_a)={!r} len(buffer_a)={!r}\nlen(pixels_b)={!r} len(buffer_b)={!r}'.format(len(pixels), len(pixels_a), len(buffer_a), len(pixels_b), len(buffer_b)))
        print('pixels {0} pixels_b'.format('==' if bytes(pixels) == bytes(pixels_b) else '!='))
        print('pixels_0 {0} pixels_b'.format('==' if bytes(pixels_0) == bytes(pixels_b) else '!='))
        image_b = PIL.Image.frombytes(image.mode, fullsize, bytes(pixels_b), 'raw', rawmode, 0, orientation)
        image_b.show()
        # newpath = r"path\to\testimage_b.png"
        # with open(newpath, 'wb+') as file_b:
        #     image_b.save(file_b, 'PNG')
        return NoReprData(image=image, pixels=pixels, pixels_a=pixels_a, pixels_b=pixels_b, buffer_a=buffer_a, buffer_b=buffer_b)
        #newimage = PIL.Image.new(rawmode,)
        # print('pixels_a {0} pixels_b\nbuffer_a {} buffer_b'.format('==' if pixels_a == pixels_b else '!=', '==' if buffer_a == buffer_b else '!='))
#         with memoryview(rgba_bytes(image, needalpha, orientation)) as pixels:
#     image.tobytes('raw', 'BGRA', stride, orientation)
# image = image.crop(bbox)
#             if image.mode != 'RGBA' and (image.mode.endswith('A') or image.mode.endswith('a')):
#                 image = image.convert('RGBA')
#             elif image.mode == 'RGBA':
#                 bitdepth = 32
#                 stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
#                 pixels = image.tobytes('raw', 'BGRA', stride, orientation)
#             elif image.mode == 'RGB':
#                 bitdepth = 24
#                 stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
#                 pixels = image.tobytes('raw', 'BGR', stride, orientation)
#             elif image.mode == 'L':
#                 bitdepth = 8
#                 stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
#                 pixels = image.tobytes('raw', 'L', stride, orientation)
#             elif image.mode == '1':
#                 bitdepth = 1
#                 stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
#                 pixels = image.tobytes('raw', '1', stride, orientation)
#             else:
#                 #TODO: Auto-convert? or fail?
#                 #image = image.convert('RGBA')
#                 #bitdepth = 32
#                 #stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
#                 #pixels = image.tobytes('raw', 'BGRA', stride, orientation)
#                 raise ValueError('Unsupported image mode {0!r} for writing'.format(image.mode))


def rgba_image(image:PIL_Image, needalpha:bool=False) -> PIL_Image:
    bands = image.getbands()
    hasalpha = 'A' in bands or 'a' in bands
    if image.mode != 'RGBA' and (needalpha or hasalpha):
        image = image.convert('RGBA')
    elif image.mode != 'RGB' and (not needalpha and not hasalpha):
        image = image.convert('RGB')
    return image
    
def rgba_bytes(image:PIL_Image, needalpha:bool=False, orientation:int=1) -> bytes: #Tuple[bytes, str]:
    image = rgba_image(image, needalpha)
    if image.mode == 'RGBA':
        bpp = 32
        mode = 'BGRA'
    elif image.mode == 'RGBA':
        bpp = 24
        mode = 'BGR'
    stride = ((image.width * bpp + 7) // 8 + 3) & ~0x3
    return image.tobytes('raw', 'BGRA', stride, orientation)#, image.mode


def pack_from_pixels(pixels:bytearray, sliceidx:int, slicelen:int, width:int, height:int, bpp:int, bpc:int, offsetx:int, offsety:int, fullwidth:int, fullheight:int, flip:bool):
    bytedepth  = bpp // 8 # only bpp 24,32 not supported by CS2
    stride     = (width * bytedepth + 3) & ~0x3
    fullstride = (fullwidth * bytedepth + 3) & ~0x3

    colstart = offsetx
    colend   = offsetx + width
    rowstart = offsety + sliceidx
    rowend   = offsety + sliceidx + slicelen

    fullstart = rowstart   * fullstride + colstart * bytedepth
    # fullend   = (rowend-1) * fullstride + colend   * bytedepth
    fullend   = (rowend-1) * fullstride + colstart * bytedepth + stride
    del colstart, colend, rowstart, rowend

    if not flip:
        normyrange = range(fullstart, fullend, fullstride)
        deltayrange = range(fullend - stride, fullstart + fullstride - 1, -fullstride)
        #deltaxrange = range(fullend - bytedepth - 1, fullend - stride - 1, -1)
        deltaxrange = range(fullstart + stride - 1, fullstart + bytedepth - 1, -1)
        deltastride = fullstride
        deltadepth = bytedepth
    else:
        normyrange = range(fullend - stride, fullstart - 1, -fullstride)
        deltayrange = range(fullstart, fullend - fullstride - stride, fullstride)
        #deltaxrange = range(fullstart + stride - 1, fullstart + bytedepth - 1, -1)
        # deltaxrange = range(fullend - stride, fullend - bytedepth, 1)
        deltaxrange = range(fullend - 1, fullend - stride + bytedepth - 1, -1)
        deltastride = -fullstride
        deltadepth = bytedepth

    # buflen = (stride * height)
    # sectlen = buflen // 4
    sectlen = (stride * height) // 4
    sect0, sect1, sect2, sect3 = sects = range(0, sectlen * 4, sectlen)

    #abstable = make_abs_tables()[0]  # fwd abstable
    #table0, table1, table2, table3 = make_weight_tables()
    #block0, block1, block2, block3 = blocks = [None] * 4
    block0 = block1 = block2 = block3 = None

    
    abstable = bytes(make_abs_tables()[0])  # fwd abstable

    # stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    # bufstart = hgslice.index * stride
    # bufend = (hgslice.index + hgslice.length) * stride

    # sectlen = slicelen * stride // 4
    _buffer = bytearray(sectlen * 4)
    buffer = None
    block0 = block1 = block2 = block3 = None
    try:
        buffer = memoryview(_buffer)
        abstable = memoryview(abstable)
        # for i, sect in enumerate(range(0, sectlen * 4, sectlen)):
        #     blocks[i] = buffer[sect:sect + sectlent]
        # block0, block1, block2, block3 = blocks
        block0 = buffer[sect0:sect0 + sectlen]
        block1 = buffer[sect1:sect1 + sectlen]
        block2 = buffer[sect2:sect2 + sectlen]
        block3 = buffer[sect3:sect3 + sectlen]
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1):
                pixels[yx] = (pixels[yx] - pixels[yx - deltastride]) & 0xff

        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] - pixels[x0 - deltadepth]) & 0xff

        # first loop through the entire delta slice and perform absolute transform
        # for y0 in normyrange:
        #     for yx in range(y0, y0 + stride, 1):
        #         pixels[yx] = abstable[pixels[yx]]
        
        # normalize pixel buffer into data blocks
        i = 0
        for y0 in normyrange:
            for yx in range(y0, y0 + stride, 4):
                # val = unpack_from('<I', pixels, src)
                # b = abstable[pixels[yx    ]]
                # v3 = ((b     ) & 0x3)
                # v2 = ((b >> 2) & 0x3)
                # v1 = ((b >> 4) & 0x3)
                # v0 = ((b >> 6) & 0x3)
                # b = abstable[pixels[yx + 1]]
                # v3 |= ((b << 2) & 0xc)
                # v2 |= ((b     ) & 0xc)
                # v1 |= ((b >> 2) & 0xc)
                # v0 |= ((b >> 4) & 0xc)
                # b = abstable[pixels[yx + 2]]
                # v3 |= ((b << 4) & 0x30)
                # v2 |= ((b << 2) & 0x30)
                # v1 |= ((b     ) & 0x30)
                # v0 |= ((b >> 2) & 0x30)
                # b = abstable[pixels[yx + 3]]
                # v3 |= ((b << 6) & 0xc0)
                # v2 |= ((b << 4) & 0xc0)
                # v1 |= ((b << 2) & 0xc0)
                # v0 |= ((b     ) & 0xc0)
                v0 = v1 = v2 = v3 = 0
                #m = 0x3
                for j in range(0, 8, 2):
                    #for j in range(0, 8, 2): # section mask to byte
                    #m = 0x3 << j
                    b = abstable[pixels[yx]]
                    yx += 1
                    v3 |= ((b     ) & 0x3) << j
                    v2 |= ((b >> 2) & 0x3) << j
                    v1 |= ((b >> 4) & 0x3) << j
                    v0 |= ((b >> 6) & 0x3) << j
                    #m <<= 2
                    # b = ((((pixels[src    ] >> k) & 0x3)) |
                    #     (((pixels[src + 1] >> k) & 0x3) << 2) |
                    #     (((pixels[src + 2] >> k) & 0x3) << 4) |
                    #     (((pixels[src + 3] >> k) & 0x3) << 6))
                    # idx |= (((pixels[src] >> k) & 0x3) << j)
                    # src += 1
                block3[i] = v3
                block2[i] = v2
                block1[i] = v1
                block0[i] = v0
                #blocks[i] = idx & 0xff
                i += 1

                # val = (table0[block0[i]] | table1[block1[i]] |
                #        table2[block2[i]] | table3[block3[i]]))

                # pixels[yx    ] = invtable[(val      ) & 0xff]
                # pixels[yx + 1] = invtable[(val >>  8) & 0xff]
                # pixels[yx + 2] = invtable[(val >> 16) & 0xff]
                # pixels[yx + 3] = invtable[(val >> 24)       ]

        
        buffer = buffer.release()
        return _buffer
    except:
        _locs = locals()
        _keys = list(_locs.keys())
        _keys.sort()
        for _k in _keys: #'x0 y0 yx i j k block0 block1 block2 block3 buffer _buffer pixels _pixels abstable'.split(' '):
            _v = _locs[_k]
            # print('{0!s}={1}'.format(_k, object.__repr__(_v)))
            _fmt = '{0!s}={1!s}'
            try:
                _fmt += ' len={0!r}'.format(len(_v))
            except:
                pass
            if _v is None or isinstance(_v, (int,float,range,slice)):
                print(_fmt.format(_k, repr(_v)))
            else:
                print(_fmt.format(_k, object.__repr__(_v)))
            # if _v is not None and not isinstance(_v, (int,float,str,range,slice)) and (not isinstance(_v, tuple) or len(_v) > 8):
            #     print('{0!s}={1}'.format(_k, object.__repr__(_v)))
            # else:
            #     print('{0!s}={1!r}'.format(_k, _v))
        print('<PIL.Image: mode={0.mode!r} width={0.width!r} height={0.height!r}')
        #print('x0={0!r} y0={0!r} yx={0!r} i={0!r} j={0!r} k={0!r}')
        raise
    finally:
        # if isinstance(pixels, memoryview):
        #     pixels.release()
        # if isinstance(buffer, memoryview):
        #     buffer.release()
        for memitem in (block0, block1, block2, block3, buffer, pixels, abstable): #blocks:
            if isinstance(memitem, memoryview):
                memitem.release()

    # first loop through the entire delta slice and perform absolute transform
    # for i in range(bufstart, bufend, 1):
    #     delta[i] = abstable[delta[i]]

    # # Iterate through each section one at a time, each pass
    # # through delta encodes a different mask (section/block) of bytes
    # i = 0
    # # Section masks: [0xc0c0c0c0, 0x30303030, 0x0c0c0c0c, 0x03030303]
    # for k in range(6, -1, -2): # current section
    #     src = bufstart
    #     for i in range(sectlen): # section iteration
    #         idx = 0
    #         val = unpack_from('<I', delta, src)
    #         b0 = b1 = b2 = b3 = 0
    #         m = 0x3
    #         for j in range(0, 8, 2): # section mask to byte
    #             m = 0x3 << j
    #             b = abstable[delta[]
    #             b3 |= (b     ) & m
    #             b >>= 2
    #             b2 |= (b >> 2) & m
    #             b >>= 2
    #             b1 |= (b >> 4) & m
    #             b >>= 2
    #             b0 |= (b >> 6) & m
    #             m <<= 2
    #             b = ((((delta[src    ] >> k) & 0x3)) |
    #                  (((delta[src + 1] >> k) & 0x3) << 2) |
    #                  (((delta[src + 2] >> k) & 0x3) << 4) |
    #                  (((delta[src + 3] >> k) & 0x3) << 6))
    #             idx |= (((delta[src] >> k) & 0x3) << j)
    #             src += 1
    #         blocks[i] = idx & 0xff
    #         dst += 1
    
    #return blocks

def unpack_into_pixels(buffer:bytes, pixels:bytearray, sliceidx:int, slicelen:int, width:int, height:int, bpp:int, bpc:int, offsetx:int, offsety:int, fullwidth:int, fullheight:int, flip:bool):
    bytedepth  = bpp // 8 # only bpp 24,32 not supported by CS2
    stride     = (width * bytedepth + 3) & ~0x3
    fullstride = (fullwidth * bytedepth + 3) & ~0x3

    colstart = offsetx
    colend   = offsetx + width
    rowstart = offsety + sliceidx
    rowend   = offsety + sliceidx + slicelen

    fullstart = rowstart   * fullstride + colstart * bytedepth
    # fullend   = (rowend-1) * fullstride + colend   * bytedepth
    fullend   = (rowend-1) * fullstride + colstart * bytedepth + stride
    del colstart, colend, rowstart, rowend

    if not flip:
        normyrange = range(fullstart, fullend, fullstride)
        deltayrange = range(fullstart + fullstride, fullend, fullstride)
        deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
        deltastride = fullstride
        deltadepth = bytedepth
    else:
        normyrange = range(fullend - stride, fullstart - 1, -fullstride)
        deltayrange = range(fullend - fullstride - stride, fullstart - 1, -fullstride)
        # deltaxrange = range(fullend - bytedepth - 1, fullend - stride - 1, -1)
        deltaxrange = range(fullend - stride + bytedepth, fullend, 1)
        deltastride = -fullstride
        deltadepth = bytedepth

    # buflen = len(buffer)
    # sectlen = buflen // 4
    sectlen = len(buffer) // 4
    sect0, sect1, sect2, sect3 = sects = range(0, sectlen * 4, sectlen)

    invtable = make_abs_tables()[1]  # inv abstable
    table0, table1, table2, table3 = make_weight_tables()
    #block0, block1, block2, block3 = blocks = [None] * 4
    block0 = block1 = block2 = block3 = None
    try:
        buffer = memoryview(buffer)
        # for i, sect in enumerate(range(0, sectlen * 4, sectlen)):
        #     blocks[i] = buffer[sect:sect + sectlent]
        # block0, block1, block2, block3 = blocks
        block0 = buffer[sect0:sect0 + sectlen]
        block1 = buffer[sect1:sect1 + sectlen]
        block2 = buffer[sect2:sect2 + sectlen]
        block3 = buffer[sect3:sect3 + sectlen]
        buffer = buffer.release()

        # inverse normalize data blocks into pixel buffer
        i = 0
        for y0 in normyrange:
            for yx in range(y0, y0 + stride, 4):
                val = (table0[block0[i]] | table1[block1[i]] |
                       table2[block2[i]] | table3[block3[i]])
                i += 1

                pixels[yx    ] = invtable[(val      ) & 0xff]
                pixels[yx + 1] = invtable[(val >>  8) & 0xff]
                pixels[yx + 2] = invtable[(val >> 16) & 0xff]
                pixels[yx + 3] = invtable[(val >> 24)       ]

        # next loop through the entire delta slice and perform inverse absolute transform
        # for y0 in normyrange:
        #     for yx in range(y0, y0 + stride, 1):
        #         pixels[yx] = abstable[pixels[yx]]

        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] + pixels[x0 - deltadepth]) & 0xff
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1):
                pixels[yx] = (pixels[yx] + pixels[yx - deltastride]) & 0xff

    finally:
        # if isinstance(pixels, memoryview):
        #     pixels.release()
        # if isinstance(buffer, memoryview):
        #     buffer.release()
        for memitem in (block0, block1, block2, block3, buffer, pixels): #blocks:
            if isinstance(memitem, memoryview):
                memitem.release()
        
        # block0, block1, block2, block3 = tuple(buffer[i:i + sectlen] for i in range(0, buflen, sectlen))

        # buflen = len(buffer)
        # sectlen = buflen // 4
        # sect0, sect1, sect2, sect3 = range(slicestart, slicestart + sectlen * 4, sectlen)
        # block0, block1, block2, block3 = blocks = tuple(buffer[i:i + sectlen] for i in range(0, buflen, sectlen))


        # if not flip:
        #     normyrange = range(fullstart, fullend, fullstride)
        #     deltayrange = range(fullstart + stride, fullend, fullstride)
        #     deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
        # else:
        #     normyrange = range(fullend - stride, fullstart - 1, fullstride)
        #     deltayrange = range(fullend - stride - stride, fullstart - 1, -fullstride)
        #     deltaxrange = range(fullend - stride + bytedepth, fullend, 1)
       
        # # inverse normalize data blocks into pixel buffer
        # i = 0
        # for y0 in normyrange:
        #     for yx in range(y0, y0 + stride, 4):
        #         val = (table0[block0[i]] | table1[block1[i]] |
        #                table2[block2[i]] | table3[block3[i]]))
        #         i += 1

        #         pixels[yx    ] = invtable[(val      ) & 0xff]
        #         pixels[yx + 1] = invtable[(val >>  8) & 0xff]
        #         pixels[yx + 2] = invtable[(val >> 16) & 0xff]
        #         pixels[yx + 3] = invtable[(val >> 24)       ]
        

        # # undelta RGB(A) channels of each previous pixel in first row
        # for x0 in deltaxrange:
        #     pixels[x0] = (pixels[x0] + pixels[x0 - bytedepth]) & 0xff
        
        # # undelta RGB(A) channels of each previous stride in all but first row
        # for y0 in deltayrange:
        #     for yx in range(y0, y0 + stride, 1)):
        #         pixels[yx] = (pixels[yx] + pixels[yx - fullstride]) & 0xff





#TODO: test import array.array type
def make_weight_tables() -> Tuple[List[int], List[int], List[int], List[int]]:
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

#TODO: test import array.array type/bytes type
#TODO: Would returning tables as (bytes, bytes) be better for efficiency?
def make_abs_tables() -> Tuple[List[int], List[int]]:
    """make_abs_tables() -> (abstable, inv_abstable)

    NOTE: These tables contain unsigned integers
    
    >>> make_abs_tables()
    # signed representation (however result is unsigned)
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

