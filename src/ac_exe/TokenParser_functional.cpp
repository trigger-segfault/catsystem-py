
// functional-specific function definitions for SCRIPT_DECODER and SCRIPT_READER
//  (OOP equivalent: TokenParser, ScriptReader)

#include "common.h"
#ifndef KCLIB_OOP

#include "TokenParser_functional.h"
#include "kclib_common.h"
#include "ghidra_types_min.h"

//undefined4 * __fastcall FUN_00410780(undefined4 *param_1)
///FID: FUN_00410780
SCRIPT_DECODER * __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this)

{
    ///TEMP: defines to stop errors
    #define FUN_00407810(num, file, num2) ((void *)0)
    #define FUN_004119b0(ptr) ((void *)ptr)

    // passed local_600 (sizeof == 0x128)
    void *newIdentifiers; // undefined4 *puVar1; // shared with current in-use puVar1
    
    this->Buffer = NULL;
    // *param_1 = 0; //0x0 (Buffer?)
    this->LastTokenType = assert_enum(0, TOKEN_NONE);
    // param_1[0x46] = 0; // 0x118 (LastTokenType?)
    void *puVar1 = newIdentifiers = (void *)FUN_00407810(8, "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h", 0x27);
    if (puVar1 == NULL)
    {
        newIdentifiers = NULL;
    }
    else
    {
        newIdentifiers = FUN_004119b0(puVar1); // Some STL container class
    }

    ///TEMP: cleanup defines to stop errors
    #undef FUN_00407810
    #undef FUN_004119b0

    // *(undefined4 **)(param_1 + 0x47) = puVar1; // 0x11c (Identifiers?)
    this->Identifiers = (void *)newIdentifiers;
    this->Position = 0;
    this->Length = 0;
    this->EnableKeywords = 1;
    return this;
    // param_1[2] = 0; // 0x8 (Position?)
    // param_1[1] = 0; // 0x4 (Length?)
    // param_1[0x48] = 1; // 0x120 (EnableKeywords?)
    // return param_1;
}

// void __fastcall FUN_004112a0(int *param_1)
///FID: FUN_004112a0
void __fastcall ScriptDecoder_dtor(SCRIPT_DECODER *this)

{
    // We can tell this is the destructor because
    // this->Identifiers isn't set to NULL afterwards
    //  (could also be a clear/reset function)
    // Wait, but then there's this->Buffer being set to NULL...
    //  maybe it's just how STL is..
    
    ///TEMP: defines to stop errors (dummy statements)
    #define FUN_00411a40(ptr) do {} while ((ptr) && false)
    #define FUN_00407760(ptr) do {} while ((ptr) && false)
    
    if (this->Buffer != NULL)
    {
        kclib_HeapFree(this->Buffer);
        this->Buffer = NULL;
    }
    if (this->Identifiers != NULL)
    {
        FUN_00411a40(this->Identifiers); // Messy function with __invalid_parameter, STL?
        // Has 0xcc, likely VS debug default mem"zero" method
        FUN_00407760((int *)this->Identifiers); // Messy function with __invalid_parameter, STL?
    }

    ///TEMP: cleanup defines to stop errors
    #define FUN_00411a40
    #define FUN_00407760

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


//undefined4 __thiscall FUN_004107e0(void *this,undefined8 *param_1)
///FID: FUN_004107e0
BOOL __thiscall ScriptDecoder_SetBuffer(SCRIPT_DECODER *this, IN const char *inBuffer)

{
    unsigned int str_len = strlen(inBuffer);
    if (str_len != 0)
    {
        str_len += 9;
        if (str_len > this->Capacity)
        {
            if (this->Buffer != NULL)
            {
                kclib_HeapFree(*(void **)this);
                this->Buffer = NULL;
                this->Capacity = 0;
                this->Length = 0;
                *(undefined4 *)this = 0;
                *(undefined4 *)((int)this + 8) = 0;
                *(undefined4 *)((int)this + 4) = 0;
            }
            //0x2a == TOKEN_CHAR_KEYWORD, woah...!
            this->Buffer = (char *)kclib_HeapAlloc(0x2a, str_len);
            this->Capacity = str_len;
        }
        memset(this->Buffer, 0, str_len); //kclib_MemZero(*(undefined8 **)this,uVar2);
        memcpy(this->Buffer, inBuffer, str_len);
        this->Length = str_len;
        this->Position = 0;
        this->LineNumber = 1;
        this->Filename[0] = '\0';
        return TRUE;
    }
    return FALSE;

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
    //         return TRUE;
    //     }
    // }
    // return FALSE;
}

//void __thiscall ScriptDecoder_Set_ScrUnk72(void *this, undefined4 newScrUnk72)
///FID: FUN_00410870
unsigned int __thiscall ScriptDecoder_SetScrUnk72(SCRIPT_DECODER *this, unsigned int newScrUnk72)

{
    this->EnableKeywords = newScrUnk72;
    return newScrUnk72;
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00411c40
void __fastcall ScriptReader_ctor(SCRIPT_READER *this)

{
    this->Buffer = NULL;
    this->Position = 0;
    this->RdrUnk3 = 0; // never seen elsewhere
}

//uint __fastcall FUN_00411d40(int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d40
BOOL __fastcall ScriptReader_IsEOF(SCRIPT_READER *this)

{
    //return (uint)(*(uint *)(param_1 + 4) <= *(uint *)(param_1 + 8));
    return (BOOL)(this->Position >= this->Length);
}


// Used to strip CRLF from script?
//void __fastcall FUN_00411d50(char **param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d50
void __fastcall ScriptReader_ConvertToLF(SCRIPT_READER *this)

{
    if (this->Buffer == NULL)
        return;

    unsigned int str_len = (unsigned int)strlen(this->Buffer);

    // temporary heap to hold script decoder buffer,
    // it seems this may be a unnecessary, since the write length
    //   is at least shorter and most equal in length,
    //   and wont override read position.
    //   at most this may be done for the double-null termination??
    char *puHeap = (char *)kclib_HeapAlloc(0, str_len + 1); // +1 for double-null termination
    memcpy(puHeap, this->Buffer, str_len);

    // replace CRLF ("\r\n") with LF ("\n")
    unsigned int j = 0; // output position
    for (unsigned int i = 0; i < str_len; i++, j++)
    {
        if (puHeap[i] == '\r' && puHeap[i + 1] == '\n') // CRLF "\r\n"
        {
            this->Buffer[j] = '\n'; // LF "\n"
            i++;
        }
        else
        {
            this->Buffer[j] = puHeap[i];
        }
    }

    // double-null terminate it seems
    this->Buffer[j] = '\0';
    this->Buffer[j + 1] = '\0';
    kclib_HeapFree(puHeap);
}

//void __thiscall FUN_00411cd0(void *this,uint param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cd0
void __thiscall ScriptReader_SetPosition(SCRIPT_READER *this, unsigned int newPosition)

{
    if (newPosition >= this->Length)
    {
        newPosition = this->Length;
    }
    this->Position = newPosition;
}

//void __fastcall FUN_00411c50(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c50
void __fastcall ScriptReader_Close(SCRIPT_READER *this)

{
    if (this->Buffer != NULL)
    {
        kclib_HeapFree(this->Buffer);
    }
}



//uint __fastcall FUN_00411c60(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c60
unsigned int __fastcall ScriptReader_FUN_00411c60(SCRIPT_READER *this)

{
    if (this->Position >= this->Length)
        return 0;

    if (shiftjis_IsCharDoubleByte(this->Buffer + this->Position))
    {
        unsigned int c1 = (unsigned int)this->Buffer[this->Position] & 0xff;
        unsigned int c2 = (unsigned int)this->Buffer[this->Position + 1] & 0xff;
        this->Position += 2;
        return this->Position & 0xffff0000 | (c2 << 8 | c1);

        // CONCAT31(x,y) - concatenates two operands together into a larger size object
        // The "3" is the size of x in bytes.
        // The "1" is the size of y in bytes.
        // The result is the 4-byte concatenation of the bits in "x" with the bits in "y". The "x" forms the most signifigant part of the result, "y" the least.
        //return this->Position & 0xffff0000 | (unsigned int)CONCAT11(c2, c1);
    }
    else
    {
        char c = this->Buffer[this->Position];
        this->Position++;
        return (this->Position & 0xffff0000) | (unsigned int)c;
    }
}


//undefined4 __thiscall FUN_00411dd0(void *this,char *filename)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411dd0
BOOL __thiscall ScriptReader_Open(SCRIPT_READER *this, IN const char *filename)

{
    int iVar2 = 1;
    //DAT_004c3430 is likely kcFileServer,
    // or whatever id used to manage integration files
    ///TMP:IGNORE: int iVar2 = kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, 0, 3); // 3 may be some identifier type, like to open as ScriptDecode2 class, etc. Or maybe flags
    if (iVar2 == 0)
        return FALSE;
        
    char *buffer = (char *)kclib_HeapAlloc(0, iVar2 + 8U);
    this->Buffer = buffer; //*(undefined8 **)this = buffer;
    memset(buffer, 0, iVar2 + 8U); //kclib_MemZero(buffer, iVar2 + 8U);
    
    ///TMP:IGNORE: kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, *(int *)this, 3);
    
    this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

    ScriptReader_ConvertToLF(this);
    
    this->Length = strlen(this->Buffer);
    return TRUE;
}

//void __thiscall FUN_00411cf0(void *this,int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cf0
void __thiscall ScriptReader_NextLine(SCRIPT_READER *this, OPTIONAL OUT char *outLine)

{
    // outLine is optional, if excluded, decoder still advances to next line

    int linepos = 0;
    while (this->Position < this->Length)
    {
        char c = this->Buffer[this->Position];
        if (c == '\n')
        {
            this->Position++;
            break;
        }
        if (outLine != NULL)
        {
            outLine[linepos] = c;
        }
        this->Position++;
        linepos++;
    }
    if (outLine != NULL)
    {
        outLine[linepos] = '\0';
    }
}


//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
BOOL __cdecl token_NextSkipWS(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token)

{
    while (token_Next(this, token))
    {
        if (token->TokenType != TOKEN_WHITESPACE)
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
                    ++*outLines; // lines-consumed in block comment? very strange
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
BOOL __stdcall token_ParseUnsignedInteger(IN const char *str, OUT int *outLength, OUT int *outValue)

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
            ++*outLength;
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
            ++*outLength;
        }
        while (str[*outLength] >= '0' && str[*outLength] <= '9')
        {
            *outValue = *outValue * 10 + str[*outLength] - '0';
            ++*outLength;
            char c = str[*outLength];
            if (c >= '0' && c <= '9')
                c -= '0'; //('0' - 0x0);
            else
                break; // not a dec digit
            *outValue = *outValue * 10 + (int)c; // c is set to value of digit
            ++*outLength;
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
        ++*outLength;
    }
    if (str[*outLength] == '.')
    {
        atof_buffer[*outLength] = '.'; // is a decimal point
        ++*outLength;
        while (true)
        {
            char c = str[*outLength];
            if (c < '0' || c > '9')
                break; // not a dec digit
            atof_buffer[*outLength] = c; // is a dec digit
            ++*outLength;
        }
    }
    else if (str[*outLength] != 'f' && str[*outLength] != 'F')
    {
        return FALSE; // no decimal point or 'f' postfix
    }
    
    atof_buffer[*outLength] = '\0'; // null-terminate buffer
    *outValue = (float) atof(atof_buffer); // floating precision only

    if (str[*outLength] == 'f' || str[*outLength] == 'F')
        ++*outLength; // include 'f' specifier in token length

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
            ++*outLength;
            break;
        }
        // limited escape character support
        if (c == '\\' && (str[*outLength + 1] == '\'' || str[*outLength + 1] == '\\'))
        {
            ++*outLength;
            c = str[*outLength];
        }
        *outValue = *outValue | (short)c;
        ++*outLength;
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
    unsigned int i = 0;
    
    for (; i < bufferSize; i++, ++*outLength)
    {
        char c = str[*outLength];
        if (c == '\0')
            break;
        if (c == '\"')
        {
            ++*outLength;
            break;
        }
        if (c == '\\' && str[*outLength + 1] == '\"')
        {
            ++*outLength;
        }
        else if (shiftjis_IsCharDoubleByte(&str[*outLength]))
        {
            outBuffer[i] = str[*outLength];
            i++, ++*outLength;
        }
        outBuffer[i] = str[*outLength];
    }

    outBuffer[i] = '\0'; // potential access violation, nice
    return TRUE;
}


///FID:cs2_full_v401/tool/ac.exe: FUN_00411190
BOOL token_ParseKeyword(IN const char *str, OUT TOKEN_TYPE *outValue)

{
    for (int i = 0; TABLE_KEYWORDS[i].ID != TOKEN_END; i++)
    {
        if (strcmp(str, TABLE_KEYWORDS[i].Name) == 0)
        {
            *outValue = (TOKEN_TYPE) TABLE_KEYWORDS[i].ID;
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
        ++*outLength;
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
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
            ++*outLength;
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
BOOL __thiscall token_ParseSymbol(SCRIPT_DECODER *this, IN const char *str, OUT int *outLength, OUT TOKEN_TYPE *outValue)

{
    *outLength = 0;
    switch(str[0])
    {
    default:
        return FALSE;
    case '\n':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        *outLength = 1;
        this->LineNumber++;
        return TRUE;
    case '\r':
        *outValue = assert_enum(0x5, TOKEN_NEWLINE);
        if (str[1] == '\n') {
            *outLength = 2;
        } else {
            *outLength = 1;
        }
        this->LineNumber++;
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
        *outValue = assert_enum(0xa7, TOKEN_QUESTION_OPERATOR);
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
            *outValue = assert_enum(0xa2, TOKEN_XOR_ASSIGN_OPERATOR);
            *outLength = 2;
        } else {
            *outValue = assert_enum(0xaa, TOKEN_XOR_OPERATOR);
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
    // *(int *)((int)this + 0x10) = *(int *)((int)this + 0x10) + 1;
    // return TRUE;
}

#endif
