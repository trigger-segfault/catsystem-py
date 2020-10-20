#pragma once

#ifndef KCLIB_MC_KCDOUBLEBUFFER_FUNCTIONAL_H
#define KCLIB_MC_KCDOUBLEBUFFER_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcDoubleBuffer.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
UNK_SMALL_STRUCT * __fastcall kcSmallStruct_ctor(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
void __fastcall kcSmallStruct_dtor(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
void * __thiscall kcSmallStruct_scalar_dtor(UNK_SMALL_STRUCT *this, int flags);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void __thiscall kcSmallStruct_AllocExtendBuffers(UNK_SMALL_STRUCT *this, unsigned int addBufferSize, unsigned int addOffsetCount);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void __fastcall kcSmallStruct_FreeBuffers(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
BOOL __fastcall kcSmallStruct_LockBuffers(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void __fastcall kcSmallStruct_UnlockBuffers(UNK_SMALL_STRUCT *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c30
int kcSmallStruct_GetStringPairLength(IN const char *strBufferPair);


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void __thiscall kcSmallStruct_SetValue(UNK_SMALL_STRUCT *this, IN const char *strKey, IN const char *strValue);


//int __thiscall kcSmallStruct_IndexOf(int this,char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412240
int __thiscall kcSmallStruct_IndexOf(UNK_SMALL_STRUCT *this, IN const char *strKey);

//int __thiscall kcSmallStruct_QuickSortIndex(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c70
int __thiscall kcSmallStruct_QuickSortIndex(UNK_SMALL_STRUCT *this, IN const char *strKey);

//void __thiscall kcSmallStruct_GetValue(int this, char *param_2, char *param_3)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004122a0
void __thiscall kcSmallStruct_GetValue(UNK_SMALL_STRUCT *this, OUT char *outBuffer, IN const char *strKey);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412030
void __thiscall kcSmallStruct_ResizePairAt(UNK_SMALL_STRUCT *this, int index, int sizeDiff);


#endif

#endif /* end include guard */
