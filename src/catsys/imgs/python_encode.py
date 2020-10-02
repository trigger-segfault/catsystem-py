
import os, sys, struct, io, zlib, collections, re, enum
import PIL
import PIL.Image
import PIL.ImageOps

# UTIL CLASS
class int_repr(int):
    _repr_format_ = '{0:08x}'
    def __new__(cls, *args, **kwargs):
        if cls is int_repr:
            if not kwargs and len(args) == 1 and isinstance(args[0], cls):
                return args[0]
        return super().__new__(cls, *args, **kwargs)
    def __repr__(self):
        return self._repr_format_.format(self)
    @classmethod
    def get_format(cls):
        return cls._repr_format_
    @classmethod
    def set_format(cls, format):
        cls._repr_format_ = format
    @classmethod
    def create(cls, item):
        if isinstance(item, cls):
            return item
        elif isinstance(item, int):
            return cls(item)
        elif isinstance(item, set):
            return item.__class__(cls.create(v) for v in item)
        elif isinstance(item, list):
            return item.__class__(cls.create(v) for v in item)
        elif isinstance(item, tuple):
            return item.__class__(cls.create(v) for v in item)
        elif isinstance(item, dict):
            return item.__class__((k,cls.create(v)) for k,v in item.items())
        else: # Not known, don't handle
            return item


def encode_delta(pixbuf, width, height, bitdepth, rowstart, rowcount):
    """
    Relations:
    * 
    * Subtractor 56
    * 

    ???
    * 
    """
    return outbuf

'''
The normalizer 16 conducts a coefficient truncation on the image data which has undergone the two-dimensional orthogonal transformation in the transformer 14, namely, on translation coefficients so as to normalize the data thereafter. In the coefficient truncation, the transformation coefficients which have undergone the orthogonal transformation are compared with a predetermined threshold value to truncate a portion of data not exceeding the threshold value. In the normalization, the translation coefficients after the coefficient truncation are divided by a predetermined quantization step value, thereby achieving a quantization of the image data.

'''

'''
The additional-bit computing section 26 is includes a subtractor 56 and a selector 58 as illustrated in [FIG. 5][fig-5]. This section 26 receives a normalized transformation coefficient from the normalizer 16 and an overflow signal from the amplitude sensor 24. The subtractor 56 is supplied with data of seven low-order bits of the translation coefficient and the most-significant bit, MSB, as the highest-order bit. The data of MSB signal is subtracted from the data of seven low-order bits. The MSB signal is "0" or "1" when the data of the seven low-order bits is positive or negative, respectively. The subtractor 56 sends a subtraction result represented with data including at most seven bits to an input A of the selector 58.


Connected to the normalizer 16 is a multiplier 34 for inputting the quantization step value to the normalizer 16. The multiplier 34 is linked to a normalization coefficient storage 22 and a selector 36, which is in turn connected to a weight table storage 38. The normalization coefficient storage 22 and the selector 36 are connected to an operator's console 40 for supplying therefrom the system with an instruction from the operator.

The weight table storage 38 is loaded with data of various kinds of weight tables Ts, as illustrates in [FIG. 10][fig-10], to be employed for the normalization in the normalizer 16. The table data items are delivered to the selector 36. Depending on an instruction of the operator supplied from the operator's console 40, a weight table T is selected by the selector 36 so as to be fed to the multiplier 34. On the other hand, according to an instruction of the operator supplied from the operator's console 40, a normalization coefficient stored in the normalization coefficient storage 22 is selectively obtained therefrom so as to be sent to the multiplier 34. The weight table content is multiplied by the data of the normalization coefficient in the multiplier 34, which thus attains a quantization step value to be adopted in the normalization.


The zero-run counter 20 of this embodiment includes an inverter 44 and a six-bit counter 46 as illustrates in [FIG. 3][fig-3]. The inverter 44 is supplied with the output from the non-zero sensor 18, namely, a zero or non-zero signal so as to produce an inverted signal thereof. The output from the inverter 44 is inverted so as to be fed to a clear terminal CLR of the counter 46. On receiving a non-zero signal "1" from the non-zero sensor 18, a signal "0" is supplied to the clear terminal CLR to clear the counter 46. The counter 46 is supplied with a translation coefficient transfer check clock via a clock input terminal CK from the two-dimensional orthogonal transformer 14 so as to count the clocks. When a signal "0" is received by the clear terminal CLR, the count value is cleared. As a consequence, the counter 46 continues the counting while the zero signal "0" being received from the non-zero sensor 18. In response thereto, the length of a zero run is counted in the zigzag scan of [FIG. 9][fig-9].

'''

"""
Produces weight tables 1-4:

00000000 00000040 000000c0

>>> weight_tables[0]
[00000000, 00000040, 00000080, 000000c0, 00004000, 00004040, 00004080, 000040c0, 00008000, 00008040, 00008080, 000080c0, 0000c000, 0000c040, 0000c080, 0000c0c0, 00400000, 00400040, 00400080, 004000c0, 00404000, 00404040, 00404080, 004040c0, 00408000, 00408040, 00408080, 004080c0, 0040c000, 0040c040, 0040c080, 0040c0c0, 00800000, 00800040, 00800080, 008000c0, 00804000, 00804040, 00804080, 008040c0, 00808000, 00808040, 00808080, 008080c0, 0080c000, 0080c040, 0080c080, 0080c0c0, 00c00000, 00c00040, 00c00080, 00c000c0, 00c04000, 00c04040, 00c04080, 00c040c0, 00c08000, 00c08040, 00c08080, 00c080c0, 00c0c000, 00c0c040, 00c0c080, 00c0c0c0, 40000000, 40000040, 40000080, 400000c0, 40004000, 40004040, 40004080, 400040c0, 40008000, 40008040, 40008080, 400080c0, 4000c000, 4000c040, 4000c080, 4000c0c0, 40400000, 40400040, 40400080, 404000c0, 40404000, 40404040, 40404080, 404040c0, 40408000, 40408040, 40408080, 404080c0, 4040c000, 4040c040, 4040c080, 4040c0c0, 40800000, 40800040, 40800080, 408000c0, 40804000, 40804040, 40804080, 408040c0, 40808000, 40808040, 40808080, 408080c0, 4080c000, 4080c040, 4080c080, 4080c0c0, 40c00000, 40c00040, 40c00080, 40c000c0, 40c04000, 40c04040, 40c04080, 40c040c0, 40c08000, 40c08040, 40c08080, 40c080c0, 40c0c000, 40c0c040, 40c0c080, 40c0c0c0, 80000000, 80000040, 80000080, 800000c0, 80004000, 80004040, 80004080, 800040c0, 80008000, 80008040, 80008080, 800080c0, 8000c000, 8000c040, 8000c080, 8000c0c0, 80400000, 80400040, 80400080, 804000c0, 80404000, 80404040, 80404080, 804040c0, 80408000, 80408040, 80408080, 804080c0, 8040c000, 8040c040, 8040c080, 8040c0c0, 80800000, 80800040, 80800080, 808000c0, 80804000, 80804040, 80804080, 808040c0, 80808000, 80808040, 80808080, 808080c0, 8080c000, 8080c040, 8080c080, 8080c0c0, 80c00000, 80c00040, 80c00080, 80c000c0, 80c04000, 80c04040, 80c04080, 80c040c0, 80c08000, 80c08040, 80c08080, 80c080c0, 80c0c000, 80c0c040, 80c0c080, 80c0c0c0, c0000000, c0000040, c0000080, c00000c0, c0004000, c0004040, c0004080, c00040c0, c0008000, c0008040, c0008080, c00080c0, c000c000, c000c040, c000c080, c000c0c0, c0400000, c0400040, c0400080, c04000c0, c0404000, c0404040, c0404080, c04040c0, c0408000, c0408040, c0408080, c04080c0, c040c000, c040c040, c040c080, c040c0c0, c0800000, c0800040, c0800080, c08000c0, c0804000, c0804040, c0804080, c08040c0, c0808000, c0808040, c0808080, c08080c0, c080c000, c080c040, c080c080, c080c0c0, c0c00000, c0c00040, c0c00080, c0c000c0, c0c04000, c0c04040, c0c04080, c0c040c0, c0c08000, c0c08040, c0c08080, c0c080c0, c0c0c000, c0c0c040, c0c0c080, c0c0c0c0]

>>> weight_tables[1]
[00000000, 00000010, 00000020, 00000030, 00001000, 00001010, 00001020, 00001030, 00002000, 00002010, 00002020, 00002030, 00003000, 00003010, 00003020, 00003030, 00100000, 00100010, 00100020, 00100030, 00101000, 00101010, 00101020, 00101030, 00102000, 00102010, 00102020, 00102030, 00103000, 00103010, 00103020, 00103030, 00200000, 00200010, 00200020, 00200030, 00201000, 00201010, 00201020, 00201030, 00202000, 00202010, 00202020, 00202030, 00203000, 00203010, 00203020, 00203030, 00300000, 00300010, 00300020, 00300030, 00301000, 00301010, 00301020, 00301030, 00302000, 00302010, 00302020, 00302030, 00303000, 00303010, 00303020, 00303030, 10000000, 10000010, 10000020, 10000030, 10001000, 10001010, 10001020, 10001030, 10002000, 10002010, 10002020, 10002030, 10003000, 10003010, 10003020, 10003030, 10100000, 10100010, 10100020, 10100030, 10101000, 10101010, 10101020, 10101030, 10102000, 10102010, 10102020, 10102030, 10103000, 10103010, 10103020, 10103030, 10200000, 10200010, 10200020, 10200030, 10201000, 10201010, 10201020, 10201030, 10202000, 10202010, 10202020, 10202030, 10203000, 10203010, 10203020, 10203030, 10300000, 10300010, 10300020, 10300030, 10301000, 10301010, 10301020, 10301030, 10302000, 10302010, 10302020, 10302030, 10303000, 10303010, 10303020, 10303030, 20000000, 20000010, 20000020, 20000030, 20001000, 20001010, 20001020, 20001030, 20002000, 20002010, 20002020, 20002030, 20003000, 20003010, 20003020, 20003030, 20100000, 20100010, 20100020, 20100030, 20101000, 20101010, 20101020, 20101030, 20102000, 20102010, 20102020, 20102030, 20103000, 20103010, 20103020, 20103030, 20200000, 20200010, 20200020, 20200030, 20201000, 20201010, 20201020, 20201030, 20202000, 20202010, 20202020, 20202030, 20203000, 20203010, 20203020, 20203030, 20300000, 20300010, 20300020, 20300030, 20301000, 20301010, 20301020, 20301030, 20302000, 20302010, 20302020, 20302030, 20303000, 20303010, 20303020, 20303030, 30000000, 30000010, 30000020, 30000030, 30001000, 30001010, 30001020, 30001030, 30002000, 30002010, 30002020, 30002030, 30003000, 30003010, 30003020, 30003030, 30100000, 30100010, 30100020, 30100030, 30101000, 30101010, 30101020, 30101030, 30102000, 30102010, 30102020, 30102030, 30103000, 30103010, 30103020, 30103030, 30200000, 30200010, 30200020, 30200030, 30201000, 30201010, 30201020, 30201030, 30202000, 30202010, 30202020, 30202030, 30203000, 30203010, 30203020, 30203030, 30300000, 30300010, 30300020, 30300030, 30301000, 30301010, 30301020, 30301030, 30302000, 30302010, 30302020, 30302030, 30303000, 30303010, 30303020, 30303030]

>>> weight_tables[2]
[00000000, 00000004, 00000008, 0000000c, 00000400, 00000404, 00000408, 0000040c, 00000800, 00000804, 00000808, 0000080c, 00000c00, 00000c04, 00000c08, 00000c0c, 00040000, 00040004, 00040008, 0004000c, 00040400, 00040404, 00040408, 0004040c, 00040800, 00040804, 00040808, 0004080c, 00040c00, 00040c04, 00040c08, 00040c0c, 00080000, 00080004, 00080008, 0008000c, 00080400, 00080404, 00080408, 0008040c, 00080800, 00080804, 00080808, 0008080c, 00080c00, 00080c04, 00080c08, 00080c0c, 000c0000, 000c0004, 000c0008, 000c000c, 000c0400, 000c0404, 000c0408, 000c040c, 000c0800, 000c0804, 000c0808, 000c080c, 000c0c00, 000c0c04, 000c0c08, 000c0c0c, 04000000, 04000004, 04000008, 0400000c, 04000400, 04000404, 04000408, 0400040c, 04000800, 04000804, 04000808, 0400080c, 04000c00, 04000c04, 04000c08, 04000c0c, 04040000, 04040004, 04040008, 0404000c, 04040400, 04040404, 04040408, 0404040c, 04040800, 04040804, 04040808, 0404080c, 04040c00, 04040c04, 04040c08, 04040c0c, 04080000, 04080004, 04080008, 0408000c, 04080400, 04080404, 04080408, 0408040c, 04080800, 04080804, 04080808, 0408080c, 04080c00, 04080c04, 04080c08, 04080c0c, 040c0000, 040c0004, 040c0008, 040c000c, 040c0400, 040c0404, 040c0408, 040c040c, 040c0800, 040c0804, 040c0808, 040c080c, 040c0c00, 040c0c04, 040c0c08, 040c0c0c, 08000000, 08000004, 08000008, 0800000c, 08000400, 08000404, 08000408, 0800040c, 08000800, 08000804, 08000808, 0800080c, 08000c00, 08000c04, 08000c08, 08000c0c, 08040000, 08040004, 08040008, 0804000c, 08040400, 08040404, 08040408, 0804040c, 08040800, 08040804, 08040808, 0804080c, 08040c00, 08040c04, 08040c08, 08040c0c, 08080000, 08080004, 08080008, 0808000c, 08080400, 08080404, 08080408, 0808040c, 08080800, 08080804, 08080808, 0808080c, 08080c00, 08080c04, 08080c08, 08080c0c, 080c0000, 080c0004, 080c0008, 080c000c, 080c0400, 080c0404, 080c0408, 080c040c, 080c0800, 080c0804, 080c0808, 080c080c, 080c0c00, 080c0c04, 080c0c08, 080c0c0c, 0c000000, 0c000004, 0c000008, 0c00000c, 0c000400, 0c000404, 0c000408, 0c00040c, 0c000800, 0c000804, 0c000808, 0c00080c, 0c000c00, 0c000c04, 0c000c08, 0c000c0c, 0c040000, 0c040004, 0c040008, 0c04000c, 0c040400, 0c040404, 0c040408, 0c04040c, 0c040800, 0c040804, 0c040808, 0c04080c, 0c040c00, 0c040c04, 0c040c08, 0c040c0c, 0c080000, 0c080004, 0c080008, 0c08000c, 0c080400, 0c080404, 0c080408, 0c08040c, 0c080800, 0c080804, 0c080808, 0c08080c, 0c080c00, 0c080c04, 0c080c08, 0c080c0c, 0c0c0000, 0c0c0004, 0c0c0008, 0c0c000c, 0c0c0400, 0c0c0404, 0c0c0408, 0c0c040c, 0c0c0800, 0c0c0804, 0c0c0808, 0c0c080c, 0c0c0c00, 0c0c0c04, 0c0c0c08, 0c0c0c0c]

>>> weight_tables[3]
[00000000, 00000001, 00000002, 00000003, 00000100, 00000101, 000]
[00000000, 00000001, 00000002, 00000003, 00000100, 00000101 ... 03030203, 03030300, 03030301, 03030302, 03030303]
[00000000, 00000001, 00000002, 00000003, 00000100, 00000101 ... 03030302, 03030303]

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, 0x40c0, ...0xc0c0c080, 0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, 0x1030, ...0x30303020, 0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, 0x040c, ...0x0c0c0c08, 0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, 0x0103, ...0x03030302, 0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, ...0xc0c0c080, 0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, ...0x30303020, 0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, ...0x0c0c0c08, 0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, ...0x03030302, 0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, ...0xc0c0c080, 0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, ...0x30303020, 0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, ...0x0c0c0c08, 0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, ...0x03030302, 0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, ...0xc0c0c080, 0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, ...0x30303020, 0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, ...0x0c0c0c08, 0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, ...0x03030302, 0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, 0x40c0, ...0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, 0x1030, ...0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, 0x040c, ...0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, 0x0103, ...0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, ...0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, ...0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, ...0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, ...0x03030303])

>>> init_weight_tables()
([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, ...0xc0c0c0c0],
 [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, ...0x30303030],
 [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, ...0x0c0c0c0c],
 [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, ...0x03030303])

[00000000, 00000001, 00000002, 00000003, 00000100, 00000101, 00000102, 00000103, 00000200, 00000201, 00000202, 00000203, 00000300, 00000301, 00000302, 00000303, 00010000, 00010001, 00010002, 00010003, 00010100, 00010101, 00010102, 00010103, 00010200, 00010201, 00010202, 00010203, 00010300, 00010301, 00010302, 00010303, 00020000, 00020001, 00020002, 00020003, 00020100, 00020101, 00020102, 00020103, 00020200, 00020201, 00020202, 00020203, 00020300, 00020301, 00020302, 00020303, 00030000, 00030001, 00030002, 00030003, 00030100, 00030101, 00030102, 00030103, 00030200, 00030201, 00030202, 00030203, 00030300, 00030301, 00030302, 00030303, 01000000, 01000001, 01000002, 01000003, 01000100, 01000101, 01000102, 01000103, 01000200, 01000201, 01000202, 01000203, 01000300, 01000301, 01000302, 01000303, 01010000, 01010001, 01010002, 01010003, 01010100, 01010101, 01010102, 01010103, 01010200, 01010201, 01010202, 01010203, 01010300, 01010301, 01010302, 01010303, 01020000, 01020001, 01020002, 01020003, 01020100, 01020101, 01020102, 01020103, 01020200, 01020201, 01020202, 01020203, 01020300, 01020301, 01020302, 01020303, 01030000, 01030001, 01030002, 01030003, 01030100, 01030101, 01030102, 01030103, 01030200, 01030201, 01030202, 01030203, 01030300, 01030301, 01030302, 01030303, 02000000, 02000001, 02000002, 02000003, 02000100, 02000101, 02000102, 02000103, 02000200, 02000201, 02000202, 02000203, 02000300, 02000301, 02000302, 02000303, 02010000, 02010001, 02010002, 02010003, 02010100, 02010101, 02010102, 02010103, 02010200, 02010201, 02010202, 02010203, 02010300, 02010301, 02010302, 02010303, 02020000, 02020001, 02020002, 02020003, 02020100, 02020101, 02020102, 02020103, 02020200, 02020201, 02020202, 02020203, 02020300, 02020301, 02020302, 02020303, 02030000, 02030001, 02030002, 02030003, 02030100, 02030101, 02030102, 02030103, 02030200, 02030201, 02030202, 02030203, 02030300, 02030301, 02030302, 02030303, 03000000, 03000001, 03000002, 03000003, 03000100, 03000101, 03000102, 03000103, 03000200, 03000201, 03000202, 03000203, 03000300, 03000301, 03000302, 03000303, 03010000, 03010001, 03010002, 03010003, 03010100, 03010101, 03010102, 03010103, 03010200, 03010201, 03010202, 03010203, 03010300, 03010301, 03010302, 03010303, 03020000, 03020001, 03020002, 03020003, 03020100, 03020101, 03020102, 03020103, 03020200, 03020201, 03020202, 03020203, 03020300, 03020301, 03020302, 03020303, 03030000, 03030001, 03030002, 03030003, 03030100, 03030101, 03030102, 03030103, 03030200, 03030201, 03030202, 03030203, 03030300, 03030301, 03030302, 03030303]
"""


"""
* Weight table storage 38: Normalization tables 1-4 `init_weight_tables()`

* Six-bit counter 46: Zero-run length encoding, bitbuffer elias gamma digits counter `BitBuffer.set_elias_gamma()`


* Amplitude sensor 24
* Additional-bit computing section 26: Sent to 

## ZERO-RUN LENGTH ENCODING

* Inverters 401,402~40n: Invert all bits 1<->0, when fed to NAND, False will only be returned when all bits are zero.
* NAND circuit 42: Detect non-zero bytes during zero-run length encoding `encode_zrle()`
* Inverter 44: Invert NAND result, True is sent when all bits are zero (for use with Elias gamma, not run-length encoding??)
* Six-bit counter 46: Counts the the number of bits, for use with Elias Gamma encoding



Negation for two's compliment negative values `pack_value()`
take MSB and LSB to detect `pack_value()`
"""


#i  = 0  # (uint) uVar3 = 0;  ## j (counter)
# weighttables[4,256]
weighttables = [0] * 0x400 #1024   # (int*) piVar6 = this + 0x15d;  ## weight tables @ [+0x100] 
# do { /*...*/ uVar3++; piVar6++; } while ((int)uVar3 < 0x100);
for i in range(0x100): #range(0, 256):
    # uint uVar1 = (((uVar3 & 0xc0) << 6 | uVar3 & 0x30) << 6 | uVar3 & 0xc) << 6 | uVar3 & 3;
    weightval = ((((i & 0xc0) << 6 | i & 0x30) << 6 | i & 0xc) << 6 | i & 0x3)
    weighttables[i+0x000] = weightval << 6  # piVar6[-0x100] = uVar1 << 6;
    weighttables[i+0x100] = weightval << 4  # *piVar6 = uVar1 << 4;
    weighttables[i+0x200] = weightval << 2  # piVar6[0x100] = uVar1 * 4;
    weighttables[i+0x300] = weightval       # piVar6[0x200] = uVar1;
# Patent Claim: (1)
def init_weight_tables():
    """init_weight_tables() -> weight_tables[4][256]

    NOTE: These tables contain unsigned integers

    Relations:
    * Normalizer 16
    * Normalization coefficient storage 22
    * Weight table storage 38
    * Absolute value generating circuit 48
    * Overflow sensor circuit 50
    
    >>> init_weight_tables()
    ([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, ...0xc0c0c080, 0xc0c0c0c0],
     [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, ...0x30303020, 0x30303030],
     [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, ...0x0c0c0c08, 0x0c0c0c0c],
     [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, ...0x03030302, 0x03030303])
    """
    tables = tuple([0] * 256 for _ in range(4))
    for i in range(256):
        val2 = (((i & 0xc0) << 6 | (i & 0x30)) << 6 | (i & 0xc)) << 6 | (i & 0x3)
        val = ((i & 0xc0) << 18) | ((i & 0x30) << 12) | ((i & 0xc) << 6) | (i & 0x3)
        #
        #val2 = (((i & 0xc0) << 6 | i & 0x30) << 6 | i & 0xc) << 6 | i & 0x3
        if val != val2:
            print('{0:08X} != {1:08X} @ {2}'.format(val, val2, i))
        tables[3][i] = val
        tables[2][i] = val << 2
        tables[1][i] = val << 4
        tables[0][i] = val << 6
    
    # Convert to tuples for performance
    return tuple(tuple(t) for t in tables)

def init_weight_tables2():
    """init_weight_tables2() -> weight_tables[4][256]

    NOTE: These tables contain unsigned integers

    Relations:
    * Normalizer 16
    * Normalization coefficient storage 22
    * Weight table storage 38
    * Absolute value generating circuit 48
    * Overflow sensor circuit 50
    
    >>> init_weight_tables2()
    ([0x0, 0x40, 0x80, 0xc0, 0x4000, 0x4040, 0x4080, ...0xc0c0c080, 0xc0c0c0c0],
     [0x0, 0x10, 0x20, 0x30, 0x1000, 0x1010, 0x1020, ...0x30303020, 0x30303030],
     [0x0, 0x04, 0x08, 0x0c, 0x0400, 0x0404, 0x0408, ...0x0c0c0c08, 0x0c0c0c0c],
     [0x0, 0x01, 0x02, 0x03, 0x0100, 0x0101, 0x0102, ...0x03030302, 0x03030303])
    """
    # tables = [0] * 0x400
    tables = tuple([0] * 256 for _ in range(4)) # tables[4][256]
    tables = tuple([0]*256, [0]*256, [0]*256, [0]*256)
    for i in range(256):
        val = (((i & 0xc0) << 6 | (i & 0x30)) << 6 | (i & 0xc)) << 6 | (i & 0x3)
        #
        tables[3][i] = val
        tables[2][i] = val << 2
        tables[1][i] = val << 4
        tables[0][i] = val << 6
    #
    # convert to tuples for performance
    return tuple(tuple(t) for t in tables)

def init_weight_tables3():
    """init_weight_tables3() -> weight_tables[4][256]

    NOTE: These tables contain unsigned integers

    Relations:
    * Normalizer 16
    * Normalization coefficient storage 22
    * Weight table storage 38
    * Absolute value generating circuit 48
    * Overflow sensor circuit 50
    
    >>> init_weight_tables3()
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


#[(i,at[0][i],norm_val(i), at[1][i], inv_norm_val(i)) for i in range(256)]

def init_abs_tables():
    """init_abs_tables() -> (abstable, inv_abstable)

    NOTE: These tables contain unsigned integers
    
    >>> init_abs_tables()
    ([0, 2, 4, 6, 8, 10, 11, ...5, 3, 1],
     [0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128])
    """
    abstable     = [0] * 0x100
    inv_abstable = [0] * 0x100
    ##[0, 2, 4, 6, 8, 10, 11, ...5, 3, 1]
    ##[0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128]
    #
    for i in range(0x80):
        i2 = i << 1
        abstable[     i] = i2 + 0
        abstable[0xff-i] = i2 + 1  # ~i # 0xff - i
        #
        ## (0xff - i) == (i ^ 0xff) == ~i  # when unsigned
        inv_abstable[i2 + 0] =      i  # *(char *)puVar2 = (char)iVar4;
        inv_abstable[i2 + 1] = 0xff-i  # ~i  # *(char *)((int)puVar2 + 1) = cVar5;
    #
    # convert to tuples for performance
    return (tuple(abstable), tuple(inv_abstable))

#TODO: Would returning tables as (bytes, bytes) be better for efficiency?
def init_abs_tables2():
    """init_abs_tables2() -> (abstable, inv_abstable)

    NOTE: These tables contain unsigned integers
    
    >>> init_abs_tables2()
    ([0, 2, 4, 6, 8, 10, 11, ...5, 3, 1],
     [0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128])
    """
    abstable     = [0] * 0x100
    inv_abstable = [0] * 0x100
    ##[0, 2, 4, 6, 8, 10, 11, ...5, 3, 1]
    ##[0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128]
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

def init_abs_tables3():
    """init_abs_tables3() -> (abstable, inv_abstable)

    NOTE: These tables contain unsigned integers
    
    >>> init_abs_tables3()
    ([0, 2, 4, 6, 8, 10, 11, ...5, 3, 1],
     [0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128])
    """
    
    abstables = tuple([0] * 0x100 for _ in range(2)) # abstables[2][256]
    ##[0, 2, 4, 6, 8, 10, 11, ...5, 3, 1]
    ##[0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128]
    #
    for i, j in zip(range(0x80), range(0, 0x100, 2)): # [0,1,2...127] [0,2,4...254]
        abstables[0][     i] = j
        abstables[0][0xff-i] = j + 1 # ~i
        #
        abstables[1][j    ] =      i
        abstables[1][j + 1] = 0xff-i # ~i
    #
    # convert to tuples for performance
    return tuple(tuple(t) for t in abstables)

def encode_zrle(deltabuf):
    """
    Relations:
    * Non-zero sensor 18
    * Zero-run counter 20
    * Additional-bit computing section 26
    * Fixed-length item generating buffer 30
    * Bit-length-computing-section 60
    * Buffer 62

    ???
    * Six-bit counter 46
    """
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


def elias_gamma_size(v):
    d = 0
    while v >> (d + 1):
        d += 1
    #
    # 1 bit minimum plus 2 bits per digit
    return (d << 1) + 1


'''
In the translation coefficient, since the low-frequency component is more effective as data when compared with the high-frequency component, the weight table T as illustrated in [FIG. 10][fig-10] is loaded with, for example, smaller values in the low-frequency portion and larger values in the high-frequency portion. The normalization of the data is accomplished by dividing transformation coefficients which have undergone the coefficient truncation by a value αT attained by multiplying the data of the table T by the normalization coefficient α. Assuming the translation coefficients prior to the normalization to be X, the coefficient X' after the normalization is represented as follows.
'''

#i  = 0  # (uint) uVar3 = 0;  ## j (counter)
# weighttables[4,256]
weighttables = [0] * 0x400 #1024   # (int*) piVar6 = this + 0x15d;  ## weight tables @ [+0x100] 
# do { /*...*/ uVar3++; piVar6++; } while ((int)uVar3 < 0x100);
for i in range(0x100): #range(0, 256):
    # uint uVar1 = (((uVar3 & 0xc0) << 6 | uVar3 & 0x30) << 6 | uVar3 & 0xc) << 6 | uVar3 & 3;
    weightval = ((((i & 0xc0) << 6 | i & 0x30) << 6 | i & 0xc) << 6 | i & 0x3)
    weighttables[i+0x000] = weightval << 6  # piVar6[-0x100] = uVar1 << 6;
    weighttables[i+0x100] = weightval << 4  # *piVar6 = uVar1 << 4;
    weighttables[i+0x200] = weightval << 2  # piVar6[0x100] = uVar1 * 4;
    weighttables[i+0x300] = weightval       # piVar6[0x200] = uVar1;
#j = 0  # (int) iVar4 = 0;
cVar5 = -1  # (char) cVar5 = -1;
# abstable[256]
abstable = [0] * 0x100  # (treated as byte*) (uint*) puVar2 = this + 0x1d;
##[0, -1, 1, -2, 2, -3, 3, ...-127, 127, -128]
for j in range(0, 0x100, 2): #range(0, 128):
    abstable[j+0] =  j  # *(char *)puVar2 = (char)iVar4;
    abstable[j+1] = ~j  # *(char *)((int)puVar2 + 1) = cVar5;
    # iVar4 = iVar4 + 1;
    # puVar2 = (undefined4 *)((int)puVar2 + 2);
    # cVar5 = cVar5 + -1;

# iVar5 = (int)((param_3 + 3U & 0xfffffffc) + ((int)(param_3 + 3U) >> 0x1f & 3U)) >> 2;
# iVar5 = 
# param_3 = 6
def encode_sections(buf): #, length):
    abstable = init_abs_tables()[0]
    length = len(buf)
    sectlen = ((length + 3) & ~0x3) >> 2  # round up divided by 4
    inbuf = bytearray(length)
    outbuf = bytearray(sectlen * 4)
    for i,b in enumerate(buf):
        inbuf[i] = abstable[b]
    #sectlen = (((length + 3) & 0xfffffffc) + (((length + 3) >> 0x1f) & 3)) >> 2
    # index of k = index of section (k == shift for section pass)
    dst = 0
    for k in range(6, -1, -2): # same as range(0, 8, 2) reversed
        src = 0
        for i in range(sectlen): # 4 passes for each section
            val = 0
            for j in range(0, 8, 2):
                #b = norm_val(inbuf[src])
                b = inbuf[src]
                src += 1
                val |= ((b >> k) & 0x3) << j
                #val |= (((b >> k) & 0x3) << j) & 0xff  # mask byte
                #val |= ((b >> (k & 0x1f)) & 0x3) << (j & 0x1f)  & 0xff  # mask byte
            outbuf[dst] = val
            dst += 1
    return outbuf

def decode_sections(buf): #, outbuf)
    inv_abstable = init_abs_tables()[1]
    table1, table2, table3, table4 = init_weight_tables2()
    length = len(buf)
    sectlen = ((length + 3) & ~0x3) >> 2  # round up divided by 4
    inbuf = buf #bytearray(length)
    outbuf = bytearray(sectlen * 4)
    #
    # table1, table2, table3, table4 = init_norm_tables()
    #
    # sectlen = len(unrlebuf) // 4
    sect1 = 0
    sect2 = sectlen
    sect3 = sectlen*2
    sect4 = sectlen*3
    # #
    # deltabuf = bytearray(len(unrlebuf))
    #
    for i in range(sectlen):
        val = table1[buf[i+sect1]] | table2[buf[i+sect2]] | table3[buf[i+sect3]] | table4[buf[i+sect4]]
        #
        i4 = i<<2
        outbuf[i4+0] = inv_abstable[val >>  0 & 0xff]
        outbuf[i4+1] = inv_abstable[val >>  8 & 0xff]
        outbuf[i4+2] = inv_abstable[val >> 16 & 0xff]
        outbuf[i4+3] = inv_abstable[val >> 24 & 0xff]
    #
    # for i,b in enumerate(outbuf):
    #     outbuf[i] = inv_abstable[b]
    return outbuf


def norm_u4(inbuf, index, length=4):
    if index is None or index == -1:
        index = 0
    else:
        inbuf = inbuf[index:]
    if length is None or length == -1:
        length = len(inbuf)
    sectlen = ((length + 3) & ~0x3) >> 2
    outbuf = bytearray(sectlen*4)
    dst = src = val = i = j = k = b = 'None'
    print('index={} length={} sectlen={}'.format(index, length, sectlen))
    dst = 0
    def fmthex(num):
        return '0x{:02x}'.format(num) if isinstance(num, int) else num
    def showstatus(indent):
        print('{}k={:<2} i={:<5} j={:<2} dst={:<5} src={:<5} val={:<4} b={:<4}'.format(''.ljust(indent), k, i, j, dst, src, fmthex(val), fmthex(b)))
    showstatus(0)
    print()
    for k in range(6, -1, -2):
        print('for k={} in range(6, -1, -2):'.format(k))
        src = 0
        showstatus(3)
        for i in range(sectlen):
            print('   for i={} in range(sectlen={}):'.format(i, sectlen))
            val = 0
            showstatus(6)
            for j in range(0, 8, 2):
                print('      for j={} in range(0, 8, 2):'.format(j))
                b = inbuf[src]
                src += 1
                val |= ((b >> k) & 0x3) << j
                showstatus(9)
            outbuf[dst] = val
            dst += 1
            showstatus(6)
    return outbuf

def encode_sections(inbuf, outbuf, length):
    sectlen = ((length + 3) & ~0x3) >> 2  # round up divided by 4
    #sectlen = (((length + 3) & 0xfffffffc) + (((length + 3) >> 0x1f) & 3)) >> 2
    inbuf = bytearray(64)
    outbuf = bytearray(64)
    param = 6
    # k = index of table
    for k in range(6, -1, -2):
        for i in range(sectlen): # dummy
            val = 0
            for j in range(0, 8, 2):
                b = inbuf[i + j >> 1]
                val |= ((b >> k) & 0x3) << j
                #val |= (((b >> k) & 0x3) << j) & 0xff  # mask byte
                #val |= ((b >> (k & 0x1f)) & 0x3) << (j & 0x1f)  & 0xff  # mask byte
            outbuf[i] = val6
        t1, t2, t3, t4 = 0, 0, 0, 0
        for j in range(0, 8, 2):
            t4 |= ((val >> (j*4 + 0)) & 0x03) << j
            t3 |= ((val >> (j*4 + 2)) & 0x03) << j
            t2 |= ((val >> (j*4 + 4)) & 0x03) << j
            t1 |= ((val >> (j*4 + 6)) & 0x03) << j

# byte * hg3_unk_encode_tablefunc_FUN_0041a290(byte *param_1,int param_2,int param_3)

# {
#   byte bVar1;
#   int iVar2;
#   byte *pbVar3;
#   byte bVar4;
#   int iVar5;
#   int iVar6;
#   int iVar7;
#   byte *pbVar8;
  
#   std_vector_func_FUN_00409a50(param_1 + param_3,8);
#   iVar5 = (int)((param_3 + 3U & 0xfffffffc) + ((int)(param_3 + 3U) >> 0x1f & 3U)) >> 2;
#   param_3 = 6;
#   iVar2 = iVar5;
#   pbVar3 = (byte *)param_2;
#   pbVar8 = param_1;
#   do {
#     while (iVar2 != 0) {
# // bVar4 = 0
# // for j in range(0, 8, 2):
# //    bVar1 = pbVar8[i+j+0]
# //    bVar4 |= ((bVar1 >> (param_3 & 0x1f)) & 0x3) << (j & 0x1f)  & 0xff  # mask byte
# // pbVar3[i] = bVar4
   
#       bVar4 = 0;
#       iVar6 = 0;
#       do {
#         bVar1 = *pbVar8;
#         iVar7 = iVar6 + 2;
#         pbVar8 = pbVar8 + 1;
#         bVar4 = bVar4 | (bVar1 >> ((byte)param_3 & 0x1f) & 3) << ((byte)iVar6 & 0x1f);
#         iVar6 = iVar7;
#       } while (iVar7 < 8);
#       *pbVar3 = bVar4;
#       iVar2 = iVar2 + -1;
#       pbVar3 = pbVar3 + 1;
#     }
#     param_3 = param_3 + -2;
#     iVar2 = iVar5;
#     pbVar8 = param_1;
#   } while (-1 < param_3);
#   hg3_unk_copy_memstr_FUN_00409980(param_1,param_2,pbVar3 + -param_2);
#   return pbVar3 + -param_2;
# }
# while uVar3 < 0x100: #256:
#     uVar1 = ((((uVar3 & 0xc0) << 6 | uVar3 & 0x30) << 6 | uVar3 & 0xc) << 6 | uVar & 0x3)
#   do {
#     uVar1 = (((uVar3 & 0xc0) << 6 | uVar3 & 0x30) << 6 | uVar3 & 0xc) << 6 | uVar3 & 3;
#     piVar6[uVar3+0x000] = uVar1 << 6  # piVar6[-0x100] = uVar1 << 6;
#     piVar6[uVar3+0x100] = uVar1 << 4  # *piVar6 = uVar1 << 4;
#     piVar6[uVar3+0x200] = uVar1 << 2  # piVar6[0x100] = uVar1 * 4;
#     piVar6[uVar3+0x300] = uVar1       # piVar6[0x200] = uVar1;
#     uVar3 = uVar3 + 1;
#     piVar6 = piVar6 + 1;
#   } while ((int)uVar3 < 0x100);
#   iVar4 = 0;
#   cVar5 = -1;
#   puVar2 = param_1 + 0x1d;
#   do {
#     *(char *)puVar2 = (char)iVar4;
#     *(char *)((int)puVar2 + 1) = cVar5;
#     iVar4 = iVar4 + 1;
#     puVar2 = (undefined4 *)((int)puVar2 + 2);
#     cVar5 = cVar5 + -1;
#   } while (iVar4 < 0x80);
#   return param_1;

def encode_normalize(deltabuf):
    """
    Relations:
    * Normalizer 16
    * Normalization coefficient storage 22
    * Weight table storage 38
    * Absolute value generating circuit 48
    * Overflow sensor circuit 50

    ???
    * Amplitude sensor 24
    * Additional-bit computing section 26
    * NAND circuit 42
    * Inverter 44
    * Six-bit counter 46
    """
    #table1, table2, table3, table4 = init_norm_tables()
    #
    sectlen = len(deltabuf) // 4
    sect1 = 0
    sect2 = sectlen
    sect3 = sectlen*2
    sect4 = sectlen*3
    #
    rlebuf = bytearray(len(deltabuf))
    #
    # val  = i & 0xc0
    #
    # val <<= 6
    # val |= i & 0x30
    #
    # val <<= 6
    # val |= i & 0x0c
    #
    # val <<= 6
    # val |= i & 0x03
    #
    # table4[i] = val
    # table3[i] = val << 2
    # table2[i] = val << 4
    # table1[i] = val << 6
    # 
    # t4 |= b[0] & 0x03
    # t3 |= b[1] & 0x0c
    # t2 |= b[2] & 0x30
    # t1 |= b[3] & 0xc0

    # tt = 0
    # tt |= b[0] <<
    
    # t1 |= b[3] & 0xc0
    # t2 |= b[3] & 0x30 << 2
    # t3 |= b[3] & 0x0c << 4
    # t4 |= b[3] & 0x03 << 6
    # t1  = b[3] & 0xc0
    # t1 <<= 6
    # t1 |= b[2] & 0x30
    # t1 <<= 6
    #
    def norm_byte(c):
        #c = ((c << 1) & 0xfe) ^ (0xff if (c & 0x80) else 0x00)
        if c & 0x80:
            c ^= 0xff
            #c = (c ^ 0xff) << 1  # (will not push MSB of 1 into bit 8 due to inversion)
        c <<= 1
        # RETURNS 0x100 MSB on negative, ignore
        if c & 0x80:
            #return (c << 1) ^ 0xff
            return (c << 1) ^ 0x1ff
        else:
            return (c << 1)
        #c = (c << 1) ^ (0xff if (c & 0x80) else 0x00)

    for i in range(sectlen):
        #val = table1[unrlebuf[i+sect1]] | table2[unrlebuf[i+sect2]] | table3[unrlebuf[i+sect3]] | table4[unrlebuf[i+sect4]]
        #
        msks = (0xc0, 0x30, 0x0c, 0x03)
        wtbs = ((0x00, 0x40, 0x80, 0xc0),
                (0x00, 0x10, 0x20, 0x30),
                (0x00, 0x04, 0x08, 0x0c),
                (0x00, 0x01, 0x02, 0x03))
        msk1 = 0xc0
        msk2 = 0x30
        msk3 = 0x0c
        msk4 = 0x03
        wtb1 = (0x00, 0x40, 0x80, 0xc0)
        wtb2 = (0x00, 0x10, 0x20, 0x30)
        wtb3 = (0x00, 0x04, 0x08, 0x0c)
        wtb4 = (0x00, 0x01, 0x02, 0x03)
        i4 = i<<2
        val = 0
        val |= norm_val(deltabuf[i4+0]) <<  0
        val |= norm_val(deltabuf[i4+1]) <<  8
        val |= norm_val(deltabuf[i4+2]) << 16
        val |= norm_val(deltabuf[i4+3]) << 24
        idxs = [0, 0, 0, 0]
        vals = (norm_val(deltabuf[i4+0]),
                norm_val(deltabuf[i4+1]),
                norm_val(deltabuf[i4+2]),
                norm_val(deltabuf[i4+3]))
        j1 = j2 = j3 = j4 = 0
        for j in range(0, 4):
            idxs[0] = 
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

# input:byte -> returns:byte
def norm_val(c):
    return ((c << 1) ^ (0xff if (c & 0x80) else 0x00)) & 0xff
    # if c & 0x80:
    #     c ^= 0xff
    # return (c << 1)
    # RETURNS 0x100 MSB on negative, ignore
    # if c & 0x80:
    #     return ((c & 0x7f) << 1) ^ 0xff
    # else:
    #     return (c << 1)
    # return ((c << 1) & 0xfe) ^ (0xff if (c & 0x80) else 0x00)

def inv_norm_val(c):
    # 0x1,0xff may be related to negative delta values(?)
    # return ((c >> 1) & 0x7f) ^ (0xff if (c & 0x1) else 0x00)
    return ((c >> 1) ^ (0xff if (c & 0x1) else 0x00)) & 0xff
    ###return ((c & 1) ? ((c >> 1) ^ 0xFF) : (c >> 1)) & 0xff

def encode_zrle(deltabuf):
    """
    Relations:
    * Non-zero sensor 18
    * Zero-run counter 20
    * Additional-bit computing section 26
    * Fixed-length item generating buffer 30
    * Bit-length-computing-section 60
    * Buffer 62

    ???
    * Six-bit counter 46
    """
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


def encode_delta(buf, width, height, bitdepth): 
    bytedepth = (bitdepth + 7) >> 3  # // 8
    stride = (width * bytedepth + 3) & ~0x3
    #
    #outbuf = bytearray(len(buf))
    outbuf = bytearray(buf)
    #
    for xy in range(stride * height - 1, stride - 1, -1):
        outbuf[xy] = (outbuf[xy] - outbuf[xy - stride]) & 0xff
        #delta_val(outbuf, xy, xy - stride)
    #
    for x0 in range(stride - 1, bytedepth - 1, -1):
        outbuf[x0] = (outbuf[x0] - outbuf[x0 - bytedepth]) & 0xff
        #delta_val(outbuf, x0, x0 - bytedepth)
    #
    return outbuf

def decode_delta(buf, width, height, bitdepth):
    bytedepth = (bitdepth + 7) >> 3  # // 8
    stride = (width * bytedepth + 3) & ~0x3
    #
    #outbuf = bytearray(len(buf))
    outbuf = bytearray(buf)
    #
    for x0 in range(bytedepth, stride, 1):
        outbuf[x0] = (outbuf[x0] + outbuf[x0 - bytedepth]) & 0xff
        #inv_delta_val(outbuf, x0, x0 - bytedepth)
    #
    for xy in range(stride, stride * height, 1):
        outbuf[xy] = (outbuf[xy] + outbuf[xy - stride]) & 0xff
        #inv_delta_val(outbuf, xy, xy - stride)
    #
    return outbuf




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
    for x0 in range(stride - 1, bytedepth - 1, -1):
        delta_val(deltabuf, x0, x0 - bytedepth)
    #
    return deltabuf

def inv_process_delta(deltabuf, width, height, bitdepth):
    bytedepth = (bitdepth + 7) >> 3  # // 8
    stride = (width * bytedepth + 3) & ~0x3
    #stride = (((width * bitdepth + 7) >> 8) + 3) & ~3  # // 8) // 4 * 4
    #
    rgbabuf = deltabuf
    #
    for x0 in range(bytedepth, stride, 1):
        inv_delta_val(rgbabuf, x0, x0 - bytedepth)
    #
    for xy in range(stride, stride * height, 1): 
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

