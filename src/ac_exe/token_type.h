#pragma once

#ifndef KCLIB_TOKEN_TYPE_H
#define KCLIB_TOKEN_TYPE_H

//#include <stdio.h>
//#include <windows.h>


typedef enum kclib_TOKEN_TYPE_ENUM
{
    TOKEN_NONE    = 0x00,   //

    // SYMBOL/OPERATOR FUNCTION

    TOKEN_POUND   = 0x01,   // "#"
    TOKEN_DOLLAR  = 0x02,   // "$"
    TOKEN_DOLLAR2 = 0x03,   // "$$"
    TOKEN_BACKSLASH = 0x04, // "\\" 
    TOKEN_NEWLINE = 0x05,   // "\n", "\r", "\r\n"
    TOKEN_WHITESPACE = 0x06, // " ", "\t"
    TOKEN_7 = 0x07, // catch all in token parse?

    TOKEN_INTEGER_LITERAL = 0x0d, // /(\d+|0[xX][0-9A-Fa-f]+|\'([^\']|\\\'|\\\\)*\')/
    TOKEN_FLOAT_LITERAL = 0x0e, // /(\d+\.\d*[fF]?|\d+[fF])/
    TOKEN_STRING_LITERAL = 0x0f, // /\"([^\"]|\\\")*\"/

    // KEYWORD TABLE

    TOKEN_IF_KEYWORD = 0x12, // "if"
    TOKEN_ELSE_KEYWORD = 0x13, // "else"
    TOKEN_WHILE_KEYWORD = 0x14, // "while"
    TOKEN_DO_KEYWORD = 0x15, // "do"
    TOKEN_FOR_KEYWORD = 0x16, // "for"
    TOKEN_SWITCH_KEYWORD = 0x17, // "switch"
    TOKEN_CASE_KEYWORD = 0x18, // "case"
    TOKEN_RANGE_KEYWORD = 0x19, // "range"
    TOKEN_DEFAULT_KEYWORD = 0x1a, // "default"
    TOKEN_GOTO_KEYWORD = 0x1b, // "goto"
    TOKEN_RETURN_KEYWORD = 0x1c, // "return"
    TOKEN_BREAK_KEYWORD = 0x1d, // "break"
    TOKEN_CONTINUE_KEYWORD = 0x1e, // "continue"
    TOKEN_STRUCT_KEYWORD = 0x1f, // "struct"
    TOKEN_UNION_KEYWORD = 0x20, // "union"
    TOKEN_ENUM_KEYWORD = 0x21, // "enum"
    
    TOKEN_FUNCTION_KEYWORD = 0x23, // "function"
    
    TOKEN_GLOBAL_KEYWORD = 0x25, // "global"
    TOKEN_HEAP_KEYWORD = 0x26, // "heap"
    TOKEN_ATSIGN_KEYWORD = 0x27, // "@"
    TOKEN_ATSIGN = 0x27, // "@"

    TOKEN_VOID_KEYWORD = 0x29, // "void"
    TOKEN_CHAR_KEYWORD = 0x2a, // "char"
    TOKEN_BYTE_KEYWORD = 0x2b, // "byte"
    TOKEN_SHORT_KEYWORD = 0x2c, // "short"
    TOKEN_WORD_KEYWORD = 0x2d, // "word"
    TOKEN_LONG_KEYWORD = 0x2e, // "long"
    TOKEN_DWORD_KEYWORD = 0x2f, // "dword"
    TOKEN_FLOAT_KEYWORD = 0x30, // "float"

    // SYMBOL/OPERATOR FUNCTION

    TOKEN_LOGICALNOT_OPERATOR = 0x80, // "!"
    TOKEN_NOTEQUAL_OPERATOR = 0x81, // "!="
    
    TOKEN_PERCENT_OPERATOR = 0x82, // "%"
    TOKEN_MODULO_OPERATOR = 0x82, // "%"
    TOKEN_MODULUS_OPERATOR = 0x82, // "%"
    TOKEN_REMAINDER_OPERATOR = 0x82, // "%"

    TOKEN_BITWISEAND_OPERATOR = 0x83, // "&"
    TOKEN_LOGICALAND_OPERATOR = 0x84, // "&&"
    TOKEN_OPENPAREN_OPERATOR = 0x85, // "("
    TOKEN_CLOSEPAREN_OPERATOR = 0x86, // ")"
    TOKEN_MULTIPLY_OPERATOR = 0x87, // "*"
    TOKEN_ADD_OPERATOR = 0x88, // "+"
    TOKEN_INCREMENT_OPERATOR = 0x89, // "++"
    
    TOKEN_COMMA_OPERATOR = 0x8b, // ","
    TOKEN_SUBTRACT_OPERATOR = 0x8c, // "-"
    TOKEN_DECREMENT_OPERATOR = 0x8d, // "--"

    TOKEN_POINTTO_OPERATOR = 0x8f, // "->"
    TOKEN_DOT_OPERATOR = 0x90, // "."
    TOKEN_ELLIPSIS_OPERATOR = 0x91, // "..."
    TOKEN_DIVIDE_OPERATOR = 0x92, // "/"
    TOKEN_COLON_OPERATOR = 0x93, // ":"
    TOKEN_SEMICOLON_OPERATOR = 0x94, // ";"
    TOKEN_LESS_OPERATOR = 0x95, // "<"
    TOKEN_LSHIFT_OPERATOR = 0x96, // "<<"
    TOKEN_LESSEQUAL_OPERATOR = 0x97, // "<="
    TOKEN_ASSIGN_OPERATOR = 0x98, // "="
    TOKEN_MODULO_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_MODULUS_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_REMAINDER_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_BITWISEAND_ASSIGN_OPERATOR = 0x9a, // "&="
    TOKEN_MULTIPLY_ASSIGN_OPERATOR = 0x9b, // "*="
    TOKEN_ADD_ASSIGN_OPERATOR = 0x9c, // "+="
    TOKEN_SUBTRACT_ASSIGN_OPERATOR = 0x9d, // "-="
    TOKEN_DIVIDE_ASSIGN_OPERATOR = 0x9e, // "/="
    TOKEN_LSHIFT_ASSIGN_OPERATOR = 0x9f, // "<<="
    TOKEN_EQUAL_OPERATOR = 0xa0, // "=="
    TOKEN_RSHIFT_ASSIGN_OPERATOR = 0xa1, // ">>="

    TOKEN_BITWISEXOR_ASSIGN_OPERATOR = 0xa2, // "^="
    TOKEN_XOR_ASSIGN_OPERATOR = 0xa2, // "^="

    TOKEN_BITWISEOR_ASSIGN_OPERATOR = 0xa3, // "|="
    TOKEN_GREATER_OPERATOR = 0xa4, // ">"
    TOKEN_GREATEREQUAL_OPERATOR = 0xa5, // ">="
    TOKEN_RSHIFT_OPERATOR = 0xa6, // ">>"
    TOKEN_QUESTIONMARK = 0xa7, // "?"
    TOKEN_OPENBRACKET_OPERATOR = 0xa8, // "["
    TOKEN_CLOSEBRACKET_OPERATOR = 0xa9, // "]"

    TOKEN_BITWISEXOR_OPERATOR = 0xaa, // "^"
    TOKEN_XOR_OPERATOR = 0xaa, // "^"

    TOKEN_OPENBRACE_OPERATOR = 0xab, // "{"
    TOKEN_BITWISEOR_OPERATOR = 0xac, // "|"
    TOKEN_LOGICALOR_OPERATOR = 0xad, // "||"
    TOKEN_CLOSEBRACE_OPERATOR = 0xae, // "}"
    
    TOKEN_BITWISENOT_OPERATOR = 0xaf, // "~"
    TOKEN_NEGATE_OPERATOR = 0xaf, // "~"

    // STANDALONE

    TOKEN_SIZEOF_OPERATOR = 0xb4, // "sizeof" (no table entry)

    TOKEN_END = 0xbd,
} TOKEN_TYPE;


typedef enum kclib_TOKEN_GROUP_ENUM
{
    GROUP_NONE = 0, // single character? when nothing parses
    GROUP_WHITESPACE = 3, // ' ', '\t'
    GROUP_4 = 4, // Unknown function
    GROUP_LITERAL = 5, // INTEGER, FLOAT, CHAR, STRING literals
    GROUP_SYMBOL = 6, // SYMBOL/OPERATOR, SIZEOF
    GROUP_KEYWORD = 7, // KEYWORD
} TOKEN_GROUP;


#pragma pack(push, 1)

// typedef struct kclib_TOKEN_VALUE_LOHIWORD
// {
//     /*$0,4*/  unsigned int Low;
//     /*$4,4*/  unsigned int High;
//     /*$8*/
// } TOKEN_VALUE_LOHIWORD;

// typedef union kclib_TOKEN_VALUE_UNION
// {
//     TOKEN_TYPE           Symbol;
//     unsigned char        Int8;
//     unsigned short       Int16;
//     unsigned int         Int32;
//     unsigned long long   Int64;
//     //signed char          Int8;
//     //short                Int16;
//     //int                  Int32;
//     //long long            Int64;
//     float                Single;
//     //double               Double;
//     TOKEN_VALUE_LOHIWORD LoHiWord;
// } TOKEN_VALUE;

// assume only 4 bytes in size... for now
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

typedef struct kclib_TOKEN_TYPE
{
    /*$0,4*/    TOKEN_GROUP TokenGroup;
    /*$4,4*/    TOKEN_TYPE TokenType;
    // /*$8,8*/    TOKEN_VALUE TokenValue;
    /*$8,4*/    TOKEN_VALUE TokenValue; // assume only 4 bytes in size for now
    /*$c,4*/    unsigned int TokUnk3; // (possibly second half of TokenValue, i.e. for QWORDs),
                                      // never seen assigned outside of UnkFunction
    /*$10,4*/   int TokUnk4; // set to 1 for literals: STRING,
                             // (maybe BOOL? flag stating difference from file, quoted)
    /*$14,4*/   unsigned int TokUnk5; // set to 4 for literals: INTEGER, FLOAT (value data size? not set for char)
    /*$18,4*/   unsigned int TokUnk6; // set to 4 for literals: INTEGER, FLOAT (value data size? not set for char)
    /*$1c,4*/   TOKEN_TYPE LiteralType; // assigned TOKEN_<TYPE>_KEYWORD for literals,
                                        // strangely VOID (0x29) for literal INTEGER,
                                        // STRING uses TOKEN_STRING_LITERAL, same as TokenType
    /*$20,400*/ char TokenText[0x400]; // buffer containing token text (STRING stores escaped text)
    /*$420*/
} TOKEN_RESULT;


//4 (1)
//0xc (21)
//0x10 (2)
//0x118 (12)
//0x11c (2)
//0x120 (1)

//(char *)(*(int *)this + *(int *)((int)this+0xc));

typedef struct kclib_SCRIPT_DECODER
{
    /*$0,4*/   char *Buffer;
    /*$4,4*/   unsigned int Length; // text buffer length
    /*$8,4*/   unsigned int LinePosition; // text buffer index of current/(end?) line
    /*$c,4*/   unsigned int TokenPosition; // text buffer index of current parse/token position
    /*$10,4*/  unsigned int LineNumber; // position of text buffer? End of line? Start of line?
    /*$118,4*/ TOKEN_TYPE LastTokenType;
    // /*$11c,4*/ unsigned int ScrUnk71;
    /*$11c,4*/ void *ScrPtr71; // possible STL class, used in token_UnkFunction
    /*$120,4*/ unsigned int ScrUnk72;
    /*$124*/
} SCRIPT_DECODER;

// typedef struct kclib_SCRIPT_DECODER
// {
//     /*$0,4*/   char *Buffer;
//     /*$4,4*/   unsigned int Length;
//     /*$8,4*/   unsigned int LinePosition;
//     /*$c,4*/   unsigned int TokenPosition; // TokenNumber?
//     /*$10*/
// } SCRIPT_DECODER;

#pragma pack(pop)


// Confirm we have the right enum/name and value,
// there was a lot of room for copy&paste errors
#define assert_enum(value, TOKEN) TOKEN; static_assert(value == TOKEN)

// Lazy solution for C++ this keyword without implementing classes
#define this thisscr


#endif /* end include guard */
