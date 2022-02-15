#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CatSystem 2 CSTL (scene localization) utility
"""

__version__ = '0.0.1'
__date__    = '2022-02-12'
__author__  = 'Robert Jordan'

#######################################################################################

import configparser, itertools, os

# from collections import OrderedDict
from typing import Dict, List, Set, Tuple


from catsys.cst import SceneScript
from catsys.cstl import SceneLocalization, SceneLanguage, SceneLocalizationMessage, SceneMessage
from catsys.tr import SceneTranslation

#######################################################################################

## CONSTANTS
ORIG_LANG = SceneTranslation.ORIG_LANG

#######################################################################################

#region INI to CSTL converter

class IniConverter:
    ## CONSTANTS:
    CSTL_SECTION:str = 'cstl'
    LANGS_PROPERTY:str = 'languages'
    MESSAGE_PREFIX:str = 'm'
    
    ## FIELDS:
    escape_chr:str
    hex_chr:str
    unicode_chr:str
    space_chr:str
    name_sep:str
    name_sep_fmt:str
    
    escapes:Dict[str,str]
    opt_escapes:Dict[str,str]
    quotes:Set[str]
    delimiters:Tuple[str]

    use_opt_escapes:bool
    require_quote_escapes:bool
    require_name_sep:bool
    #allow_trailing:bool
    strip_text:bool
    #strict:bool
    
    _escapes:Dict[str,str]
    _opt_escapes:Dict[str,str]
    _unescapes:Dict[str,str]


    #region Constructor

    def __init__(self, *, escape_chr:str='\\', hex_chr:str='x', unicode_chr:str='u', space_chr:str='_', name_sep:str=':', name_sep_fmt:str=' {0} ', escapes:Dict[str,str]={'\a':'a', '\b':'b', '\f':'f', '\n': 'n', '\r':'r', '\t':'t', '\v':'v'}, opt_escapes:Dict[str,str]={' ':'_', '\'':'\'', '"':'"'}, quotes:Set[str]={'\'', '"'}, delimiters:Tuple[str]=('=',':'), use_opt_escapes:bool=False, require_quote_escapes:bool=False, require_name_sep:bool=False, strip_text:bool=True):
        self.escape_chr = escape_chr
        self.hex_chr = hex_chr
        self.unicode_chr = unicode_chr
        self.space_chr = space_chr
        self.name_sep = name_sep
        self.name_sep_fmt = name_sep_fmt

        self.escapes = {} if escapes is None else dict(escapes)
        self.opt_escapes = {} if opt_escapes is None else dict(opt_escapes)
        self.quotes = set() if quotes is None else (set(quotes) if not isinstance(delimiters, str) else (delimiters,))
        self.delimiters = ('=',) if delimiters is None else (tuple(delimiters) if not isinstance(delimiters, str) else (delimiters,))

        self.use_opt_escapes = use_opt_escapes
        self.require_quote_escapes = require_quote_escapes
        self.require_name_sep = require_name_sep
        self.strip_text = strip_text

        self.commit_escapes()
    
    #endregion


    #region Setup

    def commit_escapes(self) -> 'IniConverter':
        self._escapes = dict(self.escapes)
        self._escapes[self.escape_chr] = self.escape_chr
        self._escapes[self.name_sep] = self.name_sep
        
        self._opt_escapes = dict(self.opt_escapes)
        if self.space_chr: self._opt_escapes[' '] = self.space_chr

        self._unescapes = dict([(v,k) for k,v in self._escapes.items()] + [(v,k) for k,v in self._opt_escapes.items()])
        return self

    def createparser(self) -> configparser.ConfigParser:
        return configparser.ConfigParser(interpolation=None,            # Disable '%' interpolation (because it wrecks absolute havoc on message text with unescaped %%'s)
                                         inline_comment_prefixes=None,  # Disabled inline comments (because it wrecks absolute havoc on message text)
                                         delimiters=('=',':'),          # Default assignment operators used by ini files
                                         comment_prefixes=('#',';'),    # Default comment prefixes used by ini files
                                         empty_lines_in_values=True     # (optional) Allow empty multilines whith indented values on following lines
                                        )
    #endregion


    #region INI formatting

    def unescape(self, text:str) -> str:
        if self.strip_text: text = text.strip()
        if not text: return ''
        newtext = []
        i = 0
        while i < len(text):
            c = text[i]
            if c == self.escape_chr:
                if i + 2 > len(text): raise ValueError(f'Trailing escape {c!r} at end of text: {text!r}')
                #
                c2 = text[i + 1]
                if c2 == self.hex_chr:
                    if i + 4 > len(text): raise ValueError(f'Trailing hex escape {text[i:]!r} at end of text: {text!r}')
                    #
                    c = chr(int(text[i+2:i+4], 16))
                    i += 4
                elif c2 == self.unicode_chr:
                    if i + 6 > len(text): raise ValueError(f'Trailing unicode escape {text[i:]!r} at end of text: {text!r}')
                    #
                    c = chr(int(text[i+2:i+6], 16))
                    i += 6
                else:
                    unc2 = self._unescapes.get(c2)
                    if unc2 is None: raise ValueError(f'Invalid escape {text[i:i+2]!r} in text: {text!r}')
                    #
                    c = unc2
                    i += 2
            elif self.require_quote_escapes and c in self.quotes:
                if i + 2 > len(text): raise ValueError(f'Trailing quote escape {text[i:]!r} at end of text: {text!r}')
                #
                c2 = text[i + 1]
                if c != c2: raise ValueError(f'Expected repeated quote character in text: {text!r}')
                i += 2
            else:
                i += 1
            newtext.append(c)
        return ''.join(newtext)

    def escape(self, text:str) -> str:
        if self.strip_text: text = text.strip()
        if not text: return ''
        newtext = []
        i = 0
        while i < len(text):
            c = text[i]
            if c == ' ' and i + 1 < len(text):
                pass # space escapes are optional, unless its trailing space
            elif c in self._escapes:
                c = self.escape_chr + self._escapes[c]
            elif ord(c) < 0x20 or ord(c) == 0x7f: # control character needs escaping
                c = f'{self.escape_chr}{self.hex_chr}{ord(c):02x}'
            # elif ord(c) > 0xff: # unicode character escapes
            #     c = f'{self.escape_chr}{self.unicode_chr}{ord(c):04x}'
            elif self.require_quote_escapes and c in self.quotes:
                c = c*2 # repeat the quote character
            i += 1
            newtext.append(c)
        return ''.join(newtext)


    def parse_line(self, line:str) -> tuple:
        split_index = -1
        i = 0
        while i < len(line):
            c = line[i]
            if c == self.escape_chr:
                i += 1 # skip escapes
            elif c == self.name_sep:
                split_index = i
                break
            i += 1
        #
        if split_index == -1: # names are optional
            if self.require_name_sep:
                raise ValueError(f'Missing name separator for line: {line!r}')
            ename, emessage = '', line
        else:
            ename, emessage = line[:split_index], line[split_index+1:]
        name    = self.unescape(ename)
        message = self.unescape(emessage)
        #
        return (name, message)

    def format_line(self, name:str, message:str) -> str:
        ename    = self.escape(name)
        emessage = self.escape(message)
        if not ename and not self.require_name_sep:
            return emessage
        else:
            return f'{ename}{self.name_sep_fmt.format(self.name_sep)}{emessage}'

    #endregion


    #region CSTL conversion

    def compile(self, inifile:str) -> SceneLocalization:
        if isinstance(inifile, configparser.ConfigParser):
            config = inifile
        else:
            config = self.createparser()
            with open(inifile, 'rt', encoding='utf-8') as f:
                config.read_string(f.read())

        sec_num = 0
        max_id = -1
        ini_messages = {}
        for sec in config.sections():
            if sec.startswith(self.MESSAGE_PREFIX):
                if not sec[len(self.MESSAGE_PREFIX):].isdigit():
                    raise ValueError(f'Expected number after {self.MESSAGE_PREFIX!r} section prefix for section {sec!r}')
                msg_id = int(sec[len(self.MESSAGE_PREFIX):], 10)
                max_id = max(max_id, msg_id)
                sec_num += 1
                ini_messages[msg_id] = config[sec]
            elif sec == self.CSTL_SECTION:
                pass
            else:
                raise ValueError(f'Unknown ini section: [{sec}]')
        #
        if self.CSTL_SECTION not in config: #.has_section():
            raise ValueError(f'Missing ini section: [{self.CSTL_SECTION}]')
        if self.LANGS_PROPERTY not in config[self.CSTL_SECTION]:
            raise ValueError(f'Missing ini section / property: [{self.CSTL_SECTION}] / {self.LANGS_PROPERTY}')
        #
        langs = [l.strip() for l in config[self.CSTL_SECTION][self.LANGS_PROPERTY].split(',')]
        # if no_orig and ORIG_LANG in langs:
        #     langs.remove(ORIG_LANG)
        #
        if sec_num != len(ini_messages) or max_id + 1 < len(ini_messages):
            raise ValueError('Duplicate or invalid message sections')
        num = max_id + 1
        #
        cstl = SceneLocalization()
        cstl.languages = [SceneLanguage(l,i) for i,l in enumerate(langs)]
        #
        cstl.messages = []
        #
        #
        for i in range(num):
            ini_message = ini_messages.get(i)
            message_langs = []
            for lang in cstl.languages:
                line = ini_message.get(lang.name) if ini_message is not None else None
                if line is None: name, content = '', ''
                else:            name, content = self.parse_line(line)
                message_langs.append(SceneMessage(i, lang, name, content))
            cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))

        
        cstl.language_num = len(langs)
        cstl.message_num = len(cstl.messages)
        cstl.signature = SceneLocalization.SIGNATURE
        return cstl
    #
    def decompile(self, cstlfile:str) -> configparser.ConfigParser:
        if isinstance(cstlfile, SceneLocalization):
            cstl = cstlfile
        else:
            cstl = SceneLocalization(cstlfile)
        
        config = self.createparser()
        config.add_section(self.CSTL_SECTION)
        config[self.CSTL_SECTION][self.LANGS_PROPERTY] = ','.join([l.name for l in cstl.languages])
        #
        for i,message in enumerate(cstl.messages):
            sec = f'{self.MESSAGE_PREFIX}{i}'
            config.add_section(sec)
            for lang in cstl.languages:
                msg = message[lang.id]
                config[sec][lang.name] = self.format_line(msg.name, msg.message)
        #
        return config
        
    #endregion


#endregion


#######################################################################################

#region CSTL x CSTL manipulation

# def join_messages2(*merges:SceneLocalization) -> SceneLocalization:
#     cstl = SceneLocalization()
#     #
#     all_llangs = list(itertools.chain(*[[l.name for l in m] for m in merges]))
#     if len(set(all_llangs)) != len(all_llangs):
#         raise ValueError('join_messages: Duplicate language names in CSTL files')
#     #
#     cstl.languages = [SceneLanguage(l,i) for i,l in enumerate(all_llangs)]
#     #
#     #
#     num = max([len(m.messages) for m in merges])
#     #
#     cstl.messages = []
#     for i in range(num):
#         message_langs = []
#         for m in merges:
#             for m_lang in m.languages:
#                 if i < len(m.messages):
#                     m_msg = m.messages[i][m_lang.id]
#                     message_langs.append(SceneMessage(i, cstl.getlanguage(m_lang.name), m_msg.name, m_msg.message))
#                 else:
#                     message_langs.append(SceneMessage(i, cstl.getlanguage(m_lang.name), '', ''))
#         cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
#     #
#     #
#     cstl.language_num = len(cstl.languages)
#     cstl.message_num = len(cstl.messages)
#     cstl.signature = SceneLocalization.SIGNATURE
#     return cstl


def join_messages(src:SceneLocalization, merge:SceneLocalization) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    cstl.languages = []
    added_langs = [l.name for l in merge.languages]
    for src_lang in src.languages:
        cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
        if src_lang.name in added_langs:
            added_langs.remove(src_lang.name)
    for l in added_langs:
        cstl.languages.append(SceneLanguage(l, len(cstl.languages)))
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            if lang.name in added_langs:
                if i < len(merge.messages):
                    m_msg = merge.messages[i][lang.name]
                    message_langs.append(SceneMessage(i, lang, m_msg.name, m_msg.message))
                else:
                    message_langs.append(SceneMessage(i, lang, '', ''))
            else:
                src_msg = src_message[lang.name]
                message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl


def replace_messages(src:SceneLocalization, merge:SceneLocalization) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    cstl.languages = []
    replaced_langs = []
    merged_langs = set([l.name for l in merge.languages])
    for src_lang in src.languages:
        cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
        if src_lang.name in merged_langs:
            replaced_langs.append(src_lang.name)
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            if lang.name in replaced_langs:
                m_msg = merge.messages[i][lang.name]
                if i < len(merge.messages):
                    message_langs.append(SceneMessage(i, lang, m_msg.name, m_msg.message))
                else:
                    message_langs.append(SceneMessage(i, lang, '', ''))
            else:
                src_msg = src_message[lang.name]
                message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl

#endregion


#######################################################################################

#region CSTL language manipulation

def delete_langs(src:SceneLocalization, *langs:str) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    if len(set(langs)) != len(langs):
        raise ValueError('delete_langs: Duplicate language names')
    #
    cstl.languages = []
    for src_lang in src.languages:
        if src_lang.name not in langs:
            cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            src_msg = src_message[lang.name]
            message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl


def keep_langs(src:SceneLocalization, *langs:str) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    if len(set(langs)) != len(langs):
        raise ValueError('keep_langs: Duplicate language names')
    #
    cstl.languages = []
    for src_lang in src.languages:
        if src_lang.name in langs:
            cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            src_msg = src_message[lang.name]
            message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl


def add_langs(src:SceneLocalization, *langs:str) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    if len(set(langs)) != len(langs):
        raise ValueError('add_langs: Duplicate language names')
    #
    cstl.languages = []
    added_langs = list(langs)
    for src_lang in src.languages:
        cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
        if src_lang.name in added_langs:
            added_langs.remove(src_lang.name)
    for l in added_langs:
        cstl.languages.append(SceneLanguage(l, len(cstl.languages)))
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            if lang.name not in added_langs:
                src_msg = src_message[lang.name]
                message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
            else:
                message_langs.append(SceneMessage(i, lang, '', ''))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl


def rename_langs(src:SceneLocalization, *langs:tuple) -> SceneLocalization:
    cstl = SceneLocalization()
    #
    if len(set(l[0] for l in langs)) != len(langs):
        raise ValueError('rename_langs: Duplicate input language names')
    if len(set(l[1] for l in langs)) != len(langs):
        raise ValueError('rename_langs: Duplicate output language names')
    #
    cstl.languages = []
    lang_dict = dict(langs)
    lang_lookup = {}
    for src_lang in src.languages:
        if src_lang.name in lang_dict:
            new_name = lang_dict[src_lang.name]
            cstl.languages.append(SceneLanguage(new_name, len(cstl.languages)))
            lang_lookup[new_name] = src_lang.name
        else:
            cstl.languages.append(SceneLanguage(src_lang.name, len(cstl.languages)))
    #
    #
    cstl.messages = []
    for i,src_message in enumerate(src.messages):
        message_langs = []
        for lang in cstl.languages:
            src_msg = src_message[lang_lookup.get(lang.name, lang.name)]
            message_langs.append(SceneMessage(i, lang, src_msg.name, src_msg.message))
        cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs))
    #
    #
    cstl.language_num = len(cstl.languages)
    cstl.message_num = len(cstl.messages)
    cstl.signature = SceneLocalization.SIGNATURE
    return cstl

#endregion


#######################################################################################

#region CST to CSTL conversion

def build_cstl(cstfile:str, langs:list=None) -> SceneLocalization:
    cst = SceneScript(cstfile)
    trans = SceneTranslation(cst, langs=langs)
    return trans.cstl

#endregion


#######################################################################################

#region ## MAIN FUNCTION ##

def main(argv:list=None):
    
    TYPE_CST = 'cst'
    TYPE_CSTL = 'cstl'
    TYPE_INI = 'ini'
    INPUT_TYPES = (TYPE_CST, TYPE_CSTL, TYPE_INI)
    OUPUT_TYPES = (TYPE_CSTL, TYPE_INI)

    #region ## PARSER SETUP ##
    import argparse
    parser = argparse.ArgumentParser()

    inputtypes = parser.add_mutually_exclusive_group()
    inputtypes.add_argument('-b','--build', metavar='CST', default=None, help='Convert a CST scene into CSTL (default output: CSTL file)')
    inputtypes.add_argument('-c', '--compile', metavar='INI', default=None, help='Compile an INI file (default output: CSTL file)')
    inputtypes.add_argument('-d', '--decompile', metavar='CSTL', default=None, help='Decompile a CSTL file into an INI file (default output: INI file)')

    parser.add_argument('-j', '--join', metavar='CSTL', default=None, help='Combine languages of input file and a CSTL file (skips duplicate languages)')
    parser.add_argument('-r', '--replace', metavar='CSTL', default=None, help='Replace languages from input file with those in CSTL file')

    # All actions:
    parser.add_argument('-o', '--output', metavar='OUTFILE', default=None, help='Output file path')
    parser.add_argument('-t', '--output-type', default=None, choices=OUPUT_TYPES, help='Output file type')

    # All actions except -b/--build-cstl
    parser.add_argument('-q', '--quotes', action='store_true', default=False, help='Escape/unescape single and double quotes by repeating the character twice')
    # parser.add_argument('-n', '--name-sep', action='store_true', default=False, help='Escape/unescape single and double quotes by repeating the character twice')
    # parser.add_argument('-s', '--strict', action='store_true', default=False, help='')

    # -i/--build-ini, -d/--decompile:
    parser.add_argument('-H', '--header', action='store_true', default=False, help='Include header comment in INI file displaying name of input CST/CSTL file')

    # parser.add_argument('--no-orig', action='store_true', default=False, help=f'Don\'t preserve {ORIG_LANG!r} properties when building from CST. Instead, use first --add-langs')
    parser.add_argument('-O', '--orig-lang', metavar='LANG', default=False, help=f'Rename {ORIG_LANG!r} language if present')
    parser.add_argument('-A', '--add-langs', metavar='LANG', nargs='+', default=None, help='Add new languages to the output, if not already present')
    parser.add_argument('-D', '--delete-langs', metavar='LANG', nargs='+', default=None, help='Remove existing languages from the output')
    parser.add_argument('-K', '--keep-langs', metavar='LANG', nargs='+', default=None, help='Remove all languages not listed from the output')
    parser.add_argument('-R', '--rename-lang', dest='rename_langs', metavar=('OLDLANG', 'NEWLANG'), nargs=2, default=[], action='append', help='Rename an existing language')


    parser.add_argument('-T', '--test', action='store_true', default=False, help=f'Test program without writing output to file')

    #endregion

    ###########################################################################

    #region ## PARSER VALIDATION ##

    args = parser.parse_args(argv)
    print(args)
    # return 0


    input_type  = None # TYPE_CST|TYPE_CSTL|TYPE_INI
    output_type = None # TYPE_CSTL|TYPE_INI

    input = None
    if args.build:
        input = args.build
        input_type, output_type = TYPE_CST, TYPE_CSTL
    elif args.compile:
        input = args.compile
        input_type, output_type = TYPE_INI, TYPE_CSTL
    elif args.decompile:
        input = args.decompile
        input_type, output_type = TYPE_CSTL, TYPE_INI
    
    output = args.output
    if args.output_type:
        output_type = args.output_type.lower()

    test = args.test


    converter = IniConverter()
    converter.require_quote_escapes = args.quotes
    #converter.strict = args.strict
    converter.commit_escapes()

    cstl = None

    #endregion

    ###########################################################################

    # Read input file:
    if test and input_type:
        print(f'Reading {output_type} from: {input!r}')
    if input_type == TYPE_CST:
        cstl = build_cstl(input)#, langs=langs)
    elif input_type == TYPE_CSTL:
        cstl = SceneLocalization(input)
    elif input_type == TYPE_INI:
        cstl = converter.compile(input)
        # cstl = compile_ini(input, no_orig=no_orig, quotes=quotes)
    else:
        if output:
            parser.error('-o/--output specified without input argument')
    

    # Transform intermediate file:
    if args.delete_langs:
        cstl = delete_langs(cstl, *args.delete_langs)
    if args.keep_langs:
        cstl = keep_langs(cstl, *args.keep_langs)
    if args.rename_langs:
        cstl = rename_langs(cstl, *args.rename_langs)
    if args.orig_lang:
        cstl = rename_langs(cstl, (ORIG_LANG, args.orig_lang))
    if args.add_langs:
        cstl = add_langs(cstl, *args.add_langs)

    if args.join:
        merge_cstl = SceneLocalization(args.join)
        cstl = join_messages(cstl, merge_cstl)
    if args.replace:
        merge_cstl = SceneLocalization(args.replace)
        cstl = replace_messages(cstl, merge_cstl)


    # Write output file:
    if test and output_type is not None:
        print(f'Writing {output_type} to: {output!r}')
    elif output_type == TYPE_CSTL:
        with open(output, 'wb') as f:
            cstl.write(f, autofill=True)
            f.flush()
    elif output_type == TYPE_INI:
        config = converter.decompile(cstl)
        # config = decompile_ini(cstl, no_orig=no_orig, quotes=quotes)
        with open(output, 'wt', encoding='utf-8') as f:
            if args.header:
                f.write(f'# {os.path.basename(input)}\n\n')
            config.write(f)
            f.flush()

    return 0

#endregion


###########################################################################

## MAIN CONDITION ##

if __name__ == '__main__':
    exit(main())
