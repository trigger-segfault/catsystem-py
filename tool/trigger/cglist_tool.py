#!/usr/bin/env python3
#-*- coding: utf-8 -*-
"""Decompile or compile CatSystem 2 CG list table `cglist.dat` files.
"""

__version__ = '1.0.0'
__date__    = '2021-10-05'
__author__  = 'Robert Jordan'

__all__ = ['CGList', 'CGTable', 'CGFrame', 'main']

#######################################################################################

import io, os, re
from struct import Struct
from typing import Dict, List, Optional, Set, Tuple, Union


#######################################################################################

#region ## HELPERS ##

def cstring(data:bytes, start:Optional[int]=None, end:Optional[int]=None, *, explicit:bool=True, encoding:Optional[str]=None) -> Union[bytes,str]:
    """cstring(data[, start[, end]], *, explicit=True) -> bytes
    cstring(data[, start[, end]], *, explicit=True, encoding) -> str

    arguments:
      - explicit : bool - when True, a null terminator MUST be found.
      - encoding : str  - when None, bytes is returned.
    """
    if encoding is None: terminator = b'\x00'
    else:                terminator = '\x00'.encode(encoding)

    idx = (data.index if explicit else data.find)(terminator,start,end)
    s = data[start : (idx if idx != -1 else end)]

    if encoding is not None:
        s = s.decode(encoding)
    return s

#endregion

#######################################################################################

#region ## CLASSES ##

class CGFrame(List[str]):
    name:str

    def __init__(self, name:str, *args, **kwargs):
        self.name = name
        super().__init__(*args, **kwargs)

    def astuple(self) -> Tuple[str,List[str]]: return (self.name, list(self))

    def __str__(self) -> str:
        return f'<{self.__class__.__name__}: {self.name!r} files={list(self)}>'
    def __repr__(self) -> str:
        return f'<{self.__class__.__name__}: {self.name!r} files={len(self)}>'
    

class CGTable(List[CGFrame]):
    number:int

    @property
    def name(self) -> str: return f'@{self.number}'

    def __init__(self, number:int, *args, **kwargs):
        self.number = number
        super().__init__(*args, **kwargs)

    def asdict(self) -> Dict[str,CGFrame]: return dict((f.name,f) for f in self)

    def __str__(self) -> str:
        return f'<{self.__class__.__name__}: {self.name!r} frames={[f.name for f in self]}>'
    def __repr__(self) -> str:
        return f'<{self.__class__.__name__}: {self.name!r} frames={len(self)}>'


class CGList(List[CGTable]):
    STRICT:bool  = True     # default strict setting
    COMPACT:bool = False    # default compact setting
    ENCODING:str = 'cp932'  # default encoding setting
    # Cs2 generally always handles integers as signed, so we will do the same I guess
    # offsetNext, tableNum, frameCount
    _TABLE:Struct = Struct('<i  i  i')
    # offsetNext, frameName, fileCount
    _FRAME:Struct = Struct('<i 64s i')
    # fileSpec,
    _FILESPEC:Struct = Struct('<64s')
    # _TABLE structure for end of file
    _END:Tuple[int,int,int] = (0, 0, 0)

    filename:Optional[str]

    @property
    def name(self) -> str: return os.path.basename(self.filename) if self.filename is not None else ''

    def __init__(self, file:Optional[Union[str,bytes,io.BufferedReader]]=None, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs):
        self.filename = None
        super().__init__(*args, **kwargs)

        if file is not None:
            if isinstance(file, str):     # file name
                self.loadf(file, *args, strict=strict, **kwargs)
            elif isinstance(file, bytes): # file data
                self.loads(file, *args, strict=strict, **kwargs)
            else:                         # file-like
                self.load(file, *args, strict=strict, encoding=encoding, **kwargs)

    def asdict(self) -> Dict[int,CGTable]: return dict((m.number,m) for m in self)

    def __str__(self) -> str:
        name = f'{self.filename!r}' if self.filename else ''
        return f'<{self.__class__.__name__}: {name} tables={[m.name for m in self]}>'
    def __repr__(self) -> str:
        name = f'{self.filename!r}' if self.filename else ''
        return f'<{self.__class__.__name__}: {name} tables={len(self)}>'


    def loadf(self, filename:str, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs) -> None:
        with open(filename, 'rb') as reader:
            self.load(reader, *args, strict=strict, encoding=encoding, filename=filename, **kwargs)

    def loads(self, data:bytes, *args, strict:bool=STRICT, encoding:str=ENCODING, filename:Optional[str]=None, **kwargs) -> None:
        with io.BytesIO(data) as reader:
            self.load(reader, *args, strict=strict, encoding=encoding, filename=filename, **kwargs)

    def load(self, reader:io.BufferedReader, *args, strict:bool=STRICT, encoding:str=ENCODING, filename:Optional[str]=None, altmode:bool=False, **kwargs) -> None:
        _TABLE,_FRAME,_FILESPEC,_END = self._TABLE,self._FRAME,self._FILESPEC,self._END

        self.filename = filename
        self.clear()

        tableBase = reader.tell()
        tableNext = 0
        if altmode:
            frameBase = tableBase + _TABLE.size
            frameNext = 0
        # Always assume at least one table(?)
        while not len(self) or tableNext:
            tableBase = (tableBase + tableNext)
            reader.seek(tableBase, io.SEEK_SET)  # always use offsetNext over current file position

            tableNext,tableNum,frameCount = _TABLE.unpack(reader.read(_TABLE.size))

            table = CGTable(tableNum)
            self.append(table)

            # We can either perform this loop by count, or by offsetNext.
            if not altmode:
                frameBase = tableBase + _TABLE.size
                frameNext = 0
            else:
                pass # otherwise we keep using the last frameNext value from any previous tables

            for j in range(frameCount):
                if strict:
                    if not altmode:
                        if len(table) and not frameNext:
                            raise ValueError(f'Strict: [Table {table.name}] Number of frame offsets ({len(table)}) less than expected frameCount ({frameCount})')
                    else:
                        if len(self) and len(self[0]) and not frameNext:
                            raise ValueError(f'Strict: [Table {table.name}] Number of frame offsets ({len(table)}) less than expected frameCount ({frameCount})')

                frameBase = (frameBase + frameNext)
                reader.seek(frameBase, io.SEEK_SET)  # always use offsetNext over current file position

                frameNext,frameName,fileCount = _FRAME.unpack(reader.read(_FRAME.size))

                frame = CGFrame(cstring(frameName, encoding=encoding))
                table.append(frame)

                for k in range(fileCount):
                    fileSpec, = _FILESPEC.unpack(reader.read(_FILESPEC.size))

                    file = cstring(fileSpec, encoding=encoding)
                    frame.append(file)


        if strict and frameNext:
            raise ValueError(f'Strict: Expected zero frame next value at end of last table, not {frameNext}')
        
        # File ends with empty table entry, strangely...
        tableHdr = _TABLE.unpack(reader.read(_TABLE.size))
        if strict and tableHdr != _END:
            raise ValueError(f'Strict: Expected empty table header at end of file, not {tableHdr}')

    def dumpf(self, filename:str, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs) -> None:
        with open(filename, 'wb') as writer:
            self.dump(writer, *args, strict=strict, encoding=encoding, **kwargs)
            writer.flush()

    def dumps(self, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs) -> bytes:
        with io.BytesIO() as writer:
            self.dump(writer, *args, strict=strict, encoding=encoding, **kwargs)
            writer.flush()
            return writer.getvalue()

    def dump(self, writer:io.BufferedWriter, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs) -> None:
        _TABLE,_FRAME,_FILESPEC,_END = self._TABLE,self._FRAME,self._FILESPEC,self._END

        if strict and not len(self): raise ValueError('Strict: Cannot write CGList with 0 tables')

        for i,table in enumerate(self):
            frameSizes = [(_FRAME.size + len(f) * _FILESPEC.size) for f in table]
            tableSize = _TABLE.size + sum(frameSizes)

            tableNext = tableSize if (i+1 < len(self)) else 0
            writer.write(_TABLE.pack(tableNext, table.number, len(table)))

            for j,frame in enumerate(table):
                # frameNext is non-zero until the final frame of the final table
                frameNext = frameSizes[j] if (tableNext or j+1 < len(table)) else 0

                if tableNext and j+1 == len(table): # Account for distance across table headers.
                    ii = i+1
                    while ii < len(self) and not len(self[ii]):
                        ii += 1
                    if ii == len(self):
                        frameNext = 0  # No more frames after this, it's all empty tables from here.
                    else:
                        frameNext += _TABLE.size * (ii - i) # Number of empty tables and final non-empty table.

                writer.write(_FRAME.pack(frameNext, frame.name.encode(encoding), len(frame)))

                for k,file in enumerate(frame):
                    writer.write(_FILESPEC.pack(file.encode(encoding)))
        
        # File always ends with dummy empty table header.
        # No idea why, when we use find-by-offset for tables...
        writer.write(_TABLE.pack(*_END))


    def compilef(self, filename:str, *args, strict:bool=STRICT, encoding:str=ENCODING, **kwargs) -> None:
        with open(filename, 'rt', encoding=encoding) as reader:
            self.compile(reader, *args, strict=strict, filename=filename, **kwargs)

    def compiles(self, text:str, *args, strict:bool=STRICT, filename:Optional[str]=None, **kwargs) -> None:
        with io.StringIO(text) as reader:
            self.compiles(reader, args, strict=strict, filename=filename, **kwargs)

    def compile(self, reader:io.TextIOWrapper, *args, strict:bool=STRICT, filename:Optional[str]=None, **kwargs) -> None:
        self.filename = filename
        self.clear()

        text = reader.read()
        def getlinenum(idx): return text.count('\n', 0, idx) + 1

        # Normalize newlines and filter out comments.
        # CRLF / CR => LF
        text.replace('\r\n', '\n').replace('\r', '\n')
        # //..... => ''
        text = re.sub(r"\/\/.*$", "", text, flags=re.MULTILINE)
        # /*...*/ => '(\n)' (with preserved newline count)
        text = re.sub(r"\/\*.*?\*\/", lambda m: '\n' * m[0].count('\n'), text, flags=0) #re.MULTILINE)

        # Check for bad block comment syntax.
        idx = text.find('*/')
        if idx != -1: raise ValueError(f'Unmatched closing block comment "*/" on line {getlinenum(idx)}')
        idx = text.find('/*')
        if idx != -1: raise ValueError(f'Unmatched opening block comment "/*" on line {getlinenum(idx)}')

        # Condense all whitespace into single spaces and trim edges.
        text = re.sub(r"[ \t]+", " ", text, flags=re.MULTILINE)

        table = frame = frameLast = None
        lineNum = 0
        for line in text.split('\n'):
            lineNum += 1
            line = line.strip()
            if not line: continue  # ignore empty lines

            c = line[0]
            if c == '@':
                try:
                    tableNum = int(line[1:], 0)
                except ValueError:
                    raise ValueError(f'Invalid table number, {line[1:]!r} is not an integer on line {lineNum}')

                table = CGTable(tableNum)
                if strict:
                    frameLast = frame
                    frame = None

                self.append(table)
            elif c == '[':
                if line[-1] != ']':
                    raise ValueError(f'Unmatched closing "]" on line {lineNum}')
                if table is None:
                    raise ValueError(f'Expected table "@#" before frame "[name]" on line {lineNum}')

                frameName = line[1:-1]
                if ']' in frameName:
                    raise ValueError(f'Invalid character, "]" within frame "[name]" on line {lineNum}')
                elif strict and '[' in frameName:
                    raise ValueError(f'Strict: Invalid character, "[" within frame "[name]" on line {lineNum}')

                frame = CGFrame(frameName)
                table.append(frame)
            else:
                if frame is None:
                    if strict and frameLast is not None:
                        raise ValueError(f'Strict: Expected frame "[name]" before file spec and after table "@#" on line {lineNum}')
                    else:
                        raise ValueError(f'Expected frame "[name]" before file spec on line {lineNum}')

                frame.append(line)


    def decompilef(self, filename:str, *args, compact:bool=COMPACT, encoding:str=ENCODING, **kwargs) -> None:
        with open(filename, 'wt', encoding=encoding) as writer:
            self.decompile(writer, *args, compact=compact, **kwargs)
            writer.flush()

    def decompiles(self, *args, compact:bool=COMPACT, **kwargs) -> str:
        with io.StringIO() as writer:
            self.decompile(writer, *args, compact=compact, **kwargs)
            writer.flush()
            return writer.getvalue()

    def decompile(self, writer:io.TextIOWrapper, *args, compact:bool=COMPACT, **kwargs) -> None:
        # All empty lines are non-mandatory
        for table in self:
            writer.write(f'@{table.number}\n')
            if not compact: writer.write('\n') # empty line after table declaration

            for frame in table:
                writer.write(f'[{frame.name}]\n')
                for file in frame:
                    writer.write(f'{file}\n')

                if not compact: writer.write('\n') # empty line after end of frame

            if not compact: writer.write('\n') # another empty line after end of table


#endregion


#######################################################################################

#region ## MAIN FUNCTION ##

def main(argv:Optional[list]=None) -> int:
    import os

    #region ## PARSER SETUP ##
    import argparse
    parser = argparse.ArgumentParser(
        description='Read and write CatSystem 2 cglist.dat and cglist.txt files',
        add_help=True)
    
    igroup = parser.add_argument_group('convert mode')
    imgroup = igroup.add_mutually_exclusive_group(required=True)
    imgroup.add_argument('-d', '--decompile', nargs=2, metavar=('DATFILE','TXTFILE'), default=None, help='decompile cglist data file to text script')
    imgroup.add_argument('-c', '--convert', nargs=2, metavar=('TXTFILE','DATFILE'), default=None, help='convert cglist text script to data file')

    fgroup = parser.add_mutually_exclusive_group(required=False)
    fgroup.add_argument('-p', '--print', action='store_true', default=False, help='don\'t write file and print to console')
    fgroup.add_argument('-f', '--same-format', dest='same_format', action='store_true', default=False, help='(debug) output file as same input format')

    parser.add_argument('-q', '--quiet', action='store_true', default=False, help='don\'t print actions being performed')
    parser.add_argument('-S', '--unstrict', dest='strict', action='store_false', default=True, help='loose parsing of file formats')
    parser.add_argument('-C', '--compact', action='store_true', default=False, help='compact output mode for cglist.txt')
    parser.add_argument('-A', '--altmode', action='store_true', default=False, help='(debug) alternate input mode for cglist.dat')
    parser.add_argument('-T', '--test', action='store_true', default=False, help='test tool without saving any changes')
    #endregion

    ###########################################################################

    #region ## PARSER VALIDATION ##

    args = parser.parse_args(argv)

    if args.decompile:
        input, output = args.decompile
        inmode = CGList.loadf
        outmode = (CGList.decompilef if not args.same_format else CGList.dumpf)
    else: #if args.convert:
        input, output = args.convert
        inmode = CGList.compilef
        outmode = (CGList.decompilef if args.same_format else CGList.dumpf)

    printmode = args.print
            
    test = args.test
    verbose = args.test or not args.quiet
    altmode = args.altmode
    compact = args.compact
    strict  = args.strict

    #endregion

    ###########################################################################

    if verbose: print(f'Loading: {input}')
    cglist = CGList()
    inmode(cglist, input, strict=strict, altmode=altmode, compact=compact)

    if output:
        if verbose: print(f'Saving:  {output}')
        if printmode:
            print(cglist.decompiles(strict=strict, altmode=altmode, compact=compact).rstrip('\r\n'))
        elif not test:
            outmode(cglist, output, strict=strict, altmode=altmode, compact=compact)

    if verbose: print('Done!')

    return 0

#endregion


###########################################################################

del Optional  # cleanup declaration-only imports


## MAIN CONDITION ##

if __name__ == '__main__':
    exit(main())
