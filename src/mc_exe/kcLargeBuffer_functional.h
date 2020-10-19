#pragma once

#ifndef KCLIB_MC_KCLARGEBUFFER_FUNCTIONAL_H
#define KCLIB_MC_KCLARGEBUFFER_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcLargeBuffer.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
UNK_LARGE_STRUCT * __fastcall kcLargeStruct_ctor(UNK_LARGE_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
void __fastcall kcLargeStruct_dtor(UNK_LARGE_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401100
void * __thiscall kcLargeStruct_scalar_dtor(UNK_LARGE_STRUCT *this, int flags);

// BOOL __thiscall kcLargeStruct_FUN_00413240(void *this, byte *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413240
BOOL __thiscall kcLargeStruct_FUN_00413240(UNK_LARGE_STRUCT *this, IN const char *str);

// int __thiscall kcLargeStruct_Add(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004130b0
int __thiscall kcLargeStruct_Add(UNK_LARGE_STRUCT *this, IN const char *str);

// int __thiscall kcLargeStruct_IndexOf(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413050
int __thiscall kcLargeStruct_IndexOf(UNK_LARGE_STRUCT *this, IN const char *str);

// undefined4 __thiscall kcLargeStruct_FUN_00413430(void *this,char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413430
BOOL __thiscall kcLargeStruct_FUN_00413430(UNK_LARGE_STRUCT *this, IN const char *str);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004133a0
void __thiscall kcLargeStruct_FUN_004133a0(UNK_LARGE_STRUCT *this, IN const char *str);

// BOOL __fastcall kcLargeStruct_FUN_00412d40(int *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412d40
BOOL __fastcall kcLargeStruct_FUN_00412d40(UNK_LARGE_STRUCT *this);

///FIXME:  This function's first parameter is the codepage, this is where we'll add 932, to fix certain locale issues
///WINAPI: BOOL IsDBCSLeadByteEx (UINT, BYTE)
///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-isdbcsleadbyteex>

//size_t __thiscall kcLargeStruct_FUN_00412db0(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412db0
int __thiscall kcLargeStruct_FUN_00412db0(UNK_LARGE_STRUCT *this, char *str);


#endif

#endif /* end include guard */
