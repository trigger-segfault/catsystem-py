#pragma once

#ifndef KCLIB_MC_KCFILEINFO_FUNCTIONAL_H
#define KCLIB_MC_KCFILEINFO_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcFileInfo.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
FILE_READER * __thiscall kcFileInfo_ctor(FILE_READER *this, IN const char *filename);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
void __fastcall kcFileInfo_dtor(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
void * __thiscall kcFileInfo_scalar_dtor(FILE_READER *this, int flags);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
BOOL __fastcall kcFileInfo_Open(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
BOOL __fastcall kcFileInfo_Close(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
BOOL __fastcall kcFileInfo_Delete(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
unsigned int __thiscall kcFileInfo_Write(FILE_READER *this, IN const unsigned char *inBuffer, unsigned int numBytes);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int __fastcall kcFileInfo_FindNextValid(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int __fastcall kcFileInfo_FindNext(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void __fastcall kcFileInfo_FindClose(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
void __fastcall kcFileInfo_SetReadMode(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void __fastcall kcFileInfo_SetWriteMode(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20 (thunk: FUN_00413770)
void __fastcall kcFileInfo_SetWriteMode_thunk(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
BOOL __fastcall kcFileInfo_UpdateModifiedTime(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
unsigned int __fastcall kcFileInfo_GetTimestamp(FILE_READER *this);

// DWORD __fastcall kcFileInfo_GetSize(HANDLE *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int __fastcall kcFileInfo_GetSize(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
char * __fastcall kcFileInfo_GetBasename(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
char * __fastcall kcFileInfo_GetExtension(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
void __thiscall kcFileInfo_ChangeExtension(FILE_READER *this, IN const char *newExtension);

#endif

#endif /* end include guard */
