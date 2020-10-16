#pragma once

#ifndef KCLIB_TOKEN_PARSE_TYPES_H
#define KCLIB_TOKEN_PARSE_TYPES_H

// functional-specific function declarations for SCRIPT_DECODER and SCRIPT_READER
//  (OOP equivalent: TokenParser, ScriptReader)

#include "common.h"
#ifndef KCLIB_OOP

#include "TokenParser.h"
#include "ScriptReader.h"


// Catch all for token parsing functions and script decoder
// Not well-named, not well organized
// 
// Big/work-in-progress functions are mostly in token_parse.cpp
// All the smaller working stuff is in token_parse_types.cpp


//undefined4 * __fastcall FUN_00410780(undefined4 *param_1)
///FID: FUN_00410780
SCRIPT_DECODER * __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this);

// void __fastcall FUN_004112a0(int *param_1)
///FID: FUN_004112a0
void __fastcall ScriptDecoder_dtor(SCRIPT_DECODER *this);


//undefined4 __thiscall FUN_004107e0(void *this,undefined8 *param_1)
///FID: FUN_004107e0
BOOL __thiscall ScriptDecoder_SetBuffer(SCRIPT_DECODER *this, IN const char *inBuffer);

//void __thiscall ScriptDecoder_Set_ScrUnk72(void *this, undefined4 newScrUnk72)
///FID: FUN_00410870
unsigned int __thiscall ScriptDecoder_SetScrUnk72(SCRIPT_DECODER *this, unsigned int newScrUnk72);


///READER:
void __fastcall ScriptReader_ctor(SCRIPT_READER *this);
///READER:
BOOL __fastcall ScriptReader_IsEOF(SCRIPT_READER *this);

// Used to strip CRLF from script?
//void __fastcall FUN_00411d50(char **param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d50
void __fastcall ScriptReader_ConvertToLF(SCRIPT_READER *this);

//undefined4 __thiscall FUN_00411dd0(void *this,char *filename)
///READER:
///FID:cs2_full_v401/tool/ac.exe: FUN_00411dd0
BOOL __thiscall ScriptReader_Open(SCRIPT_READER *this, IN const char *filename);

//void __fastcall FUN_00411c50(int *param_1)
///READER:
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c50
void __fastcall ScriptReader_Close(SCRIPT_READER *this);


//uint __fastcall FUN_00411c60(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c60
unsigned int __fastcall ScriptReader_FUN_00411c60(SCRIPT_READER *this);

//void __thiscall FUN_00411cd0(void *this,uint param_1)
///READER:
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cd0
void __thiscall ScriptReader_SetPosition(SCRIPT_READER *this, unsigned int newPosition);

// may return unsigned int
///READER:
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cf0
void __thiscall ScriptReader_NextLine(SCRIPT_READER *this, OPTIONAL OUT char *outLine);

//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
BOOL __cdecl token_NextSkipWS(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);

//void __thiscall FUN_004112e0(void *this,undefined8 *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_004112e0
BOOL __thiscall token_Next(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);
// BOOL __thiscall foo_token_Next(SCRIPT_DECODER *this, IN const char *str, OUT TOKEN_RESULT *token, OUT void *param_1);

const char * __stdcall token_SkipComments(IN const char *str, OUT int *outLines);

// Identifier lookup function probably
// strange returned structure is likely identifier
BOOL __thiscall token_LookupIdentifier(void *this, IN const char *str, OUT unsigned char *outBuffer);

BOOL __stdcall token_ParseUnsignedInteger(IN const char *str, OUT int *outLength, OUT int *outValue);

BOOL __stdcall token_ParseFloat(IN const char *str, OUT int *outLength, OUT float *outValue);

BOOL __stdcall token_ParseChar(IN const char *str, OUT int *outLength, OUT unsigned short *outValue);

BOOL __stdcall token_ParseString(IN const char *str, OUT int *outLength, OUT char *outBuffer, unsigned int bufferSize);

BOOL __thiscall token_ParseSymbol(SCRIPT_DECODER *this, IN const char *str, OUT int *outLength, OUT TOKEN_TYPE *outValue);

BOOL __stdcall token_ParseKeyword(IN const char *str, OUT TOKEN_TYPE *outValue);

BOOL __stdcall token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer);

#endif

#endif /* end include guard */
