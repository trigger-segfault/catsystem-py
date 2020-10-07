#pragma once

#ifndef KCLIB_TOKEN_TYPE_H
#define KCLIB_TOKEN_TYPE_H

#include "common.h"


// general group identifier of token's type
typedef enum kclib_TOKEN_GROUP_ENUM
{
    GROUP_NONE          = 0, // single character? when nothing parses

    GROUP_3 = 3, // <unused>
    GROUP_2 = 2, // <unused>

    GROUP_WHITESPACE    = 3, // ' ', '\t'
    GROUP_IDENTIFIER    = 4, // identifier related to LookupIdentifier (maybe 'not found')
    GROUP_LITERAL       = 5, // INTEGER, FLOAT, CHAR, STRING literals
    GROUP_SYMBOL        = 6, // SYMBOL/OPERATOR, SIZEOF
    GROUP_KEYWORD       = 7, // KEYWORD
} TOKEN_GROUP;

// identifier of token type
typedef enum kclib_TOKEN_TYPE_ENUM
{
    TOKEN_NONE = 0x00, // unknown, none?

    // SYMBOL/OPERATOR PARSE:

    TOKEN_POUND         = 0x01, // "#"
    TOKEN_DOLLAR        = 0x02, // "$"
    TOKEN_DOLLAR2       = 0x03, // "$$" (escaped?)
    TOKEN_BACKSLASH     = 0x04, // "\\" 
    TOKEN_NEWLINE       = 0x05, // "\n", "\r", "\r\n"
    TOKEN_WHITESPACE    = 0x06, // " ", "\t"
    TOKEN_IDENTIFIER    = 0x07, // catch all for valid identifier token

    TOKEN_8 = 0x08, // <unused>
    TOKEN_9 = 0x09, // <unused>
    TOKEN_a = 0x0a, // <unused>
    TOKEN_b = 0x0b, // <unused>
    TOKEN_c = 0x0c, // <unused>

    // LITERAL PARSE:

    TOKEN_INTEGER_LITERAL   = 0x0d, // /(\d+|0[xX][0-9A-Fa-f]+|\'([^\']|\\\'|\\\\)*\')/
    TOKEN_FLOAT_LITERAL     = 0x0e, // /(\d+\.\d*[fF]?|\d+[fF])/
    TOKEN_STRING_LITERAL    = 0x0f, // /\"([^\"]|\\\")*\"/

    TOKEN_10 = 0x10, // <unused>
    TOKEN_11 = 0x11, // <unused>

    
    // KEYWORD TABLE PARSE:

    TOKEN_IF_KEYWORD        = 0x12, // "if"
    TOKEN_ELSE_KEYWORD      = 0x13, // "else"
    TOKEN_WHILE_KEYWORD     = 0x14, // "while"
    TOKEN_DO_KEYWORD        = 0x15, // "do"
    TOKEN_FOR_KEYWORD       = 0x16, // "for"
    TOKEN_SWITCH_KEYWORD    = 0x17, // "switch"
    TOKEN_CASE_KEYWORD      = 0x18, // "case"
    TOKEN_RANGE_KEYWORD     = 0x19, // "range"
    TOKEN_DEFAULT_KEYWORD   = 0x1a, // "default"
    TOKEN_GOTO_KEYWORD      = 0x1b, // "goto"
    TOKEN_RETURN_KEYWORD    = 0x1c, // "return"
    TOKEN_BREAK_KEYWORD     = 0x1d, // "break"
    TOKEN_CONTINUE_KEYWORD  = 0x1e, // "continue"
    TOKEN_STRUCT_KEYWORD    = 0x1f, // "struct"
    TOKEN_UNION_KEYWORD     = 0x20, // "union"
    TOKEN_ENUM_KEYWORD      = 0x21, // "enum"

    TOKEN_22 = 0x22, // <unused>

    TOKEN_FUNCTION_KEYWORD  = 0x23, // "function"

    TOKEN_24 = 0x24, // <unused>

    TOKEN_GLOBAL_KEYWORD    = 0x25, // "global"
    TOKEN_HEAP_KEYWORD      = 0x26, // "heap"
    TOKEN_ATSIGN_KEYWORD    = 0x27, // "@"

    TOKEN_28 = 0x28, // <unused>

    TOKEN_VOID_KEYWORD      = 0x29, // "void"
    TOKEN_CHAR_KEYWORD      = 0x2a, // "char"
    TOKEN_BYTE_KEYWORD      = 0x2b, // "byte"
    TOKEN_SHORT_KEYWORD     = 0x2c, // "short"
    TOKEN_WORD_KEYWORD      = 0x2d, // "word"
    TOKEN_LONG_KEYWORD      = 0x2e, // "long"
    TOKEN_DWORD_KEYWORD     = 0x2f, // "dword"
    TOKEN_FLOAT_KEYWORD     = 0x30, // "float"

    // SYMBOL/OPERATOR PARSE:

    TOKEN_LOGICALNOT_OPERATOR   = 0x80, // "!"
    TOKEN_NOTEQUAL_OPERATOR     = 0x81, // "!="
    TOKEN_MODULUS_OPERATOR      = 0x82, // "%"
    TOKEN_BITWISEAND_OPERATOR   = 0x83, // "&"
    TOKEN_LOGICALAND_OPERATOR   = 0x84, // "&&"
    TOKEN_OPENPAREN_OPERATOR    = 0x85, // "("
    TOKEN_CLOSEPAREN_OPERATOR   = 0x86, // ")"
    TOKEN_MULTIPLY_OPERATOR     = 0x87, // "*"
    TOKEN_ADD_OPERATOR          = 0x88, // "+"
    TOKEN_INCREMENT_OPERATOR    = 0x89, // "++"

    TOKEN_8a = 0x8a, // <unused>

    TOKEN_COMMA_OPERATOR        = 0x8b, // ","
    TOKEN_SUBTRACT_OPERATOR     = 0x8c, // "-"
    TOKEN_DECREMENT_OPERATOR    = 0x8d, // "--"

    TOKEN_8e = 0x8e, // <unused>

    TOKEN_POINTTO_OPERATOR      = 0x8f, // "->"
    TOKEN_DOT_OPERATOR          = 0x90, // "."
    TOKEN_ELLIPSIS_OPERATOR     = 0x91, // "..."
    TOKEN_DIVIDE_OPERATOR       = 0x92, // "/"
    TOKEN_COLON_OPERATOR        = 0x93, // ":"
    TOKEN_SEMICOLON_OPERATOR    = 0x94, // ";"
    TOKEN_LESS_OPERATOR         = 0x95, // "<"
    TOKEN_LSHIFT_OPERATOR       = 0x96, // "<<"
    TOKEN_LESSEQUAL_OPERATOR    = 0x97, // "<="
    TOKEN_ASSIGN_OPERATOR       = 0x98, // "="
    TOKEN_MODULUS_ASSIGN_OPERATOR   = 0x99, // "%="
    TOKEN_BITWISEAND_ASSIGN_OPERATOR= 0x9a, // "&="
    TOKEN_MULTIPLY_ASSIGN_OPERATOR  = 0x9b, // "*="
    TOKEN_ADD_ASSIGN_OPERATOR       = 0x9c, // "+="
    TOKEN_SUBTRACT_ASSIGN_OPERATOR  = 0x9d, // "-="
    TOKEN_DIVIDE_ASSIGN_OPERATOR    = 0x9e, // "/="
    TOKEN_LSHIFT_ASSIGN_OPERATOR    = 0x9f, // "<<="
    TOKEN_EQUAL_OPERATOR            = 0xa0, // "=="
    TOKEN_RSHIFT_ASSIGN_OPERATOR    = 0xa1, // ">>="
    TOKEN_XOR_ASSIGN_OPERATOR       = 0xa2, // "^="
    TOKEN_BITWISEOR_ASSIGN_OPERATOR = 0xa3, // "|="
    TOKEN_GREATER_OPERATOR      = 0xa4, // ">"
    TOKEN_GREATEREQUAL_OPERATOR = 0xa5, // ">="
    TOKEN_RSHIFT_OPERATOR       = 0xa6, // ">>"
    TOKEN_QUESTION_OPERATOR     = 0xa7, // "?"
    TOKEN_OPENBRACKET_OPERATOR  = 0xa8, // "["
    TOKEN_CLOSEBRACKET_OPERATOR = 0xa9, // "]"
    TOKEN_XOR_OPERATOR          = 0xaa, // "^"
    TOKEN_OPENBRACE_OPERATOR    = 0xab, // "{"
    TOKEN_BITWISEOR_OPERATOR    = 0xac, // "|"
    TOKEN_LOGICALOR_OPERATOR    = 0xad, // "||"
    TOKEN_CLOSEBRACE_OPERATOR   = 0xae, // "}"
    TOKEN_BITWISENOT_OPERATOR   = 0xaf, // "~"

    TOKEN_b0 = 0xb0, // <unused>
    TOKEN_b1 = 0xb1, // <unused>
    TOKEN_b2 = 0xb2, // <unused>
    TOKEN_b3 = 0xb3, // <unused>

    // STANDALONE PARSE:

    TOKEN_SIZEOF_OPERATOR = 0xb4, // "sizeof" (manually checked)

    TOKEN_b5 = 0xb5, // <unused>
    TOKEN_b6 = 0xb6, // <unused>
    TOKEN_b7 = 0xb7, // <unused>
    TOKEN_b8 = 0xb8, // <unused>
    TOKEN_b9 = 0xb9, // <unused>
    TOKEN_ba = 0xba, // <unused>
    TOKEN_bb = 0xbb, // <unused>
    TOKEN_bc = 0xbc, // <unused>

    TOKEN_END = 0xbd, // signals end of keyword table
} TOKEN_TYPE;



#pragma pack(push, 1)

// typedef struct kclib_TOKEN_VALUE_LOHIWORD
// {
//     /*$0,4*/  unsigned int Low;
//     /*$4,4*/  unsigned int High;
//     /*$8*/
// } TOKEN_VALUE_LOHIWORD;

// token result value union
// (assume only 4 bytes in size... for now)
typedef union kclib_TOKEN_VALUE_UNION
{
    TOKEN_TYPE          Symbol;
    //signed char         Int8;
    //long long           Int64;
    //unsigned char       UInt8;
    //unsigned long long  UInt64;
    short               Int16;
    int                 Int32;
    unsigned short      UInt16;
    unsigned int        UInt32;
    float               Single;
    //double              Double;
    //TOKEN_VALUE_LOHIWORD LoHiWord;
} TOKEN_VALUE;

// result of token parse function
typedef struct kclib_TOKEN_TYPE
{
    /*$0,4*/    TOKEN_GROUP TokenGroup;
    /*$4,4*/    TOKEN_TYPE TokenType;
    /*$8,4*/    TOKEN_VALUE TokenValue; // assume only 4 bytes in size for now
    /*$c,4*/    unsigned int TokUnk3; // possibly second half of TokenValue, i.e. for QWORDs,
                                      // potentially identifier related, like lookup pointer,
                                      //  as its only ever assigned by token_LookupIdentifier
    /*$10,4*/   int TokUnk4; // set to 1 for literals: STRING,
                             // (maybe BOOL? flag stating difference from file, quoted)
    /*$14,4*/   unsigned int TokUnk5; // set to 4 for literals: INTEGER, FLOAT
                                      // (value data size? not set for char)
    /*$18,4*/   unsigned int TokUnk6; // set to 4 for literals: INTEGER, FLOAT
                                      // (value data size? not set for char)
    /*$1c,4*/   TOKEN_TYPE LiteralType; // assigned TOKEN_<TYPE>_KEYWORD for literals,
                                        // strangely VOID (0x29) for literal INTEGER,
                                        // STRING uses TOKEN_STRING_LITERAL, same as TokenType
    /*$20,400*/ char TokenText[0x400]; // buffer containing token text (STRING stores escaped text)
    /*$420*/
} TOKEN_RESULT;

    // unsigned char local_16c[320]; //undefined8 local_16c [40];
    // TOKEN_GROUP  local_28; //assigned to token->TokenGroup
    // TOKEN_TYPE   local_24; //assigned to token->TokenType
    // int          local_20; //assigned to token->TokUnk4
    // unsigned int local_1c; //assigned to token->TokUnk3
    // unsigned int local_18; //assigned to token->TokUnk5
    // unsigned int local_14; //assigned to token->TokUnk6
    // TOKEN_TYPE   local_c;  //assigned to token->LiteralType

// layout of returned identifier lookup structure,
// based on what we know is assigned to TOKEN_RESULT
typedef struct kclib_IDENTIFIER_INFO
{
    /*$0,140*/  char IdnBuf0[0x140]; // unknown buffer
    /*$140,4*/  unsigned int IdnUnk1; // unknown field, likely attached to buffer above
    /*$144,4*/  TOKEN_GROUP TokenGroup;
    /*$148,4*/  TOKEN_TYPE TokenType;
    /*$14c,4*/  int IdnTokUnk4;
    /*$150,4*/  unsigned int IdnTokUnk3; // possible pointer to identifier
                                         // (only unused field in TOKEN_RESULT)
    /*$154,4*/  unsigned int IdnTokUnk5;
    /*$158,4*/  unsigned int IdnTokUnk6;
    /*$15c,4*/  unsigned int IdnUnk8;
    /*$160,4*/  TOKEN_TYPE LiteralType;
    /*$164,4*/  unsigned int IdnUnk10;
    /*$168*/
} IDENTIFIER_INFO;

//4 (1)
//0xc (21)
//0x10 (2)
//0x118 (12)
//0x11c (2)
//0x120 (1)

//(char *)(*(int *)this + *(int *)((int)this+0xc));

// ScriptDecode2 structure (class)
typedef struct kclib_SCRIPT_DECODER
{
    /*$0,4*/    char *Buffer;
    /*$4,4*/    unsigned int Length; // text buffer length
    /*$8,4*/    unsigned int LinePosition; // text buffer index of current/(end?) line
    /*$c,4*/    unsigned int TokenPosition; // text buffer index of current parse/token position
    /*$10,4*/   unsigned int LineNumber; // position of text buffer? End of line? Start of line?
    /*$14,104*/ char _pad_ScrUnk5[0x104]; // size is MAX_PATH, very significant
    /*$118,4*/  TOKEN_TYPE LastTokenType;
    /*$11c,4*/  void *Identifiers; // possible STL class, used in token_LookupIdentifier
    /*$120,4*/  unsigned int ScrUnk72;
    /*$124*/
} SCRIPT_DECODER;


// table entry in TABLE_KEYWORDS
typedef struct kclib_KEYWORD_ENTRY
{
    /*$0,4*/   unsigned int ID; // real type: TOKEN_TYPE
    /*$4,10*/  char Name[16];
    /*$14*/
} KEYWORD_ENTRY;

#pragma pack(pop)


// table of keyword entries, lookup done in token_ParseKeyword
extern KEYWORD_ENTRY TABLE_KEYWORDS[29];


#endif /* end include guard */
