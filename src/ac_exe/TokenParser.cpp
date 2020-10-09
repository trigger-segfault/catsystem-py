
// OOP-specific class definitions for TokenParser
//  (functional equivalent: SCRIPT_DECODER)

#include "common.h"
#ifdef KCLIB_OOP

#include "TokenParser.h"
#include "kclib_common.h"


//undefined4 * __fastcall FUN_00410780(undefined4 *param_1)
///FID: FUN_00410780
kclib::TokenParser::TokenParser()
{
    // ///TEMP: defines to stop errors
    // #define FUN_00407810(num, file, num2) ((void *)0)
    // #define FUN_004119b0(ptr) ((void *)ptr)

    // passed local_600 (sizeof == 0x128)
    // void *newIdentifiers; // undefined4 *puVar1; // shared with current in-use puVar1
    
    this->Buffer = nullptr;
    // *param_1 = 0; //0x0 (Buffer?)
    this->LastTokenType = assert_enum(0, TOKEN_NONE);
    // param_1[0x46] = 0; // 0x118 (LastTokenType?)
    // void *puVar1 = newIdentifiers = (void *)FUN_00407810(8, "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h", 0x27);
    // if (puVar1 == nullptr)
    // {
    //     newIdentifiers = nullptr;
    // }
    // else
    // {
    //     newIdentifiers = FUN_004119b0(puVar1); // Some STL container class
    // }

    // ///TEMP: cleanup defines to stop errors
    // #undef FUN_00407810
    // #undef FUN_004119b0

    // *(undefined4 **)(param_1 + 0x47) = puVar1; // 0x11c (Identifiers?)
    ///IGNORE: We're not handling the identifiers table for now
    this->Identifiers = nullptr; //(void *)newIdentifiers;
    this->Position = 0;
    this->Length = 0;
    this->EnableKeywords = 1;
    // return this;
    // param_1[2] = 0; // 0x8 (Position?)
    // param_1[1] = 0; // 0x4 (Length?)
    // param_1[0x48] = 1; // 0x120 (EnableKeywords?)
    // return param_1;
}

// void __fastcall FUN_004112a0(int *param_1)
///FID: FUN_004112a0
kclib::TokenParser::~TokenParser()
{
    // We can tell this is the destructor because
    // this->Identifiers isn't set to NULL afterwards
    //  (could also be a clear/reset function)
    // Wait, but then there's this->Buffer being set to NULL...
    //  maybe it's just how STL is..
    
    // ///TEMP: defines to stop errors (dummy statements)
    // #define FUN_00411a40(ptr) do {} while ((ptr) && false)
    // #define FUN_00407760(ptr) do {} while ((ptr) && false)
    
    if (this->Buffer != nullptr)
    {
        delete[] this->Buffer; //kclib_HeapFree(this->Buffer);
        this->Buffer = nullptr;
    }
    ///IGNORE: We're not handling identifiers table right now
    // if (this->Identifiers != nullptr)
    // {
    //     FUN_00411a40(this->Identifiers); // Messy function with __invalid_parameter, STL?
    //     // Has 0xcc, likely VS debug default mem"zero" method
    //     FUN_00407760((int *)this->Identifiers); // Messy function with __invalid_parameter, STL?
    // }

    // ///TEMP: cleanup defines to stop errors
    // #define FUN_00411a40
    // #define FUN_00407760

    // int **ppiVar1;
    // if (*param_1 != 0) {
    //   kclib_HeapFree(*(void **)param_1);
    //   *param_1 = 0;
    // }
    // ppiVar1 = (int **)param_1[0x47];
    // if (ppiVar1 != (int **)0x0) {
    //   FUN_00411a40(ppiVar1); // Messy function with __invalid_parameter, STL?
    //   // Has 0xcc, likely VS debug default mem"zero" method
    //   FUN_00407760((int *)ppiVar1); // Messy function with __invalid_parameter, STL?
    // }
    // return;
}

const char * kclib::TokenParser::GetBuffer() const
{
    return this->Buffer;
}

//undefined4 __thiscall FUN_004107e0(void *this,undefined8 *param_1)
///FID: FUN_004107e0
bool kclib::TokenParser::SetBuffer(IN const char *inBuffer)
{
    unsigned int str_len = std::strlen(inBuffer);
    if (str_len != 0)
    {
        str_len += 9;
        if (str_len > this->Capacity)
        {
            if (this->Buffer != nullptr)
            {
                delete[] this->Buffer; //kclib_HeapFree(this->Buffer);
                this->Buffer = nullptr;
                this->Capacity = 0;
                this->Length = 0;
            }
            //0x2a == TOKEN_CHAR_KEYWORD, woah...!
            this->Buffer = new char[str_len]; //this->Buffer = (char *)kclib_HeapAlloc(0x2a, str_len);
            this->Capacity = str_len;
        }
        std::memset(this->Buffer, 0, str_len); //kclib_MemZero(*(undefined8 **)this,uVar2);
        std::memcpy(this->Buffer, inBuffer, str_len);
        this->Length = str_len;
        this->Position = 0;
        this->LineNumber = 1;
        this->Filename[0] = '\0';
        return true;
    }
    return false;

    // void *pvVar1;
    // uint uVar2;
    
    // uVar2 = 0;
    // if (*(char *)inBuffer != '\0') {
    //     do {
    //         uVar2 = uVar2;
    //         uVar2 = uVar2 + 1;
    //     } while (*(char *)(uVar2 + (int)inBuffer) != '\0');
    //     if (uVar2 != 0) {
    //         uVar2 += 9;
    //         if (*(uint *)((int)this + 8) < uVar2) {
    //             if (*(int *)this != 0) {
    //                 kclib_HeapFree(*(void **)this);
    //                 *(undefined4 *)this = 0;
    //                 *(undefined4 *)((int)this + 8) = 0;
    //                 *(undefined4 *)((int)this + 4) = 0;
    //             }
    //             pvVar1 = kclib_HeapAlloc(0x2a, uVar2);
    //             *(void **)this = pvVar1;
    //             *(uint *)((int)this + 8) = uVar2;
    //         }
    //         memset(*(undefined8 **)this, 0, uVar2); //kclib_MemZero(*(undefined8 **)this,uVar2);
    //         memcpy(*(undefined8 **)this, inBuffer, uVar2);
    //         *(uint *)((int)this + 4) = uVar2;
    //         *(undefined4 *)((int)this + 0xc) = 0;
    //         *(undefined4 *)((int)this + 0x10) = 1;
    //         *(undefined *)((int)this + 0x14) = 0;
    //         return true;
    //     }
    // }
    // return false;
}

unsigned int kclib::TokenParser::GetLineNumber() const
{
    return this->LineNumber;
}

unsigned int kclib::TokenParser::GetLength() const
{
    return this->Length;
}

unsigned int kclib::TokenParser::GetCapacity() const
{
    return this->Capacity;
}

unsigned int kclib::TokenParser::GetPosition() const
{
    return this->Position;
}

kclib::TOKEN_TYPE kclib::TokenParser::GetLastTokenType() const
{
    return this->LastTokenType;
}


bool kclib::TokenParser::GetEnableKeywords() const
{
    return this->EnableKeywords;
}

//void __thiscall ScriptDecoder_Set_ScrUnk72(void *this, undefined4 newScrUnk72)
///FID: FUN_00410870
bool kclib::TokenParser::SetEnableKeywords(bool newEnableKeywords)
{
    this->EnableKeywords = newEnableKeywords;
    return newEnableKeywords;
}

//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
bool kclib::TokenParser::NextToken(OUT kclib::TOKEN_RESULT *token)
{
    while (this->ParseToken(token))
    {
        if (token->TokenType == TOKEN_WHITESPACE)
            return true;
    }
    return false;
}

//char * token_SkipComments(char *param_1,int *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411200
const char * kclib::TokenParser::SkipComments(IN const char *str, OUT int *outLines)
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
            return nullptr; // EOF

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
                    return nullptr; // EOF
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
                    return nullptr; // EOF
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
bool kclib::TokenParser::ParseInteger(IN const char *str, OUT int *outLength, OUT int *outValue)
{
    if (str[0] < '0' || str[0] > '9')
        return false; // not an numeric literal

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
            return false; // no floating points, strangely no checks for 'f'
    }
    return true;
}

//void __stdcall FUN_00411000(char *param_1,int *param_2,float *param_3)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411000
bool kclib::TokenParser::ParseFloat(IN const char *str, OUT int *outLength, OUT float *outValue)
{
    if (str[0] < '0' || str[0] > '9')
        return false; // not a numeric literal

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
        return false; // no decimal point or 'f' postfix
    }
    
    atof_buffer[*outLength] = '\0'; // null-terminate buffer
    *outValue = (float) std::atof(atof_buffer); // floating precision only

    if (str[*outLength] == 'f' || str[*outLength] == 'F')
        *outLength++; // include 'f' specifier in token length

    return true;
}

//undefined4 __stdcall FUN_00410e90(char *param_1, int *param_2, ushort *param_3)
///FID:cs2_full_v401/tool/ac.exe: FUN_00410e90
bool kclib::TokenParser::ParseChar(IN const char *str, OUT int *outLength, OUT unsigned short *outValue)
{
    int iVar1;
    
    if (*str != '\'')
    {
        return false;
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
    return true;
}

//undefined4 __stdcall FUN_00410de0(char *param_1,int *param_2,int param_3,uint param_4)
///FID:cs2_full_v401/tool/ac.exe: FUN_00410de0
bool kclib::TokenParser::ParseString(IN const char *str, OUT int *outLength, OUT char *outBuffer, unsigned int bufferSize)
{
    if (*str != '\"')
    {
        return false;
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
        else if (kclib::IsCharDoubleByte(&str[*outLength]))
        {
            outBuffer[bufpos] = str[*outLength];
            bufpos++, *outLength++;
        }
        outBuffer[bufpos] = str[*outLength];
    }

    outBuffer[bufpos] = '\0'; // potential access violation, nice
    return true;
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00411190
bool kclib::TokenParser::ParseKeyword(IN const char *str, OUT kclib::TOKEN_TYPE *outValue)
{
    for (int i = 0; TABLE_KEYWORDS[i].ID != TOKEN_END; i++)
    {
        if (strcmp(str, TABLE_KEYWORDS[i].Name) == 0)
        {
            *outValue = (TOKEN_TYPE) TABLE_KEYWORDS[i].ID;
            return true;
        }
    }
    return false;
}

//REGEX: /(@|[A-Za-z_][0-9A-Za-z_]*)/
///FID:cs2_full_v401/tool/ac.exe: FUN_00411100
bool kclib::TokenParser::ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer)
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
        return false;
    }
    outBuffer[*outLength] = '\0';
    return true;
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00410880
bool kclib::TokenParser::ParseSymbol(IN const char *str, OUT int *outLength, OUT kclib::TOKEN_TYPE *outValue)
{
    *outLength = 0;
    switch(str[0])
    {
    default:
        return false;
    case '\n':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        *outLength = 1;
        this->LineNumber++;
        return true;
    case '\r':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        if (str[1] == '\n') {
            *outLength = 2;
        } else {
            *outLength = 1;
        }
        this->LineNumber++;
        return true;
    case '!':
        if (str[1] == '=') { // "!="
            *outValue = assert_enum(0x81, TOKEN_NOTEQUAL_OPERATOR);
            *outLength = 2;
        } else { // "!"
            *outValue = assert_enum(0x80, TOKEN_LOGICALNOT_OPERATOR);
            *outLength = 1;
        }
        return true;
    case '#':
        *outValue = assert_enum(0x1, TOKEN_POUND);
        *outLength = 1;
        return true;
    case '$':
        if (str[1] == '$') {
            *outValue = assert_enum(0x3, TOKEN_DOLLAR2);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x2, TOKEN_DOLLAR);
            *outLength = 1;
        }
        return true;
    case '%':
        if (str[1] == '=') {
            *outValue = assert_enum(0x99, TOKEN_MODULUS_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x82, TOKEN_MODULUS_OPERATOR);
            *outLength = 1;
        }
        return true;
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
        return true;
    case '(':
        *outValue = assert_enum(0x85, TOKEN_OPENPAREN_OPERATOR);
        *outLength = 1;
        return true;
    case ')':
        *outValue = assert_enum(0x86, TOKEN_CLOSEPAREN_OPERATOR);
        *outLength = 1;
        return true;
    case '*':
        if (str[1] == '=') {
            *outValue = assert_enum(0x9b, TOKEN_MULTIPLY_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x87, TOKEN_MULTIPLY_OPERATOR);
            *outLength = 1;
        }
        return true;
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
        return true;
    case ',':
        *outValue = assert_enum(0x8b, TOKEN_COMMA_OPERATOR);
        *outLength = 1;
        return true;
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
        return true;
    case '.':
        if (str[1] == '.' && str[2] == '.') {
            *outValue = assert_enum(0x91, TOKEN_ELLIPSIS_OPERATOR);
            *outLength = 3;
        } else {
            *outValue = assert_enum(0x90, TOKEN_DOT_OPERATOR);
            *outLength = 1;
        }
        return true;
    case '/':
        if (str[1] == '=') {
            *outValue = assert_enum(0x9e, TOKEN_DIVIDE_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x92, TOKEN_DIVIDE_OPERATOR);
            *outLength = 1;
        }
        return true;
    case ':':
        *outValue = assert_enum(0x93, TOKEN_COLON_OPERATOR);
        *outLength = 1;
        return true;
    case ';':
        *outValue = assert_enum(0x94, TOKEN_SEMICOLON_OPERATOR);
        *outLength = 1;
        return true;
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
        return true;
    case '=':
        if (str[1] == '=') {
            *outValue = assert_enum(0xa0, TOKEN_EQUAL_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0x98, TOKEN_ASSIGN_OPERATOR);
            *outLength = 1;
        }
        return true;
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
        return true;
    case '?':
        *outValue = assert_enum(0xa7, TOKEN_QUESTION_OPERATOR);
        *outLength = 1;
        return true;
    case '[':
        *outValue = assert_enum(0xa8, TOKEN_OPENBRACKET_OPERATOR);
        *outLength = 1;
        return true;
    case '\\':
        *outValue = assert_enum(0x4, TOKEN_BACKSLASH);
        *outLength = 1;
        return true;
    case ']':
        *outValue = assert_enum(0xa9, TOKEN_CLOSEBRACKET_OPERATOR);
        *outLength = 1;
        return true;
    case '^':
        if (str[1] == '=') {
            *outValue = assert_enum(0xa2, TOKEN_XOR_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0xaa, TOKEN_XOR_OPERATOR);
            *outLength = 1;
        }
        return true;
    case '{':
        *outValue = assert_enum(0xab, TOKEN_OPENBRACE_OPERATOR);
        *outLength = 1;
        return true;
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
        return true;
    case '}':
        *outValue = assert_enum(0xae, TOKEN_CLOSEBRACE_OPERATOR);
        *outLength = 1;
        return true;
    case '~':
        *outValue = assert_enum(0xaf, TOKEN_BITWISENOT_OPERATOR);
        *outLength = 1;
        return true;
    }
    return false;
    // *outLength = 1;
    // *(int *)((int)this + 0x10) = *(int *)((int)this + 0x10) + 1;
    // return true;
}

#endif
