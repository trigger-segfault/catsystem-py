#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CatSystem 2 CatScene and CSTL scene localization setup

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-09-29'
__author__  = 'Robert Jordan'

__all__ = ['SceneTranslation']

#######################################################################################

from .cst import *
from .cstl import *


class SceneTranslation(object):
    ORIG_LANG:str = 'ORIG'
    @classmethod
    def open(cls, filepath:str) -> 'SceneTranslation':
        return cls(SceneScript(filepath), SceneLocalization(filepath+'l'))
    def __init__(self, cst:SceneScript, cstl:SceneLocalization=None, langs:list=None):
        if cstl is None:
            cstl = SceneLocalization()
            cstl.messages = []
            cstl.languages = []
            if langs is None:
                langs = [self.ORIG_LANG]
            else: #if langs is not None:
                assert(not isinstance(langs, str))
                assert(len(langs)!=0)
            cstl.languages = [SceneLanguage(str(l), i) for i,l in enumerate(langs)]
        else:
            assert(langs is None)
        self.cst = cst
        self.cstl = cstl
        self.messages:[SceneLocalizationMessage] = []
        self.orig_lines:[(SceneLine)] = []
        if langs is None:
            self.languages:(SceneLanguage) = tuple([SceneLanguage(self.ORIG_LANG, 0)] + [SceneLanguage(l.name, l.id+1) for l in cstl.languages])
        else:
            self.languages:(SceneLanguage) = tuple([SceneLanguage(l.name, l.id) for l in cstl.languages])
        # for i, (input, message) in enumerate(zip(cst.inputs, cstl.messages)):
        for i, input in enumerate(cst.inputs):
            name:str = ''
            content:str = ''
            start = input[1]
            end = start+input[0]
            lines:[SceneLine] = []
            for line in cst.lines[start:end]:
                if is_msgline(line.kind):
                    if line.kind == SceneLineType.NAME:
                        name += line.content + name
                    elif line.kind == SceneLineType.MESSAGE:
                        content = line.content + content
                    lines.append(line)
            self.orig_lines.append(tuple(lines))
            # index = input[1]
            # start = index-1
            # end = index-1
            # while is_msgline(cst.lines[start-1].kind):
            #     start -= 1
            #     line = cst.lines[start]
            #     if line.kind == SceneLineType.NAME:
            #         name += line.content + name
            #     elif line.kind == SceneLineType.MESSAGE:
            #         content = line.content + content
            # lines = cst.lines[start:end]
            message_langs:[SceneMessage] = [SceneMessage(i, self.languages[0], name, content)]
            if langs is not None:
                message_langs2:[SceneMessage] = [SceneMessage(i, cstl.languages[0], name, content)]
                for j, message_lang in enumerate(self.languages):
                    if j == 0:
                        pass
                    message_langs.append(SceneMessage(i, message_lang, '', ''))
                    message_langs.append(SceneMessage(i, cstl.languages[j], '', ''))
                self.messages.append(SceneLocalizationMessage(i, self.languages, message_langs))
                cstl.messages.append(SceneLocalizationMessage(i, cstl.languages, message_langs2))
            else:
                for j, message_lang in enumerate(self.languages[1:]):
                    message = cstl.messages[i][message_lang.name]
                    message_langs.append(SceneMessage(i, self.getlanguage(message_lang.name), message.name, message.message))
                # for j, message_lang in enumerate(cstl.languages):
                #     # if langs is not None and j == 0:
                #     #     continue
                #     # message_langs.append(SceneMessage(i, self.getlanguage(message_lang.language), message_lang.name, message_lang.message))
                #     if langs is not None:
                #         if j == 0:
                #             continue
                #         message_lang = cstl.messages[i]
                #         message_langs.append(SceneMessage(i, self.getlanguage(message_lang.name), '', ''))
                #     else:
                #     message = cstl.messages[i]
                #     message_langs.append(SceneMessage(i, self.getlanguage(message.language.name), message.name, message.message))
                self.messages.append(SceneLocalizationMessage(i, self.languages, message_langs))
            #
            cstl.language_num = len(cstl.languages)
            cstl.message_num = len(cstl.messages)
            cstl.signature = SceneLocalization.SIGNATURE
    def getlanguage(self, lang:str) -> SceneLanguage:
        return self.languages[self.languages.index(lang) if isinstance(lang, str) else lang]
    def index(self, item) -> int:
        return self.languages.index(item) if isinstance(item, str) else self.messages.index(item)
    def __len__(self) -> int:
        return len(self.messages)
    def __nonzero__(self) -> bool:
        return bool(self.messages)
    def __contains__(self, item) -> bool:
        return (item in self.languages) if isinstance(item, str) else (item in self.messages)
    def __getitem__(self, item) -> (SceneLocalizationMessage):
        if isinstance(item, str):
            return SceneLocalizationLanguage(self.messages, self.getlanguage(item))
        return self.messages[item]
    def __iter__(self):
        return iter(self.messages)
    def __repr__(self) -> str:
        return repr(tuple(iter(self)))
    def __str__(self) -> str:
        return str(tuple(iter(self)))

