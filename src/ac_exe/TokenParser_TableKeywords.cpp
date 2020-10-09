
// Standalone file to define extern TABLE_KEYWORDS (no code)

#include "TokenParser.h"

#ifndef KCLIB_OOP
// extern defined in "TokenParser.h"
KEYWORD_ENTRY TABLE_KEYWORDS[29] = {
#else
using namespace kclib;

// extern defined in "TokenParser.h"
kclib::KEYWORD_ENTRY kclib::TABLE_KEYWORDS[29] = {
#endif
//kclib::KEYWORD_ENTRY kclib::TABLE_KEYWORDS[29] = {
    { assert_enum(0x12, TOKEN_IF_KEYWORD), "if" },
    { assert_enum(0x13, TOKEN_ELSE_KEYWORD), "else" },
    { assert_enum(0x14, TOKEN_WHILE_KEYWORD), "while" },
    { assert_enum(0x15, TOKEN_DO_KEYWORD), "do" },
    { assert_enum(0x16, TOKEN_FOR_KEYWORD), "for" },
    { assert_enum(0x17, TOKEN_SWITCH_KEYWORD), "switch" },
    { assert_enum(0x18, TOKEN_CASE_KEYWORD), "case" },
    { assert_enum(0x19, TOKEN_RANGE_KEYWORD), "range" },
    { assert_enum(0x1a, TOKEN_DEFAULT_KEYWORD), "default" },
    { assert_enum(0x1b, TOKEN_GOTO_KEYWORD), "goto" },
    { assert_enum(0x1c, TOKEN_RETURN_KEYWORD), "return" },
    { assert_enum(0x1d, TOKEN_BREAK_KEYWORD), "break" },
    { assert_enum(0x1e, TOKEN_CONTINUE_KEYWORD), "continue" },
    { assert_enum(0x1f, TOKEN_STRUCT_KEYWORD), "struct" },
    { assert_enum(0x20, TOKEN_UNION_KEYWORD), "union" },
    { assert_enum(0x21, TOKEN_ENUM_KEYWORD), "enum" },
    
    { assert_enum(0x23, TOKEN_FUNCTION_KEYWORD), "function" },

    { assert_enum(0x25, TOKEN_GLOBAL_KEYWORD), "global" },
    { assert_enum(0x26, TOKEN_HEAP_KEYWORD), "heap" },
    { assert_enum(0x27, TOKEN_ATSIGN_KEYWORD), "@" },
    
    { assert_enum(0x29, TOKEN_VOID_KEYWORD), "void" },
    { assert_enum(0x2a, TOKEN_CHAR_KEYWORD), "char" },
    { assert_enum(0x2b, TOKEN_BYTE_KEYWORD), "byte" },
    { assert_enum(0x2c, TOKEN_SHORT_KEYWORD), "short" },
    { assert_enum(0x2d, TOKEN_WORD_KEYWORD), "word" },
    { assert_enum(0x2e, TOKEN_LONG_KEYWORD), "long" },
    { assert_enum(0x2f, TOKEN_DWORD_KEYWORD), "dword" },
    { assert_enum(0x30, TOKEN_FLOAT_KEYWORD), "float" },
    
    { assert_enum(0xbd, TOKEN_END), "" }
};

static_assert(TOKEN_END == 0xbd);
