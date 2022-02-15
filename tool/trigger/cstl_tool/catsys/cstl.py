#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CatSystem 2 CSTL scene localizations type (.cstl)

WARNING: This module is deprecated, and will be changed or removed,
         once it's made obsolete.
"""

__version__ = '0.0.1'
__date__    = '2020-09-29'
__author__  = 'Robert Jordan'

#######################################################################################

import io, struct
from struct import unpack, pack, iter_unpack

#WARNING: deprecated parent imports
from .util import _read_bytes, _read_uint8, _read_uint8_string, _write_stream, _write_uint8, _write_uint8_string


class SceneLanguage(str):
    def __new__(cls, name:str, index:int=...):
        if isinstance(name, SceneLanguage):
            name = name.name
        return str.__new__(cls, name)
    def __init__(self, name:str, index:int=...):
        if isinstance(name, SceneLanguage):
            if index is Ellipsis:
                index = name.id
            name = name.name
        elif index is Ellipsis:
            raise ValueError('Expected index with language name')
        self.name:str = name
        self.id:int = index
    @staticmethod
    @property
    def DEFAULT(cls) -> 'SceneLanguage':
        return SceneLanguage('default', -1)
    @property
    def is_default(self) -> bool:
        return self.name == 'default'


class SceneMessage(object):
    @classmethod
    def read(cls, index:int, language:SceneLanguage, data:bytes=None, offset:int=...) -> ('SceneMessage',int):
        if data is not None:
            data = _read_bytes(data)
        if offset is Ellipsis:
            offset = 0
        if data is None:
            raise ValueError('No message data to read')
        if offset is None:
            raise ValueError('No message data offset to read at')
        if offset: message_data = data[offset:]
        else:      message_data = data
        start:int = offset
        name, offset = _read_uint8_string(data, offset)
        message, offset = _read_uint8_string(data, offset)
        message_data:bytes = data[start:offset]
        return (cls(index, language, name, message, data=message_data), offset)
    def write(self, file):
        return _write_uint8_string(file, self.name) + _write_uint8_string(file, self.message)
    def __init__(self, index:int, language:SceneLanguage, name:str, message:str, data:bytes=None):
        self.message_data:bytes = data
        self.language:SceneLanguage = SceneLanguage(language)
        self.id:int = index
        self.name:str = name
        self.message:str = message
    # def __init__(self, language:SceneLanguage, index:int, data:bytes=None, offset:int=...):
    #     self.message_data:bytes = None
    #     self.language:SceneLanguage = SceneLanguage(language)
    #     self.id:int = index
    #     self.name:str = None
    #     self.message:str = None
    #     if data is not None:
    #         self.read(data, offset)
    def __repr__(self) -> str:
        return f'({repr(self.language)}, {repr(self.id)}, {repr(self.name)}, {repr(self.message)})'
    def __str__(self) -> str:
        name:str = '@'
        if self.name:
            name:str = f'<{self.name}>'
        return f'${self.language}:{self.id}$ {name} {self.message}'
    @property
    def is_monologue(self) -> bool:
        return not self.name

# def langid(name:str, languages:[SceneLanguage]):
#     pass

class SceneLocalizationMessage(object):
    def __init__(self, index:int, languages:[SceneLanguage], messages:[SceneMessage]):
        self.languages:(SceneLanguage) = languages
        self.messages:(SceneMessage) = tuple(messages)
        self.id:int = index
    def index(self, item) -> int:
        return self.languages.index(item) if isinstance(item, str) else self.messages.index(item)
    def items(self) -> (str, SceneMessage):
        return zip(self.languages, self.messages)
    def __len__(self) -> int:
        return len(self.messages)
    def __nonzero__(self) -> bool:
        return bool(self.messages)
    def __contains__(self, item) -> bool:
        return (item in self.languages) if isinstance(item, str) else (item in self.messages)
    def __getitem__(self, item) -> SceneMessage:
        return self.messages[self.languages.index(item) if isinstance(item, str) else item]
    def __iter__(self) -> int:
        return iter(self.messages)
    def __repr__(self) -> str:
        return repr(self.messages)
    def __str__(self) -> str:
        return str(self.messages)

class SceneLocalizationLanguage_iterator(object):
    def __init__(self, messages:[SceneLocalizationMessage], language:SceneLanguage):
        if isinstance(messages, SceneLocalization):
            messages = messages.messages
        #elif isinstance(messages, (list, tuple))
        self._messages_iter = iter(messages)
        #self.messages:[SceneLocalizationMessage] = messages
        self.language:SceneLanguage = language
    def __iter__(self) -> 'SceneLocalizationLanguage_iterator':
        return self
    def __next__(self) -> SceneMessage:
        result:SceneLocalizationMessage = next(self._messages_iter)
        return result.messages[self.language.id]
        #raise StopIteration

class SceneLocalizationLanguage(object):
    def __init__(self, messages:[SceneLocalizationMessage], language:SceneLanguage):
        if isinstance(messages, SceneLocalization):
            messages = messages.messages
        self.messages:[SceneLocalizationMessage] = messages
        self.language:SceneLanguage = language
    def index(self, item) -> int:
        for i, message in enumerate(self.messages):
            if message.messages[self.language.id] == item:
                return i
        return -1
    def __len__(self) -> int:
        return len(self.messages)
    def __nonzero__(self) -> bool:
        return bool(self.messages)
    def __contains__(self, item) -> bool:
        return item in self.messages
    def __getitem__(self, item) -> SceneMessage:
        # return 
        # return self.messages[item].messages[self.language.id]
        if isinstance(item, slice):
            return tuple(m.messages[self.language.id] for m in self.messages[item])
        return self.messages[item].messages[self.language.id]
        # result = self.messages[item]
        # if isinstance(result, SceneLocalizationMessage):
        #     return result.messages[self.language.id]
        # return tuple(r.messages[self.language.id] for r in result)
    def __iter__(self) -> SceneLocalizationLanguage_iterator:
        return SceneLocalizationLanguage_iterator(self.messages, self.language)
    def __repr__(self) -> str:
        return repr(tuple(iter(self)))
    def __str__(self) -> str:
        return str(tuple(iter(self)))

class SceneLocalization(object):
    EXTENSION:str = '.cstl'
    SIGNATURE:str = 'CSTL'
    def read(self, data:bytes=None, offset:int=...):
        if data is not None:
            data = _read_bytes(data)
        if offset is Ellipsis:
            offset = 0
        if data is None:
            raise ValueError('No file data to read')
        if offset is None:
            raise ValueError('No file data offset to read at')
        if offset: self.file_data = data[offset:]
        else:      self.file_data = data
        self.signature:str = unpack('4s', self.file_data[0:4])[0].decode('ascii')
        # Possibly part of 8-byte signature expected with CatScene (.cst)
        self.unk_value:int = unpack('i', self.file_data[4:8])[0]
        offset:int = 8
        self.language_num, offset = _read_uint8(self.file_data, offset)
        self.languages:[SceneLanguage] = []
        for i in range(self.language_num):
            lang, offset = _read_uint8_string(self.file_data, offset)
            self.languages.append(SceneLanguage(lang, i))
        self.message_num, offset = _read_uint8(self.file_data, offset)
        self.messages:[SceneLocalizationMessage] = []
        for i in range(self.message_num):
            message_langs:(SceneMessage) = []
            for lang in self.languages:
                #msg_name, offset = _read_uint8_string(data, offset)
                #msg_message, offset = _read_uint8_string(data, offset)
                #msg_data:bytes = data[start:offset]
                message, offset = SceneMessage.read(i, lang, data, offset)
                #message:SceneMessage = SceneMessage(i, lang, msg_name, msg_message, data=msg_data)

                #message:SceneMessage = SceneMessage(i, lang, self.file_data, offset)
                #offset += len(message.message_data)
                message_langs.append(message)
            self.messages.append(SceneLocalizationMessage(i, self.languages, message_langs))
            #self.messages2.append(message_langs)
    def write(self, file, autofill=False):
        if file is not None:
            file = _write_stream(file)
        if autofill:
            self.signature = 'CSTL'
            self.unk_value = 0
            self.language_num = len(self.languages)
            self.message_num = len(self.messages)
        file.write(pack('<4sI', self.signature.encode('ascii'), self.unk_value))
        # file.write(pack('<4sII', self.signature.encode('ascii'), self.unk_value, self.language_num))
        # file.write(pack('<4', self.signature.encode('ascii')))
        # file.write(pack('<I', self.unk_value))
        # file.write(pack('<I', self.language_num))
        _write_uint8(file, self.language_num)
        for i in range(self.language_num):
            _write_uint8_string(file, self.languages[i])
        _write_uint8(file, self.message_num)
        for i in range(self.message_num):
            for j in range(self.language_num):
                self.messages[i][j].write(file)
        # file.close()
    def __init__(self, data:bytes=None, offset:int=...):
        self.file_data:bytes = None
        self.signature:str = ''
        # Possibly part of 8-byte signature expected with CatScene (.cst)
        self.unk_value:int = 0
        self.language_num:int = 0
        self.languages:[SceneLanguage] = []
        self.message_num:int = 0
        self.messages:[SceneLocalizationMessage] = []
        #self.messages2:[(SceneMessage)] = []
        if data is not None:
            self.read(data, offset)
    @property
    def string_total(self) -> int:
        # 2 strings per lang: orig, new?
        # language_num strings per language
        return (self.message_num * 2) * self.language_num
    def getlanguage(self, lang:str) -> SceneLanguage:
        return self.languages[self.languages.index(lang) if isinstance(lang, str) else lang]
    def index(self, item) -> int:
        return self.languages.index(item) if isinstance(lang, str) else self.messages.index(item)
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
    
