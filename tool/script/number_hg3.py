#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Generate template HG-3 images with numbered frames (great for testing)

Requires the PIL (pillow) image library package

NOTE: This expects the this script file to keep its current relative path
to the catsys package. (i.e. /tool/script/<script>.py -> ../../src -> /src(/catsys))
"""

__version__ = '1.0.0'
__date__    = '2020-10-26'
__author__  = 'Robert Jordan'

__all__ = ['NumberDrawer', 'Hg3Writer', 'write_range']

#######################################################################################

import collections, io, os, struct, sys, zlib
import inspect  # needed to simplify importing catsys
                # and to get same-directory image filepath
from typing import Type

# PIL (pillow) Image library imports
import PIL
import PIL.Image
import PIL.ImageOps

##src: <https://stackoverflow.com/a/18489147>
THISDIR = os.path.dirname(inspect.getsourcefile(lambda:0))
NUMBERS_IMAGE_PATH = os.path.join(THISDIR, r"number_hexdigits_0-9a-fA-F.png")

# catsys library imports
try:
    from catsys.imgs import _hg
except ImportError:
    print('# add catsys package to module path')
    sys.path.append(os.path.realpath(os.path.join(THISDIR, '../../src')))
    from catsys.imgs import _hg

## PREDECLARE TYPES ##

PIL_Image = Type['PIL.Image']



class NumberDrawer(object):
    NUMERALS = '0123456789abcdef'
    #
    def __init__(self, fontpath:str, charset:str, crop:tuple, spacing:int=0, offset:tuple=(0,0), back:tuple=(0,0,0,0), fill:tuple=(0,0,0,255)):
        #
        if not isinstance(crop, tuple): crop = (crop, crop)
        elif len(crop) == 1: crop *= 2
        #
        if not isinstance(spacing, tuple): spacing = (spacing, 0)
        elif len(spacing) == 1: spacing *= 2
        #
        if not isinstance(offset, tuple): offset = (offset, offset)
        elif len(offset) == 1: offset *= 2
        #
        if back is None: back = (0, 0, 0, 0)
        elif not isinstance(back, tuple): back = (back, back, back, 255)
        elif len(back) == 3: back += (255,)
        #
        if fill is None: fill = (0, 0, 0, 255)
        elif not isinstance(fill, tuple): fill = (fill, fill, fill, 255)
        elif len(fill) == 3: fill += (255,)
        #
        self.crop = crop
        self.spacing = spacing
        self.offset = offset
        self.back = back
        self.fill = fill
        #
        self.font_image = image = PIL.Image.open(fontpath)
        self.charset = charset
        # self.digits = [None] * 16
        print(crop, spacing, offset)
        self.digits = dict((c, image.crop((
                offset[0] + i * (crop[0] + spacing[0]),
                offset[1] + i * (spacing[1]),
                offset[0] + i * (crop[0] + spacing[0]) + crop[0],
                offset[1] + i * (spacing[1]) + crop[1]
            ))) for i,c in enumerate(charset))
        print([(
                offset[0] + i * (crop[0] + spacing[0]),
                offset[1] + i * (spacing[1]),
                offset[0] + i * (crop[0] + spacing[0]) + crop[0],
                offset[1] + i * (spacing[1]) + crop[1]
            ) for i in range(16 + 6)])
    #
    @classmethod
    def baseN(cls, num:int, base:int, numerals='0123456789abcdefghijklmnopqrstuvwxyz'):
        ##src: <https://stackoverflow.com/a/2267428>
        return ((num == 0) and numerals[0]) or (cls.baseN(num // base, base, numerals).lstrip(numerals[0]) + numerals[num % base])
    #
    def scale(self, img:PIL_Image, scale:int) -> PIL_Image:
        ##src: <https://gist.github.com/alexras/4720743>
        if scale is None or scale == 1:
            return img
        #
        return img.resize((img.size[0] * scale, img.size[1] * scale), PIL.Image.NEAREST)
    #
    def colorize(self, img:PIL_Image, back:tuple, fill:tuple) -> PIL_Image:
        ##src: <https://stackoverflow.com/a/3766325>
        if back is None and fill is None:
            return img
        #
        if back is None: pass #back = (0, 0, 0, 0)
        elif not isinstance(back, tuple): back = (back, back, back, 255)
        elif len(back) == 3: back += (255,)
        #
        if fill is None: pass #fill = (0, 0, 0, 255)
        elif not isinstance(fill, tuple): fill = (fill, fill, fill, 255)
        elif len(fill) == 3: fill += (255,)
        #
        pixdata = img.load()
        #
        for y in range(img.size[1]):
            for x in range(img.size[0]):
                if pixdata[x, y] == self.back:
                    pixdata[x, y] = back
                elif pixdata[x, y] == self.fill:
                    pixdata[x, y] = fill
        return img
    #
    def draw_number(self, num:int, base:int=..., *, space:int=1, border:tuple=(0,0,0,0), scale:int=None, back:tuple=None, fill:tuple=None) -> PIL_Image:
        if isinstance(num, str) and base is Ellipsis:
            numstr = base.format(num)
        elif isinstance(base, str):
            # format string
            numstr = base.format(num)
        elif isinstance(base, int):
            if not isinstance(base, int):
                raise TypeError('{0.__class__.__name__} draw_number() argument \'base\' must be int type, not {1.__class__.__name__}'.format(self, base))
            elif not (1 < base <= 16):
                raise ValueError('{0.__class__.__name__} draw_number() argument \'base\' must be between 2-16, not {1!r}'.format(self, base))
            #
            numstr = self.baseN(num, base)
        #
        print('numstr={0!r}'.format(numstr))
        #
        if not isinstance(space, tuple): space = (space, 0)
        elif len(space) == 1: space *= 2
        #
        if not isinstance(border, tuple): border = (border, border, border, border)
        elif border and len(border) <= 2: border *= 4 // len(border)
        #
        x = border[0]
        y = border[1]
        width  = (x + border[2] + self.crop[0]) + max(0, len(numstr) - 1) * (self.crop[0] + space[0])
        height = (y + border[3] + self.crop[1]) #+ max(0, (len(numstr) - 1) * (space[1])
        #
        img = PIL.Image.new('RGBA', (width, height)) #, fill)
        #
        for i,d in enumerate(numstr):
            # img.paste(self.digits[self.charset.index(d)], (x, y))
            if d != ' ' and d != '\t':
                img.paste(self.digits[d], (x, y))
            x += self.crop[0] + space[0]
            y += space[1]
        #
        img = self.scale(img, scale)
        img = self.colorize(img, back, fill)
        return img


_NumberDrawer_inst = NumberDrawer(NUMBERS_IMAGE_PATH, "0123456789abcdefABCDEF", (5,7), (1, 0), (0,0), back=(0, 0, 0, 0), fill=(0, 0, 0, 255))
drawnum = _NumberDrawer_inst.draw_number
# drawnum(124, '{0:04X}', border=(2,10), scale=4, fill=(0, 20, 40), back=(255, 255, 255)).show()

## PIL HELPERS ##

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
    return image.tobytes('raw', mode, stride, orientation), image.mode


## HG-3 WRITER ##

class Hg3Writer:
    def __init__(self, filename:str, *, header_size:int=12, version:int=0x300):
        self.signature = b'HG-3'
        self.header_size = header_size
        self.version = version
        self.filename = filename
        self.stream = None
        self.lasttag = -1
        self.lastframe = -1
    #
    # START/END IMAGE:
    #
    def start(self):
        self.stream.write(struct.pack('<4s I I', self.signature, self.header_size, self.version))
        self.stream.seek(self.header_size - 12, io.SEEK_CUR)
    def end(self):
        self.end_frame(True)
    #
    # START/END FRAME:
    #
    def start_frame(self, frameid:int):
        self.end_frame(False)
        self.lastframe = self.stream.tell()
        # print('start_frame -> write at offset {0:d}'.format(self.stream.tell()))
        self.stream.write(struct.pack('<I I', 0, frameid))
    def end_frame(self, islast:bool=False):
        self.stream.flush()
        nextoffset = self.stream.tell()
        self.end_tag(True)
        if self.lastframe != -1:
            # nextoffset = (nextoffset + 7) & ~0x7
            self.stream.seek(self.lastframe)
            if not islast:
                nextoffset = (nextoffset + 7) & ~0x7
                offset = nextoffset - self.lastframe
            else:
                offset = 0
            #offset = nextoffset - self.lastframe
            # print('end_frame -> write at offset {0:d}'.format(self.stream.tell()))
            self.stream.write(struct.pack('<I', offset))
            self.stream.flush()
            self.stream.seek(nextoffset)
        self.lastframe = -1 #if islast else nextoffset
    #
    # START/END TAG:
    #
    def start_tag(self, tagname:str):
        self.end_tag(False)
        self.lasttag = self.stream.tell()
        if isinstance(tagname, str):
            tagname = tagname.encode('ascii')
        # print('start_tag -> write at offset {0:d}'.format(self.stream.tell()))
        self.stream.write(struct.pack('<8s I I', tagname, 0, 0))
    def end_tag(self, islast:bool=False):
        self.stream.flush()
        nextoffset = self.stream.tell()
        if self.lasttag != -1:
            self.stream.seek(self.lasttag + 8)
            length = nextoffset - self.lasttag - 16
            if not islast:
                #nextoffset = (nextoffset + 7) & ~0x7
                offset = nextoffset - self.lasttag
            else:
                offset = 0
            # print('end_tag -> write at offset {0:d}'.format(self.stream.tell()))
            self.stream.write(struct.pack('<I I', offset, length))
            self.stream.flush()
            self.stream.seek(nextoffset)
        self.lasttag = -1 #if islast else nextoffset
    #
    # WRITE FRAME:
    #
    def write_frame(self, frameid:int, image, flip:bool=False):
        self.start_frame(frameid)
        #
        #if not isinstance(image, (bytes,bytearray,memoryview)):
        image = rgba_image(image)
        pixels, mode = rgba_bytes(image, 1 if flip else -1) # flipping is default behavior so 1 for flip=True
        bpp = 32 if mode == 'RGBA' else 24
        depth = 0
        width, height = image.size
        canvasx, canvasy = (0, 0)
        canvaswidth, canvasheight = image.size
        originx, originy = (0, 0)
        # self.write_tag('stdinfo', struct.pack('<II HH ii II ?xxx ii', width, height, bpp, depth, canvasx, canvasy, canvaswidth, canvasheight, bpp == 32, originx, originy))
        # else:
        #     pixels = image
        
        self.write_stdinfo(width, height, bpp, depth, canvasx, canvasy, canvaswidth, canvasheight, bpp == 32, originx, originy)
        data, cmd = _hg.encode_slice(_hg.HgSlice(0, height), pixels, width, height, bpp)
        self.write_img(0, 0, height, data, cmd)

        self.write_cptype(0)
    #
    # WRITE TAG:
    #
    def write_tag(self, name, data:bytes):
        self.start_tag(name)
        self.stream.write(data)
    #
    # WRITE TAGS CUSTOM:
    #
    def write_stdinfo(self, width:int, height:int, bpp:int, depth:int=..., canvasx:int=..., canvasy:int=..., canvaswidth:int=..., canvasheight:int=..., transparent:bool=..., originx:int=..., originy:int=...):
        if depth is Ellipsis or depth == 8: depth = 0
        if canvasx is Ellipsis: canvasx = 0
        if canvasy is Ellipsis: canvasy = 0
        if canvaswidth is Ellipsis: canvaswidth = width - canvasx
        if canvasheight is Ellipsis: canvasheight = height - canvasy
        if transparent is Ellipsis: transparent = (bpp == 32)
        if originx is Ellipsis: originx = 0
        if originy is Ellipsis: originy = 0
        self.write_tag('stdinfo', struct.pack('<II HH ii II ?xxx ii', width, height, bpp, depth, canvasx, canvasy, canvaswidth, canvasheight, transparent, originx, originy))
    def write_img(self, imgid:int, index:int, length:int, data:bytes, cmd:bytes):
        # Image slice
        datalen, cmdlen = (len(data), len(cmd))
        #FIXME: Best compression (9) probably isn't required
        data = zlib.compress(data, level=9)
        cmd = zlib.compress(cmd, level=9)
        self.start_tag('img{0:04d}'.format(imgid))
        self.stream.write(struct.pack('<II II II', index, length, len(data), datalen, len(cmd), cmdlen))
        self.stream.write(data)
        self.stream.write(cmd)
    def write_ats(self, atsid:int, x:int, y:int, width:int, height:int, rawcolor:int):
        # Image attribute
        self.write_tag('ats{0:04d}'.format(atsid), struct.pack('<ii II I', x, y, width, height, rawcolor))
    def write_imgmode(self, imgmode:bool=False):
        # False is default, and not required
        self.write_tag('imgmode', struct.pack('<?xxx', imgmode))
    def write_cptype(self, cptype:int=0):
        # Always appears at end of tags for frame (0 is default, but is it required?)
        self.write_tag('cptype', struct.pack('<I', cptype))
    #
    # OPEN/CLOSE:
    #
    @property
    def is_open(self):
        return bool(self.stream)

    def open(self):
        self.close()
        if not self.stream:
            self.stream = open(self.filename, 'wb+')
            self.start()
            return True
        return False
    def close(self):
        if self.stream:
            self.end()
            stream = self.stream
            if hasattr(self.stream, 'flush'):
                self.stream.flush()
            if hasattr(self.stream, 'close'):
                self.stream.close()
            self.stream = None
            return stream
        return self.stream
    
    def __enter__(self):
        self.open()
        return self
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()


def write_range(filename:str, startnum, count, base='{0:4d}', startid=0, stopid=None, *, border=(2,10), scale=4, fill=(0, 20, 40, 255), back=(255, 255, 255, 255), flip:bool=False):
    with Hg3Writer(filename) as hg3:
        for i in range(count):
            if i + 1 == count and stopid is not None:
                frameid = stopid
            else:
                frameid = startid + i
            framenum = startnum + i
            frameimage = drawnum(framenum, base, border=border, scale=scale, fill=fill, back=back)
            hg3.write_frame(frameid, frameimage, flip=flip)


def main(argv:list=None) -> int:
    import argparse
    parser = argparse.ArgumentParser(description='Draw multiframe HG-3 images with rendered numbers', add_help=True)

    parser.add_argument('filename', metavar='FILENAME', action='store', help='Output filename')
    parser.add_argument('startnum', metavar='START', action='store', type=int, default=0, help='Starting number (default=0)')
    parser.add_argument('count', metavar='COUNT', action='store', type=int, help='Number of frames')
    parser.add_argument('-i', dest='startid', action='store', type=int, required=False, default=0, help='First sequential ID')
    parser.add_argument('-I', dest='stopid', action='store', type=int, required=False, default=None, help='Last non-sequential ID (optional)')
    parser.add_argument('-b', dest='base', action='store', required=False, default='{0:4d}', help='Number base or format')

    args = parser.parse_args(argv)
    print(args)
    print(args.filename)
    write_range(args.filename, args.startnum, args.count, args.base, args.startid, args.stopid)
    return 0


if __name__ == '__main__':
    # import sys
    # write_range(sys.argv[1:])
    exit(main())
