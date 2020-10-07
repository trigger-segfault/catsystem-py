#include "token_parse_types.h"
#include "table_keywords.h"
#include "ghidra_types_min.h"
#include "shiftjis.h"
#include <stdio.h>
#include <windows.h>

///FID:cs2_full_v401/tool/ac.exe: FUN_00411c40
void __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this)

{
    this->Buffer = NULL;
    this->LinePosition = 0;
    this->TokenPosition = 0;
}

// uint __fastcall FUN_00411d40(int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d40
BOOL __fastcall ScriptDecoder_IsEOF(SCRIPT_DECODER *this)

{
    //int param_1;
    //return (uint)(*(uint *)(param_1 + 4) <= *(uint *)(param_1 + 8));
    //return (BOOL)(this->Length <= this->LinePosition);
    return (BOOL)(this->LinePosition >= this->Length);
}


// void __thiscall FUN_00411cf0(void *this,int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cf0
void __thiscall ScriptDecoder_NextLine(SCRIPT_DECODER *this, OUT char *outLine)

{
    // outLine is optional, if excluded, decoder still advances to next line

    int linepos = 0;
    while (this->LinePosition < this->Length)
    {
        char c = this->Buffer[this->LinePosition];
        if (c == '\n')
        {
            this->LinePosition++;
            break;
        }
        if (outLine != NULL)
        {
            outLine[linepos] = c;
        }
        this->LinePosition++;
        linepos++;
    }
    if (outLine != NULL)
    {
        outLine[linepos] = '\0';
    }
    return;
    //return linepos;
}

static_assert(TOKEN_END == 0xbd);


//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
BOOL __cdecl token_Next(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token)

{
    while (token_Parse(this, token))
    {
        if (token->TokenType == TOKEN_WHITESPACE)
            return TRUE;
    }
    return FALSE;
}


//char * token_SkipComments(char *param_1,int *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411200
const char * __stdcall token_SkipComments(IN const char *str, OUT int *outLines)

{
    // outLines is only incremented within block comments
    // line continuations do not increment the script reader line number
    // it seems CRLF is required
    // /*/ is a valid block comment
    // \ is a valid line continuation
    // block comments can cause line continuation behavior

    *outLines = 0;

    while (true)
    {
        if (str[0] == '\0')
            return NULL; // EOF

        if (str[0] == '\\' && str[1] == '\r' && str[2] == '\n') // line continuation
        {
            str += 3; // continue line check
        }
        else if (str[0] == '/' && str[1] == '/') // line comment
        {
            // no risk for: out of bounds at str[1], thanks to '\0' check at end of loop
            while (str[0] != '\r' || str[1] != '\n') // do while loop, easier to visually follow as while 
            {
                str++;
                if (str[0] == '\0')
                    return NULL; // EOF
            }
            // line check will end, first token will be newline '\r\n'
        }
        else if (str[0] == '/' && str[1] == '*') // block comment (/*/ is valid)
        {
            // no risk for: out of bounds at str[1], thanks to '\0' check at end of loop
            while (str[0] != '*' || str[1] != '/') // do while loop, easier to visually follow as while
            {
                if (str[0] == '\r' && str[1] == '\n')
                {
                    *outLines++; // lines-consumed in block comment? very strange
                    str += 2;
                }
                else
                {
                    str++;
                }
                if (str[0] == '\0')
                    return NULL; // EOF
            }
            str += 2; // continue line check, apparently block comments do line continuation
        }
        else
        {
            break; // return str;
        }
    }
    return str; // next non-comment token
}


//undefined4 __stdcall FUN_00410f10(char *param_1,int *param_2,int *param_3)
///FID:cs2_full_v401/tool/ac.exe: FUN_00410f10
BOOL __stdcall token_ParseInteger(IN const char *str, OUT int *outLength, OUT int *outValue)

{
    if (str[0] < '0' || str[0] > '9')
        return FALSE; // not an numeric literal

    *outLength = 0;
    *outValue  = 0;
    
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        // hexidecimal literal (0x)
        *outLength += 2;
        while (true)
        {
            char c = str[*outLength];
            if (c >= '0' && c <= '9')
                c -= '0'; //('0' - 0x0);
            else if (c >= 'a' && c <= 'f')
                c -= ('a' - 0xa);
            else if (c >= 'A' && c <= 'F')
                c -= ('A' - 0xA);
            else
                break; // not a hex digit
            *outValue = *outValue * 0x10 + (int)c; // c is set to value of digit
            *outLength++;
        }
    }
    else
    {
        // decimal literal (avoid floating literal, cancel at '.')
        while (true)
        {
            char c = str[*outLength];
            if (c >= '0' && c <= '9')
                c -= '0'; //('0' - 0x0);
            else
                break; // not a dec digit
            *outValue = *outValue * 10 + (int)c; // c is set to value of digit
            *outLength++;
        }
        while (str[*outLength] >= '0' && str[*outLength] <= '9')
        {
            *outValue = *outValue * 10 + str[*outLength] - '0';
            *outLength++;
            char c = str[*outLength];
            if (c >= '0' && c <= '9')
                c -= '0'; //('0' - 0x0);
            else
                break; // not a dec digit
            *outValue = *outValue * 10 + (int)c; // c is set to value of digit
            *outLength++;
        }
        if (str[*outLength] == '.')
            return FALSE; // no floating points, strangely no checks for 'f'
    }
    return TRUE;
}


//void __stdcall FUN_00411000(char *param_1,int *param_2,float *param_3)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411000
BOOL __stdcall token_ParseFloat(IN const char *str, OUT int *outLength, OUT float *outValue)

{
    if (str[0] < '0' || str[0] > '9')
        return FALSE; // not a numeric literal

    *outLength = 0;
    *outValue  = 0.00000000;
    
    char atof_buffer[1024];

    // floating decimal literal (require '.' point or 'f' specifier)
    while (true)
    {
        char c = str[*outLength];
        if (c < '0' || c > '9')
            break; // not a dec digit
        atof_buffer[*outLength] = c; // is a dec digit
        *outLength++;
    }
    if (str[*outLength] == '.')
    {
        atof_buffer[*outLength] = '.'; // is a decimal point
        *outLength++;
        while (true)
        {
            char c = str[*outLength];
            if (c < '0' || c > '9')
                break; // not a dec digit
            atof_buffer[*outLength] = c; // is a dec digit
            *outLength++;
        }
    }
    else if (str[*outLength] != 'f' && str[*outLength] != 'F')
    {
        return FALSE; // no decimal point or 'f' postfix
    }
    
    atof_buffer[*outLength] = '\0'; // null-terminate buffer
    *outValue = (float) atof(atof_buffer); // floating precision only

    if (str[*outLength] == 'f' || str[*outLength] == 'F')
        *outLength++; // include 'f' specifier in token length

    return TRUE;
}

//undefined4 __stdcall FUN_00410e90(char *param_1, int *param_2, ushort *param_3)
///FID:cs2_full_v401/tool/ac.exe: FUN_00410e90
BOOL __stdcall token_ParseChar(IN const char *str, OUT int *outLength, OUT unsigned short *outValue)

{
    int iVar1;
    
    if (*str != '\'')
    {
        return FALSE;
    }
    *outLength = 1;
    *outValue  = 0;

    // yup, this is as broken as it looks (first char value always <<= 8, unless EOF)
    while (str[*outLength] == '\0')
    {
        // shift character value before checking for exit quote. Very bad
        *outValue = *outValue << 8;
        char c = str[*outLength];
        if (c == '\'')
        {
            *outLength++;
            break;
        }
        // limited escape character support
        if (c == '\\' && (str[*outLength + 1] == '\'' || str[*outLength + 1] == '\\'))
        {
            *outLength++;
            c = str[*outLength];
        }
        *outValue = *outValue | (short)c;
        *outLength++;
    }
    return TRUE;
}


//undefined4 __stdcall FUN_00410de0(char *param_1,int *param_2,int param_3,uint param_4)
///FID:cs2_full_v401/tool/ac.exe: FUN_00410de0
BOOL __stdcall token_ParseString(IN const char *str, OUT int *outLength, OUT char *outBuffer, unsigned int bufferSize)

{
    if (*str != '\"')
    {
        return FALSE;
    }

    *outLength = 1;
    unsigned int bufpos = 0;
    
    for (int i = 0; i < bufferSize; i++, *outLength++)
    {
        char c = str[*outLength];
        if (c == '\0')
            break;
        if (c == '\"')
        {
            *outLength++;
            break;
        }
        if (c == '\\' && str[*outLength + 1] == '\"')
        {
            *outLength++;
        }
        else if (shiftjis_IsCharDoubleByte(&str[*outLength]))
        {
            outBuffer[bufpos] = str[*outLength];
            bufpos++, *outLength++;
        }
        outBuffer[bufpos] = str[*outLength];
    }

    outBuffer[bufpos] = '\0'; // potential access violation, nice
    return TRUE;
}


///FID:cs2_full_v401/tool/ac.exe: FUN_00411190
BOOL token_ParseKeyword(IN const char *str, OUT int *outValue)

{
    for (int i = 0; TABLE_KEYWORDS[i].ID != TOKEN_END; i++)
    {
        if (strcmp(str, TABLE_KEYWORDS[i].Name) == 0)
        {
            *outValue = TABLE_KEYWORDS[i].ID;
            return TRUE;
        }
    }
    return FALSE;
}

//REGEX: /(@|[A-Za-z_][0-9A-Za-z_]*)/
///FID:cs2_full_v401/tool/ac.exe: FUN_00411100
BOOL token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer)

{
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


///FID:cs2_full_v401/tool/ac.exe: FUN_00410880
BOOL __thiscall token_ParseSymbol(void *thiscls, IN const char *str, OUT int *outLength, OUT int *outValue)

{
    *outLength = 0;
    switch(str[0])
    {
    default:
        return FALSE;
    case '\n':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        *outLength = 1;
        *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
        return TRUE;
    case '\r':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        if (str[1] == '\n') {
            *outLength = 2;
        } else {
            *outLength = 1;
        }
        *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
        return TRUE;
    case '!':
        if (str[1] == '=') { // "!="
            *outValue = assert_enum(0x81, TOKEN_NOTEQUAL_OPERATOR);
            *outLength = 2;
        } else { // "!"
            *outValue = assert_enum(0x80, TOKEN_LOGICALNOT_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '#':
        *outValue = assert_enum(0x1, TOKEN_POUND);
        *outLength = 1;
        return TRUE;
    case '$':
        if (str[1] == '$') {
            *outValue = assert_enum(0x3, TOKEN_DOLLAR2);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x2, TOKEN_DOLLAR);
            *outLength = 1;
        }
        return TRUE;
    case '%':
        if (str[1] == '=') {
            *outValue = assert_enum(0x99, TOKEN_MODULUS_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x82, TOKEN_MODULUS_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '&':
        if (str[1] == '&') {
            *outValue = assert_enum(0x84, TOKEN_LOGICALAND_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = assert_enum(0x9a, TOKEN_BITWISEAND_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x83, TOKEN_BITWISEAND_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '(':
        *outValue = assert_enum(0x85, TOKEN_OPENPAREN_OPERATOR);
        *outLength = 1;
        return TRUE;
    case ')':
        *outValue = assert_enum(0x86, TOKEN_CLOSEPAREN_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '*':
        if (str[1] == '=') {
            *outValue = assert_enum(0x9b, TOKEN_MULTIPLY_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x87, TOKEN_MULTIPLY_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '+':
        if (str[1] == '+') {
            *outValue = assert_enum(0x89, TOKEN_INCREMENT_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = assert_enum(0x9c, TOKEN_ADD_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x88, TOKEN_ADD_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case ',':
        *outValue = assert_enum(0x8b, TOKEN_COMMA_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '-':
        if (str[1] == '-') {
            *outValue = assert_enum(0x8d, TOKEN_DECREMENT_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = assert_enum(0x9d, TOKEN_SUBTRACT_ASSIGN_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '>') {
            *outValue = assert_enum(0x8f, TOKEN_POINTTO_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x8c, TOKEN_SUBTRACT_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '.':
        if (str[1] == '.' && str[2] == '.') {
            *outValue = assert_enum(0x91, TOKEN_ELLIPSIS_OPERATOR);
            *outLength = 3;
        } else {
            *outValue = assert_enum(0x90, TOKEN_DOT_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '/':
        if (str[1] == '=') {
            *outValue = assert_enum(0x9e, TOKEN_DIVIDE_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x92, TOKEN_DIVIDE_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case ':':
        *outValue = assert_enum(0x93, TOKEN_COLON_OPERATOR);
        *outLength = 1;
        return TRUE;
    case ';':
        *outValue = assert_enum(0x94, TOKEN_SEMICOLON_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '<':
        if (str[1] == '<' && str[2] == '=') {
            *outValue = assert_enum(0x9f, TOKEN_LSHIFT_ASSIGN_OPERATOR);
            *outLength = 3;
        } else if (str[1] == '<') {
            *outValue = assert_enum(0x96, TOKEN_LSHIFT_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = assert_enum(0x97, TOKEN_LESSEQUAL_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x95, TOKEN_LESS_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '=':
        if (str[1] == '=') {
            *outValue = assert_enum(0xa0, TOKEN_EQUAL_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x98, TOKEN_ASSIGN_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '>':
        if (str[1] == '>' && str[2] == '=') {
            *outValue = assert_enum(0xa1, TOKEN_RSHIFT_ASSIGN_OPERATOR);
            *outLength = 3;
        } else if (str[1] == '>') {
            *outValue = assert_enum(0xa6, TOKEN_RSHIFT_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '=') {
            *outValue = assert_enum(0xa5, TOKEN_GREATEREQUAL_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0xa4, TOKEN_GREATER_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '?':
        *outValue = assert_enum(0xa7, TOKEN_QUESTIONMARK);
        *outLength = 1;
        return TRUE;
    case '[':
        *outValue = assert_enum(0xa8, TOKEN_OPENBRACKET_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '\\':
        *outValue = assert_enum(0x4, TOKEN_BACKSLASH);
        *outLength = 1;
        return TRUE;
    case ']':
        *outValue = assert_enum(0xa9, TOKEN_CLOSEBRACKET_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '^':
        if (str[1] == '=') {
            *outValue = assert_enum(0xa2, TOKEN_BITWISEXOR_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0xaa, TOKEN_BITWISEXOR_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '{':
        *outValue = assert_enum(0xab, TOKEN_OPENBRACE_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '|':
        if (str[1] == '=') {
            *outValue = assert_enum(0xa3, TOKEN_BITWISEOR_ASSIGN_OPERATOR);
            *outLength = 2;
        } else if (str[1] == '|') {
            *outValue = assert_enum(0xad, TOKEN_LOGICALOR_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0xac, TOKEN_BITWISEOR_OPERATOR);
            *outLength = 1;
        }
        return TRUE;
    case '}':
        *outValue = assert_enum(0xae, TOKEN_CLOSEBRACE_OPERATOR);
        *outLength = 1;
        return TRUE;
    case '~':
        *outValue = assert_enum(0xaf, TOKEN_BITWISENOT_OPERATOR);
        *outLength = 1;
        return TRUE;
    }
    return FALSE;
    // *outLength = 1;
    // *(int *)((int)thiscls + 0x10) = *(int *)((int)thiscls + 0x10) + 1;
    // return TRUE;
}


