#pragma once

#ifndef KCLIB_TOKEN_PARSE_TYPES_H
#define KCLIB_TOKEN_PARSE_TYPES_H

#include "token_type.h"
//#include "ghidra_types_min.h"
///TODO: better alternative for BOOL (4-byte boolean)
#include <windows.h>


// Catch all for token parsing functions and script decoder
// Not well-named, not well organized
// 
// Big/work-in-progress functions are mostly in token_parse.cpp
// All the smaller working stuff is in token_parse_types.cpp



void __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this);

BOOL __fastcall ScriptDecoder_IsEOF(SCRIPT_DECODER *this);


// may return unsigned int
void __thiscall ScriptDecoder_NextLine(SCRIPT_DECODER *this, OUT char *outLine);

//undefined4 __cdecl FUN_004036b0(void *param_1,undefined8 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004036b0
BOOL __cdecl token_Next(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);

//void __thiscall FUN_004112e0(void *this,undefined8 *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_004112e0
BOOL __thiscall token_Parse(SCRIPT_DECODER *this, OUT TOKEN_RESULT *token);
// BOOL __thiscall foo_token_Parse(SCRIPT_DECODER *this, IN const char *str, OUT TOKEN_RESULT *token, OUT void *param_1);

const char * __stdcall token_SkipComments(IN const char *str, OUT int *outLines);

BOOL __thiscall token_UnkFunction(void *this, IN const char *str, OUT unsigned char *outBuffer);

BOOL __stdcall token_ParseInteger(IN const char *str, OUT int *outLength, OUT int *outValue);

BOOL __stdcall token_ParseFloat(IN const char *str, OUT int *outLength, OUT float *outValue);

BOOL __stdcall token_ParseChar(IN const char *str, OUT int *outLength, OUT unsigned short *outValue);

BOOL __stdcall token_ParseString(IN const char *str, OUT int *outLength, OUT char *outBuffer, unsigned int bufferSize);

BOOL __thiscall token_ParseSymbol(SCRIPT_DECODER *this, IN const char *str, OUT int *outLength, OUT TOKEN_RESULT *outValue);

BOOL __stdcall token_ParseKeyword(IN const char *str, OUT TOKEN_RESULT *outValue);

BOOL __stdcall token_ParseIdentifier(IN const char *str, OUT int *outLength, OUT char *outBuffer);

#endif /* end include guard */
