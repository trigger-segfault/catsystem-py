#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Print the commands of compiled .anm script files
"""

__version__ = '1.0.0'
__date__    = '2020-10-09'
__author__  = 'Robert Jordan'

#######################################################################################

import io, os, struct, sys

COMMANDS = [
    '',
    'set ',
    'loop ',
    'jump ',
    'if ',
    'ife ',
    'ifn ',
    'ifg ',
    'ifs ',
    'ifge ',
    'ifse ',
    'max ',
    'blend ',
    'disp ',
    'pos ',
    'wait ',
    'add ',
    'sub ',
]

KINDS = [
    '',
    '@',
    'label_',
]

class AnmParam:
    def __init__(self, kind:int=0, value:int=0):
        self.kind  = kind
        self.value = value
    def __repr__(self):
        return '{0}{1}'.format(KINDS[self.kind], self.value)
class AnmLine:
    def __init__(self, command:int, params:list):
        self.command = command
        self.params  = params
    def __repr__(self):
        return '{0}{1}'.format(COMMANDS[self.command], ' '.join(repr(p) for p in self.params))
class AnmFile:
    def __init__(self, filename:str):
        self.signature = 'ANM'
        self.filename = filename
        self.lines = []
        with open(filename, 'rb') as f:
            self.signature = struct.unpack('<4s', f.read(4))[0].rstrip(b'\x00').decode('latin-1')
            self.unk1, self.line_count = struct.unpack('<iI', f.read(8))
            self.unk3, self.unk4, self.unk5, self.unk6, self.unk7 = struct.unpack('<5i', f.read(20))
            for i in range(self.line_count):
                command = struct.unpack('<I', f.read(4))[0]
                params = []
                for j in range(8):
                    kind, value = struct.unpack('<Ii', f.read(8))
                    params.append(AnmParam(kind, value))
                self.lines.append(AnmLine(command, params))
                print(self.lines[-1])
    def __repr__(self):
        return '<{0.signature} {0.filename!r} lines={1!r}>'.format(self, len(self.lines))
    def __len__(self):
        return len(self.lines)
    def __iter__(self):
        return iter(self.lines)
    def index(self, key):
        return self.lines.index(key)
    def __getitem__(self, key):
        return self.lines[key]
    def __setitem__(self, key, value):
        self.lines[key] = value
    def __delitem__(self, key):
        del self.lines[key]
    def __contains__(self, key):
        return key in self.lines


def main(args=None):
    if args is None: args = sys.argv[1:]

    filename = args[0]
    anmfile = AnmFile(filename)
    return 0

if __name__ == '__main__':
    exit(main())
