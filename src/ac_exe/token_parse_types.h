#pragma once

#ifndef KCLIB_TOKEN_PARSE_TYPES_H
#define KCLIB_TOKEN_PARSE_TYPES_H

#include "common.h"
#include "token_type.h"


// Catch all for token parsing functions and script decoder
// Not well-named, not well organized
// 
// Big/work-in-progress functions are mostly in token_parse.cpp
// All the smaller working stuff is in token_parse_types.cpp



void __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this);

BOOL __fastcall ScriptDecoder_IsEOF(SCRIPT_DECODER *this);

// Used to strip CRLF from script?
//void __fastcall FUN_00411d50(char **param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d50
void __fastcall ScriptDecoder_ConvertToLF(SCRIPT_DECODER *this);

//undefined4 __thiscall FUN_00411dd0(void *this,char *filename)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411dd0
BOOL __thiscall ScriptDecoder_Open(SCRIPT_DECODER *this, IN const char *filename);

//void __fastcall FUN_00411c50(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c50
void __fastcall ScriptDecoder_Close(SCRIPT_DECODER *this);


//uint __fastcall FUN_00411c60(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c60
unsigned int __fastcall ScriptDecoder_FUN_00411c60(SCRIPT_DECODER *this);

//void __thiscall FUN_00411cd0(void *this,uint param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cd0
void __thiscall ScriptDecoder_SetPosition(SCRIPT_DECODER *this, unsigned int newPosition);

// may return unsigned int
void __thiscall ScriptDecoder_NextLine(SCRIPT_DECODER *this, OPTIONAL OUT char *outLine);

//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
BOOL __cdecl token_Next(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);

//void __thiscall FUN_004112e0(void *this,undefined8 *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_004112e0
BOOL __thiscall token_Parse(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);
// BOOL __thiscall foo_token_Parse(SCRIPT_DECODER *this, IN const char *str, OUT TOKEN_RESULT *token, OUT void *param_1);

const char * __stdcall token_SkipComments(IN const char *str, OUT int *outLines);

// Identifier lookup function probably
// strange returned structure is likely identifier
BOOL __thiscall token_LookupIdentifier(void *this, IN const char *str, OUT unsigned char *outBuffer);

BOOL __stdcall token_ParseInteger(IN const char *str, OUT int *outLength, OUT int *outValue);

BOOL __stdcall token_ParseFloat(IN const char *str, OUT int *outLength, OUT float *outValue);

BOOL __stdcall token_ParseChar(IN const char *str, OUT int *outLength, OUT unsigned short *outValue);

BOOL __stdcall token_ParseString(IN const char *str, OUT int *outLength, OUT char *outBuffer, unsigned int bufferSize);

BOOL __thiscall token_ParseSymbol(SCRIPT_DECODER *this, IN const char *str, OUT int *outLength, OUT TOKEN_TYPE *outValue);

BOOL __stdcall token_ParseKeyword(IN const char *str, OUT TOKEN_TYPE *outValue);

BOOL __stdcall token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer);

#endif /* end include guard */
