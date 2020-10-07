#pragma once

#ifndef KCLIB_TABLE_KEYWORDS_H
#define KCLIB_TABLE_KEYWORDS_H

//#include <windows.h>
//#include "token_type.h"


///TODO: make TABLE_KEYWORDS extern in .cpp file
//       figure out clean method for assert_enum with keyword ID field


#pragma pack(push, 1)

typedef struct kclib_KEYWORD_ENTRY
{
    /*$0,4*/   unsigned int ID; // actually TOKEN_TYPE
    /*$4,10*/  char Name[16];
} KEYWORD_ENTRY;

#pragma pack(pop)


KEYWORD_ENTRY TABLE_KEYWORDS[] = {
    { 0x12, "if" },
    { 0x13, "else" },
    { 0x14, "while" },
    { 0x15, "do" },
    { 0x16, "for" },
    { 0x17, "switch" },
    { 0x18, "case" },
    { 0x19, "range" },
    { 0x1a, "default" },
    { 0x1b, "goto" },
    { 0x1c, "return" },
    { 0x1d, "break" },
    { 0x1e, "continue" },
    { 0x1f, "struct" },
    { 0x20, "union" },
    { 0x21, "enum" },
    
    { 0x23, "function" },

    { 0x25, "global" },
    { 0x26, "heap" },
    { 0x27, "@" },
    
    { 0x29, "void" },
    { 0x2a, "char" },
    { 0x2b, "byte" },
    { 0x2c, "short" },
    { 0x2d, "word" },
    { 0x2e, "long" },
    { 0x2f, "dword" },
    { 0x30, "float" },
    
    { 0xbd, "" }
};

#endif /* end include guard */
