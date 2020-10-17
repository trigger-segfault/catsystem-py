#pragma once

#ifndef KCLIB_MC_KCFILEINFO_FUNCTIONAL_H
#define KCLIB_MC_KCFILEINFO_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcFileInfo.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
FILE_READER * __thiscall kcUnkFile_ctor(FILE_READER *this, IN const char *filename);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
void __fastcall kcUnkFile_dtor(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
void * __thiscall kcUnkFile_scalar_dtor(FILE_READER *this, int flags);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
BOOL __fastcall kcUnkFile_Open(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
BOOL __fastcall kcUnkFile_Close(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
BOOL __fastcall kcUnkFile_Delete(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
unsigned int __thiscall kcUnkFile_Write(FILE_READER *this, IN const unsigned char *inBuffer, unsigned int numBytes);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int __fastcall kcUnkFile_FindLoop(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int __fastcall kcUnkFile_FindNext(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void __fastcall kcUnkFile_FindClose(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
void __fastcall kcUnkFile_SetReadMode(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void __fastcall kcUnkFile_SetWriteMode(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20
void __fastcall kcUnkFile_SetWriteMode_thunk(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
BOOL __fastcall kcUnkFile_UpdateModifiedTime(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
unsigned int __fastcall kcUnkFile_GetMSDOSTimestamp(FILE_READER *this);

// DWORD __fastcall kcUnkFile_GetFileSize(HANDLE *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int __fastcall kcUnkFile_GetFileSize(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
char * __fastcall kcUnkFile_FUN_00413960(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
char * __fastcall kcUnkFile_GetExtension(FILE_READER *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
void __thiscall kcUnkFile_ChangeExtension(FILE_READER *this, IN const char *ext);

#endif

#endif /* end include guard */
