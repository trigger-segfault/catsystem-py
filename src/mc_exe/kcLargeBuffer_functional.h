#pragma once

#ifndef KCLIB_MC_KCLARGEBUFFER_FUNCTIONAL_H
#define KCLIB_MC_KCLARGEBUFFER_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcLargeBuffer.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
UNK_LARGE_STRUCT * __fastcall kcLargeStruct_ctor(UNK_LARGE_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
void __fastcall kcLargeStruct_dtor(UNK_LARGE_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401100
void * __thiscall kcLargeStruct_scalar_dtor(UNK_LARGE_STRUCT *this, int flags);

#endif

#endif /* end include guard */
