import struct, zlib, enum


#########################################################
## SOURCE: https://github.com/coreboot/bios_extract/blob/master/src/lh5_extract.c

LZHUFF5_DICBIT:int = 13  # 2^13 =  8KB sliding dictionary
MAXMATCH:int = 256  # formerly F (not more than 255 + 1)
THRESHOLD:int = 3  # choose optimal value
NP:int = (LZHUFF5_DICBIT + 1)
NT:int = (16 + 3)  # USHORT + THRESHOLD
NC:int = (255 + MAXMATCH + 2 - THRESHOLD)

PBIT:int = 4  # smallest integer such that (1 << PBIT) > * NP
TBIT:int = 5  # smallest integer such that (1 << TBIT) > * NT
CBIT:int = 9  # smallest integer such that (1 << CBIT) > * NC

#  #if NT > NP #define NPT NT #else #define NPT NP #endif
NPT:int = 0x80

# ushort[]
left:[int]  = [0]*(2 * NC - 1)
right:[int] = [0]*(2 * NC - 1)

# ushort[]
c_table:[int] = [0]*4096  # decode
pt_table:[int] = [0]*256  # decode

# uchar[]
c_len:bytearray = bytearray(NC)
pt_len:bytearray = bytearray(NPT)


## CRC Calculation.
CRCPOLY:int = 0xA001  # CRC-16 (x^16+x^15+x^2+1)

def CRC16Calculate(Buffer:bytes, BufferSize:int) -> int:
  # ushort[]
  CRCTable:[int] = [0]*0x100
  # ushort
  crc:int = 0
  # int
  i:int = 0
  # FIrst, initialise our CRCTable
  for i in range(0x100):
    r:int = i
    j:int = 0
    for j in range(8):
      if r & 0x1:
        r = (r >> 1) ^ CRCPOLY
      else:
        r >>= 1
    CRCTable[i] = r & 0xffff
  # now go over the entire Buffer
  crc = 0
  for i in range(BufferSize):
    crc = CRCTable[(crc ^ Buffer[i]) & 0xFF] ^ (crc >> 8)
  return crc & 0xffff

def le32toh(x:bytes) -> int:
  return struct.unpack('<I', x)[0]
def le16toh(x:bytes) -> int:
  return struct.unpack('<H', x)[0]

class LH5Header(object):
  """LH5HeaderParse(Buffer:bytes, BufferSize:int)"""
  #def LH5HeaderParse(self, Buffer:bytes, BufferSize:int, original_size:int=None, packed_size:int=None, name:str=None, crc:int=None):
  def __init__(self, Buffer:bytes, BufferSize:int):
    self.offset:int = 0
    self.header_size:int = 0
    self.checksum:int = 0
    self.name_length:int = 0
    self.name:str = None
    self.crc:int = 0
    self.original_size:int = 0
    self.packed_size:int = 0
    self.header_level:int = 0
    # Custom
    self.extend_size:int = 0
    if BufferSize < 27:
      raise ValueError("Error: Packed Buffer is too small to contain an lha header.")
    # check attribute
    if Buffer[19] != 0x20:
      raise ValueError("Error: Invalid lha header attribute byte.")
    # check method
    if Buffer[2:7] != b'-lh5-':
      raise ValueError("Error: Compression method is not LZHUFF5.")
    # check header level
    self.header_level = Buffer[20]
    if Buffer[20] != 1 and Buffer[20] != 0:
      raise ValueError("Error: Header level %d is not supported" % Buffer[20])
    # read in the full header
    self.header_size = Buffer[0]
    if BufferSize < (self.header_size + 2):
      raise ValueError("Error: Packed Buffer is too small to contain the full header.")
    # verify checksum
    self.checksum = Buffer[1]
    if self.calc_sum(Buffer[2:self.header_size+2], self.header_size) != self.checksum:
      raise ValueError("Error: Invalid lha header checksum.")
    self.packed_size = le32toh(Buffer[7:11])
    self.original_size = le32toh(Buffer[11:15])
    self.name_length = Buffer[21]
    self.name = Buffer[22:22+self.name_length]
    self.crc = le16toh(Buffer[22+self.name_length:22+self.name_length+2])
    self.offset = self.header_size + 2
    # Skip extended headers
    #while True:
    #if self.header_level == 0:
    #  self.offset = 
    # else:
    while self.header_level != 0:
      extend_size = le16toh(Buffer[self.offset-2:self.offset])
      self.extend_size += extend_size
      if not extend_size:  # extend_size == 0
        break
      self.packed_size -= extend_size
      self.offset += extend_size
      if BufferSize < self.offset:
        raise ValueError("Error: Buffer to small to contain extended header.")
    print('header_size=%d' % self.header_size)
    print('header_level=%d' % self.header_level)
    print('packed_size=%d' % self.packed_size)
    print('original_size=%d' % self.original_size)
    print('name_length=%d' % self.name_length)
    print('name="%s"' % self.name.decode('shiftjis'))
  def calc_sum(self, p:bytes, p_len:int) -> int:
    return sum(p[0:p_len]) & 0xff
    # p_sum:int = 0
    # for i in range(p_len):
    #   p_sum += p[i]
    #   #p_len -= 1
    # return p_sum & 0xff

# class LH5Header(object):
#   def __init__(self, Buffer:bytes=None, BufferSize:int=None):
#     self.offset:int = 0
#     self.header_size:int = 0
#     self.checksum:int = 0
#     self.name_length:int = 0
#     self.name:str = None
#     self.crc:int = 0
#     self.original_size:int = 0
#     self.packed_size:int = 0
#     if Buffer is not None and BufferSize is not None:
#       self.LH5HeaderParse(Buffer, BufferSize)
#   #def LH5HeaderParse(self, Buffer:bytes, BufferSize:int, original_size:int=None, packed_size:int=None, name:str=None, crc:int=None):
#   def LH5HeaderParse(self, Buffer:bytes, BufferSize:int):
#     self.offset:int = 0
#     self.header_size:int = 0
#     self.checksum:int = 0
#     self.name_length:int = 0
#     if BufferSize < 27:
#       raise ValueError("Error: Packed Buffer is too small to contain an lha header.")
#     # check attribute
#     if Buffer[19] != 0x20:
#       raise ValueError("Error: Invalid lha header attribute byte.")
#     # check method
#     if Buffer[2:7] != b'-lh5-':
#       raise ValueError("Error: Compression method is not LZHUFF5.")
#     # check header level
#     if Buffer[20] != 1:
#       raise ValueError("Error: Header level %d is not supported" % Buffer[20])
#     # read in the full header
#     self.header_size = Buffer[0]
#     if BufferSize < (self.header_size + 2):
#       raise ValueError("Error: Packed Buffer is too small to contain the full header.")
#     # verify checksum
#     self.checksum = Buffer[1]
#     if self.calc_sum(Buffer[2:self.header_size+2]) != self.checksum:
#       raise ValueError("Error: Invalid lha header checksum.")
#     self.packed_size = le32toh(Buffer[7:11])
#     self.original_size = le32toh(Buffer[11:15])
#     self.name_length = Buffer[21]
#     self.name = Buffer[22:22+self.name_length]
#     self.crc = le16toh(Buffer[22+self.name_length:22+self.name_length+2])
#     self.offset = self.header_size + 2
#     # Skip extended headers
#     while True:
#       extend_size = le16toh(Buffer[self.offset-2:self.offset])
#       if not extend_size:  # extend_size == 0
#         break
#       self.packed_size -= extend_size
#       self.offset += extend_size
#       if BufferSize < self.offset:
#         raise ValueError("Error: Buffer to small to contain extended header.")
#   def calc_sum(self, p:bytes, p_len:int) -> int:
#     return sum(p[0:p_len]) & 0xff
#     # p_sum:int = 0
#     # for i in range(p_len):
#     #   p_sum += p[i]
#     #   #p_len -= 1
#     # return p_sum & 0xff

## Bit handling code.

class BitBuf(object):
  """Bit handling code."""
  def __init__(self, Buffer:bytes, BufferSize:int):
    # uchar*
    self.CompressedBuffer:bytes = Buffer
    # int
    self.CompressedOffset:int = 0
    self.CompressedSize:int = BufferSize
    # ushort
    self.bitbuf:int = 0
    # uchar
    self.subbitbuf:int = 0  
    self.bitcount:int = 0
  def fillbuf(self, n:int): # Shift bitbuf n bits left, read n bits
    while n > self.bitcount:
      n -= self.bitcount
      self.bitbuf = (self.bitbuf << self.bitcount) + (self.subbitbuf >> (8 - self.bitcount))
      #self.bitbuf = ((self.bitbuf << self.bitcount) + (self.subbitbuf >> (8 - self.bitcount))) & 0xffff
      if self.CompressedOffset < self.CompressedSize:
        self.subbitbuf = self.CompressedBuffer[self.CompressedOffset]
        self.CompressedOffset += 1
      else:
        self.subbitbuf = 0
      self.bitcount = 8
    self.bitcount -= n
    self.bitbuf = (self.bitbuf << n) + (self.subbitbuf >> (8 - n))
    #self.bitbuf = ((self.bitbuf << n) + (self.subbitbuf >> (8 - n))) & 0xffff
    self.subbitbuf <<= n
    #self.subbitbuf = (self.subbitbuf << n) & 0xff
    self.bitbuf &= 0xffff
    self.subbitbuf &= 0xff
  def getbits(self, n:int) -> int:
    # ushort
    x:int = 0
    x = self.bitbuf >> (16 - n)
    self.fillbuf(n)
    return x & 0xffff
  def peekbits(self, n:int) -> int:
    # ushort
    x:int = 0
    x = self.bitbuf >> (16 - n)
    return x

class LH5Decoder(object):
  """LHA extraction."""
  def __init__(self, PackedBuffer:bytes, PackedBufferSize:int):
    # ## Bit handling code.
    # # uchar*
    # self.CompressedBuffer:bytes = None
    # # int
    # self.CompressedOffset:int = 0
    # self.CompressedSize:int = 0
    # # ushort
    # self.bitbuf:int = 0
    # # uchar
    # self.subbitbuf:int = 0  
    # self.bitcount:int = 0
    # ushort[]
    self.left:[int]  = [0]*(2 * NC - 1)
    self.right:[int] = [0]*(2 * NC - 1)
    # ushort[]
    self.c_table:[int] = [0]*4096  # decode
    self.pt_table:[int] = [0]*256  # decode
    # uchar[]
    self.c_len:bytearray = bytearray(NC)
    self.pt_len:bytearray = bytearray(NPT)
    #self.bbuf:BitBuf = None
    self.bbuf:BitBuf = BitBuf(PackedBuffer, PackedBufferSize)
    self.bbuf.fillbuf(2 * 8)

  #def LH5Decode(self, PackedBuffer:bytes, PackedBufferSize:int, OutputBuffer:bytearray, OutputBufferSize:int) -> int:
  def Decode(self, OutputBuffer:bytearray, OutputBufferSize:int) -> int:
    # ushort
    blocksize:int = 0
    # uint
    i:int = 0
    c:int = 0
    # int
    n:int = 0
    # see -> __init__
    #self.bbuf:BitBuf = BitBuf(PackedBuffer, PackedBufferSize)
    #self.bbuf.fillbuf(2 * 8)
    while n < OutputBufferSize:
      if blocksize == 0:
        blocksize = self.bbuf.getbits(16)
        if self.read_pt_len(NT, TBIT, 3) == -1:
          return -1
        if self.read_c_len() == -1:
          return -1
        if self.read_pt_len(NP, PBIT, -1) == -1:
          return -1
        # if self.read_pt_len(NT, TBIT, 3) == -1:
        #   return -1
      blocksize -= 1
      c = self.decode_c_st1()
      if c < 256:
        OutputBuffer[n] = c
        n += 1
      else:
        # int
        length:int = c - 256 + THRESHOLD
        offset:int = 1 + self.decode_p_st1()
        if offset > n:
          return -1
        for i in range(length):
          OutputBuffer[n] = OutputBuffer[n - offset]
          n += 1
    return 0
  def make_table(self, nchar:int, bitlen:bytearray, tablebits:int, table:[int]) -> int:
    # ushort[]
    count:[int]  = [0]*17  # count of bitlen
    weight:[int] = [0]*17  # 0x10000ul >> bitlen
    start:[int]  = [0]*17  # first code of bitlen
    # ushort
    total:int = 0
    # uint
    i:int = 0
    l:int = 0
    # int
    j:int = 0
    k:int = 0
    m:int = 0
    n:int = 0
    avail:int = 0
    # ushort*
    #p:bytes = b''
    p_ptr:[int] = None
    p_off:int = 0
    avail= nchar
    # initialize
    for i in range(1, 16+1):
      count[i] = 0
      weight[i] = (1 << (16 - i)) & 0xffff
    # count
    for i in range(nchar):
      if bitlen[i] > 16:
        # CVE-2006-4335
        raise ValueError("Error: Bad table (case a)")
      else:
        count[bitlen[i]] += 1
    # calculate first code
    total = 0
    #print('make_table()')
    for i in range(1, 16+1):
      start[i] = total & 0xffff
      total += weight[i] * count[i]
    if ((total & 0xffff) != 0) or (tablebits > 16):  # 16 for weight below
      raise ValueError("Error: make_table(): Bad table (case b) | (total & 0xffff) != 0) or (tablebits > 16) | total=%s, tablebits=%d" % (hex(total), tablebits))
    # shift data for make table
    m = 16 - tablebits
    for i in range(1, tablebits+1):
      start[i] >>= m
      weight[i] >>= m
    # initialize
    j = start[tablebits + 1] >> m
    k = min(1 << tablebits, 4096)
    if j != 0:
      for i in range(j, k):
        table[i] = 0
    # create table and tree
    for j in range(nchar):
      k = bitlen[j]
      if k == 0:
        continue
      l = start[k] + weight[k]
      if k <= tablebits:
        # code in table
        l = min(l, 4096)
        for i in range(start[k], l):
          table[i] = j
      else:
        # code not in table
        i = start[k]
        if (i >> m) > 4096:
          # CVE-2006-4337
          raise ValueError("Error: Bad table (case c)")
        # p = &table[i >> m]
        p_off = i >> m
        p_ptr = table
        i <<= tablebits
        n = k - tablebits
        # make tree (n length)
        while n > 0:  #while (--n >= 0)
          n -= 1
          if p_ptr[p_off] == 0:  # *p == 0
            self.right[avail] = self.left[avail] = 0
            p_ptr[p_off] = avail  # *p = avail++
            avail += 1
          if i & 0x8000:
            # p = &right[*p]
            p_off = p_ptr[p_off]
            p_ptr = self.right
          else:
            # p = &left[*p]
            p_off = p_ptr[p_off]
            p_ptr = self.left
          i <<= 1
        p_ptr[p_off] = j  # *p = j
      start[k] = l
    return 0
  def read_pt_len(self, nn:int, nbit:int, i_special:int) -> int:
    # int
    i:int = 0
    c:int = 0
    n:int = 0
    n = self.bbuf.getbits(nbit)
    if n == 0:
      c = self.bbuf.getbits(nbit)
      for i in range(nn):
        self.pt_len[i] = 0
      for i in range(256):
        self.pt_table[i] = c
    else:
      i = 0
      while i < min(n, NPT):
        c = self.bbuf.peekbits(3)
        if c != 7:
          self.bbuf.fillbuf(3)
        else:
          # ushort
          mask:int = 1 << (16 - 4)
          while mask & self.bbuf.bitbuf:
            mask >>= 1
            c += 1
          self.bbuf.fillbuf(c - 3)
        self.pt_len[i] = c
        i += 1
        if i == i_special:
          c = self.bbuf.getbits(2)
          while c > 0 and i < NPT:  # while (--c >= 0 && i < NPT)
            c -= 1
            self.pt_len[i] = 0
            i += 1
      while i < nn:
        self.pt_len[i] = 0
        i += 1
      if self.make_table(nn, self.pt_len, 8, self.pt_table) == -1:
        return -1
    return 0
  def read_c_len(self) -> int:
    # int
    i:int = 0
    c:int = 0
    n:int = 0
    n = self.bbuf.getbits(CBIT)
    if n == 0:
      c = self.bbuf.getbits(CBIT)
      for i in range(NC):
        self.c_len[i] = 0
      if i in range(4096):
        self.c_table[i] = c
    else:
      i = 0
      while i < min(n, NC):
        c = self.pt_table[self.bbuf.peekbits(8)]
        if c >= NT:
          # ushort
          mask:int = 1 << (16 - 9)
          firstdo:bool = True
          while (c >= NT and (mask or c != self.left[c])) or firstdo: # CVE-2006-4338
            firstdo = False
            if self.bbuf.bitbuf & mask:
              c = self.right[c]
            else:
              c = self.left[c]
            mask >>= 1
        self.bbuf.fillbuf(self.pt_len[c])
        if c <= 2:
          if c == 0:
            c = 1
          elif c == 1:
            c = self.bbuf.getbits(4) + 3
          else:
            c = self.bbuf.getbits(CBIT) + 20
          while c > 0:  # while (--c >= 0)
            c -= 1
            self.c_len[i] = 0
            i += 1
        else:
          self.c_len[i] = c - 2
          i += 1
      while i < NC:
        self.c_len[i] = 0
        i += 1
      if self.make_table(NC, self.c_len, 12, self.c_table) == -1:
        return -1
    return 0
  def decode_c_st1(self) -> int:  # ushort
    # ushort
    j:int = 0
    mask:int = 0
    j = self.c_table[self.bbuf.peekbits(12)]
    if j < NC:
      self.bbuf.fillbuf(self.c_len[j])
    else:
      self.bbuf.fillbuf(12)
      mask = 1 << (16 - 1)
      firstdo:bool = True
      while (j >= NC and (mask or j != self.left[j])) or firstdo:  # CVE-2006-4338
        firstdo = False
        if self.bbuf.bitbuf & mask:
          j = self.right[j]
        else:
          j = self.left[j]
        mask >>= 1
      self.bbuf.fillbuf(self.c_len[j] - 12)
    return j
  def decode_p_st1(self) -> int:  # ushort
    # ushort
    j:int = 0
    mask:int = 0
    j = self.pt_table[self.bbuf.peekbits(8)]
    if j < NP:
      self.bbuf.fillbuf(self.pt_len[j])
    else:
      self.bbuf.fillbuf(8)
      mask = 1 << (16 - 1)
      firstdo:bool = True
      while (j >= NP and (mask or j != self.left[j])) or firstdo:  # CVE-2006-4338
        firstdo = False
        if self.bbuf.bitbuf & mask:
          j = self.right[j]
        else:
          j = self.left[j]
        mask >>= 1
      self.bbuf.fillbuf(self.pt_len[j] - 8)
    if j != 0:
      j = (1 << (j - 1)) + self.bbuf.getbits(j - 1)
    return j
  # def LH5HeaderParse(self, Buffer:bytes, BufferSize:int, original_size:int, packed_size:int, name:str, crc:int):
  #   offset:int = 0
  #   header_size:int = 0
  #   checksum:int = 0
  #   name_length:int = 0
  #   if BufferSize < 27:
  #     raise ValueError("Error: Packed Buffer is too small to contain an lha header.")
  #   # check attribute
  #   if Buffer[19] != 0x20:
  #     raise ValueError("Error: Invalid lha header attribute byte.")
  #   # check method
  #   if Buffer[2:7] != b'-lh5-':
  #     raise ValueError("Error: Compression method is not LZHUFF5.")
  #   # check header level
  #   if Buffer[20] != 1:
  #     raise ValueError("Error: Header level %d is not supported" % Buffer[20])
  #   # read in the full header
  #   header_size = Buffer[0]
  #   if BufferSize < (header_size + 2):
  #     raise ValueError("Error: Packed Buffer is too small to contain the full header.")
  #   # verify checksum
  #   checksum = Buffer[1]
  #   if self.calc_sum(Buffer[2:header_size+2]) != checksum:
  #     raise ValueError("Error: Invalid lha header checksum.")
  #   packed_size = le32toh(Buffer[7:11])
  #   original_size = le32toh(Buffer[11:15])
  #   name_length = Buffer[21]
  #   name = Buffer[22:22+name_length]
  #   crc = le16toh(Buffer[22+name_length:22+name_length+2])
  #   offset = header_size + 2
  #   # Skip extended headers
  #   while True:
  #     extend_size = le16toh(Buffer[offset-2:offset])
  #     if not extend_size:  # extend_size == 0
  #       break
  #     packed_size -= extend_size
  #     offset += extend_size
  #     if BufferSize < offset:
  #       raise ValueError("Error: Buffer to small to contain extended header.")

def extract_all(argc:int, argv:[str]):
  import os
  filename:str = None
  # ushort
  header_crc:int = 0
  # uint
  header_size:int = 0
  original_size:int = 0
  packed_size:int = 0
  # int
  #infd:int = 0
  #outfd:int = 0
  LHABufferSize:int = 0
  # uchar*
  LHABuffer:bytes
  OutBuffer:bytearray
  if argc < 2:
    #raise ValueError("Error: archive file not specified")
    print("Error: archive file not specified")
    return 1
  # open archive file
  entries=[]
  with open(argv[1], mode='rb') as infd:
    import os
    arcname:str = os.path.split(argv[1])[1]
    arcname_noext:str = os.path.splitext(arcname)[0]
    #basedir:str = './extracted-lzh/'+arcname
    basedir:str = 'C:/Users/Trigger/Downloads/LHA_EXTRACTED/'+arcname
    print(basedir)
    listmode:bool = False
    if argc >= 3:
      if argv[2] == '-l':
        listmode = True
    if not os.path.isdir(basedir) and not listmode:
      os.makedirs(basedir)

    start_pos:int = 0
    LHABuffer = infd.read(21)
    if len(LHABuffer) == 0:
      print("Error: empty lzh file %s" % argv[1])
      return 1
    has_entry:bool = LHABuffer[0] != 0
    while has_entry:
      from catsys.archive.lzh.LHAHeader import LHAEntry
      header = LHAEntry(parent=None)
      needed:int = header.read_inc(LHABuffer, encoding='shiftjis', parent=None)
      #print (needed)
      while needed != 0:
        added_buf = infd.read(needed)
        if len(added_buf) < needed:
          print("Error: read file returned smaller buffer size %d than needed %d" % (len(added_buf), needed))
          return 1
        LHABuffer += added_buf
        needed = header.read_inc(LHABuffer, encoding='shiftjis', parent=None)
        #print (needed)
      #print("FOUND ENTRY: %s lv-%d %s (%d, %d) @ 0x%x" % (header.methodid, header.level, header.name, header.packed_size, header.original_size, header.lha_timestamp))
      print("FOUND ENTRY: %s lv-%d \"%s\" (%d -> %d)  %s" % (header.methodid, header.level, header.name, header.packed_size, header.original_size, header.timestamp))
      # basedir:str = './extracted-lzh'
      # if not os.path.isdir(basedir):
      #   os.mkdir(basedir)
      entries.append(header)
      if not listmode:
        if infd.tell() != start_pos + header.data_offset:
          infd.seek(start_pos + header.data_offset)
        LHABuffer = infd.read(header.packed_size)
        if len(LHABuffer) < header.packed_size:
            print("Error: Buffer size %d too small for compressed file %d" % (len(LHABuffer), header.packed_size))
            return 1
          
        if header.methodid == '-lh0-' or header.methodid == '-lzs-':
          print('No compression on file %s' % header.name)
          OutBuffer = LHABuffer
        else:
          print('%s compression on file %s' % (header.methodid, header.name))
          OutBuffer = bytearray(header.original_size)
          decoder = LH5Decoder(LHABuffer, header.packed_size) #, OutBuffer, header.original_size)
          decoder.Decode(OutBuffer, header.original_size)

        filepath:str = os.path.join(basedir, header.name)
        if CRC16Calculate(OutBuffer, header.original_size) != header.crc:
          print("Warning: invalid CRC on \"%s\"" % header.name)
        entrydir, entryname = os.path.split(filepath)
        if not os.path.isdir(entrydir):
          os.makedirs(entrydir)
        with open(filepath, mode='wb+') as outfd:
          outfd.write(OutBuffer)
        if header.timestamp is not None:
          ts = header.timestamp.timestamp()
          os.utime(filepath, (ts, ts))
        print("Decoded file %s" % header.name)

      start_pos = start_pos+header.total_size
      #infd.seek(start_pos+header.total_size)
      infd.seek(start_pos)
      #start_pos
      # NEW BUFFER:
      LHABuffer = infd.read(21)
      if len(LHABuffer) == 0:
        print("Error: Expected at least one byte after entry")
        return 1
      else:
        has_entry = LHABuffer[0] != 0
      #continue
  return 0
  #     #with open(filepath, mode='wb+') as outfd:
  #       # grow file
  #       #outfd.seek(original_size-1)
  #       #outfd.write(b'\x00')
  #     # OutBuffer = bytearray(header.original_size)
  #     # decoder = LH5Decoder(LHABuffer[data_offset:], packed_size) #, OutBuffer, original_size)
  #     # decoder.Decode(OutBuffer, original_size)
      
  #     header.read(LHABuffer, encoding='shiftjis', parent=None)
  #     # THIS IS OFFSET, not header_size field
  #     #header_size = header.offset
  #     data_offset = header.data_offset
  #     original_size = header.original_size
  #     packed_size = header.packed_size
  #     filename = header.name#.decode('shift_jis')
  #     header_crc = header.crc
  #     if not data_offset:
  #       return 1
  #     print((data_offset + packed_size, LHABufferSize))
  #     #WHY IS THIS AN ERROR?
  #     if (data_offset + packed_size) < LHABufferSize:
  #       print("Error: LHA archive is bigger than \"%s\"" % argv[1])
  #       return 1
  #     filepath:str = os.path.join(basedir, header.name)
  #     #with open(filepath, mode='wb+') as outfd:
  #       # grow file
  #       #outfd.seek(original_size-1)
  #       #outfd.write(b'\x00')
  #     OutBuffer = bytearray(original_size)
  #     decoder = LH5Decoder(LHABuffer[data_offset:], packed_size) #, OutBuffer, original_size)
  #     decoder.Decode(OutBuffer, original_size)
      
  #   if CRC16Calculate(OutBuffer, header.original_size) != header.crc:
  #     print("Warning: invalid CRC on \"%s\"" % header.name)
  #   with open(filepath, mode='wb+') as outfd:
  #     outfd.write(OutBuffer)
  #   print("Decoded file %s" % filename)

  #   LHABufferOffset = 0x565
  #   LHABufferEnd = 0x74D1
  #   LHABufferSize = LHABufferEnd - LHABufferOffset
  #   infd.seek(LHABufferOffset)
  #   LHABuffer = infd.read(LHABufferEnd - LHABufferOffset)
  #   #LHABufferSize = os.path.getsize(argv[1])
  #   #LHABuffer = f.read()
  # #### NEW ####
  # from catsys.archive.lzh.LHAHeader import LHAEntry

  # header = LHAEntry(parent=None)
  # header.read(LHABuffer, encoding='shiftjis', parent=None)
  # # THIS IS OFFSET, not header_size field
  # #header_size = header.offset
  # data_offset = header.data_offset
  # original_size = header.original_size
  # packed_size = header.packed_size
  # filename = header.name#.decode('shift_jis')
  # header_crc = header.crc
  # if not data_offset:
  #   return 1
  # print((data_offset + packed_size, LHABufferSize))
  # #WHY IS THIS AN ERROR?
  # if (data_offset + packed_size) < LHABufferSize:
  #   print("Error: LHA archive is bigger than \"%s\"" % argv[1])
  #   return 1
  # basedir:str = './extracted-lzh'
  # import os
  # if not os.path.isdir(basedir):
  #   os.mkdir(basedir)
  # filepath:str = os.path.join(basedir, filename)
  # #with open(filepath, mode='wb+') as outfd:
  #   # grow file
  #   #outfd.seek(original_size-1)
  #   #outfd.write(b'\x00')
  # OutBuffer = bytearray(original_size)
  # decoder = LH5Decoder(LHABuffer[data_offset:], packed_size) #, OutBuffer, original_size)
  # decoder.Decode(OutBuffer, original_size)

  # if CRC16Calculate(OutBuffer, original_size) != header_crc:
  #   print("Warning: invalid CRC on \"%s\"" % filename)
  # with open(filepath, mode='wb+') as outfd:
  #   outfd.write(OutBuffer)
  # print("Decoded file %s" % filename)
  # return 0


#######################
## SOURCE: https://github.com/coreboot/bios_extract/blob/effb120babde7e351c253f7c485848c8ce455dc0/src/lh5_test.c
def main(argc:int, argv:[str]):
  filename:str = None
  # ushort
  header_crc:int = 0
  # uint
  header_size:int = 0
  original_size:int = 0
  packed_size:int = 0
  # int
  #infd:int = 0
  #outfd:int = 0
  LHABufferSize:int = 0
  # uchar*
  LHABuffer:bytes
  OutBuffer:bytearray
  if argc != 2:
    #raise ValueError("Error: archive file not specified")
    print("Error: archive file not specified")
    return 1
  # open archive file
  with open(argv[1], mode='rb') as infd:
    LHABufferOffset = 0x565
    LHABufferEnd = 0x74D1
    LHABufferSize = LHABufferEnd - LHABufferOffset
    infd.seek(LHABufferOffset)
    LHABuffer = infd.read(LHABufferEnd - LHABufferOffset)
    #LHABufferSize = os.path.getsize(argv[1])
    #LHABuffer = f.read()
  #### NEW ####
  from catsys.archive.lzh.LHAHeader import LHAEntry

  header = LHAEntry(parent=None)
  header.read(LHABuffer, encoding='shiftjis', parent=None)
  # THIS IS OFFSET, not header_size field
  #header_size = header.offset
  data_offset = header.data_offset
  original_size = header.original_size
  packed_size = header.packed_size
  filename = header.name#.decode('shift_jis')
  header_crc = header.crc
  if not data_offset:
    return 1
  print((data_offset + packed_size, LHABufferSize))
  #WHY IS THIS AN ERROR?
  if (data_offset + packed_size) < LHABufferSize:
    print("Error: LHA archive is bigger than \"%s\"" % argv[1])
    return 1
  basedir:str = './extracted-lzh'
  import os
  if not os.path.isdir(basedir):
    os.mkdir(basedir)
  filepath:str = os.path.join(basedir, filename)
  #with open(filepath, mode='wb+') as outfd:
    # grow file
    #outfd.seek(original_size-1)
    #outfd.write(b'\x00')
  OutBuffer = bytearray(original_size)
  decoder = LH5Decoder(LHABuffer[data_offset:], packed_size) #, OutBuffer, original_size)
  decoder.Decode(OutBuffer, original_size)

  if CRC16Calculate(OutBuffer, original_size) != header_crc:
    print("Warning: invalid CRC on \"%s\"" % filename)
  with open(filepath, mode='wb+') as outfd:
    outfd.write(OutBuffer)
  print("Decoded file %s" % filename)
  return 0

  #### OLD ####
  header = LH5Header(LHABuffer, LHABufferSize)
  # THIS IS OFFSET, not header_size field
  header_size = header.offset
  original_size = header.original_size
  packed_size = header.packed_size
  filename = header.name.decode('shift_jis')
  header_crc = header.crc
  if not header_size:
    return 1
  print((header_size + packed_size, LHABufferSize))
  #WHY IS THIS AN ERROR?
  if (header_size + packed_size) < LHABufferSize:
    print("Error: LHA archive is bigger than \"%s\"" % argv[1])
    return 1
  basedir:str = './extracted-lzh'
  import os
  if not os.path.isdir(basedir):
    os.mkdir(basedir)
  filepath:str = os.path.join(basedir, filename)
  #with open(filepath, mode='wb+') as outfd:
    # grow file
    #outfd.seek(original_size-1)
    #outfd.write(b'\x00')
  OutBuffer = bytearray(original_size)
  decoder = LH5Decoder(LHABuffer[header_size:], packed_size) #, OutBuffer, original_size)
  decoder.Decode(OutBuffer, original_size)

  if CRC16Calculate(OutBuffer, original_size) != header_crc:
    print("Warning: invalid CRC on \"%s\"" % filename)
  with open(filepath, mode='wb+') as outfd:
    outfd.write(OutBuffer)
  print("Decoded file %s" % filename)
  return 0
if __name__ == '__main__':
  import sys
  print((len(sys.argv), sys.argv))
  #main(len(sys.argv), sys.argv)
  extract_all(len(sys.argv), sys.argv)
  #main(sys.argv)
