

#pragma once

#ifndef KCLIB_MC_KCMESSAGECOMPILER_FUNCTIONAL_H
#define KCLIB_MC_KCMESSAGECOMPILER_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcMessageCompiler.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413f10
kcMessageCompiler * __fastcall kcMessageCompiler_ctor(kcMessageCompiler *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413dc0
void __fastcall kcMessageCompiler_set_0x1c0cc4_to_0x1c0cc8(kcMessageCompiler *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ee0
void __thiscall kcMessageCompiler_set_0x1c0cd8(kcMessageCompiler *this, UNK_LARGE_STRUCT *newLargeStruct);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ec0
void __thiscall kcMessageCompiler_set_0x1c042c(kcMessageCompiler *this, UNK_SMALL_STRUCT *newSmallStruct);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413f00
void kcMessageCompiler_dtor(kcMessageCompiler *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010a0
void * __thiscall kcMessageCompiler_scalar_dtor(kcMessageCompiler *this, int flags);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004156e0
BOOL __thiscall kcMessageCompiler_Compile(kcMessageCompiler *this, IN const char *filename);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414eb0
unsigned int __fastcall kcMessageCompiler_FUN_00414eb0(kcMessageCompiler *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413de0
unsigned int __fastcall kcMessageCompiler_FUN_00413de0(kcMessageCompiler *this);


/* WARNING: Could not reconcile some variable overlaps */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413c30
void __fastcall kcMessageCompiler_FUN_00413c30(kcMessageCompiler *this);


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ff0
void __thiscall kcMessageCompiler_AddLine(kcMessageCompiler *this, KCLINE_TYPE lineType, IN const char *str);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414b20
void __thiscall kcMessageCompiler_ParseName(kcMessageCompiler *this, IN const char *line);

#endif

#endif /* end include guard */
