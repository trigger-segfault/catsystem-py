#!C:/Env/Lib/WPy64-3680/python-3.6.8.amd64/python.exe
# -*- coding: utf-8 -*-

import os, sys, struct, io, zlib, collections, re, enum
import PIL
import PIL.Image
import PIL.ImageOps

#table4 bits: 0000 0011
#table3 bits: 0000 1100
#table2 bits: 0011 0000
#table1 bits: 1100 0000

#### CLASSES / STRUCTS ####


### BitBuffer ###

class BitBuffer(object):
    """
    Elias camma code bit buffer reader and writer.
    """
    __slots__ = ('buf', 'index', 'bit')
    def __init__(self, buf):
        self.buf = buf
        self.index = 0
        self.bit = 0
    #
    # REPR:
    #
    def __repr__(self):
        return '<{0.__class__.__name__}: index={0.index} bit={0.bit} {0.buf.__class__.__name__} @ 0x{1:016X}>'.format(self, id(self.buf))
    #
    # READ:
    #
    def get_bit(self):
        """
        Read the next bit in the buffer. Returns True or False
        """
        if self.bit > 7:
            self.bit = 0
            self.index += 1
        
        self.bit += 1
        return bool((self.buf[self.index] >> (self.bit - 1)) & 0x1)
    
    def get_elias_gamma(self):
        """
        Read the next Elias gamma value in the buffer.
        """
        digits = 0
        while not self.get_bit():
            digits += 1
        
        value = 1 << digits
        while digits:
            digits -= 1 # subtract before shift
            if self.get_bit():
                value |= (1 << digits)
        
        return value
    #
    # WRITE:
    #
    def set_bit(self, flag):
        """
        Write the next bit to the buffer.
        (expands the buffer if bytearray is too small)
        """
        if self.index == 0 and self.bit == 0 and self.index >= len(self.buf):
            self.buf.append(0)
        elif self.bit > 7:
            self.bit = 0
            self.index += 1
            self.buf.append(0)
        
        self.bit += 1
        if flag:
            self.buf[self.index] |= (0x1 << (self.bit - 1))
        else:
            self.buf[self.index] &= (0x1 << (self.bit - 1)) ^ 0xff
    
    def set_elias_gamma(self, value):
        """
        Write the next Elias gamme value to the buffer.
        (expands the buffer if bytearray is too small)
        """
        if not (0 < value <= 0xffffffff):
            raise ValueError('{0.__class__.__name__} set_elias_gamma() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
            self.set_bit(False)
        
        self.set_bit(True)
        
        while digits > 0:
            digits -= 1
            self.set_bit(bool((value >> digits) & 0x1))
    #
    # STATIC HELPERS:
    #
    @staticmethod
    def elias_gamma_size(value):
        """
        Returns the size in bits of the value, when converted to Elias gamma universal code.
        """
        if not (0 < value <= 0xffffffff):
            raise ValueError('{0.__class__.__name__} elias_gamma_size() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
        
        # 1 bit minimum plus 2 bits per digit
        return digits * 2 + 1


class BitBuffer(object):
    """
    Elias camma code bit buffer reader and writer.
    """
    __slots__ = ('buf', 'idx', 'bit')
    def __init__(self, buf):
        self.buf = buf
        self.idx = 0
        self.bit = 0
    #
    # REPR:
    #
    def __repr__(self):
        return '<{0.__class__.__name__}: idx={0.idx} bit={0.bit} {0.buf.__class__.__name__} @ 0x{1:016X}>'.format(self, id(self.buf))
    #
    # READ:
    #
    def get_bit(self):
        """
        Read the next bit in the buffer. Returns True or False
        """
        if self.bit > 7:
            self.bit = 0
            self.idx += 1
        
        self.bit += 1
        return (self.buf[self.idx] >> (self.bit - 1)) & 0x1
    
    def get_elias_gamma(self):
        """
        Read the next Elias gamma value in the buffer.
        """
        digits = 0
        while not self.get_bit():
            digits += 1
        
        value = 1 << digits
        while digits:
            digits -= 1 # subtract before shift
            if self.get_bit():
                value |= (1 << digits)
        
        return value
    #
    # WRITE:
    #
    def set_bit(self, flag):
        """
        Write the next bit to the buffer.
        (expands the buffer if bytearray is too small)
        """
        if self.idx == 0 and self.bit == 0 and self.idx >= len(self.buf):
            self.buf.append(0)
        elif self.bit > 7:
            self.bit = 0
            self.idx += 1
            self.buf.append(0)
        
        self.bit += 1
        if flag:
            self.buf[self.idx] |= (0x1 << (self.bit - 1))
        else:
            self.buf[self.idx] &= (0x1 << (self.bit - 1)) ^ 0xff
    
    def set_elias_gamma(self, value):
        """
        Write the next Elias gamme value to the buffer.
        (expands the buffer if bytearray is too small)
        """
        # if not (0 < value <= 0xffffffff):
        #     raise ValueError('{0.__class__.__name__} set_elias_gamma() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        b = self.bit
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
            self.set_bit(0)
        
        self.set_bit(1)
        
        while digits > 0:
            digits -= 1
            self.set_bit((value >> digits) & 0x1)
    #
    # STATIC HELPERS:
    #
    @staticmethod
    def elias_gamma_size(value):
        """
        Returns the size in bits of the value, when converted to Elias gamma universal code.
        """
        if not (0 < value <= 0xffffffff):
            raise ValueError('{0.__class__.__name__} elias_gamma_size() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
        
        # 1 bit minimum plus 2 bits per digit
        return digits * 2 + 1

class BitBuffer(object):
    """
    Elias camma code bit buffer reader and writer.
    """
    __slots__ = ('buf', 'idx', 'bit')
    def __init__(self, buf):
        self.buf = buf
        self.idx = 0
        self.bit = 0
    #
    # REPR:
    #
    def __repr__(self):
        return '<{0.__class__.__name__}: idx={0.idx} bit={0.bit} {0.buf.__class__.__name__} @ 0x{1:016X}>'.format(self, id(self.buf))
    #
    # READ:
    #
    def get_bit(self):
        """
        Read the next bit in the buffer. Returns True or False
        """
        if self.bit > 7:
            self.bit = 0
            self.idx += 1
        
        self.bit += 1
        return (self.buf[self.idx] >> (self.bit - 1)) & 0x1
    
    def get_elias_gamma(self):
        """
        Read the next Elias gamma value in the buffer.
        """
        digits = 0
        if self.bit > 7:
            self.bit = 0
            self.idx += 1
        
        self.bit += 1
        while not ((self.buf[self.idx] >> (self.bit - 1)) & 0x1):
            digits += 1
            if self.bit > 7:
                self.bit = 0
                self.idx += 1
            
            self.bit += 1
        
        value = 1 << digits
        while digits:
            digits -= 1
            if self.bit > 7:
                self.bit = 0
                self.idx += 1
            
            self.bit += 1
            if (self.buf[self.idx] >> (self.bit - 1)) & 0x1:
                value |= (1 << digits)

        # self.bit += 1
        # digits = 0
        # while not self.get_bit():
        #     digits += 1
        
        # value = 1 << digits
        # while digits:
        #     digits -= 1 # subtract before shift
        #     if self.get_bit():
        #         value |= (1 << digits)
        
        return value
    #
    # WRITE:
    #
    def set_bit(self, flag):
        """
        Write the next bit to the buffer.
        (expands the buffer if bytearray is too small)
        """
        if self.idx == 0 and self.bit == 0 and self.idx >= len(self.buf):
            self.buf.append(0)
        elif self.bit > 7:
            self.bit = 0
            self.idx += 1
            self.buf.append(0)
        
        self.bit += 1
        if flag:
            self.buf[self.idx] |= (0x1 << (self.bit - 1))
        else:
            self.buf[self.idx] &= (0x1 << (self.bit - 1)) ^ 0xff
    
    def set_elias_gamma(self, value):
        """
        Write the next Elias gamme value to the buffer.
        (expands the buffer if bytearray is too small)
        """
        # if not (0 < value <= 0xffffffff):
        #     raise ValueError('{0.__class__.__name__} set_elias_gamma() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        b = self.bit
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
            self.set_bit(0)
        
        self.set_bit(1)
        
        while digits > 0:
            digits -= 1
            self.set_bit((value >> digits) & 0x1)
    #
    # STATIC HELPERS:
    #
    @staticmethod
    def elias_gamma_size(value):
        """
        Returns the size in bits of the value, when converted to Elias gamma universal code.
        """
        if not (0 < value <= 0xffffffff):
            raise ValueError('{0.__class__.__name__} elias_gamma_size() argument \'value\' must be positive integer between {2!r} and {3!r}, not {1!r}'.format(self, value, 1, 0xffffffff))
        
        digits = 0
        while value >> (digits + 1):
            digits += 1
        
        # 1 bit minimum plus 2 bits per digit
        return digits * 2 + 1


def process_rle(deltabuf):
    databuf, cmdbuf = bytearray(), bytearray()
    deltalen = len(deltabuf) 
    copyflag = (deltabuf[0] != 0)
    #
    cmd = BitBuffer(cmdbuf)
    #
    cmd.set_bit(copyflag)
    cmd.set_elias_gamma(deltalen)
    # 
    i = 0
    while i < deltalen:
        n = 1
        if copyflag: # copy fill
            while i+n < deltalen and deltabuf[i+n] != 0:
                n += 1
            #
            databuf.extend(deltabuf[i:i+n])
            #
        else: # zero fill
            while i+n < deltalen and deltabuf[i+n] == 0:
                n += 1
        #
        cmd.set_elias_gamma(n)
        #
        i += n
        copyflag = not copyflag
    #
    return databuf, cmdbuf

def inv_process_rle(databuf, cmdbuf):
    cmd = BitBuffer(cmdbuf)
    copyflag = cmd.get_bit()
    
    unrlelen = cmd.get_elias_gamma()
    unrlebuf = bytearray(unrlelen)
    
    i = 0
    d = 0
    while i < unrlelen:
        n = cmd.get_elias_gamma()
        
        if copyflag: # copy fill
            unrlebuf[i:i+n] = databuf[d:d+n]
            d += n
        #else: # zero fill
        #    unrlebuf[i:i+n] = bytes(n)
        
        i += n
        copyflag = not copyflag
    
    return unrlebuf


# Patent Claim: (1)
def init_norm_tables():
    tables = tuple([0] * 256 for _ in range(4))
    for i in range(256):
        val = ((i & 0xc0) << 18) | ((i & 0x30) << 12) | ((i & 0x0c) << 6) | (i & 0x03)
        tables[3][i] = val
        tables[2][i] = val << 2
        tables[1][i] = val << 4
        tables[0][i] = val << 6
    
    # Convert to tuples for performance
    return tuple(tuple(t) for t in tables)

def init_norm_tables2():
    # Produces all variations of a 2-bit number in a crumb of each byte
    # i.e. table1: 0xc0c08040
    #      table2: 0x30302010
    #      table3: 0x0c0c0804
    #      table4: 0x03030201
    #
    #>>> table4
    # 00000000 00000001 00000002 00000003
    # 00000100 00000101 00000102 00000103...
    #
    tables = tuple([0] * 256 for _ in range(4))
    table1, table2, table3, table4 = tables
    #
    for i in range(256):
        val = i & 0xc0
        #
        val <<= 6
        val |= i & 0x30
        #
        val <<= 6
        val |= i & 0x0C
        #
        val <<= 6
        val |= i & 0x03
        #
        table4[i] = val
        table3[i] = val << 2
        table2[i] = val << 4
        table1[i] = val << 6
    #
    #tables = tuple(tuple(t) for t in tables)
    tables = (table1, table2, table3, table4)
    return tables

def printtable(table,cols=5,format='{0:08x}', indent=2):
    if cols is None: cols = len(table)
    print('['.ljust(indent), end='')
    for i,v in enumerate(table):
        if i > 0:
            if (i % cols) == 0:
                print(',')
                print(''.ljust(indent), end='')
            else:
                print(', ', end='')
        print(format.format(v), end='')
    print(']')


# input:byte -> returns:byte
def norm_val(c):
    return ((c << 1) & 0xfe) ^ (0xff if (c & 0x80) else 0x00)

def inv_norm_val(c):
    # 0x1,0xff may be related to negative delta values(?)
    return ((c >> 1) & 0x7f) ^ (0xff if (c & 0x1) else 0x00)
    ###return ((c & 1) ? ((c >> 1) ^ 0xFF) : (c >> 1)) & 0xff


def process_norm(deltabuf): 
    table1, table2, table3, table4 = init_norm_tables()
    #
    sectlen = len(deltabuf) // 4
    sect1 = 0
    sect2 = sectlen
    sect3 = sectlen*2
    sect4 = sectlen*3
    #
    rlebuf = bytearray(len(deltabuf))
    #
    for i in range(sectlen):
        #val = table1[unrlebuf[i+sect1]] | table2[unrlebuf[i+sect2]] | table3[unrlebuf[i+sect3]] | table4[unrlebuf[i+sect4]]
        #
        i4 = i<<2
        val = 0
        val |= norm_val(deltabuf[i4+0]) <<  0
        val |= norm_val(deltabuf[i4+1]) <<  8
        val |= norm_val(deltabuf[i4+2]) << 16
        val |= norm_val(deltabuf[i4+3]) << 24
        #
        t1, t2, t3, t4 = 0, 0, 0, 0
        for j in range(0, 8, 2):
            t4 |= ((val >> (j*4 + 0)) & 0x03) << j
            t3 |= ((val >> (j*4 + 2)) & 0x03) << j
            t2 |= ((val >> (j*4 + 4)) & 0x03) << j
            t1 |= ((val >> (j*4 + 6)) & 0x03) << j
        rlebuf[i+sect1] = t1
        rlebuf[i+sect2] = t2
        rlebuf[i+sect3] = t3
        rlebuf[i+sect4] = t4
    #
    return rlebuf

def inv_process_norm(unrlebuf):
    table1, table2, table3, table4 = init_norm_tables()
    #
    sectlen = len(unrlebuf) // 4
    sect1 = 0
    sect2 = sectlen
    sect3 = sectlen*2
    sect4 = sectlen*3
    #
    deltabuf = bytearray(len(unrlebuf))
    #
    for i in range(sectlen):
        val = table1[unrlebuf[i+sect1]] | table2[unrlebuf[i+sect2]] | table3[unrlebuf[i+sect3]] | table4[unrlebuf[i+sect4]]
        #
        i4 = i<<2
        deltabuf[i4+0] = inv_norm_val(val >>  0)
        deltabuf[i4+1] = inv_norm_val(val >>  8)
        deltabuf[i4+2] = inv_norm_val(val >> 16)
        deltabuf[i4+3] = inv_norm_val(val >> 24)
    #
    return deltabuf



def delta_val(buf, a, b):
    buf[a] = (buf[a] - buf[b]) & 0xff

def inv_delta_val(buf, a, b):
    buf[a] = (buf[a] + buf[b]) & 0xff


def process_delta(rgbabuf, width, height, bitdepth): 
    bytedepth = (bitdepth + 7) >> 3  # // 8
    stride = (width * bytedepth + 3) & ~0x3
    #stride = (((width * bitdepth + 7) >> 8) + 3) & ~3  # // 8) // 4 * 4
    #
    deltabuf = bytearray(rgbabuf)
    #
    for xy in range(stride * height - 1, stride - 1, -1):
        delta_val(deltabuf, xy, xy - stride)
    #
    for x in range(stride - 1, bytedepth - 1, -1):
        delta_val(deltabuf, x, x - bytedepth)
    #
    return deltabuf

def inv_process_delta(deltabuf, width, height, bitdepth):
    bytedepth = (bitdepth + 7) >> 3  # // 8
    stride = (width * bytedepth + 3) & ~0x3
    #stride = (((width * bitdepth + 7) >> 8) + 3) & ~3  # // 8) // 4 * 4
    #
    rgbabuf = deltabuf
    #
    for x0 in range(bytedepth, stride):
        inv_delta_val(rgbabuf, x0, x0 - bytedepth)
    #
    for xy in range(stride, stride * height): 
        inv_delta_val(rgbabuf, xy, xy - stride)
    #
    return rgbabuf



def process(rgbabuf, width, height, bitdepth):
    deltabuf = process_delta(rgbabuf, width, height, bitdepth)
    normbuf = process_norm(deltabuf)
    databuf, cmdbuf = process_rle(normbuf)
    return (databuf, cmdbuf)

def inv_process(databuf, cmdbuf, width, height, bitdepth):
    normbuf = inv_process_rle(databuf, cmdbuf)
    deltabuf = inv_process_norm(normbuf)
    rgbabuf = inv_process_delta(deltabuf, width, height, bitdepth)
    return rgbabuf


"""
a = HgxInfo()
a.width = 93
a.height = 50
a.bitdepth = 24
ba = bytearray(a.bufferlen)
for n in range(len(ba)):
    ba[n] = ((n // 3 + (11 - n % 23) + 256) & 0xff)
da = delta_rgba(ba, a.width, a.height, a.bitdepth)
"""


def st(fmt, name, fields, regex=None, **kwargs):
    import collections, struct, re
    class mystruct(collections.namedtuple(name, fields, **kwargs)):
        _struct_ = struct.Struct(fmt)
        if regex is not None:
            _regex_ = re.compile(regex) if isinstance(regex, str) else regex
        @classmethod
        def unpack(cls, buffer):
            return cls(*cls._struct_.unpack(buffer))
        @classmethod
        def unpack_from(cls, buffer, offset=0):
            return cls(*cls._struct_.unpack_from(buffer, offset))
        @classmethod
        def iter_unpack(cls, buffer):
            return (cls(*t) for t in cls._struct_.iter_unpack(buffer))
        @classmethod
        def read(cls, stream):
            return cls.unpack(stream.read(cls.calcsize()))
        
        def pack(self):
            return self._struct_.pack(*self)
        def pack_into(self, buffer, offset):
            return self._struct_.pack_into(buffer, offset, *self)
        def write(self, stream):
            return stream.write(self.pack())
        
        @classmethod
        def calcsize(cls):
            return cls._struct_.size

    mystruct.__name__ = name
    mystruct.__qualname__ = name
    return mystruct

Hg3Hdr_struct = st('<4sII', 'Hg3Hdr_struct', ('signature', 'hdrsize', 'version'))
Hg3Frm_struct = st('<II', 'Hg3Frm_struct', ('offsetnext', 'id'))
Hg3Tag_struct = st('<8sII', 'Hg3Tag_struct', ('name', 'offsetnext', 'length'))



Hg3Tag_stdinfo = st('<III iiII 3x? ii', 'Hg3Tag_stdinfo', ('width', 'height', 'bitdepth', 'offsetx', 'offsety', 'fullwidth', 'fullheight', 'transparent', 'basex', 'basey'))

Hg3Tag_img = st('<II IIII 0s0s', 'Hg3Tag_img', ('field1', 'field2', 'datalen', 'orig_datalen', 'cmdlen', 'orig_cmdlen', 'databuf', 'cmdbuf'))
Hg3Tag_ats = st('<iiII I', 'Hg3Tag_ats', ('x', 'y', 'width', 'height', 'color'))
Hg3Tag_img_al = st('<II0s', 'Hg3Tag_img_al', ('alphalen', 'orig_alphalen', 'alphabuf'))
Hg3Tag_img_jpg = st('<0s', 'Hg3Tag_img_jpg', ('rawdata'))
Hg3Tag_img_wbp = st('<0s', 'Hg3Tag_img_wbp', ('rawdata'))


Hg3Tag_imgmode = st('<3x?', 'Hg3Tag_imgmode', ('imgmode'))
Hg3Tag_cptype = st('<I', 'Hg3Tag_cptype', ('cptype'))

 

class HideBuffer(object):
    __slots__ = ('name', 'data',)
    def __init__(self, data):
        self.data = data
    def __repr__(self):
        if not self.data or self.data is Ellipsis:
            return '<{0.data!r}>'.format(self)
        return '<{0.data.__class__.__name__} @ 0x{1:016X}>'.format(self, id(self.data))
    def __call__(self):
        return self.data

# class HideBuffer(object):
#     __slots__ = ('name', 'data',)
#     def __init__(self, name, data):
#         self.name = name or self.__class__.__name__
#         self.data = data
#     def __repr__(self):
#         if not self.data or self.data is Ellipsis:
#             return '<{0.name}: {0.data!r}>'.format(self)
#         return '<{0.name}: {0.data.__class__.__name__} @ 0x{1:016X}>'.format(self, id(self.data))
#     def __call__(self):
#         return self.data


class Hg3Tag_struct(Hg3Tag_struct):
    @property
    def id(self):
        import re
        name = name if hasattr('encode') else name.decode()
        m = re.match(r"^[A-Za-z]+(\d{4,})\x00*$", self.name)
        return int(m[1]) if m else None
    @property
    def hasid(self):
        import re
        name = name if hasattr('encode') else name.decode()
        return bool(re.match(r"^[A-Za-z]+(\d{4,})\x00*$", self.name))

class Hg3Tag(object):
    __slots__ = ('offset', 'tag', 'data')
    _tags_ = {'stdinfo': Hg3Tag_stdinfo, 'imgmode': Hg3Tag_imgmode, 'cptype': Hg3Tag_cptype, 'ats': Hg3Tag_ats, 'img': Hg3Tag_img, 'img_al': Hg3Tag_img_al, 'img_jpg': Hg3Tag_img_jpg, 'img_wbp': Hg3Tag_img_wbp}
    _imgs_ = {'img': Hg3Tag_img, 'img_al': Hg3Tag_img_al, 'img_jpg': Hg3Tag_img_jpg, 'img_wbp': Hg3Tag_img_wbp}
    _ids_ = {'ats': Hg3Tag_ats, 'img': Hg3Tag_img}
    def __init__(self, data=None):
        self.offset =  0
        self.tag = Hg3Tag_struct(bytes(8), 0, 0)
        self.data = data
    @property
    def name(self):
        return self.tag.name if hasattr(self.tag.name, 'decode') else self.tag.name.decode('latin-1')
    @name.setter
    def name(self, name):
        newname = name.encode('latin-1') if not hasattr(name, 'decode') else name
        self.tag = self.tag._replace(name=newname)
    @property
    def offsetnext(self):
        return self.tag.offsetnext
    @offsetnext.setter
    def offsetnext(self, offsetnext):
        self.tag = self.tag._replace(offsetnext=offsetnext)
    @property
    def length(self):
        return self.tag.offsetnext
    @length.setter
    def length(self, length):
        self.tag = self.tag._replace(length=length)
    @property
    def prefixname(self):
        import re
        m = re.match(r"^([^0-9\x00]*)(\d{4,})\x00*$", self.name)
        return m[1] if m else self.name
        # curname = self.tag.name if hasattr(self.name, 'decode') else self.name.decode('latin-1')
        # return curname.rstrip('\x00')
    @property
    def prefixid(self):
        import re
        m = re.match(r"^([^0-9\x00]*)(\d{4,})\x00*$", self.name)
        return (m[1], int(m[2])) if m else None
    @property
    def prefix(self):
        import re
        m = re.match(r"^([^0-9\x00]*)(\d{4,})\x00*$", self.name)
        return m[1] if m else None
    @property
    def id(self):
        import re
        m = re.match(r"^([^0-9\x00]*)(\d{4,})\x00*$", self.name)
        return int(m[2]) if m else None
    @id.setter
    def id(self, id):
        import re
        m = re.match(r"^([^0-9\x00]*)(\d{4,})\x00*$", self.name)
        self.name = '{0}{1:04d}'.format(m[1], id).ljust(8, '\x00')
    
    def getid(self, prefix):
        import re
        prefix = prefix if hasattr(prefix, 'decode') else prefix.decode('latin-1')
        p = r"^({0})(\d{4,})\x00*$".format(re.escape(prefix))
        m = re.match(p, self.name)
        return int(m[2]) if m else None
    # def getprefix(self, prefix):
    #     import re
    #     prefix = prefix if hasattr(prefix, 'decode') else prefix.decode('latin-1')
    #     p = r"^({0})(\d{4,})\x00*$".format(re.escape(prefix))
    #     m = re.match(p, self.name)
    #     return m[1] if m else None
    def setid(self, prefix, id):
        prefix = prefix if hasattr(prefix, 'decode') else prefix.decode('latin-1')
        self.name = '{0}{1:04d}'.format(prefix, id).ljust(8, '\x00')
    def hasid(self, prefix):
        import re
        prefix = prefix if hasattr(prefix, 'decode') else prefix.decode('latin-1')
        p = r"^({0})(\d{4,})\x00*$".format(re.escape(prefix))
        return bool(re.match(p, self.name))
    def setname(self, name):
        name = self.name if hasattr(self.name, 'decode') else self.name.decode('latin-1')
        self.name = name.ljust(8, '\x00')
    def hasname(self, name):
        name = name if hasattr(name, 'decode') else name.decode('latin-1')
        return self.name == name.rstrip('\x00')
    
    def isimage(self):
        return self.hasid('img') or self.hasname('img_al') or self.hasname('img_jpg') or self.hasname('img_wbp')
    
    @classmethod
    def read(self, stream, readbufs=False):
        self = cls()
        self.offset = stream.tell()
        self.tag = Hg3Tag.read(stream)
        self.data = None
        return self

import enum

class ImageType(enum.Enum):
    # img%04d HG-X processed image data (databuf, cmdbuf)
    HG   = 'img'
    # img_jpg raw JPEG file data (rawdata)
    JPEG = 'jpg'
    # Combination img_jpg + img_al raw JPEG file data with alpha mask channel
    JPEG_AL = 'jpg_al'
    # img_wbp raw WEBP file data (rawdata)
    WEBP = 'wbp'
    # img_al raw alpha channel data (alphabuf, with no stride padding, Not a valid CS2 type!)
    ALPHA = 'al'
    # img_png raw BMP file data (rawdata, Not an existing CS2 type!)
    BMP = 'bmp'
    # img_png raw GIF file data (rawdata, Not an existing CS2 type!)
    GIF = 'gif'
    # img_png raw PNG file data (rawdata, Not an existing CS2 type!)
    PNG = 'png'

TAG_IMAGE  = 'img'
TAG_IMAGE_JPEG  = 'img_jpg'
TAG_IMAGE_WEBP  = 'img_wbp'
TAG_IMAGE_ALPHA  = 'img_al'
TAG_ATTRIBUTE = 'ats'
TAG_STDINFO = 'stdinfo'
TAG_CPTYPE = 'cptype'
TAG_IMGMODE = 'imgmode'

REGEX_ATTRIBUTE = re.compile(r"^(?P<name>ats)(?P<id>\d{4,5})$")
REGEX_IMAGE_HG = re.compile(r"^(?P<name>img)(?P<id>\d{4,5})$")


# REGEX_STDINFO = re.compile(r"^(?P<name>stdinfo)(?:\x00|$)")
# REGEX_CPTYPE = re.compile(r"^(?P<name>cptype)(?:\x00|$)")
# REGEX_IMGMODE = re.compile(r"^(?P<name>imgmode)(?:\x00|$)")
# REGEX_ATTRIBUTE = re.compile(r"^(?P<name>ats(?P<id>\d{4,5}))(?:\x00|$)")
# REGEX_IMAGE_HG = re.compile(r"^(?P<name>img(?P<id>\d{4,5}))(?:\x00|$)")
# REGEX_IMAGE_JPEG = re.compile(r"^(?P<name>img_(?P<id>jpg))(?:\x00|$)")
# REGEX_IMAGE_WEBP = re.compile(r"^(?P<name>img_(?P<id>wbp))(?:\x00|$)")
# REGEX_IMAGE_ALPHA = re.compile(r"^(?P<name>img_(?P<id>al))(?:\x00|$)")


# img%04d HG-X processed image data (databuf, cmdbuf)
IMAGE_HG   = 'img'
# img_jpg raw JPEG file data (rawdata)
IMAGE_JPEG = 'jpg'
# Combination img_jpg + img_al raw JPEG file data with alpha mask channel
IMAGE_JPEG_AL = 'jpg_al'
# img_wbp raw WEBP file data (rawdata)
IMAGE_WEBP = 'wbp'

# img_al raw alpha channel data (alphabuf, with no stride padding, Not a valid CS2 type!)
IMAGE_ALPHA = 'al'

# # img_png raw BMP file data (rawdata, Not an existing CS2 type!)
# IMAGE_BMP = 'bmp'
# # img_png raw GIF file data (rawdata, Not an existing CS2 type!)
# IMAGE_GIF = 'gif'
# # img_png raw PNG file data (rawdata, Not an existing CS2 type!)
# IMAGE_PNG = 'png'

import re

tag_regex = re.compile(r"^(?P<tag>(?P<name>ats|img|imgmode|cptype|stdinfo)(?:(?<=img)_(?:(?P<format>al|jpg|wbp|(?P<unknown>[a-z_]*)))|(?P<id>(?<=ats|img)\d{4,})|(?<!ats|img)))(?:\x00|$)")

# class Hg3ImageData(collections.abc.Sequence):
#     def __init__(self, tags=None):
#         img_type = None
#         img_tags, img_ids, all_img_tags = [], [], []
#         all_img_tags = []
#         self.format = None
#         self.stdinfo = None #, Hg3Tag_stdinfo(0, 0, 0,  0, 0, 0, 0,  False,  0, 0)
#         for i,tag in enumerate(tags or ()):
#             m = tag_regex.match(tag.name)
#             if m and m['name'] == 'stdinfo':
#                 if
#             elif m and m['name'] == 'img':
#                 img_id, img_fmt, img_unk = m['id'], m['format'], m['unknown']
#                 img_key = img_fmt if img_id is None else int(img_id)
#                 all_img_tags.append((img_key, tag))
#                 if self.format is not None:
#                     if self.format == IMAGE_JPEG and fmt == IMAGE_ALPHA:
#                         self.format = IMAGE_JPEG_AL
#                         img_tags.append((fmt, tag))
#                     elif self.format == IMAGE_ALPHA and fmt == IMAGE_JPEG:
#                         # Not supported by CatSystem2, we'll support it though
#                         self.format = IMAGE_JPEG_AL
#                         img_tags.append((fmt, tag))
#                     elif self.format == IMAGE_HG and id is not None:
#                         img_ids.append((int(id), tag))
#                         img_tags.append((int(id), tag))
#                 else:
#                     if id is not None:
#                         img_ids.append((int(id), tag))
#                         img_tags.append((int(id), tag))
#                         self.format = IMAGE_HG
#                     elif fmt == IMAGE_JPEG:
#                         img_tags.append((fmt, tag))
#                         self.format = IMAGE_JPEG
#                     elif fmt == IMAGE_WEBP:
#                         img_tags.append((fmt, tag))
#                         self.format = IMAGE_WEBP
#                     elif fmt == IMAGE_ALPHA:
#                         # Not supported by CatSystem2, we'll support it though
#                         img_tags.append((fmt, tag))
#                         self.format = IMAGE_ALPHA
#         self.image_type = 


# class Hg3ImageData(collections.abc.Sequence):
#     def __init__(self, tags=None):
#         img_type, img_type_unk = None, None
#         img_tags, img_ids, img_unks = [], [], []
#         all_img_tags = []
#         for tag in (tags or ()):
#             m = tag_regex.match(tag.name)
#             if m and m['name'] == 'img':
#                 img_id, img_fmt, img_unk = m['id'], m['format'], m['unknown']
#                 img_key = (img_fmt or img_unk) if img_id is None else int(img_id)
#                 all_img_tags.append(img_key, tag)
#                 if img_type is not None:
#                     if img_type == IMAGE_JPEG and img_fmt == IMAGE_ALPHA:
#                         img_type = IMAGE_JPEG_AL
#                         img_tags.append((img_fmt, tag))
#                     elif img_type == IMAGE_ALPHA and img_fmt == IMAGE_JPEG:
#                         # Not supported by CatSystem2, we'll support it though
#                         img_type = IMAGE_JPEG_AL
#                         img_tags.append((img_fmt, tag))
#                     elif img_type == IMAGE_HG and img_id is not None:
#                         img_ids.append((int(img_id), tag))
#                         img_tags.append((int(img_id), tag))
#                 else:
#                     if img_id is not None:
#                         img_ids.append((int(img_id), tag))
#                         img_tags.append((int(img_id), tag))
#                         img_type = IMAGE_HG
#                     elif img_fmt == IMAGE_JPEG:
#                         img_tags.append((img_fmt, tag))
#                         img_type = IMAGE_JPEG
#                     elif img_fmt == IMAGE_WEBP:
#                         img_tags.append((img_fmt, tag))
#                         img_type = IMAGE_WEBP
#                     elif img_fmt == IMAGE_ALPHA:
#                         # Not supported by CatSystem2, we'll support it though
#                         img_tags.append((img_fmt, tag))
#                         img_type = img_fmt
#                     elif img_unk is not None:
#                         img_unks.append((img_unk, tag))
#                         if img_type_unk is None:

#                         # Not supported by CatSystem2
#                     img_tags.append(tag)
                    
#         self.img_tags = () if img_tags is None else tuple(img_tags)
#         self.image_type
#     def image_type(self):
#         for t in self.img_tags:

#     def orderedtags(self):





class Hg3Frame(object):
    __slots__ = ('offset', 'frame', 'tags', 'stdinfo', 'imgmode', 'cptype', 'attributes', 'image_data', 'image_type')
    def __init__(self):
        self.offset =  0
        self.frame = Hg3Frm_struct(0, 0)
        self.tags = collections.OrderedDict()
        self.stdinfo = Hg3Tag_stdinfo(0, 0, 0,  0, 0, 0, 0,  False,  0, 0)
        self.imgmode = False
        self.cptype = 0
        self.attributes = collections.OrderedDict()
        self.image_data = collections.OrderedDict()
        self.image_type = None
    @property
    def id(self):
        return self.frame.id
    @id.setter
    def id(self, id):
        self.frame = self.frame._replace(id=id)
    @property
    def offsetnext(self):
        return self.frame.offsetnext
    @offsetnext.setter
    def offsetnext(self, offsetnext):
        self.frame = self.frame._replace(offsetnext=offsetnext)
    @classmethod
    def read(cls, stream, readbufs=False, **kwargs):
        self = cls()
        self.frame = Hg3Frm_struct.read(stream)
        
        while not self.tags or tag.offsetnext: #self.tags[-1].offsetnext:
            position = stream.tell()
            tag = Hg3Tag_struct.read(stream)
            name = tag.name.rstrip(b'\x00').decode('latin-1')
            
            if name == 'stdinfo':
                tag.data = Hg3Tag_stdinfo.read(stream)
                self.stdinfo = tag.data
            elif name == 'imgmode':
                tag.data = Hg3Tag_imgmode.read(stream)
                self.imgmode = tag.data.imgmode
            elif name == 'cptype':
                tag.data = Hg3Tag_cptype.read(stream)
                self.cptype = tag.data.cptype
            elif name == 'img_wbp':
                tag.data = Hg3Tag_img_wbp.read(stream)
                if readbufs:
                    tag.data = tag.data._replace(rawdata=HideBuffer(stream.read(tag.length)))
                if self.image_type is None:
                    self.image_type = 'wbp'
                    self.image_data['wbp'] = tag.data
            elif name == 'img_jpg':
                tag.data = Hg3Tag_img_jpg.read(stream)
                if readbufs:
                    tag.data = tag.data._replace(rawdata=HideBuffer(stream.read(tag.length)))
                if self.image_type is None:
                    self.image_type = 'jpg'
                    self.image_data['jpg'] = tag.data
            elif name == 'img_al':
                tag.data = Hg3Tag_img_al.read(stream)
                if readbufs:
                    #tag.data = tag.data._replace(alphabuf=zlib.decompress(stream.read(tag.data.alphalen)))
                    tag.data = tag.data._replace(alphabuf=HideBuffer(stream.read(tag.data.alphalen)))
                if self.image_type is None:
                    # Not a standalone CatSystem2 format, support it anyway
                    self.image_type = 'al'
                    self.image_data['al'] = tag.data
                elif self.image_type == 'jpg':
                    self.image_type = 'jpg_al'
                    self.image_data['al'] = tag.data
            elif re.match(r"^img(\d{4,5})$", name):
                img_id = int(re.match(r"^img(\d{4,5})$", name)[1])
                tag.data = Hg3Tag_img.read(stream)
                if readbufs:
                    #tag.data = tag.data._replace(databuf=HideBuffer(zlib.decompress(stream.read(tag.data.datalen))))
                    #tag.data = tag.data._replace(cmdbuf=HideBuffer(zlib.decompress(stream.read(tag.data.cmdlen))))
                    tag.data = tag.data._replace(databuf=HideBuffer(stream.read(tag.data.datalen)))
                    tag.data = tag.data._replace(cmdbuf=HideBuffer(stream.read(tag.data.cmdlen)))
                if self.image_type is None or self.image_type == 'img':
                    self.image_type = 'img'
                    self.image_data[img_id] = tag.data
            elif re.match(r"^ats(\d{4,5})$", name):
                ats_id = int(re.match(r"^ats(\d{4,5})$", name)[1])
                tag.data = Hg3Tag_ats.read(stream)
                self.attributes[ats_id] = tag.data
            
            self.tags[name] = tag
            stream.seek(position + tag.offsetnext)
        return self
    
    def write(self, stream, **kwargs):
        self.frame.write(stream)
        
        self.tags = collections.OrderedDict()
        #self.tags['stdinfo'] = self.stdinfo
        if self.stdinfo is not None:
            name = 'stdinfo'
            length = Hg3Tag_stdinfo.calcsize()
            data = self.stdinfo
            tag = Hg3Tag_struct(name.ljust(8, '\x00').encode('latin-1'), data, length)
            self.tags[name] = tag
        for img_key,img in self.image_data.items():
            if isinstance(img_key, int):
                name = 'img{0:04d}'.format(img_key)
                length = Hg3Tag_img.calcsize() + img.datalen + img.cmdlen
            elif img_key == 'al':
                name = 'img_al'
                length = Hg3Tag_al.calcsize() + img.alphalen
            elif img_key == 'jpg':
                name = 'img_jpg'
                length = Hg3Tag_jpg.calcsize() + len(img.rawdata())
            elif img_key == 'wbp':
                name = 'img_wbp'
                length = Hg3Tag_wbp.calcsize() + len(img.rawdata())
            data = img
            tag = Hg3Tag_struct(name.ljust(8, '\x00').encode('latin-1'), data, length)
            self.tags[name] = tag
        for ats_key,ats in self.attributes.items():
            name = 'ats{0:04d}'.format(ats_key)
            length = Hg3Tag_ats.calcsize()
            data = ats
            tag = Hg3Tag_struct(name.ljust(8, '\x00').encode('latin-1'), data, length)
            self.tags[name] = tag
        if self.imgmode is not None: #self.imgmode: #bool eval
            name = 'imgmode'
            length = Hg3Tag_imgmode.calcsize()
            data = Hg3Tag_imgmode(self.imgmode)
            tag = Hg3Tag_struct(name.ljust(8, '\x00').encode('latin-1'), data, length)
            self.tags[name] = tag
        if self.cptype is not None:
            name = 'cptype'
            length = Hg3Tag_cptype.calcsize()
            data = Hg3Tag_cptype(self.cptype)
            tag = Hg3Tag_struct(name.ljust(8, '\x00').encode('latin-1'), data, length)
            self.tags[name] = tag

        for i,(name,tag) in enumerate(self.tags.items()):
            #while not self.tags or tag.offsetnext: #self.tags[-1].offsetnext:
            position = stream.tell()
            # tag.offsetnext = 0
            data = tag.offsetnext
            print(tag)
            tag = tag._replace(offsetnext=0)
            tag.write(stream)
            
            #name = tag.name.rstrip(b'\x00').decode('latin-1')
            


            if name == 'stdinfo':
                data.write(stream)
                # tag.data = Hg3Tag_stdinfo.read(stream)
                # self.stdinfo = tag.data
            elif name == 'imgmode':
                data.write(stream)
                # tag.data = Hg3Tag_imgmode.read(stream)
                # self.imgmode = tag.data.imgmode
            elif name == 'cptype':
                data.write(stream)
                # tag.data = Hg3Tag_cptype.read(stream)
                # self.cptype = tag.data.cptype
            elif name == 'img_wbp':
                data._replace(rawdata=b'').write(stream)
                stream.write(data.rawdata())
                # tag.data = Hg3Tag_img_wbp.read(stream)
                # if readbufs:
                #     tag.data = tag.data._replace(rawdata=HideBuffer(stream.read(tag.length)))
                # if self.image_type is None:
                #     self.image_type = 'wbp'
                #     self.image_data['wbp'] = tag.data
            elif name == 'img_jpg':
                data._replace(rawdata=b'').write(stream)
                stream.write(data.rawdata())
                # tag.data = Hg3Tag_img_jpg.read(stream)
                # if readbufs:
                #     tag.data = tag.data._replace(rawdata=HideBuffer(stream.read(tag.length)))
                # if self.image_type is None:
                #     self.image_type = 'jpg'
                #     self.image_data['jpg'] = tag.data
            elif name == 'img_al':
                data._replace(alphabuf=b'').write(stream)
                stream.write(data.alphabuf())
                # tag.data = Hg3Tag_img_al.read(stream)
                # if readbufs:
                #     #tag.data = tag.data._replace(alphabuf=zlib.decompress(stream.read(tag.data.alphalen)))
                #     tag.data = tag.data._replace(alphabuf=HideBuffer(stream.read(tag.data.alphalen)))
                # if self.image_type is None:
                #     # Not a standalone CatSystem2 format, support it anyway
                #     self.image_type = 'al'
                #     self.image_data['al'] = tag.data
                # elif self.image_type == 'jpg':
                #     self.image_type = 'jpg_al'
                #     self.image_data['al'] = tag.data
            elif re.match(r"^img(\d{4,5})$", name):
                data._replace(databuf=b'', cmdbuf=b'').write(stream)
                stream.write(data.databuf())
                stream.write(data.cmdbuf())
                # img_id = int(re.match(r"^img(\d{4,5})$", name)[1])
                # tag.data = Hg3Tag_img.read(stream)
                # if readbufs:
                #     #tag.data = tag.data._replace(databuf=HideBuffer(zlib.decompress(stream.read(tag.data.datalen))))
                #     #tag.data = tag.data._replace(cmdbuf=HideBuffer(zlib.decompress(stream.read(tag.data.cmdlen))))
                #     tag.data = tag.data._replace(databuf=HideBuffer(stream.read(tag.data.datalen)))
                #     tag.data = tag.data._replace(cmdbuf=HideBuffer(stream.read(tag.data.cmdlen)))
                # if self.image_type is None or self.image_type == 'img':
                #     self.image_type = 'img'
                #     self.image_data[img_id] = tag.data
            elif re.match(r"^ats(\d{4,5})$", name):
                data.write(stream)
                # ats_id = int(re.match(r"^ats(\d{4,5})$", name)[1])
                # tag.data = Hg3Tag_ats.read(stream)
                # self.attributes[ats_id] = tag.data
            
            #TODO: do padding?
            if tag.length != stream.tell() - position - tag.calcsize():
                raise ValueError('unexpected tag length {0} instead of {1}'.format(tag.length, stream.tell() - position - tag.calcsize()))
            tag = tag._replace(length=stream.tell() - position - tag.calcsize())
            #tag.length = stream.tell() - position - tag.calcsize()
            nextposition = stream.tell()
            if i + 1 < len(self.tags):
                tag = tag._replace(offsetnext=nextposition - position)
                # tag.offsetnext = nextposition - position
                stream.seek(position, 0)
                tag.write(stream)
                stream.seek(nextposition, 0)
            self.tags[name] = tag
            # else:
            #     tag = tag._replace(offsetnext=0)
        return self
    
    # def read_image(self):
    #     if self.image_type == 'img':
    #         return self.read_image_hg()
    #     elif self.image_type == 'jpg':
    #         return self.read_image_jpg()
    #     elif self.image_type == 'jpg_al':
    #         return self.read_image_jpg_al()
    #     elif self.image_type == 'al':
    #         #
    #         return self.read_image_al()
    #     elif self.image_type == 'wbp':
    #         return self.read_image_wbp()

    # def read_image_jpg_al(self):


class Hg3Image(object):
    __slots__ = ('header', 'frames',)
    def __init__(self):
        self.header = Hg3Hdr_struct(b'HG-3', 12, 0x300)
        self.frames = collections.OrderedDict()
    @classmethod
    def read(cls, stream, readbufs=False, **kwargs):
        if isinstance(stream, (str,bytes)):
            with open(stream, mode='rb') as file:
                return cls.read(file, readbufs=readbufs, **kwargs)
        
        self = cls()
        self.header = Hg3Hdr_struct.read(stream)
        
        while not self.frames or frame.offsetnext: #self.frames[-1].offsetnext:
            position = stream.tell()
            frame = Hg3Frame.read(stream, readbufs=readbufs, **kwargs)
            self.frames[frame.id] = frame #.append(frame) #[frame.id] = frame
            stream.seek(position + frame.offsetnext)
        
        return self
    def write(self, stream, **kwargs):
        if isinstance(stream, (str,bytes)):
            with open(stream, mode='wb+') as file:
                res = self.write(file, **kwargs)
                file.flush()
                return res
        
        self.header.write(stream)

        for i,frame in enumerate(self.frames.values()):
            position = stream.tell()
            frame.offsetnext = 0
            frame.write(stream, **kwargs)

            #TODO: do padding?
            nextposition = stream.tell()
            if i + 1 < len(self.frames):
                frame.offsetnext = nextposition - position
                stream.seek(position, 0)
                frame.frame.write(stream)
                stream.seek(nextposition, 0)
            else:
                frame.offsetnext = 0
            
            

    ## https://pillow.readthedocs.io/en/stable/handbook/tutorial.html
    def read_image(self, frame, *, flip=False, expand=False):
        if isinstance(frame, int):
            frame = list(self.frames.values())[frame]
        reader = getattr(self, 'read_image_{0}'.format(frame.image_type), None)
        if not reader:
            raise ValueError('No reader exists for image type {0.image_type!r}'.format(frame))
        return reader(frame, flip=flip, expand=expand)
        # if frame.image_type == 'img':
        #     return self.read_image_img()
        # elif frame.image_type == 'jpg':
        #     return self.read_image_jpg()
        # elif frame.image_type == 'jpg_al':
        #     return self.read_image_jpg_al()
        # elif frame.image_type == 'al':
        #     #
        #     return self.read_image_al()
        # elif frame.image_type == 'wbp':
        #     return self.read_image_wbp()

    def read_image_wbp(self, frame, *, flip=False, expand=False, addrgb=False, addalpha=False, **kwargs):
        import PIL, io
        image = PIL.Image.open(io.BytesIO(frame.image_data['wbp'].rawdata()))
        image = self.image_apply_options(frame, image, flip=flip, expand=expand, addrgb=addrgb, addalpha=addalpha, **kwargs)
        return image
    def read_image_jpg(self, frame, *, flip=False, expand=False, addrgb=False, addalpha=False, **kwargs):
        import PIL, io
        image = PIL.Image.open(io.BytesIO(frame.image_data['jpg'].rawdata()))
        image = self.image_apply_options(frame, image, flip=flip, expand=expand, addrgb=addrgb, addalpha=addalpha, **kwargs)
        return image
    def read_image_jpg_al(self, frame, *, flip=False, expand=False, addrgb=False, addalpha=False, alphamask='unused', **kwargs):
        import PIL, io
        image = self.read_image_jpg(frame)
        image = image.putalpha(self.read_image_al(frame, alphamask=True))
        image = self.image_apply_options(frame, image, flip=flip, expand=expand, addrgb=addrgb, addalpha=addalpha, **kwargs)
        return image
    def read_image_al(self, frame, *, flip=False, expand=False, addrgb=False, addalpha=False, alphamask=True):
        import PIL, io
        #stdinfo = frame.stdinfo
        width, height, bitdepth, offsetx, offsety, fullwidth, fullheight = frame.stdinfo[0:7]
        stride = width * 1 # 1 bytes per alpha channel pixel
        buffersize = stride * height
        orientation = 1 if flip else -1

        img_al = frame.image_data['al']
        alphabuf = zlib.decompress(img_al.alphabuf())
        if alphamask:
            image = PIL.Image.frombytes('L', frame.stdinfo[0:2], bytes(alphabuf), 'raw', 'L', 0, orientation)
        else:
            image = PIL.Image.frombytes('RGBA', frame.stdinfo[0:2], bytes(alphabuf), 'raw', 'A', 0, orientation)
        image = self.image_apply_options(frame, image, expand=expand, addrgb=addrgb, addalpha=addalpha, **kwargs)
        return image
        alphabuf = None


    def read_image_img(self, frame, *, flip=False, expand=False, addrgb=False, addalpha=False, **kwargs):
        import PIL, io
        #stdinfo = frame.stdinfo
        width, height, bitdepth, offsetx, offsety, fullwidth, fullheight = frame.stdinfo[0:7]
        stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
        buffersize = stride * height
        orientation = 1 if flip else -1

        ids = list(frame.image_data.keys())
        ids.sort()
        ## https://pillow.readthedocs.io/en/stable/handbook/concepts.html?highlight=concept-modes#modes
        for img_id in ids:
            #
            img = frame.image_data[img_id]
            databuf, cmdbuf = zlib.decompress(img.databuf()), zlib.decompress(img.cmdbuf())
            pixels = inv_process(databuf, cmdbuf, *frame.stdinfo[0:3])
            #TODO: Multiple img id parts
            #
            if frame.stdinfo.bitdepth == 24: # 1 pixel per 3 bytes, 24-bit BGR
                image = PIL.Image.frombytes('RGB', frame.stdinfo[0:2], bytes(pixels), 'raw', 'BGR', stride, orientation)
            elif frame.stdinfo.bitdepth == 32: # 1 pixel per 4 bytes, 32-bit BGRA
                image = PIL.Image.frombytes('RGBA', frame.stdinfo[0:2], bytes(pixels), 'raw', 'BGRA', stride, orientation)
            elif frame.stdinfo.bitdepth == 8: # 1 pixel per byte, 8-bit black and white
                image = PIL.Image.frombytes('L', frame.stdinfo[0:2], pixels, bytes(pixels), 'raw', 'L', stride, orientation)
            elif frame.stdinfo.bitdepth == 1: # 8 pixels per byte, 1-bit black or white
                image = PIL.Image.frombytes('1', frame.stdinfo[0:2], pixels, bytes(pixels), 'raw', '1', stride, orientation)
            else:
                raise ValueError('bitdepth of {0.bitdepth} bits-per-pixel is not supported'.format(frame.stdinfo))
            image = self.image_apply_options(frame, image, expand=expand, addrgb=addrgb, addalpha=addalpha, **kwargs)
            return image
            pixels = None

    def image_apply_options(self, frame, image, *, flip=False, expand=False, addrgb=False, addalpha=False, **kwargs):
        import PIL, io
        #stdinfo = frame.stdinfo
        width, height, bitdepth, offsetx, offsety, fullwidth, fullheight = frame.stdinfo[0:7]
        expand = expand and (offsetx != 0 or offsety != 0 or fullwidth != width or fullheight != height)
        #addrgb = addrgb and image.mode != 'RGB' and image.mode != 'RGBA' and image.mode != 'RGBa'
        addalpha = (expand or addalpha) # and image.mode != 'RGBA'

        if flip:
            image = PIL.ImageOps.flip(image)
        
        if not image.mode.startswith('RGB'):
            if addrgb:
                if addalpha or image.mode.endswith('A'):
                    image = image.convert('RGBA')
                elif not addalpha and image.mode.endswith('a'):
                    image = image.convert('RGBa')
                else:
                    image = image.convert('RGB')
            elif addalpha and not image.mode.endswith('A'):
                image = image.putalpha(PIL.Image.new('L', image.size, 255))
        elif addalpha and image.mode != 'RGBA':
            image = image.convert('RGBA')
        # if image.mode != 'RGBA' and addalpha:
        #     image = image.convert('RGBA')
        # if image.mode != 'RGBA':
        #     if image.mode == 'RGBa' and addalpha:
        #         image = image.convert('RGBA')
        #     if not image.mode.startswith('RGB') and not addrgb and addalpha:
        #         image = image.putalpha(PIL.Image.new('L', image.size, 255))
        #     elif not image.mode.startswith('RGB') and addrgb and addalpha:

        # if addrgb and addalpha:
        #     image = image.convert('RGBA')
        # elif addrgb and not image.mode.startswith('RGB'):
        #     if image.mode.endswith('A') or image.mode.endswith('a'):
        #         image = image.convert('RGB{0}'.format(image.mode[-1]))
        #     else:
        #         image = image.convert('RGB')
        # if addalpha and not image.mode.endswith('A'): # and frame.stdinfo.bitdepth != 32:
        #     image = image.putalpha(PIL.Image.new('L', image.size, 255))
        if expand: # and (offsetx != 0 or offsety != 0 or fullwidth != width or fullheight != height):
            # border=(offsetx, offsety, fullwidth - width - offsetx, fullheight - height - offsety)
            # print('size={0.size} border={1}'.format(image, border))
            # print(border)
            image = PIL.ImageOps.expand(image, border=(offsetx, offsety, fullwidth - width - offsetx, fullheight - height - offsety))
        return image
    
    def write_image_img(self, frame, image, *, flip=False, expand=True, **kwargs):
        if isinstance(image, (str,bytes)):
            with PIL.Image.open(image) as imagefile:
                return self.write_image_img(frame, imagefile, flip=flip, expand=expand, **kwargs)
        
        frame.image_data = collections.OrderedDict()
        if not flip: # Opposite for tobytes... TODO: explain (PIL thing, not CS2)
            image = PIL.ImageOps.flip(image)
        if expand:
            bbox = image.getbbox()
            offsetx, offsety = bbox[0:2]
            fullwidth, fullheight = image.size
            width, height = (bbox[2] - offsetx, bbox[3] - offsety)
            #stride = ((width * bitdepth + 7) // 8 + 3) & ~0x3
            ## https://stackoverflow.com/a/58567453
            transparent = image.mode == 'RGBA' and image.getextrema()[3][0] < 255
            orientation = 1 if flip else -1
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
            buffersize = stride * height
            #TODO: Handle basex, basey rewriting for expand
            frame.stdinfo = stdinfo = Hg3Tag_stdinfo(width, height, bitdepth, offsetx, offsety, fullwidth, fullheight, transparent, frame.stdinfo.basex, frame.stdinfo.basey)
            img_rows = [(0, 0, height//2), (height//2, 32, height - height//2)]
            #img_rows = [(0, 0, height)]
            for img_id,(row, src, rowlen) in enumerate(img_rows):

                #orig_databuf, orig_cmdbuf = process(pixels[row*stride:(row+rowlen)*stride], *frame.stdinfo[0:3])
                orig_databuf, orig_cmdbuf = process(pixels[src*stride:(src+rowlen)*stride], *frame.stdinfo[0:1], rowlen, *frame.stdinfo[2:3])
                databuf, cmdbuf = zlib.compress(orig_databuf), zlib.compress(orig_cmdbuf) #tuple(map(zlib.compress, ))
                img = Hg3Tag_img(row, rowlen, len(databuf), len(orig_databuf), len(cmdbuf), len(orig_cmdbuf), HideBuffer(databuf), HideBuffer(cmdbuf))
                frame.image_data[img_id] = img
                orig_databuf = orig_cmdbuf = None
                databuf = cmdbuf = None
                #DEBUG: fuckery with rgb channels for showing off
                #pixels = image.tobytes('raw', 'RGBA', stride, orientation)


# def gethg3(path=None):
#     if isinstance(path, (bytes,bytearray)):
#         return path
#     f = open(path or "/plex/library/sys_mwnd_name_01.hg3", "rb")
#     hgdata = f.read()
#     f.close()
#     return hgdata



# hg3img = nt('hg3img', ('frm', 'offset', 'tags', 'info', 'img', 'chunks'))


# def readframe(ha, offset):
#     frm = hg3frm.unpack(ha, offset)
#     print('frame {0:04d}'.format(frm.id))
#     tagoff = offset + 8
#     tags = []
#     info = None
#     img = None
#     cptype = None
#     imgmode = None
#     while not tags or tag.offsetnext:
#         toff = tagoff+16
#         tag = hg3tag.unpack(ha, tagoff) 
#         print('  tag {0!r}'.format(tag.name))
#         tags.append(tag)
#         if tag.name == b'stdinfo\x00':
#             info = stdinfo.unpack(ha, toff)
#         elif tag.name.startswith(b'img') and (b'0' <= tag.name[3:4] <= b'9'):
#             img = imgtag.unpack(ha, toff)
#             chunks = [HgxChunk(img)]
#             import zlib
#             toff += 24
#             chunks[0].databuf = zlib.decompress(ha[toff:toff + img.compdatalen])
#             toff += img.compdatalen
#             chunks[0].cmdbuf = zlib.decompress(ha[toff:toff + img.compcmdlen])
#         elif tag.name == b'cptype\x00\x00':
#             cptype = struct.unpack_from('<I', ha, toff)
#         elif tag.name == b'imgmode\x00':
#             imgmode = struct.unpack_from('<I', ha, toff)
#         else:
#             pass
#         tagoff += tag.offsetnext
#     return hg3img(frm, offset, tags, info, img, chunks)


# def readhg3(path=None):
#     ha = gethg3(path)
#     frames = []
#     offset = 12
#     while not frames or frm.frm.offsetnext:
#         frm = readframe(ha, offset)
#         frames.append(frm)
#         offset += frm.frm.offsetnext
#     return frames

# def comparehg3(index, path=None):
#     if isinstance(path, tuple):
#         hf = path 
#     elif isinstance(path, list):
#         hf = path[index]
#     else:
#         hf = readhg3(path)[index]
#     #
#     ch = hf.chunks[0]
#     chbufs = (ch.databuf, ch.cmdbuf)
#     rgbabuf = unprocess(chbufs, *hf.info[0:3])
#     newbufs = process(rgbabuf, *hf.info[0:3])
#     return (chbufs[0] == newbufs[0], chbufs[1] == newbufs[1], tuple(len(b) for b in chbufs), tuple(len(b) for b in newbufs))


