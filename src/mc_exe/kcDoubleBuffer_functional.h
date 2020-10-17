#pragma once

#ifndef KCLIB_MC_KCDOUBLEBUFFER_FUNCTIONAL_H
#define KCLIB_MC_KCDOUBLEBUFFER_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcDoubleBuffer.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
UNK_SMALL_STRUCT * __fastcall kcSmallStruct_ctor(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
void __fastcall kcSmallStruct_dtor(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
void * __thiscall kcSmallStruct_scalar_dtor(UNK_SMALL_STRUCT *this, int flags);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void __thiscall kcSmallStruct_AllocBuffers(UNK_SMALL_STRUCT *this, unsigned int sizeA, unsigned int sizeB);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void __fastcall kcSmallStruct_FreeBuffers(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
BOOL __fastcall kcSmallStruct_LockBuffers(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void __fastcall kcSmallStruct_UnlockBuffers(UNK_SMALL_STRUCT *this);


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void __thiscall kcSmallStruct_FUN_00412310(UNK_SMALL_STRUCT *this, char *strA, char *strB);


#endif

#endif /* end include guard */
