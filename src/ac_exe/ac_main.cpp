#include <stdio.h>
#include <windows.h>
#include <vector>

////// WINAPI USED //////

// DWORD GetFullPathNameA(
//   LPCSTR lpFileName,
//   DWORD  nBufferLength,
//   LPSTR  lpBuffer,
//   LPSTR  *lpFilePart
// );
// <https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfullpathnamea>

void __cdecl ac_CoUninitializeCleanup(void)

{

}

BOOL __cdecl ac_CoInitializeSetup(unsigned int param_1, int param_2)

{
    // undefined4 *puVar1;
    // void *this;
    // int iVar2;
    
    // _guard_check_icall();
    // CoInitialize((LPVOID)0x0);
    // if (param_2 != 0) {
    //     puVar1 = (undefined4 *)
    //             FUN_00407810(0x18,"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
    //                         ,0x27);
    //     if (puVar1 != (undefined4 *)0x0) {
    //         FUN_0042efd0(puVar1);
    //     }
    // }
    // if (DAT_004c29dc == 0) {
    //     this = (void *)FUN_00407810(0x580,
    //                             "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
    //                             ,0x27);
    //     if (this != (void *)0x0) {
    //         FUN_004071d0(this,param_1,param_2);
    //     }
    // }
    // FUN_004229b0(0xa00000);
    // if (DAT_004c3430 == 0) {
    //     puVar1 = (undefined4 *)
    //          FUN_00407810(0x13524,
    //                       "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",0x27
    //                      );
    //     if (puVar1 != (undefined4 *)0x0) {
    //     FUN_0040ab40(puVar1);
    //     }
    // }
    // iVar2 = FUN_00407810(1,"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",0x27)
    // ;
    // if (iVar2 != 0) {
    //     FUN_0042e9e0(iVar2);
    // }
    // FUN_00405c90();
    return TRUE;
}

//BOOL __cdecl FUN_00412200(const unsigned char *param_1)
BOOL __cdecl shiftjis_IsCharDoubleByte(IN const char *lpChar)

{
    const unsigned char *ch = (const unsigned char *)lpChar;
    if (ch == NULL || ch[0] == 0x00)
        return FALSE;
    // ch[0] in range [$81-$9f, $e0-$ef]
    // ch[1] in range [$40-$7e, $80-$fc]
    return ((ch[0] > 0x80 && ch[0] < 0xa0) || (ch[0] > 0xdf && ch[0] < 0xf0)) &&
           ((ch[1] > 0x3f && ch[1] < 0x7f) || (ch[1] > 0x7f && ch[1] < 0xfd));
}
//BOOL __cdecl FUN_00412520(char *param_1, char *param_2)
BOOL __cdecl shiftjis_GetParentDirectory(const char *fullpath, char *outParentdir)

{
    int seppos = 0;
    // get index of last path separator
    for (int i = 0; fullpath[i] != '\0'; i++)
    {
        if (shiftjis_IsCharDoubleByte(&fullpath[i]))
        {
            i++;
        }
        else if (fullpath[i] == '\\' || fullpath[i] == '/')
        {
            seppos = i;
        }
    }
    // copy basepath
    //memcpy(outParentdir, fullpath, seppos);
    for (int i = 0; i < seppos; i++)
    {
        outParentdir[i] = fullpath[i];
    }
    outParentdir[seppos] = '\0';

    return seppos != 0;
}   


BOOL __cdecl shiftjis_GetAbsolutePath(IN const char *filename, OUT char *outFullpath, OUT char **outBasename)

{
    LPSTR basename;
    
    if (!GetFullPathNameA(filename, MAX_PATH, outFullpath, &basename))
    {
        return FALSE;
    }
    if (outBasename != (char **)NULL)
    {
        *outBasename = basename;
    }
    return TRUE;
}

//undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
BOOL __cdecl shiftjis_ChangeExtension(char *filename, char *extension)

{
    if (filename == NULL)
        return FALSE;
    
    int extpos = -1;
    int namelen = strlen(filename);
    for (int i = namelen - 1; i >= 0; i--)
    {
        if (filename[i] == '\\' || filename[i] == '/')
        {
            extpos = namelen;
            break;
        }
        if (filename[i] == '.')
        {
            extpos = i;
            break;
        }
    }
    if (extension != NULL)
    {
        filename[extpos++] = '.';
        for (int i = 0; extension[i] != '\0'; i++)
        {
            filename[extpos++] = extension[i];
        }
    }
    filename[extpos] = '\0';
    return TRUE;
}

// //undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
// BOOL __cdecl shiftjis_ChangeExtension(char *filename, char *extension)

// {

//     char cVar1;
//     char *pcVar2;
//     undefined4 *puVar3;
//     uint uVar4;
//     undefined4 *puVar5;
//     undefined2 *puVar6;
//     undefined4 *puVar7;
    
//     if (param_1 == (char *)0x0) {
//         return 0;
//     }
//     pcVar2 = param_1;
//     do {
//         cVar1 = *pcVar2;
//         pcVar2 = pcVar2 + 1;
//     } while (cVar1 != '\0');
//     pcVar2 = pcVar2 + (-1 - (int)(param_1 + 1));
//     while( true ) {
//         if ((int)pcVar2 < 0) goto LAB_00412129;
//         cVar1 = pcVar2[(int)param_1];
//         if ((cVar1 == '\\') || (cVar1 == '/')) break;
//         if (cVar1 == '.') goto LAB_00412129;
//         pcVar2 = pcVar2 + -1;
//     }
//     pcVar2 = param_1;
//     do {
//         cVar1 = *pcVar2;
//         pcVar2 = pcVar2 + 1;
//     } while (cVar1 != '\0');
//     pcVar2 = pcVar2 + -(int)(param_1 + 1);
//     LAB_00412129:
//     if (pcVar2 == (char *)0xffffffff) {
//         pcVar2 = param_1;
//         do {
//             cVar1 = *pcVar2;
//             pcVar2 = pcVar2 + 1;
//         } while (cVar1 != '\0');
//         pcVar2 = pcVar2 + -(int)(param_1 + 1);
//     }
//     pcVar2[(int)param_1] = '\0';
//     if (param_2 != (undefined4 *)0x0) {
//         puVar6 = (undefined2 *)(param_1 + -1);
//         do {
//             pcVar2 = (char *)((int)puVar6 + 1);
//             puVar6 = (undefined2 *)((int)puVar6 + 1);
//         } while (*pcVar2 != '\0');
//         *puVar6 = 0x2e;
//         puVar3 = param_2;
//         do {
//             cVar1 = *(char *)puVar3;
//             puVar3 = (undefined4 *)((int)puVar3 + 1);
//         } while (cVar1 != '\0');
//         puVar7 = (undefined4 *)(param_1 + -1);
//         do {
//             pcVar2 = (char *)((int)puVar7 + 1);
//             puVar7 = (undefined4 *)((int)puVar7 + 1);
//         } while (*pcVar2 != '\0');
//         uVar4 = (uint)(undefined4 *)((int)puVar3 - (int)param_2) >> 2;
//         puVar5 = param_2;
//         while (uVar4 != 0) {
//             uVar4 -= 1;
//             *puVar7 = *puVar5;
//             puVar5 = puVar5 + 1;
//             puVar7 = puVar7 + 1;
//         }
//         uVar4 = (uint)(undefined4 *)((int)puVar3 - (int)param_2) & 3;
//         while (uVar4 != 0) {
//             uVar4 -= 1;
//             *(undefined *)puVar7 = *(undefined *)puVar5;
//             puVar5 = (undefined4 *)((int)puVar5 + 1);
//             puVar7 = (undefined4 *)((int)puVar7 + 1);
//         }
//     }
//     return 1;
// }


typedef enum {
  CMD_ID    = 0, // [ID] [min] (max)
  CMD_SET   = 1, // set [var] [min] (max)
  CMD_LOOP  = 2, // loop [var] [label]
  CMD_JUMP  = 3, // jump [label]
  CMD_IF    = 4, // if [var] [label]
  CMD_IFE   = 5, // ife [var] [value] [label]
  CMD_IFN   = 6, // ifn [var] [value] [label]
  CMD_IFG   = 7, // ifg [var] [value] [label]
  CMD_IFS   = 8, // ifs [var] [value] [label]
  CMD_IFGE  = 9, // ifge [var] [value] [label]
  CMD_IFSE  = 10, // ifse [var] [value] [label]
  CMD_MAX   = 11, // max [var]
  CMD_BLEND = 12, // blend [param]
  CMD_DISP  = 13, // disp [param]
  CMD_POS   = 14, // pos [x] [y]
  CMD_WAIT  = 15, // wait [min] [max]
  CMD_ADD   = 16, // add [var] [value]
  CMD_SUB   = 17  // sub [var] [value]
} ANM_CMD;

typedef enum {
  TYPE_CONST    = 0, // 16
  TYPE_VARIABLE = 1, // @16
  TYPE_LABEL    = 2  // MyLabel
} VAR_TYPE;


// "ANM\0" (little endian)
#define MAGIC_ANM 0x4d4e41

// KCANMSCRIPT STRUCTS:

#pragma pack(push, 1)

// (field offset,size prefixes are in hex)

typedef struct {
    /*0,4*/   VAR_TYPE VarType;
    /*4,4*/   UINT Value;
    /*8*/
} ANM_ARG;

typedef struct {
    /*0,4*/   ANM_CMD CmdType;
    /*4,40*/  ANM_ARG Args[8];
    /*44*/
} ANM_TIMELINE;


typedef struct {
    // /*0,18*/  CHAR UnkName[24]; // 0x0:0x16?
    /*0,18*/  UINT padding[6];
    /*18,1c*/ INT **Field72Unk6ptr;
    /*1c,20*/ INT Field72Unk7;
    /*20*/
} ANM_UNK72;

typedef struct {
    /*0,4*/   UINT TimelineCount; // Number of timelines
    // /*4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
    /*4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
    /*8,4*/   UINT Unk2; // (unknown)
    /*c,4*/   UINT Unk3; // (unknown)
    /*10,4*/  UINT Counter; // used in CMD_ID, CMD_WAIT, and during command loop
    /*14,4*/  INT Instruction; // instruction ptr index
    /*18,4*/  UINT Wait; // used in CMD_ID, CMD_WAIT
    /*1c,4*/  UINT MaxFrame; // used in CMD_MAX
    /*20,ff*/ INT Variables[64]; // live variables
    /*120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
    /*124,4*/ FLOAT Unk73; // (unknown)
    /*128,4*/ INT FrameID; // current frame
    /*12c,4*/ INT Blend; // used in CMD_BLEND
    /*130,4*/ BOOL Disp; // used in CMD_DISP
    /*134,4*/ INT PosX; // used in CMD_POS
    /*138,4*/ INT PosY; // used in CMD_POS
    /*140*/
} ANM_SCRIPT;

typedef struct {
    /*0,4*/   UINT Magic; // "ANM\0" MAGIC_ANM
    /*4,4*/   UINT AnmUnk1;
    /*8,4*/   UINT TimelineCount;
    /*c,4*/   UINT AnmUnk3;
    /*10,4*/  UINT AnmUnk4;
    /*14,4*/  UINT AnmUnk5;
    /*18,4*/  UINT AnmUnk6;
    /*1c,4*/  UINT AnmUnk7;
    /*20*/
} ANM_FILEHEADER;

typedef struct {
    /*0,20*/  ANM_FILEHEADER Header;
    /*20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
} ANM_FILEINFO;

#pragma pack(pop)


typedef struct {
    CHAR *Buffer;
    UINT Length;
    UINT Position;
    UINT ScrUnk3;
} AC_SCRIPTDECODER;

// uint __fastcall FUN_00411d40(int param_1)
BOOL __fastcall ac_ScriptDecoder_IsEOF(AC_SCRIPTDECODER *acscr)

{
    return (BOOL)(acscr->Length <= acscr->Position);
}

//undefined4 __stdcall FUN_00410f10(char *param_1,int *param_2,int *param_3)
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
//         c = str[*outLength];
//         while (('/' < cVar1 && (str[*outLength] < ':'))) {
//             iVar2 = *outValue;
//             *outValue = iVar2 * 10;
//             *outValue = (int)str[*outLength] + -0x30 + iVar2 * 10;
//             *outLength = *outLength + 1;
//             cVar1 = str[*outLength];
//         }
//         if (str[*outLength] == '.') {
//             return FALSE;
//         }
//     }
//         while (true)
//         {
//             while (true)
//             {
//                 while ((cVar1 = str[*outLength], '/' < cVar1 && (cVar1 < ':')))
//                 {
//                     *outValue = *outValue * 0x10 + (int)cVar1 + -0x30;
//                     *outLength = *outLength + 1;
//                 }
//                 if ((cVar1 < 'a') || ('f' < cVar1)) break;
//                 *outValue = *outValue * 0x10 + (int)cVar1 + -0x57;
//                 *outLength = *outLength + 1;
//             }
//             if ((cVar1 < 'A') || ('F' < cVar1)) break;
//             *outValue = *outValue * 0x10 + (int)cVar1 + -0x37;
//             *outLength = *outLength + 1;
//         }
//     }
//     char cVar1;
//     int iVar2;
//     if ((*str < '0') || ('9' < *str)) {
//         return FALSE;
//     }
//     *outLength = 0;
//     *outValue = 0;
//     if ((*str == '0') && ((str[1] == 'x' || (str[1] == 'X')))) {
//         *outLength = *outLength + 2;
//         while( true ) {
//             while( true ) {
//                 while ((cVar1 = str[*outLength], '/' < cVar1 && (cVar1 < ':'))) {
//                     *outValue = *outValue * 0x10 + (int)cVar1 + -0x30;
//                     *outLength = *outLength + 1;
//                 }
//                 if ((cVar1 < 'a') || ('f' < cVar1)) break;
//                 *outValue = *outValue * 0x10 + (int)cVar1 + -0x57;
//                 *outLength = *outLength + 1;
//             }
//             if ((cVar1 < 'A') || ('F' < cVar1)) break;
//             *outValue = *outValue * 0x10 + (int)cVar1 + -0x37;
//             *outLength = *outLength + 1;
//         }
//     }
//     else {
//         cVar1 = str[*outLength];
//         while (('/' < cVar1 && (str[*outLength] < ':'))) {
//             iVar2 = *outValue;
//             *outValue = iVar2 * 10;
//             *outValue = (int)str[*outLength] + -0x30 + iVar2 * 10;
//             *outLength = *outLength + 1;
//             cVar1 = str[*outLength];
//         }
//         if (str[*outLength] == '.') {
//             return FALSE;
//         }
//     }
//     return TRUE;
// }


//void __stdcall FUN_00411000(char *param_1,int *param_2,float *param_3)
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
        else if (!shiftjis_IsCharDoubleByte(&str[*outLength]))
        {
            outBuffer[bufpos] = str[*outLength];
            bufpos++, *outLength++;
        }
        outBuffer[bufpos] = str[*outLength];
    }

    outBuffer[bufpos] = '\0'; // potential access violation, nice
    return TRUE;
}
//     while (bufpos >= bufferSize && str[*outLength] != '\0')
//     {
//         char c = str[*outLength];
//         if (c == '\"')
//         {
//             *outLength++;
//             break;
//         }
//         if (!shiftjis_IsCharDoubleByte(&str[*outLength]))
//         {
//             outBuffer[bufpos] = str[*outLength];
//             bufpos++; *outLength++;
//             outBuffer[bufpos] = str[*outLength];
//         }
//         else 
//         {
//             if (c == '\"')
//             {
//                 *outLength++;
//                 break;
//             }
//             // no double-backslash escape makes sense in Windows env
//             if (c == '\\' && str[*outLength + 1] == '\"')
//             {
//                 *outLength++;
//                 c = str[*outLength];
//             }
//             outBuffer[bufpos] = c;
//         }
//         bufpos++; *outLength++;
//         if (bufpos >= bufferSize)
//         {
//             break; // potential access violation, see blow
//         }
//     }
//     outBuffer[bufpos] = '\0'; // potential access violation, nice
//     return TRUE;

//     if (bufferSize != 0)
//     {
//         while (str[*outLength] != '\0')
//         {
//             if (!shiftjis_IsCharDoubleByte(&str[*outLength]))
//             {
//                 char c = str[*outLength];
//                 if (c == '\"') {
//                     *outLength++;
//                     break;
//                 }
//                 if (c == '\\' && str[*outLength + 1] != '\"')
//                 {
//                     *outLength++;
//                     c = str[*outLength];
//                 }
//                 else
//                 {
//                     outBuffer[bufpos] = c;
//                 }
//                     if (str[*outLength + 1] != '\"')
//                     {
//                         outBuffer[bufpos] = '\\';
//                         // *(undefined *)(uVar4 + param_3) = 0x5c;
//                         goto LAB_00410e5a;
//                     }
//                     else
//                     {
//                         *outLength++;
//                         c = str[*outLength];
//                     }
//                     // *param_2 = iVar1 + 1;
//                     // cVar3 = param_1[iVar1 + 1];
//                 }
//                 else
//                 {
//                     outBuffer[bufpos] = c;
//                 }
                
                
//                 // *(char *)(uVar4 + param_3) = cVar3;
//             }
//             else
//             {
//                 outBuffer[bufpos] = str[*outLength];
//                 *outLength++;
//                 bufpos++;
//                 outBuffer[bufpos] = str[*outLength];
//             }
//             *outLength++;
//             bufpos++;
//             if (bufpos >= bufferSize)
//             {
//                 outBuffer[bufpos] = '\0'; // potential memory leak, nice
//                 return TRUE;
//             }
//             *param_2 = *param_2 + 1;
//             uVar4 += 1;
//             if (param_4 <= uVar4) {
//                 *(undefined *)(uVar4 + param_3) = 0;
//                 return TRUE;
//             }
//         }
//     }

//     int iVar1;
//     BOOL BVar2;
//     char cVar3;
//     uint uVar4;
    
//     if (*param_1 != '\"') {
//         return FALSE;
//     }
//     uVar4 = 0;
//     *param_2 = 1;
//     if (param_4 != 0) {
//         while (param_1[*param_2] != 0) {
//             BVar2 = shiftjis_IsCharDoubleByte((byte *)(param_1 + *param_2));
//             if (BVar2 == 0) {
//                 iVar1 = *param_2;
//                 cVar3 = param_1[iVar1];
//                 if (cVar3 == '\"') {
//                 *param_2 = *param_2 + 1;
//                 break;
//                 }
//                 if (cVar3 == '\\') {
//                     if (param_1[iVar1 + 1] != '\"') {
//                         *(undefined *)(uVar4 + param_3) = 0x5c;
//                         goto LAB_00410e5a;
//                     }
//                     *param_2 = iVar1 + 1;
//                     cVar3 = param_1[iVar1 + 1];
//                 }
//                 *(char *)(uVar4 + param_3) = cVar3;
//             }
//             else {
//                 *(char *)(uVar4 + param_3) = param_1[*param_2];
//                 *param_2 = *param_2 + 1;
//                 uVar4 += 1;
//                 *(char *)(uVar4 + param_3) = param_1[*param_2];
//             }
//         LAB_00410e5a:
//             *param_2 = *param_2 + 1;
//             uVar4 += 1;
//             if (param_4 <= uVar4) {
//                 *(undefined *)(uVar4 + param_3) = 0;
//                 return TRUE;
//             }
//         }
//     }
//     *(undefined *)(uVar4 + param_3) = 0;
//     return TRUE;
// }



//     c = str[*outLength];
//     while (c >= '0' && c <= '9')
//     {
//         atof_buffer[*outLength] = c; // is a dec digit
//         outLength++;
//         c = str[*outLength];
//     }
//     c = str[*outLength];
//     if (c == '.')
//     {
//         atof_buffer[*outLength] = c; // is a decimal point
//         outLength++;
//         c = str[*outLength];
//         while (c >= '0' && c <= '9')
//         {
//             atof_buffer[*outLength] = c; // is a dec digit
//             *outLength++;
//             c = str[*outLength];
//         }
//     }
//     else if (c != 'f' && c != 'F')
//     {
//         return FALSE; // no decimal point or 'f' postfix
//     }
    
//     atof_buffer[*outLength] = '\0';
//     *outValue = (float)atof(atof_buffer);
//     if (str[*outLength] == 'f' || str[*outLength] == 'F')
//         *outLength++;
    
//     if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
//     {
//     char cVar1;
//     int iVar2;
//     double dVar3;
//     char local_404 [1024];
    
//     if ((*str < '0') || ('9' < *str)) goto LAB_004110d7;
//     *outLength = 0;
//     *outValue = 0.00000000;
//     cVar1 = str[*outLength];
//     while ('/' < cVar1) {
//         iVar2 = *outLength;
//         if ('9' < str[iVar2]) break;
//         local_404[iVar2] = str[iVar2];
//         *outLength = iVar2 + 1;
//         cVar1 = str[iVar2 + 1];
//     }
//     iVar2 = *outLength;
//     cVar1 = str[iVar2];
//     if (cVar1 == '.') {
// LAB_0041107a:
//         if ((cVar1 != 'f') && (cVar1 != 'F')) {
//             local_404[iVar2] = cVar1;
//             iVar2 += 1;
//             *outLength = iVar2;
//             cVar1 = str[iVar2];
//             while (('/' < cVar1 && (str[iVar2] < ':'))) {
//                 local_404[iVar2] = str[iVar2];
//                 iVar2 += 1;
//                 *outLength = iVar2;
//                 cVar1 = str[iVar2];
//             }
//         }
//     }
//     else {
//         if (cVar1 != 'f') {
//             if (cVar1 != 'F') goto LAB_004110d7;
//             goto LAB_0041107a;
//         }
//     }
//     local_404[*outLength] = '\0';
//     dVar3 = atof(local_404);
//     *outValue = (float)dVar3;
//     if ((str[*outLength] == 'f') || (str[*outLength] == 'F')) {
//         *outLength = *outLength + 1;
//     }
// LAB_004110d7:
//     return;
// }

//undefined4 __thiscall
FUN_00410880(void *this,undefined *param_1,undefined4 *param_2,undefined4 *param_3)



// undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)

{
  int iVar1;
  
  while( true ) {
    if (false) {
      return 1;
    }
    iVar1 = FUN_004112e0(param_1,param_2); // deep rabbit hole, token parsing (i.e. dec 10,hex 0x1f, floating 0.0f, "string", 'char', keywords, operators?, etc)
    if (iVar1 == 0) break;
    if (*(int *)((int)param_2 + 4) != 6) {
      return 1;
    }
  }
  return 0;
}


// void __thiscall FUN_00411cf0(void *this,int param_1)
void __thiscall ac_ScriptDecoder_NextLine(AC_SCRIPTDECODER *acscr, char *outLine)

{
    int linepos = 0;
    while (acscr->Position < acscr->Length) {
        char c = acscr->Buffer[acscr->Position];
        if (c == '\n') {
            acscr->Position++;
            break;
        }
        if (outLine != NULL) {
            outLine[linepos] = c;
        }
        acscr->Position++;
        linepos++;
    }
    if (outLine != NULL) {
        outLine[linepos] = '\0';
    }
    return;
    //return linepos;
}

void __fastcall ac_ScriptDecoder_ctor(AC_SCRIPTDECODER *acscr)

{
    acscr->Buffer = NULL;
    acscr->Position = 0;
    acscr->ScrUnk3 = 0;
}

// There's A LOT of nasty C++ STL (std::vector, std::list) in these functions.
// Recreating using the decompiled code as 'guide',
//  rather than as the desired result may work better

void __cdecl ac_parseLines(char *filename)

{


    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        local_604 += 1;
        //JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        
        local_80c = local_604;
        local_8 = (uint)local_8._1_3_ << 8;
        FUN_00411c50(acscr);
        local_8 = 0xffffffff;
        FUN_004112a0(local_600);
        return;
    }

    int iVar1;
    uint uVar2;
    int *piVar3;
    int iVar4;
    int **in_FS_OFFSET;
    undefined local_854 [8];
    int local_84c [2];
    undefined local_844 [8];
    int local_83c [2];
    undefined local_834 [8];
    undefined local_82c [8];
    int local_824 [2];
    undefined local_81c [8];
    int local_814 [2];
    int local_80c;
    undefined8 local_808 [64];
    int local_604;
    int local_600 [74];
    undefined4 local_4d8 [8];
    undefined4 local_4b8;
    int local_4b4;
    int local_4b0;
    int local_4ac;
    int local_4a8;
    int local_4a4;
    undefined4 local_4a0;
    undefined4 local_49c;
    int local_498;
    undefined4 local_494;
    int local_490;
    undefined4 local_48c;
    int local_488;
    undefined8 local_458;
    int local_450;
    char local_438 [1028];
    uint local_34;
    int local_30;
    int local_2c;
    undefined4 local_28;
    AC_SCRIPTDECODER acscr; //int local_24 [4];
    int local_14;
    int *local_10;
    undefined *puStack12;
    undefined4 local_8;
  
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_0044fd03;
    // local_10 = *in_FS_OFFSET;
    // local_34 = DAT_004817cc ^ (uint)&stack0xfffffffc;
    // *(int ***)in_FS_OFFSET = &local_10;
    FUN_00410780(local_600);
    local_8 = 0;
    local_28 = 0;
    FUN_00403cf0(&local_4b0);
    FUN_00403cf0(&local_4a8);
    FUN_00411c40(&acscr);
    local_8._0_1_ = 1;
    local_604 = 0;
    FUN_00410870(local_600, 0);
    FUN_00403be0((int)&DAT_004a29bc);
    FUN_00403be0((int)&DAT_004a29a0);
    // open file
    iVar1 = FUN_00411dd0(acscr, filename);
    if (iVar1 == 0) {
        local_604 += 1;
        //JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        
        local_80c = local_604;
        local_8 = (uint)local_8._1_3_ << 8;
        FUN_00411c50(&acscr);
        local_8 = 0xffffffff;
        FUN_004112a0(local_600);
        return;
    }

    // Find all labels/lines
    local_30 = 0;
    local_14 = 1;
    while (uVar2 = ac_ScriptDecoder_IsEOF(&acscr), uVar2 == 0)
    {
        ac_ScriptDecoder_NextLine(&acscr, (int)local_808);// FUN_00411cf0(acscr, (int)local_808);
        FUN_004107e0(local_600, local_808);
        iVar1 = FUN_004036b0(local_600, &local_458);
        if ((iVar1 != 0) && (local_458._4_4_ == 1))
        {
            iVar1 = FUN_004036b0(local_600, &local_458);
            if (iVar1 == 0)
            {
                //JP: printf("error (%d) : 不正なラベル名です。\n", local_14);
                printf("error (%d) : Illegal label name.\n", local_14);
                local_604 += 1;
                break;
            }         
            piVar3 = FUN_00403860(local_814, local_438);
            local_4a8 = *piVar3;
            local_4a4 = piVar3[1];
            piVar3 = (int *)FUN_00403b50(&DAT_004a29a0, local_81c);
            uVar2 = operator!=(&local_4a8, piVar3);
            if ((uVar2 & 0xff) != 0) {
                //JP: printf("error (%d) : %s このラベル名は既に使用されています。\n", local_14, local_438);
                printf("error (%d) : %s Label name already in use.\n", local_14, local_438);
                local_604 += 1;
                break;
            }
            local_4b8 = 0xffffffff;
            local_4b4 = local_30;
            strcpy_s((char *)local_4d8, 0x20, local_438);
            FUN_00403cb0(&DAT_004a29a0, local_4d8);
            local_30 += 1;
        }
        local_14 += 1;
    }
    // parse commands
    FUN_00411cd0(acscr, 0);
    local_30 = 0;
    local_14 = 1;
    while (uVar2 = ac_ScriptDecoder_IsEOF(&acscr), uVar2 == 0) {
        FUN_00411cf0(acscr, (int)local_808);
       
        memset(&local_4a0, 0, sizeof(ANM_TIMELINE)); // FUN_00412740(&local_4a0, 0x44);
        FUN_004107e0(local_600, local_808);
        iVar1 = FUN_004036b0(local_600, &local_458);
        if (iVar1 != 0) {
            if (local_458._4_4_ == 1) {
                iVar1 = FUN_004036b0(local_600,&local_458);
                if (iVar1 != 0) {
                    piVar3 = FUN_00403860(local_824,local_438);
                    local_4a8 = *piVar3;
                    local_4a4 = piVar3[1];
                    piVar3 = (int *)FUN_00403b50(&DAT_004a29a0,local_82c);
                    uVar2 = operator!=(&local_4a8,piVar3);
                    if ((uVar2 & 0xff) != 0) {
                    iVar1 = FUN_00403d10(&local_4a8);
                    *(int *)(iVar1 + 0x20) = local_30;
                    }
                }
            }
            else {
            if (local_458._4_4_ == 0xd) {
                local_4a0 = 0;
                local_49c = 0;
                local_498 = local_450;
                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                if (iVar1 == 0) {
                printf(s_error_(%d)_:_004513e4,local_14);
                local_604 += 1;
                }
                else {
                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                if (iVar1 == 0) {
                    local_48c = local_494;
                    local_488 = local_490;
                }
                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                local_30 += 1;
                }
            }
            else {
                if (local_458._4_4_ == 0x27) {
                local_4a0 = 0;
                local_49c = 1;
                iVar1 = FUN_004036b0(local_600,&local_458);
                if (iVar1 == 0) {
                    printf(s_error_(%d)_:_0045140c,local_14);
                    local_604 += 1;
                }
                else {
                    if ((local_450 < 0x40) && (-1 < local_450)) {
                    local_498 = local_450;
                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                    if (iVar1 == 0) {
                        printf(s_error_(%d)_:_0045146c,local_14);
                        local_604 += 1;
                    }
                    else {
                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                        if (iVar1 == 0) {
                        local_48c = local_494;
                        local_488 = local_490;
                        }
                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                        local_30 += 1;
                    }
                    }
                    else {
                    printf(s_error_(%d)_:_(0_63)_00451430,local_14);
                    local_604 += 1;
                    }
                }
                }
                else {
                iVar1 = stricmp(local_438,"jump");
                if (iVar1 == 0) {
                    local_4a0 = 3;
                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                    if (iVar1 == 0) {
                    printf(s_error_(%d)_:_0045149c,local_14);
                    local_604 += 1;
                    }
                    else {
                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                    local_30 += 1;
                    }
                }
                else {
                    iVar1 = stricmp(local_438,"loop");
                    if (iVar1 == 0) {
                    local_4a0 = 2;
                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                    if (iVar1 == 0) {
                        printf(s_error_(%d)_:_004514cc,local_14);
                        local_604 += 1;
                    }
                    else {
                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                        if (iVar1 == 0) {
                        printf(s_error_(%d)_:_004514f4,local_14);
                        local_604 += 1;
                        }
                        else {
                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                        local_30 += 1;
                        }
                    }
                    }
                    else {
                    iVar1 = stricmp(local_438,"set");
                    if (iVar1 == 0) {
                        local_4a0 = 1;
                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                        if (iVar1 == 0) {
                        printf(s_error_(%d)_:_00451520,local_14);
                        local_604 += 1;
                        }
                        else {
                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                        if (iVar1 == 0) {
                            printf(s_error_(%d)_:_00451548,local_14);
                            local_604 += 1;
                        }
                        else {
                            iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                            if (iVar1 == 0) {
                            local_48c = local_494;
                            local_488 = local_490;
                            }
                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                            local_30 += 1;
                        }
                        }
                    }
                    else {
                        iVar1 = stricmp(local_438,"if");
                        if (iVar1 == 0) {
                        local_4a0 = 4;
                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                        if (iVar1 == 0) {
                            printf(s_error_(%d)_:_00451574,local_14);
                            local_604 += 1;
                        }
                        else {
                            iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                            if (iVar1 == 0) {
                            printf(s_error_(%d)_:_0045159c,local_14);
                            local_604 += 1;
                            }
                            else {
                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                            local_30 += 1;
                            }
                        }
                        }
                        else {
                        iVar1 = stricmp(local_438,"ife");
                        if (iVar1 == 0) {
                            local_4a0 = 5;
                            iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                            if (iVar1 == 0) {
                            printf(s_error_(%d)_:_004515c8,local_14);
                            local_604 += 1;
                            }
                            else {
                            iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                            if (iVar1 == 0) {
                                printf(s_error_(%d)_:_004515f0,local_14);
                                local_604 += 1;
                            }
                            else {
                                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_00451618,local_14);
                                local_604 += 1;
                                }
                                else {
                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                local_30 += 1;
                                }
                            }
                            }
                        }
                        else {
                            iVar1 = stricmp(local_438,"ifn");
                            if (iVar1 == 0) {
                            local_4a0 = 6;
                            iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                            if (iVar1 == 0) {
                                printf(s_error_(%d)_:_00451644,local_14);
                                local_604 += 1;
                            }
                            else {
                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_0045166c,local_14);
                                local_604 += 1;
                                }
                                else {
                                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451694,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                    local_30 += 1;
                                }
                                }
                            }
                            }
                            else {
                            iVar1 = stricmp(local_438,"ifg");
                            if (iVar1 == 0) {
                                local_4a0 = 7;
                                iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_004516c0,local_14);
                                local_604 += 1;
                                }
                                else {
                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_004516e8,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451710,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                    local_30 += 1;
                                    }
                                }
                                }
                            }
                            else {
                                iVar1 = stricmp(local_438,"ifs");
                                if (iVar1 == 0) {
                                local_4a0 = 8;
                                iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14
                                                    );
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_0045173c,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451764,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045178c,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                    }
                                    }
                                }
                                }
                                else {
                                iVar1 = stricmp(local_438,"ifge");
                                if (iVar1 == 0) {
                                    local_4a0 = 9;
                                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                        local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_004517bc,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_004517e4,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045180c,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                        }
                                    }
                                    }
                                }
                                else {
                                    iVar1 = stricmp(local_438,"ifse");
                                    if (iVar1 == 0) {
                                    local_4a0 = 10;
                                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                        local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045183c,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_00451864,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                        if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_0045188c,local_14);
                                            local_604 += 1;
                                        }
                                        else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                        }
                                        }
                                    }
                                    }
                                    else {
                                    iVar1 = stricmp(local_438,"max");
                                    if (iVar1 == 0) {
                                        local_4a0 = 0xb;
                                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                            local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_004518b8,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                        }
                                    }
                                    else {
                                        iVar1 = stricmp(local_438,"blend");
                                        if (iVar1 == 0) {
                                        local_4a0 = 0xc;
                                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                            local_14);
                                        if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_004518e8,local_14);
                                            local_604 += 1;
                                        }
                                        else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                        }
                                        }
                                        else {
                                        iVar1 = stricmp(local_438,"disp");
                                        if (iVar1 == 0) {
                                            local_4a0 = 0xd;
                                            iVar1 = FUN_004036f0(local_600,
                                                                (undefined4 *)(&local_4a0 + 1),local_14
                                                                );
                                            if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_00451918,local_14);
                                            local_604 += 1;
                                            }
                                            else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                            }
                                        }
                                        else {
                                            iVar1 = stricmp(local_438,"pos");
                                            if (iVar1 == 0) {
                                            local_4a0 = 0xe;
                                            iVar1 = FUN_004036f0(local_600,
                                                                (undefined4 *)(&local_4a0 + 1),
                                                                local_14);
                                            if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_00451944,local_14);
                                                local_604 += 1;
                                            }
                                            else {
                                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_0045196c,local_14);
                                                local_604 += 1;
                                                }
                                                else {
                                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                                local_30 += 1;
                                                }
                                            }
                                            }
                                            else {
                                            iVar1 = stricmp(local_438,"wait");
                                            if (iVar1 == 0) {
                                                local_4a0 = 0xf;
                                                iVar1 = FUN_004036f0(local_600,
                                                                    (undefined4 *)(&local_4a0 + 1),
                                                                    local_14);
                                                if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_0045199c,local_14);
                                                local_604 += 1;
                                                }
                                                else {
                                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                if (iVar1 == 0) {
                                                    local_494 = local_49c;
                                                    local_490 = local_498;
                                                }
                                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                                local_30 += 1;
                                                }
                                            }
                                            else {
                                                iVar1 = stricmp(local_438,"add");
                                                if (iVar1 == 0) {
                                                local_4a0 = 0x10;
                                                iVar1 = FUN_004036f0(local_600,
                                                                    (undefined4 *)(&local_4a0 + 1),
                                                                    local_14);
                                                if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_004519c8,local_14);
                                                    local_604 += 1;
                                                }
                                                else {
                                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                    if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_004519f0,local_14);
                                                    local_604 += 1;
                                                    }
                                                    else {
                                                    FUN_00403ba0(&DAT_004a29bc,
                                                                (undefined4 *)&local_4a0);
                                                    local_30 += 1;
                                                    }
                                                }
                                                }
                                                else {
                                                iVar1 = stricmp(local_438,"sub");
                                                if (iVar1 == 0) {
                                                    local_4a0 = 0x11;
                                                    iVar1 = FUN_004036f0(local_600,
                                                                        (undefined4 *)(&local_4a0 + 1),
                                                                        local_14);
                                                    if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_00451a1c,local_14);
                                                    local_604 += 1;
                                                    }
                                                    else {
                                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14
                                                                        );
                                                    if (iVar1 == 0) {
                                                        printf(s_error_(%d)_:_00451a44,local_14);
                                                        local_604 += 1;
                                                    }
                                                    else {
                                                        FUN_00403ba0(&DAT_004a29bc,
                                                                    (undefined4 *)&local_4a0);
                                                        local_30 += 1;
                                                    }
                                                    }
                                                }
                                                }
                                            }
                                            }
                                        }
                                        }
                                    }
                                    }
                                }
                                }
                            }
                            }
                        }
                        }
                    }
                    }
                }
                }
            }
            }
        }
        local_14 += 1;
        }
        piVar3 = (int *)FUN_00403c80(&DAT_004a29bc,local_834);
        local_4b0 = *piVar3;
        local_4ac = piVar3[1];
        while( true ) {
        piVar3 = (int *)FUN_00403b50(&DAT_004a29bc,local_844);
        uVar2 = operator!=(&local_4b0,piVar3);
        if ((uVar2 & 0xff) == 0) break;
        local_2c = 0;
        while (local_2c < 8) {
            iVar1 = FUN_00403d10(&local_4b0);
            if (*(int *)(iVar1 + 4 + local_2c * 8) == 2) {
            iVar1 = FUN_00403d10(&local_4b0);
            piVar3 = FUN_00403900(local_84c,*(int *)(iVar1 + 8 + local_2c * 8));
            local_4a8 = *piVar3;
            local_4a4 = piVar3[1];
            piVar3 = (int *)FUN_00403b50(&DAT_004a29a0,local_854);
            uVar2 = operator!=(&local_4a8,piVar3);
            if ((uVar2 & 0xff) == 0) {
                printf(s_error_:_00451a6c);
                break;
            }
            iVar1 = FUN_00403d10(&local_4b0);
            *(undefined4 *)(iVar1 + 4 + local_2c * 8) = 0;
            iVar1 = FUN_00403d10(&local_4a8);
            iVar4 = FUN_00403d10(&local_4b0);
            *(undefined4 *)(iVar4 + 8 + local_2c * 8) = *(undefined4 *)(iVar1 + 0x20);
            }
            local_2c += 1;
        }
        FUN_00403d30(&local_4b0,local_83c);
    }
    local_8 = (uint)local_8._1_3_ << 8;
    FUN_00411c50(acscr);
    local_8 = 0xffffffff;
    FUN_004112a0(local_600);
}

void __cdecl ac_WriteAnmFile(char *filename)

{
    FILE *file = fopen(filename, "wb+");
    if (file == NULL)
    {
        //JP: printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        printf("Output file %s cannot be opened.\n", filename);
        return;
    }
    std::vector<ANM_TIMELINE> timelines;
    ANM_FILEHEADER anmhdr = {0};
    anmhdr.Magic = MAGIC_ANM; // "ANM\0"
    anmhdr.AnmUnk1 = 0;
    anmhdr.TimelineCount = (UINT) timelines.size();
    fwrite((void *)&anmhdr, sizeof(anmhdr), 1, file);

    for (int i = 0; i < anmhdr.TimelineCount; i++)
    {
        const ANM_TIMELINE &anmline = timelines[i];

        fwrite((void *)&anmline, sizeof(ANM_TIMELINE), 1, file);
    }

    fclose(file);

    //unsigned int anmhdr[8];
    anmhdr[0] = MAGIC_ANM; // "ANM\0"
    anmhdr[1] = 0;
    anmhdr[2] = TimelineCount; //FUN_00412740((undefined8 *)&local_150,0x20);
    fwrite((void *)&anmhdr[0], sizeof(anmhdr), 1, file);
    // fwrite("ANM\0", 4, 1, file);
    // fwrite("ANM\0", 4, 1, file);



    fclose(file);
    uint uVar1;
    BOOL BVar2;
    int *piVar3;
    LPCVOID buffer;
    int iVar4;
    undefined4 *puVar5;
    undefined4 *puVar6;
    int **in_FS_OFFSET;
    DWORD bytesCount;
    undefined local_1b4 [8];
    int local_1ac [2];
    undefined local_1a4 [8];
    undefined4 local_19c;
    undefined4 local_198 [18];
    undefined4 local_150;
    undefined4 local_14c;
    undefined4 local_148;
    undefined4 kcfile [69];
    uint local_1c;
    int local_18;
    int local_14;
    int *local_10;
    undefined *puStack12;
    undefined4 local_8;
    
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_0044fd36;
    // local_10 = *in_FS_OFFSET;
    // uVar1 = DAT_004817cc ^ (uint)&stack0xfffffffc;
    // *(int ***)in_FS_OFFSET = &local_10;
    local_1c = uVar1;
    kcFile_init(kcfile);
    local_8 = 0;
    FUN_00403cf0(&local_18);
    BVar2 = kcFile_Open(kcfile,filename,0x402);
    if (BVar2 == 0) {
        //JP: printf("出力ファイル %s がオープンできません。\n", filename, uVar1);
        printf("Output file %s cannot be opened.\n", filename, uVar1);
        local_19c = 0;
        local_8 = 0xffffffff;
        kcFile_dtor(kcfile);
    }
    else {
        unsigned int hdr[8];
        memset(&hdr[0], 0, sizeof(hdr));
        hdr[0] = MAGIC_ANM; // "ANM\0"
        hdr[1] = 0;
        hdr[2] = TimelineCount; //FUN_00412740((undefined8 *)&local_150,0x20);
        DWORD bytesWritten;
        WriteFile(hFile, &hdr[0], sizeof(hdr), &bytesWritten, NULL);
        //memset()
        FUN_00412740((undefined8 *)&local_150,0x20);
        local_150 = MAGIC_ANM; // "ANM\0"
        local_14c = 0;
        local_148 = FUN_00403b80((int)&DAT_004a29bc);
        kcFile_Write(kcfile,&local_150,0x20);
        piVar3 = (int *)FUN_00403c80(&DAT_004a29bc,local_1a4);
        local_18 = *piVar3;
        local_14 = piVar3[1];
        while( true ) {
            piVar3 = (int *)FUN_00403b50(&DAT_004a29bc,local_1b4);
            uVar1 = operator!=(&local_18,piVar3);
            if ((uVar1 & 0xff) == 0) break;
            puVar5 = (undefined4 *)FUN_00403fe0(&local_18);
            iVar4 = 0x11;
            puVar6 = local_198;
            while (iVar4 != 0) {
                iVar4 += -1;
                *puVar6 = *puVar5;
                puVar5 = puVar5 + 1;
                puVar6 = puVar6 + 1;
            }
            bytesCount = 0x44;
            buffer = (LPCVOID)FUN_00403fe0(&local_18);
            kcFile_Write(kcfile,buffer,bytesCount);
            FUN_00403d30(&local_18,local_1ac);
        }
        printf("anm out -> %s", filename);
        kcFile_Close((int)kcfile);
        local_8 = 0xffffffff;
        kcFile_dtor(kcfile);
    }
    // *in_FS_OFFSET = local_10;
    // FUN_00413008();
    return;
}

int main(int argc, char** argv)

{
    if (argc < 0)
    {
        //JP: printf("アニメーションスクリプトコンバータ\n");
        printf("Animation script converter\n");
        printf("usage : ac [file] [file] ...\n");
        return 1;
    }

    ac_CoInitializeSetup(0x7f, 0); //FUN_00405b10(0x7f,0); CoInitialize and setup
    for (int i = 1; i < argc; i++)
    {
        char argFullpath[MAX_PATH + 4]; //char local_35c [264];
        char argParentdir[MAX_PATH + 4]; //char local_254 [264];
        shiftjis_GetAbsolutePath(argv[i], argFullpath, NULL); // FUN_00411ff0(argv[i], local_35c, (LPSTR *)0x0);
        shiftjis_GetParentDirectory(argFullpath, argParentdir); // FUN_00412520(local_35c, local_254);

        _WIN32_FIND_DATAA findData;
        HANDLE hFindFile = FindFirstFileA(argv[i], (LPWIN32_FIND_DATAA)&findData);
        if (hFindFile == INVALID_HANDLE_VALUE)
        {
            //JP: printf("ファイルが見つかりません : %s\n", argv[i]);
            printf("File not found : %s\n", argv[i]);
            continue;
        }
        
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                int iVar1 = ac_parseLines(argFullpath);
                if (iVar1 == 0)
                {
                    sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                    shiftjis_ChangeExtension(argFullpath, "anm"); //FUN_004120e0(argFullpath,(undefined4 *)"anm");
                    ac_WriteAnmFile(argFullpath); //FUN_00403990(argFullpath);
                }
            }
        } while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&findData));

        FindClose(hFindFile);
    }
    ac_CoUninitializeCleanup();
    return 0;
}
