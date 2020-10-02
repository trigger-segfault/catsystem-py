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

# local imports
from ._baseimg import ImageContainer, ImageFrame


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

def get_color(color:Union[int, Tuple[int,int,int], Tuple[int,int,int,int]]) -> Color:
        if not isinstance(color, int):
            if len(color) == 3:
                return Color(*color[0], 0xff)
            return Color(*color)
        return Color((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff)


class HgAttribute(object):
    __slots__ = ('x', 'y', 'width', 'height', 'rawcolor')
    #
    def __init__(self, x:int, y:int, width:int, height:int, color:Union[int, Color]):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        if not isinstance(color, int):
            if len(color) == 3:
                color = Color(*color[0], 0xff)
            else:
                color = Color(*color)
        else:
            color = Color((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff)
        self.color = color
    #
    @property
    def point(self) -> Point: return Point(self.x, self.y)
    @point.setter
    def point(self, point:Size): self.x, self.y = point
    #
    #
    @property
    def size(self) -> Size: return Size(self.width, self.height)
    @size.setter
    def size(self, size:Size): self.width, self.height = size
    #
    @property
    def rawcolor(self) -> int:
        return ((self.color[0] << 16) | (self.color[1] <<  8) |
                (self.color[2]      ) | (self.color[3] << 24))
    @rawcolor.setter
    def rawcolor(self, rawcolor):
        self.color = Color(
            (rawcolor >> 16) & 0xff, (rawcolor >>  8) & 0xff,
            (rawcolor      ) & 0xff, (rawcolor >> 24) & 0xff)

class StandardInfo(object):
    __slots__ = ('width', 'height', 'bpp', 'offsetx', 'offsety', 'fullwidth', 'fullheight', 'transparent', 'originx', 'originy')
    #
    def __init__(self, width:int=0, height:int=0, bpp:int=0, offsetx:int=0, offsety:int=0, fullwidth:int=0, fullheight:int=0, transparent:bool=False, originx:int=0, originy:int=0):
        self.width  = 0
        self.height = 0
        self.bpp    = 0  # pixel bit depth (bits per pixel)
        self.bpc    = 0  # channel bit depth (bits per channel) 0 => 8
        self.canvasx    = 0
        self.canvasy    = 0
        self.canvaswidth  = 0
        self.canvasheight = 0
        self.transparent = False
        self.originx = 0
        self.originy = 0
    @property
    def size(self) -> Size: return Size(self.width, self.height)
    @size.setter
    def size(self, size:Size): self.width, self.height = size
    #
    @property
    def canvas(self) -> Rect: return Rect(self.canvasx, self.canvasy, self.canvaswidth, self.canvasheight)
    @canvas.setter
    def canvas(self, canvasrect:Rect): self.canvasx, self.canvasy, self.canvaswidth, self.canvasheight = canvasrect
    #
    @property
    def canvassize(self) -> Size: return Size(self.canvaswidth, self.canvasheight)
    @canvassize.setter
    def canvassize(self, canvassize:Size): self.canvaswidth, self.canvasheight = canvassize
    #
    @property
    def canvaspos(self) -> Point: return Point(self.canvasx, self.canvasy)
    @canvaspos.setter
    def canvaspos(self, canvaspos:Point): self.canvasx, self.canvasy = canvaspos
    #
    @property
    def origin(self) -> Point: return Point(self.originx, self.originy)
    @origin.setter
    def origin(self, origin:Point): self.originx, self.originy = origin
    #
    @property
    def bytedepth(self) -> int: return (self.bpp + 7) // 8
    # @bytedepth.setter
    # def bytedepth(self, bytedepth:int): self.bpp = bytedepth * 8
    @property
    def stride(self) -> int: return (self.width * self.bpp + 7) // 8
    @property
    def buffersize(self) -> int: return (self.stride * self.height)
    @property
    def hasalpha(self) -> bool: return self.bpp == 32
    #
    @property
    def depthmax(self) -> int:
        return (((1 << self.depth ** 2) - 1) & 0xff) if self.depth else 0xff

class StandardInfo(object):
    """StandardInfo(**kwargs) -> stdinfo with assigned kwargs
    """
    __slots__ = ('size', 'bpp', 'depth', 'canvassize', 'canvaspos', 'transparent', 'origin')

    def __init__(self, **kwargs):
        self.size = Size(0, 0)
        self.canvassize = Size(0, 0)
        self.canvaspos = Point(0, 0)
        self.origin = Point(0, 0)
        self.transpareny = False
        self.bpp = 0  # 24 or 32 required
        self.bpc = 0  # only used in HG-2, (see source of info)
        #<https://github.com/morkt/GARbro/blob/c5e13f6db1d24a62eb621c38c6fc31387338d857/ArcFormats/CatSystem/ImageHG2.cs#L117-L126>

        for key,val in kwargs.items():
            setattr(self, key, val)
    #
    #ATTRIBUTE PROPERTIES:
    #
    @property
    def width(self) -> int: return self.size[0]
    @width.setter
    def width(self, width): self.size = Size(width, self.size[1])
    @property
    def height(self) -> int: return self.size[1]
    @height.setter
    def height(self, height): self.size = Size(self.size[0], height)
    #
    @property
    def canvaswidth(self) -> int: return self.canvassize[0]
    @canvaswidth.setter
    def canvaswidth(self, canvaswidth): self.canvassize = Size(canvaswidth, self.canvassize[1])
    @property
    def canvasheight(self) -> int: return self.canvassize[1]
    @canvasheight.setter
    def canvasheight(self, canvasheight): self.canvassize = Size(self.canvassize[0], canvasheight)
    #
    @property
    def canvasx(self) -> int: return self.canvaspos[0]
    @canvasx.setter
    def canvasx(self, canvasx): self.canvaspos = Point(canvasx, self.canvaspos[1])
    @property
    def canvasy(self) -> int: return self.canvaspos[1]
    @canvasy.setter
    def canvasy(self, canvasy): self.canvaspos = Point(self.canvaspos[0], canvasy)
    #
    @property
    def originx(self) -> int: return self.origin[0]
    @originx.setter
    def originx(self, originx): self.origin = Point(originx, self.origin[1])
    @property
    def originy(self) -> int: return self.origin[1]
    @originy.setter
    def originy(self, originy): self.origin = Point(self.origin[0], originy)
    #
    #CALCULATED PROPERTIES:
    #
    @property
    def bytedepth(self) -> int: return (self.bpp + 7) // 8
    @bytedepth.setter
    def bytedepth(self, bytedepth): self.bpp = bytedepth * 8
    #
    #
    #ALIAS PROPERTIES
    #
    @property
    def bitdepth(self) -> int: return self.bpp
    @bitdepth.setter
    def bitdepth(self, bitdepth): self.bpp = bitdepth
    #
    @property
    def channeldepth(self) -> int: return self.bpc
    @channeldepth.setter
    def channeldepth(self, channeldepth): self.bpc = channeldepth
    
    @property
    def channelmax(self) -> int:
        return ((2**self.bpc - 1) & 0xff) if self.bpc else 0xff # bitmask for number of bits
    # @bytedepth.setter
    # def bytedepth(self, bytedepth:int):
    #     self.bpp = bytedepth * 8
    #
    @property
    def stride(self) -> int: return (self.width * self.bpp + 7) // 8
    #
    @property
    def buffersize(self) -> int: return (self.stride * self.height)
    #
    @property
    def offsetstride(self) -> int: return (self.canvasx * self.bpp + 7) // 8
    #
    @property
    def canvasstride(self) -> int: return (self.canvaswidth * self.bpp + 7) // 8
    @property
    def canvasbuffersize(self) -> int: return (self.canvasstride * self.canvasheight)
    #
    @property
    def hasalpha(self) -> bool: return self.bpp == 32
    # [Flags]
    # public enum HgxOptions {
    #     None = 0,
    #     Flip = (1 << 0), // Flip vertically (applied after crop when encoding)
    #     Crop = (1 << 2), // Expand or Shrink
    # }

class HgOptions(enum.IntFlag):
    """FLIP: vertically flip the image
    CROP: expand or shrink the image around transparent area
    """
    #NONE = 0 # no options
    FLIP = (1 << 0)  # vertical flip
    CROP = (1 << 1)  # shrink/expand canvas size around transparency
    _CHANGE_ALPHA = (1 << 2)
    ADD_ALPHA = _CHANGE_ALPHA | 0
    REMOVE_ALPHA = _CHANGE_ALPHA | (1 << 3)

class HgEncoding(object):
    """HgEncoding() -> HG image encoder/decoder
    """
    _weighttables:Tuple[List[int], List[int], List[int], List[int]] = make_weight_tables()
    _abstables:Tuple[List[int], List[int]] = make_abs_tables()
    def __init__(self):
        self.pixels = bytearray()
        self.options = HgOptions(0)
        self.size = Size(0, 0)
        self.bpp = 0
        self.depth = 0
        self.canvassize = Size(0, 0)
        self.canvaspos = Point(0, 0)
        self.transparent = False
        self.origin = Point(0, 0)

    def unpack_slicemem(self, data:bytes, cmd:bytes):
        block0 = block1 = block2 = block3 = buffer = None
        try:
            data = memoryview(zlib.decompress(data))
            cmd  = memoryview(zlib.decompress(cmd))

            bifbuf = BitBuffer(cmd, len(cmd))
            copyflag = bitbuf.read_flag() # is first run non-zero data?
            buflen = bitbuf.read_eliasgamma() # length of output buffer (usually height x stride)
            buffer = memoryview(bytearray(buflen))

            data = data.release()
            cmd = cmd.release()

        except:
            # if isinstance(data, memoryview):
            #     data.release()
            # if isinstance(cmd, memoryview):
            #     cmd.release()
            if isinstance(buffer, memoryview):
                buffer.release()
            raise

                buffer = unpack_datamem(data, cmd)
        finally:
            if isinstance(data, memoryview):
                data.release()
            if isinstance(cmd, memoryview):
                cmd.release()
            if isinstance(buffer, memoryview):
                buffer.release()
        with memoryview(data) as data:
            with memoryview(cmd) as cmd:
                buffer = unpack_datamem(data, cmd)


                bifbuf = BitBuffer(cmd, len(cmd))

                copyflag = bitbuf.read_flag() # is first run non-zero data?
                buflen = bitbuf.read_eliasgamma() # length of output buffer (usually height x stride)
                _buffer = bytearray(buflen) # already filled with zero-bytes
                buffer = unpack_datamem(data, cmd)
        memoryblocks = [None] * 4
        with memoryview
    def unpack_zrle_pt1(self, data:memoryview, cmd:memoryview) -> Tuple[BitBuffer, copyflag, bytearray]:

        # with memoryview(zlib.decompress(data)) as data:
        #     with memoryview(zlib.decompress(cmd)) as cmd:
        #         return unpack_datamem2(data, cmd)
    
    # def unpack_datamem(self, data:memoryview, cmd:memoryview) -> bytearray:
    #     data = zlib.decompress(data)
    #     cmd  = zlib.decompress(cmd)
    #     with memoryview(zlib.decompress(data)) as data:
    #         with memoryview(zlib.decompress(cmd)) as cmd:
    #             return unpack_datamem2(data, cmd)
def make_fwd_weight_tables() -> List[Tuple[int], Tuple[int], Tuple[int], Tuple[int]]:
    pass


def rgba_image(image:PIL.Image, needalpha:bool=False):
    bands = image.getbands()
    hasalpha = 'A' in bands or 'a' in bands
    if image.mode != 'RGBA' and (needalpha or hasalpha):
        image = image.convert('RGBA')
    elif image.mode != 'RGB' and (not needalpha and not hasalpha):
        image = image.convert('RGB')
    return image
    
def rgba_bytes(image:PIL.Image, needalpha:bool=False, orientation:int=1) -> Tuple[PIL.Image, str]:
    image = rgba_image(image, needalpha)
    if image.mode == 'RGBA':
        bpp = 32
        mode = 'BGRA'
    elif image.mode == 'RGBA':
        bpp = 24
        mode = 'BGR'
    stride = ((image.width * bpp + 7) // 8 + 3) & ~0x3
    return (image.tobytes('raw', 'BGRA', stride, orientation), image.mode)


    if image.mode == 'RGBA':
        bpp = 32
        mode = 'BGRA'
    elif image.mode == 'RGBA':
        bpp = 24
        mode = 'BGR'
    stride = ((image.width * bpp + 7) // 8 + 3) & ~0x3
    pixels = image.tobytes('raw', 'BGRA', stride, orientation)


    if image.mode != 'RGBA' and (image.mode.endswith('A') or image.mode.endswith('a')):
        image = image.convert('RGBA')
    elif image.mode == 'RGBA':
        bitdepth = 32
        stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        pixels = image.tobytes('raw', 'BGRA', stride, orientation)
    elif image.mode == 'RGB':
        bitdepth = 24
        stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        pixels = image.tobytes('raw', 'BGR', stride, orientation)
    elif image.mode == 'L':
        bitdepth = 8
        stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        pixels = image.tobytes('raw', 'L', stride, orientation)
    elif image.mode == '1':
        bitdepth = 1
        stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        pixels = image.tobytes('raw', '1', stride, orientation)
    else:
        #TODO: Auto-convert? or fail?
        #image = image.convert('RGBA')
        #bitdepth = 32
        #stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        #pixels = image.tobytes('raw', 'BGRA', stride, orientation)
        raise ValueError('Unsupported image mode {0!r} for writing'.format(image.mode))

def run_tests():
    import PIL
    import PIL.Image
    import PIL.ImageOps
    needalpha = False
    orientation = 1
    imgpath = r"path\to\testimage.png"
    with PIL.Image.open(imgpath) as imgfile:
        image = rgba_image(imgfile, needalpha)
        hasalpha = image.mode == 'RGBA'
        mode = image.mode
        with memoryview(rgba_bytes(image, needalpha, orientation)):
    image.tobytes('raw', 'BGRA', stride, orientation)
image = image.crop(bbox)
            if image.mode != 'RGBA' and (image.mode.endswith('A') or image.mode.endswith('a')):
                image = image.convert('RGBA')
            elif image.mode == 'RGBA':
                bitdepth = 32
                stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
                pixels = image.tobytes('raw', 'BGRA', stride, orientation)
            elif image.mode == 'RGB':
                bitdepth = 24
                stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
                pixels = image.tobytes('raw', 'BGR', stride, orientation)
            elif image.mode == 'L':
                bitdepth = 8
                stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
                pixels = image.tobytes('raw', 'L', stride, orientation)
            elif image.mode == '1':
                bitdepth = 1
                stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
                pixels = image.tobytes('raw', '1', stride, orientation)
            else:
                #TODO: Auto-convert? or fail?
                #image = image.convert('RGBA')
                #bitdepth = 32
                #stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
                #pixels = image.tobytes('raw', 'BGRA', stride, orientation)
                raise ValueError('Unsupported image mode {0!r} for writing'.format(image.mode))



def pack_from_pixels(pixels:bytearray, sliceidx:int, slicelen:int, width:int, height:int, bpp:int, bpc:int, offsetx:int, offsety:int, fullwidth:int, fullheight:int, flip:bool):
    bytedepth  = bpp // 8 # only bpp 24,32 not supported by CS2
    stride     = (width * pixlen + 3) & ~0x3
    fullstride = (fullwidth * bytedepth + 3) & ~0x3

    colstart = offsetx
    colend   = offsetx + width
    rowstart = offsety + sliceidx
    rowend   = offsety + sliceidx + slicelen

    fullstart = rowstart * fullstride + colstart * bytedepth
    fullend   = rowend   * fullstride + colend   * bytedepth
    del colstart, colend, rowstart, rowend

    if not flip:
        normyrange = range(fullstart, fullend, fullstride)
        deltayrange = range(fullstart + stride, fullend, fullstride)
        deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
    else:
        normyrange = range(fullend - stride, fullstart - 1, fullstride)
        deltayrange = range(fullend - stride - stride, fullstart - 1, -fullstride)
        deltaxrange = range(fullend - stride + bytedepth, fullend, 1)

    # buflen = len(buffer)
    # sectlen = buflen // 4
    sectlen = len(buffer) // 4
    # sect0, sect1, sect2, sect3 = sects = range(0, sectlen * 4, sectlen)

    abstable = make_abs_tables()[0]  # fwd abstable
    #table0, table1, table2, table3 = make_weight_tables()
    #block0, block1, block2, block3 = blocks = [None] * 4
    block0 = block1 = block2 = block3 = None

    
    abstable = bytes(make_abs_tables()[0])  # fwd abstable

    stride = ((width * bpp + 7) // 8 + 3) & ~0x3
    bufstart = hgslice.index * stride
    bufend = (hgslice.index + hgslice.length) * stride

    sectlen = slicelen * stride // 4
    blocks = bytearray(sectlen * 4)
    block0 = block1 = block2 = block3 = None
    try:
        buffer = memoryview(buffer)
        abstable = memoryview(abstable)
        # for i, sect in enumerate(range(0, sectlen * 4, sectlen)):
        #     blocks[i] = buffer[sect:sect + sectlent]
        # block0, block1, block2, block3 = blocks
        block0 = buffer[sect0:sect0 + sectlen]
        block1 = buffer[sect1:sect1 + sectlen]
        block2 = buffer[sect2:sect2 + sectlen]
        block3 = buffer[sect3:sect3 + sectlen]
        buffer = buffer.release()

        # normalize pixel buffer into data blocks
        i = 0
        for y0 in normyrange:
            for yx in range(y0, y0 + stride, 4):
                # val = unpack_from('<I', pixels, src)
                # b = abstable[pixels[xy    ]]
                # v3 = ((b     ) & 0x3)
                # v2 = ((b >> 2) & 0x3)
                # v1 = ((b >> 4) & 0x3)
                # v0 = ((b >> 6) & 0x3)
                # b = abstable[pixels[xy + 1]]
                # v3 |= ((b << 2) & 0xc)
                # v2 |= ((b     ) & 0xc)
                # v1 |= ((b >> 2) & 0xc)
                # v0 |= ((b >> 4) & 0xc)
                # b = abstable[pixels[xy + 2]]
                # v3 |= ((b << 4) & 0x30)
                # v2 |= ((b << 2) & 0x30)
                # v1 |= ((b     ) & 0x30)
                # v0 |= ((b >> 2) & 0x30)
                # b = abstable[pixels[xy + 3]]
                # v3 |= ((b << 6) & 0xc0)
                # v2 |= ((b << 4) & 0xc0)
                # v1 |= ((b << 2) & 0xc0)
                # v0 |= ((b     ) & 0xc0)
                # v0 = v1 = v2 = v3 = 0
                #m = 0x3
                for j in range(0, 8, 2):
                    #for j in range(0, 8, 2): # section mask to byte
                    #m = 0x3 << j
                    b = abstable[pixels[xy]]
                    xy += 1
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


        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] + pixels[x0 - bytedepth]) & 0xff
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1)):
                pixels[yx] = (pixels[yx] + pixels[yx - fullstride]) & 0xff

    finally:
        # if isinstance(pixels, memoryview):
        #     pixels.release()
        # if isinstance(buffer, memoryview):
        #     buffer.release()
        for memitem in (block0, block1, block2, block3, buffer, pixels, abstable): #blocks:
            if isinstance(memitem, memoryview):
                memitem.release()

    # first loop through the entire delta slice and perform absolute transform
    for i in range(bufstart, bufend, 1):
        delta[i] = abstable[delta[i]]

    # Iterate through each section one at a time, each pass
    # through delta encodes a different mask (section/block) of bytes
    i = 0
    # Section masks: [0xc0c0c0c0, 0x30303030, 0x0c0c0c0c, 0x03030303]
    for k in range(6, -1, -2): # current section
        src = bufstart
        for i in range(sectlen): # section iteration
            idx = 0
            val = unpack_from('<I', delta, src)
            b0 = b1 = b2 = b3 = 0
            m = 0x3
            for j in range(0, 8, 2): # section mask to byte
                m = 0x3 << j
                b = abstable[delta[]
                b3 |= (b     ) & m
                b >>= 2
                b2 |= (b >> 2) & m
                b >>= 2
                b1 |= (b >> 4) & m
                b >>= 2
                b0 |= (b >> 6) & m
                m <<= 2
                b = ((((delta[src    ] >> k) & 0x3)) |
                     (((delta[src + 1] >> k) & 0x3) << 2) |
                     (((delta[src + 2] >> k) & 0x3) << 4) |
                     (((delta[src + 3] >> k) & 0x3) << 6))
                idx |= (((delta[src] >> k) & 0x3) << j)
                src += 1
            blocks[i] = idx & 0xff
            dst += 1
    
    return blocks

def unpack_data(data:bytes, cmd:bytes) -> bytearray:
    buffer = None
    try:
        data = memoryview(zlib.decompress(data))
        cmd  = memoryview(zlib.decompress(cmd))

        bifbuf = BitBuffer(cmd, len(cmd))
        copyflag = bitbuf.read_flag() # is first run non-zero data?
        buflen = bitbuf.read_eliasgamma() # length of output buffer (usually height x stride)
        buffer = memoryview(bytearray(buflen))

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

        return  _buffer # underlying bytearray
    finally:
        # if isinstance(data, memoryview):
        #     data.release()
        # if isinstance(cmd, memoryview):
        #     cmd.release()
        # if isinstance(buffer, memoryview):
        #     buffer.release()
        for memitem in (data, cmd, buffer):
            if isinstance(memitem, memoryview):
                memitem.release()
    
def unpack_into_pixels(buffer:bytes, pixels:bytearray, sliceidx:int, slicelen:int, width:int, height:int, bpp:int, bpc:int, offsetx:int, offsety:int, fullwidth:int, fullheight:int, flip:bool):
    bytedepth  = bpp // 8 # only bpp 24,32 not supported by CS2
    stride     = (width * pixlen + 3) & ~0x3
    fullstride = (fullwidth * bytedepth + 3) & ~0x3

    colstart = offsetx
    colend   = offsetx + width
    rowstart = offsety + sliceidx
    rowend   = offsety + sliceidx + slicelen

    fullstart = rowstart * fullstride + colstart * bytedepth
    fullend   = rowend   * fullstride + colend   * bytedepth
    del colstart, colend, rowstart, rowend

    if not flip:
        normyrange = range(fullstart, fullend, fullstride)
        deltayrange = range(fullstart + stride, fullend, fullstride)
        deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
    else:
        normyrange = range(fullend - stride, fullstart - 1, fullstride)
        deltayrange = range(fullend - stride - stride, fullstart - 1, -fullstride)
        deltaxrange = range(fullend - stride + bytedepth, fullend, 1)

    # buflen = len(buffer)
    # sectlen = buflen // 4
    sectlen = len(buffer) // 4
    # sect0, sect1, sect2, sect3 = sects = range(0, sectlen * 4, sectlen)

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
                       table2[block2[i]] | table3[block3[i]]))
                i += 1

                pixels[yx    ] = invtable[(val      ) & 0xff]
                pixels[yx + 1] = invtable[(val >>  8) & 0xff]
                pixels[yx + 2] = invtable[(val >> 16) & 0xff]
                pixels[yx + 3] = invtable[(val >> 24)       ]


        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] + pixels[x0 - bytedepth]) & 0xff
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1)):
                pixels[yx] = (pixels[yx] + pixels[yx - fullstride]) & 0xff

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

def unpack_data(self, data:bytes, cmd:bytes) -> bytearray:
    data = zlib.decompress(data)
    cmd  = zlib.decompress(cmd)
    bifbuf = BitBuffer(cmd, len(cmd))

    copyflag = bitbuf.read_flag() # is first run non-zero data?
    buflen = bitbuf.read_eliasgamma() # length of output buffer (usually height x stride)
    sectlen = buflen // 4
    blocks = tuple(bytes(sectlen) for _ in range(4))
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

    def unpack_slice(self, hgslice:HgSlice, hgdata:HgData, pixels:bytearray, stdinfo:StandardInfo, options:HgOptions=HgOptions(0)):
        stride = stdinfo.stride
        bytedepth = stdinfo.bytedepth
        channeldepth = stdinfo.channeldepth
        width = stdinfo.width
        height = stdinfo.height
        canvasx = stdinfo.canvasx
        canvasy = stdinfo.canvasy
        canvaswidth = stdinfo.canvaswidth
        canvasheight = stdinfo.canvasheight
        #
        #
        data = hgdata.data #TODO: zlib.decompress()
        cmd  = hgdata.cmd  #TODO: zlib.decompress()
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
        
        table0, table1, table2, table3 = tables = make_weight_tables()
        invtable = make_abs_tables()[1]  # inv abstable

        pixlen = bpp // 8 # only bpp 24,32 not supported by CS2
        stride = (width * pixlen + 3) & ~0x3
        stridefull = (canvaswidth * pixlen + 3) & ~0x3
        stridestart = (canvasx * pixlen + 3) & ~0x3
        #
        slicestride = stride
        slicestart = hgslice.index * stridefull + stridestart
        sliceend = (hgslice.index + hgslice.length) * stridefull + stridestart
        sliceheight = hgslice.length
        #
        sliceidx = hgslice.index
        slicelen = hgslice.len

        bytedepth  = bpp // 8 # only bpp 24,32 not supported by CS2
        stride     = (width * pixlen + 3) & ~0x3
        fullstride = (canvaswidth * bytedepth + 3) & ~0x3

        colstart = canvasx
        colend   = canvasx + width
        rowstart = canvasy + sliceidx
        rowend   = canvasy + sliceidx + slicelen

        fullstart = rowstart * fullstride + colstart * bytedepth
        fullend   = rowend   * fullstride + colend   * bytedepth


        outstart = (canvasy + sliceidx) * stridefull + canvasx * pixlen
        outend = outstart + slicelen * stridefull + stride

        canvasidx
        #
        sectlen = buflen // 4

        block0, block1, block2, block3 = blocks = tuple(buffer[i:i + sectlen] for i in range(0, buflen, sectlen))
        
        # bytedepth = (bpp + 7) // 8
        # stride = ((width * bpp + 7) // 8 + 3) & ~0x3
        # stridefull = ((canvaswidth * bpp + 7) // 8 + 3) & ~0x3
        # stridestart = ((canvasx * bpp + 7) // 8 + 3) & ~0x3
        # strideend = stridestart + stride
        # #
        # slicestart = hgslice.index * stride
        # sliceend = (hgslice.index + hgslice.length) * stride
        # #
        # sectlen = buflen // 4
        # sect0, sect1, sect2, sect3 = range(slicestart, slicestart + sectlen * 4, sectlen)

        u4_s = struct.Struct('<I')


        stride = ((canvasx * bpp + 7) // 8 + 3) & ~0x3
        fullstride = ((canvasx * bpp + 7) // 8 + 3) & ~0x3
        
        stride = ((width * bpp + 7) // 8 + 3) & ~0x3
        bufstart = hgslice.index * stride
        #bufend = (hgslice.index + hgslice.length) * stride

        sectlen = hgslice.length * stride // 4
        #sect0, sect1, sect2, sect3 = range(bufstart, bufstart + sectlen * 4, sectlen)
        sect0, sect1, sect2, sect3 = range(0, sectlen * 4, sectlen)
        delta = bytearray(sectlen * 4)

        if not flip:
            normyrange = range(fullstart, fullend, fullstride)
            deltayrange = range(fullstart + stride, fullend, fullstride)
            deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
        else:
            normyrange = range(fullend - stride, fullstart - 1, fullstride)
            deltayrange = range(fullend - stride - stride, fullstart - 1, -fullstride)
            deltaxrange = range(fullend - stride + bytedepth, fullend, 1)
       
        # inverse normalize data blocks into pixel buffer
        i = 0
        for y0 in normyrange:
            for yx in range(y0, y0 + stride, 4):
                val = (table0[block0[i]] | table1[block1[i]] |
                       table2[block2[i]] | table3[block3[i]]))
                i += 1

                pixels[yx    ] = invtable[(val      ) & 0xff]
                pixels[yx + 1] = invtable[(val >>  8) & 0xff]
                pixels[yx + 2] = invtable[(val >> 16) & 0xff]
                pixels[yx + 3] = invtable[(val >> 24)       ]
        

        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] + pixels[x0 - bytedepth]) & 0xff
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1)):
                pixels[yx] = (pixels[yx] + pixels[yx - fullstride]) & 0xff


        u4_1 = struct.Struct('<I')
        u1_packin = u4_1.pack_into

        
        bi = 0
        yrange = range(fullstart, fullend, fullstride)
        if flip: yrange = reversed(yrange)
        for yj in (reversed(yrange) if flip else yrange): #range(slicestart, sliceend, stridefull):
            for xj in range(yj, yj + stride, 4):
                u1_packin(pixels, xj, (table0[block0[i]] | table1[block1[i]] |
                                       table2[block2[i]] | table3[block3[i]]))
                bi += 1
        
        # undelta RGB(A) channels of each previous pixel in first row
         
        for x1, x0 in zip(range(bufstart + bytedepth, bufstart + stride, 1),
                          range(bufstart, bufstart + stride - bytedepth, 1)):
            delta[x1] = (delta[x1] + delta[x0]) & 0xff
        yranges   (range(fullstart + stride, fullend, fullstride),
                   range(fullstart, fullend - stride, fullstride)):
        if flip: = tuple(reversed(yr) for yr in yranges)
        yranges = (range(fullstart + bytedepth, fullstart + stride, fullstride),
                   range(fullstart, fullstart + stride - bytedepth, fullstride))
        if flip: = tuple(reversed(yr) for yr in yranges)

        yrange1 = range(bufstart + bytedepth, bufstart + stride, 1)
        yrange0 = range(bufstart, bufstart + stride - bytedepth, 1)
        if flip: yrange1, yrange0 = reversed(yrange1), reversed(yrange0)
        yrange = zip(yrange1, yrange0)
        # undelta RGB(A) channels of each previous stride in all but first row
        for y1, y0 in zip(range(bufstart + stride, bufstart, 1),
                          range(bufstart, bufstart - stride, 1)):
            delta[y1] = (delta[y1] + delta[y0]) & 0xff


        # if flip:
        #     yrange = range(sliceend - 1, slicestart - 1, -stridefull)
        # else:
        # yrange = range(slicestart, sliceend, stridefull)
        # if flip: yrange = reversed(yrange)

        i = 0
        yrange = range(slicestart, sliceend, stridefull)
        for yj in (reversed(yrange) if flip else yrange): #range(slicestart, sliceend, stridefull):
            for xj in range(yj, yj + stride, 4):
                val = (table0[block0[i]] | table1[block1[i]] |
                       table2[block2[i]] | table3[block3[i]]))
                i += 1

                delta[j    ] = invtable[(val      ) & 0xff]
                delta[j + 1] = invtable[(val >>  8) & 0xff]
                delta[j + 2] = invtable[(val >> 16) & 0xff]
                delta[j + 3] = invtable[(val >> 24)       ]
                u1_packin(pixels, xj, (table0[block0[i]] | table1[block1[i]] |
                                       table2[block2[i]] | table3[block3[i]]))
        
            xranges = (range(bufstart + bytedepth, bufstart + stride, 1),
                       range(bufstart, bufstart + stride - bytedepth, 1))
        if not flip:
            normyrange = range(fullstart, fullend, fullstride)
            deltayrange = range(fullstart + stride, fullend, fullstride)
            deltaxrange = range(fullstart + bytedepth, fullstart + stride, 1)
        else:
            normyrange = range(fullend - stride, fullstart - 1, fullstride)
            deltayrange = range(fullend - stride - stride, fullstart - 1, -fullstride)
            deltaxrange = range(fullend - stride + bytedepth, fullend, 1)
       
        # inverse normalize data blocks into pixel buffer
        i = 0
        for y0 in normyrange:
            for yx in range(y0, y0 + stride, 4):
                val = (table0[block0[i]] | table1[block1[i]] |
                       table2[block2[i]] | table3[block3[i]]))
                i += 1

                pixels[yx    ] = invtable[(val      ) & 0xff]
                pixels[yx + 1] = invtable[(val >>  8) & 0xff]
                pixels[yx + 2] = invtable[(val >> 16) & 0xff]
                pixels[yx + 3] = invtable[(val >> 24)       ]
        

        # undelta RGB(A) channels of each previous pixel in first row
        for x0 in deltaxrange:
            pixels[x0] = (pixels[x0] + pixels[x0 - bytedepth]) & 0xff
        
        # undelta RGB(A) channels of each previous stride in all but first row
        for y0 in deltayrange:
            for yx in range(y0, y0 + stride, 1)):
                pixels[yx] = (pixels[yx] + pixels[yx - fullstride]) & 0xff



    #
    ### INITIALIZATION TABLES ###
    #

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
    #sect0, sect1, sect2, sect3 = range(bufstart, bufstart + sectlen * 4, sectlen)
    sect0, sect1, sect2, sect3 = range(0, sectlen * 4, sectlen)
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
    for xi, xj in zip(range(bufstart + bytedepth, bufend + stride, 1),
                      range(bufstart, bufend + stride - bytedepth, 1)):
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

del enum, collections  # only used during type declarations
del Iterable, Iterator, List, Optional, NoReturn, Tuple, Type, Union  # only used during declarations
