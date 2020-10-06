
#include <windows.h>

typedef enum {
    TOKEN_NONE    = 0x00,   //

    // SYMBOL/OPERATOR FUNCTION

    TOKEN_POUND   = 0x01,   // "#"
    TOKEN_DOLLAR  = 0x02,   // "$"
    TOKEN_DOLLAR2 = 0x03,   // "$$"
    TOKEN_BACKSLASH = 0x04, // "\\" 
    TOKEN_NEWLINE = 0x05,   // "\n", "\r", "\r\n"
    TOKEN_WHITESPACE = 0x06, // " ", "\t"

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
    TOKEN_BITWISEOR_ASSIGN_OPERATOR = 0xa3, // "|="
    TOKEN_GREATER_OPERATOR = 0xa4, // ">"
    TOKEN_GREATEREQUAL_OPERATOR = 0xa5, // ">="
    TOKEN_RSHIFT_OPERATOR = 0xa6, // ">>"
    TOKEN_QUESTIONMARK = 0xa7, // "?"
    TOKEN_OPENBRACKET_OPERATOR = 0xa8, // "["
    TOKEN_CLOSEBRACKET_OPERATOR = 0xa9, // "]"
    TOKEN_BITWISEXOR_OPERATOR = 0xaa, // "^"
    TOKEN_OPENBRACE_OPERATOR = 0xab, // "{"
    TOKEN_BITWISEOR_OPERATOR = 0xac, // "|"
    TOKEN_LOGICALOR_OPERATOR = 0xad, // "||"
    TOKEN_CLOSEBRACE_OPERATOR = 0xae, // "}"
    TOKEN_BITWISENOT_OPERATOR = 0xaf, // "~"

    // STANDALONE

    TOKEN_SIZEOF_OPERATOR = 0xb4, // "sizeof" (no table entry)

    TOKEN_END = 0xbd,
} TOKEN_TYPE;

typedef struct {
    UINT ID;
    CHAR Name[16];
} KEYWORD_ENTRY;

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


//char * FUN_00411200(char *param_1, int *param_2)
char * token_UnkParseLines(char *param_1, int *param_2)

{

    // Handles comments, block comments, etc
    char cVar1;
    
    *param_2 = 0;
    cVar1 = *param_1;
    joined_r0x00411214:
    if (cVar1 == '\0') {
        return (char *)0x0;
    }
    if (cVar1 == '\\') {
        if (param_1[1] != '\r') {
            return param_1;
        }
        if (param_1[2] != '\n') {
            return param_1;
        }
        param_1 = param_1 + 3;
    }
    else {
        if (cVar1 != '/') {
            return param_1;
        }
        if (param_1[1] != '/') {
            if (param_1[1] != '*') {
                return param_1;
            }
            do {
                if ((cVar1 == '\r') && (param_1[1] == '\n')) {
                    param_1 = param_1 + 2;
                    *param_2 = *param_2 + 1;
                }
                else {
                    if ((cVar1 == '*') && (param_1[1] == '/')) goto LAB_0041128a;
                    param_1 = param_1 + 1;
                }
                cVar1 = *param_1;
                if (cVar1 == '\0') {
                    return (char *)0x0;
                }
            } while( true );
        }
        while ((cVar1 != '\r' || (param_1[1] != '\n'))) {
            cVar1 = param_1[1];
            param_1 = param_1 + 1;
            if (cVar1 == '\0') {
                return (char *)0x0;
            }
        }
    }
    goto LAB_0041128d;
    LAB_0041128a:
    param_1 = param_1 + 2;
    LAB_0041128d:
    cVar1 = *param_1;
    goto joined_r0x00411214;
}



BOOL token_ParseKeyword(IN const char *str, OUT int *outValue)

{
    for (int i = 0; TABLE_KEYWORDS[i].ID != 0xbd; i++)
    {
        if (strcmp(str, TABLE_KEYWORDS[i].Name) == 0)
        {
            *outValue = TABLE_KEYWORDS[i].ID;
            return TRUE;
        }
    }
    return FALSE;
}

// BOOL token_ParseKeyword(IN const char *str, OUT int *outValue)

// {
//     int iVar1;
//     int iVar2;
    
//     iVar2 = 0;
//     if (TABLE_KEYWORD_ID != 0xbd) {
//         iVar1 = 0;
//         do {
//             iVar1 = strcmp(str,&TABLE_KEYWORD_NAME + iVar1);
//             if (iVar1 == 0) break;
//             iVar2 += 1;
//             iVar1 = iVar2 * 0x14;
//         } while ((&TABLE_KEYWORD_ID)[iVar2 * 5] != 0xbd);
//     }
//     if ((&TABLE_KEYWORD_ID)[iVar2 * 5] != 0xbd) {
//         *outValue = (&TABLE_KEYWORD_ID)[iVar2 * 5];
//         return 1;
//     }
//     return 0;
// }

//undefined4 FUN_00411100(char *param_1,int *param_2,undefined *param_3)
BOOL token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer)

{
    //char cVar1;
    // int iVar2;
    
    *outLength = 0;

    char c = *str;
    if (c == '@')
    {
        outBuffer[0] = '@';
        *outLength++;
    }
    else if ((c >= 'a' || c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
    {
        while (true)
        {
            c = str[*outLength];
            if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') &&
                (c < '0' || c > '9') && c != '_')
            {
                break;
            }
            outBuffer[*outLength] = c;
            *outLength++;
        }
    }
    else
    {
        return FALSE;
    }
    outBuffer[*outLength] = '\0';
    return TRUE;
}

//undefined4 FUN_00411100(char *param_1,int *param_2,undefined *param_3)
BOOL token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer)

{
    //char cVar1;
    // int iVar2;
    
    *outLength = 0;

    char c = *str;
    if (c == '@')
    {
        outBuffer[0] = '@';
        *outLength++;
        // outBuffer[*outLength] = '\0';
        // return TRUE;
    }
    else if ((c >= 'a' || c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
    {
        while (true)
        {
            c = str[*outLength];
            if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') &&
                (c < '0' || c > '9') && c != '_')
            {
                break;
            }
            outBuffer[*outLength] = c;
            *outLength++;
        }
    }
    else
    {
        return FALSE;
    }
    if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && c != '_')
    {
        return FALSE;
    }
    while (true)
    {
        c = str[*outLength];
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') &&
            (c < '0' || c > '9') && c != '_')
        {
            break;
        }
        outBuffer[*outLength] = c;
        *outLength++;
    }
    outBuffer[*outLength] = '\0';
    return TRUE;
}

BOOL __thiscall token_ParseSymbol(void *thiscls, char *str, int *outLength, int *outValue)

{
    // char c;
    // BOOL result;
    
    // result = 1;
    *outLength = 0;
    switch(str[0])
    {
    default:
        return FALSE;
    case '\n':
        *outValue = 0x5; //TOKEN_NEWLINE;
        *outLength = 1;
        *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
        return TRUE;
    case '\r':
        *outValue = 0x5; //TOKEN_NEWLINE;
        if (str[1] == '\n') {
            *outLength = 2;
        } else {
            *outLength = 1;
        }
        *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
        return TRUE;
    case '!':
        if (str[1] == '=') { // "!="
            *outValue = 0x81; //TOKEN_NOTEQUAL_OPERATOR;
            *outLength = 2;
        } else { // "!"
            *outValue = 0x80; //TOKEN_LOGICALNOT_OPERATOR;
            *outLength = 1;
        }
        return TRUE;
    case '#':
        *outValue = 0x1; //TOKEN_POUND;
        *outLength = 1;
        return TRUE;
    case '$':
        if (str[1] == '$') {
            *outValue = 0x3; //TOKEN_DOLLAR2;
            *outLength = 2;
        } else {
            *outValue = 0x2; //TOKEN_DOLLAR;
            *outLength = 1;
        }
        return TRUE;
    case '%':
        if (str[1] == '=') {
            *outValue = 0x99;
            *outLength = 2;
        } else {
            *outValue = 0x82;
            *outLength = 1;
        }
        return TRUE;
    case '&':
        if (str[1] == '&') {
            *outValue = 0x84;
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = 0x9a;
            *outLength = 2;
        } else {
            *outValue = 0x83;
            *outLength = 1;
        }
        return TRUE;
    case '(':
        *outValue = 0x85;
        *outLength = 1;
        return TRUE;
    case ')':
        *outValue = 0x86;
        *outLength = 1;
        return TRUE;
    case '*':
        if (str[1] == '=') {
            *outValue = 0x9b;
            *outLength = 2;
        } else {
            *outValue = 0x87;
            *outLength = 1;
        }
        return TRUE;
    case '+':
        if (str[1] == '+') {
            *outValue = 0x89;
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = 0x9c;
            *outLength = 2;
        } else {
            *outValue = 0x88;
            *outLength = 1;
        }
        return TRUE;
    case ',':
        *outValue = 0x8b;
        *outLength = 1;
        return TRUE;
    case '-':
        if (str[1] == '-') {
            *outValue = 0x8d;
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = 0x9d;
            *outLength = 2;
        } else if (str[1] == '>') {
            *outValue = 0x8f;
            *outLength = 2;
        } else {
            *outValue = 0x8c;
            *outLength = 1;
        }
        return TRUE;
    case '.':
        if (str[1] == '.' && str[2] == '.') {
            *outValue = 0x91;
            *outLength = 3;
        } else {
            *outValue = 0x90;
            *outLength = 1;
        }
        return TRUE;
    case '/':
        if (str[1] == '=') {
            *outValue = 0x9e;
            *outLength = 2;
        } else {
            *outValue = 0x92;
            *outLength = 1;
        }
        return TRUE;
    case ':':
        *outValue = 0x93;
        *outLength = 1;
        return TRUE;
    case ';':
        *outValue = 0x94;
        *outLength = 1;
        return TRUE;
    case '<':
        if (str[1] == '<' && str[2] == '=') {
            *outValue = 0x9f;
            *outLength = 3;
        } else if (str[1] == '<') {
            *outValue = 0x96;
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = 0x97;
            *outLength = 2;
        } else {
            *outValue = 0x95;
            *outLength = 1;
        }
        return TRUE;
    case '=':
        if (str[1] == '=') {
            *outValue = 0xa0;
            *outLength = 2;
        } else {
            *outValue = 0x98;
            *outLength = 1;
        }
        return TRUE;
    case '>':
        if (str[1] == '>' && str[2] == '=') {
            *outValue = 0xa1;
            *outLength = 3;
        } else if (str[1] == '>') {
            *outValue = 0xa6;
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = 0xa5;
            *outLength = 2;
        } else {
            *outValue = 0xa4;
            *outLength = 1;
        }
        return TRUE;
    case '?':
        *outValue = 0xa7;
        *outLength = 1;
        return TRUE;
    case '[':
        *outValue = 0xa8;
        *outLength = 1;
        return TRUE;
    case '\\':
        *outValue = 0x4;
        *outLength = 1;
        return TRUE;
    case ']':
        *outValue = 0xa9;
        *outLength = 1;
        return TRUE;
    case '^':
        if (str[1] == '=') {
            *outValue = 0xa2;
            *outLength = 2;
        } else {
            *outValue = 0xaa;
            *outLength = 1;
        }
        return TRUE;
    case '{':
        *outValue = 0xab;
        *outLength = 1;
        return TRUE;
    case '|':
        if (str[1] == '=') {
            *outValue = 0xa3;
            *outLength = 2;
        } else if (str[1] == '|') {
            *outValue = 0xad;
            *outLength = 2;
        } else {
            *outValue = 0xac;
            *outLength = 1;
        }
        return TRUE;
    case '}':
        *outValue = 0xae;
        *outLength = 1;
        return TRUE;
    case '~':
        *outValue = 0xaf;
        *outLength = 1;
        return TRUE;
    }
    return FALSE;
    // *outLength = 1;
    // *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
    // return TRUE;
}

