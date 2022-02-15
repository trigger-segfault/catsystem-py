#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CatSystem 2 key generation utility
"""

__version__ = '0.0.1'
__date__    = '2022-02-15'
__author__  = 'Robert Jordan'

#######################################################################################

import os

from catsys.crypt import read_cs2codes, keyfile_generate, globalkey_read, globalkey_build, globalkey_generate, vcode_seed
from catsys.crypt import KEY_KEY, KEY_DEBUG, KEY_DIRECT, KEY_KEYCOM, KEY_GK

#######################################################################################

#region ## MAIN FUNCTION ##

def main(argv:list=None):

    #region ## KEY TYPE INFO ##
    KEY_EXT = '.dat'

    TYPE_DEBUG  = 'debug'
    TYPE_DIRECT = 'direct'
    TYPE_CD     = 'cd'
    TYPE_COMMON = 'common'
    TYPE_GLOBAL = 'global'

    KEY_TYPE_MAP = {
        TYPE_DEBUG: (TYPE_DEBUG,  KEY_DEBUG,  "Enables debug mode features."),
        TYPE_DIRECT:(TYPE_DIRECT, KEY_DIRECT, "Standard key format (startup.xml: <direct>1</direct>)"),
        TYPE_CD:    (TYPE_CD,     KEY_KEY,    "CD key format       (startup.xml: <direct>0</direct>)"),
        TYPE_COMMON:(TYPE_COMMON, KEY_KEYCOM, "Old key format, predates direct/cd types"),
        TYPE_GLOBAL:(TYPE_GLOBAL, KEY_GK,     "UNSUPPORTED! Hashes of exes to prevent tampering"),
    }
    TYPE_LIST = (TYPE_DEBUG, TYPE_DIRECT, TYPE_CD, TYPE_COMMON, TYPE_GLOBAL)
    
    tl = max(len(t) for t,k,h in KEY_TYPE_MAP.values())
    kl = max(len(k + KEY_EXT) for t,k,h in KEY_TYPE_MAP.values())
    key_table = []
    for t in TYPE_LIST:
        _,k,h = KEY_TYPE_MAP[t]
        key_table.append(t.ljust(tl) + '|' + (k+KEY_EXT).ljust(kl) + '|' + h)
    #endregion

    ###########################################################################

    #region ## PARSER SETUP ##
    import argparse
    parser = argparse.ArgumentParser(
        description=(
        "Generator for different types of CatSystem 2 keys.\n"
        "\n"
        "TYPE  |FILENAME         |DESCRIPTION\n" + '\n'.join(key_table)
        # "debug |cs2_debug_key.dat|Enables debug mode features.\n"
        # "direct|direct.dat       |Standard key format (startup.xml: <direct>1</direct>)\n"
        # "cd    |key.dat          |CD key format       (startup.xml: <direct>0</direct>)\n"
        # "common|key_com.dat      |Old key format, rarely seen\n"
        # "global|cs2_gk.dat       |UNSUPPORTED! Hashes of exes to prevent tampering"
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter) # for unrestricted use of newlines in description


    parser.add_argument('key_type', metavar='KEYTYPE', choices=TYPE_LIST, default=None,
        help='Type of key to generate or read. (see --help for more info')

    parser.add_argument('input', metavar='CS2EXE',
        help='Input Cs2 executable (may not be the main startup exe, sometimes uses `.bin` extension, most common name: `cs2.exe`)')
    
    #parser.add_argument('output', metavar='OUTKEYFILE', default=None, nargs='?', help='Output key file (uses default key filename if omitted)')
    parser.add_argument('-o', '--output', metavar='OUTKEYFILE', default=None, help='Output file path')

    gkgroup = parser.add_mutually_exclusive_group()
    gkgroup.add_argument('-r', '--gk-read', metavar='GLOBALKEYFILE', dest='gk_read', default=None,
        help='Read an existing global key file and print the "tamper-proof" file names with MD5 hashes')
    gkgroup.add_argument('-w', '--gk-write', metavar='TAMPERFILES', dest='gk_write', nargs='+', default=[],
        help='Write a new global key file containing the specified "tamper-proof" file names')

    parser.add_argument('-2', '--gk-v2', dest='gk_version2', action='store_true', default=False,
        help='Use version 2 when reading/writing global key files (newer key version with longer name fields)')


    parser.add_argument('-y', '--overwrite', dest='overwrite', action='store_true', default=False,
        help='Overwrite output file if it already exists')

    parser.add_argument('-T', '--test', action='store_true', default=False,
        help='Test program without writing output to file')

    #endregion

    ###########################################################################

    #region ## PARSER VALIDATION ##

    args = parser.parse_args(argv)
    print_args = False
    if print_args: print(args)
    # return 0

    key_type  = args.key_type
    input  = args.input
    output = args.output
    gk_read = args.gk_read
    gk_write = args.gk_write
    gk_version2 = args.gk_version2

    if not output:
        output = KEY_TYPE_MAP[key_type][1] + KEY_EXT
    
    if key_type == TYPE_GLOBAL:
        #parser.error(f'Key type {key_type} not supported!')
        if not gk_read and not gk_write:
            parser.error('Must specify --gk-read or --gk-write for global key type')
    elif gk_read and gk_write or gk_version2:
        parser.error('Cannot specify --gk-read, --gk-write, or --gk-v2 for non-global key types')

    overwrite = args.overwrite
    test = args.test

    try:
        # pefile module is needed to extract vcodes. Check now so we can produce a helpful error.
        import pefile
    except ImportError:
        parser.error("This tool requires the pefile module.\n"
                     "You can install the pefile module by running the following command:\n"
                     "$ python -m pip install pefile")

    #endregion

    ###########################################################################

    # Read input file:
    keydata = None
    if key_type == TYPE_GLOBAL:
        # parser.error(f'Key type {key_type!r} not supported!')
        codes = read_cs2codes(input)
        #seed = vcode_seed(codes.vcode1)
        #
        if gk_read:
            globalkeys = globalkey_read(codes.vcode1, gk_read, version2=gk_version2)
        else:
            globalkeys = globalkey_build(*gk_write)
            keydata = globalkey_generate(codes.vcode1, *globalkeys, version2=gk_version2)
        
        nl = max(max(len(gk.name) for gk in globalkeys), len('FILENAME'))
        if print_args: print()
        print(f'{"FILENAME".ljust(nl)}|HASH')
        for gk in globalkeys:
            print(f'{gk.name.ljust(nl)}|{gk.md5hash.hex()}')
        print()
    else:
        codes = read_cs2codes(input)
        print(f'{os.path.basename(input)}: keycode={codes.keycode}, vcode1={codes.vcode1}, vcode2={codes.vcode2}')
        keydata = keyfile_generate(codes.vcode1, KEY_TYPE_MAP[key_type][1])


    # Write output file:
    if gk_read:
        pass # nothing to write
    elif os.path.exists(output) and not overwrite:
        parser.error(f'Output file already exists: {output!r}\nDid you mean to use the -y (overwrite) option?')
    elif test:
        print(f'Writing {key_type} key to: {output!r}')
    else:
        with open(output, 'wb') as f:
            f.write(keydata)
            f.flush()

    return 0

#endregion


###########################################################################

## MAIN CONDITION ##

if __name__ == '__main__':
    exit(main())
