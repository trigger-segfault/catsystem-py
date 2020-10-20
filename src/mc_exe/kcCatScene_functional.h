#pragma once

#ifndef KCLIB_MC_KCCATSCENE_FUNCTIONAL_H
#define KCLIB_MC_KCCATSCENE_FUNCTIONAL_H

#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcCatScene.h"


//unsigned int * __thiscall kcMacroReader_initRun(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
kcCatScene * __thiscall kcCatScene_ctor(kcCatScene *this, IN const char *filename);

// undefined4 __thiscall kcCatScene_Read(int this,char *filename)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
BOOL __thiscall kcCatScene_Read(kcCatScene *this, IN const char *filename);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
BOOL __fastcall kcCatScene_LockBuffers(kcCatScene *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
void __fastcall kcCatScene_UnlockBuffers(kcCatScene *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
BOOL __fastcall kcCatScene_IsLocked(kcCatScene *this);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
const char * __thiscall kcCatScene_GetLineAt(kcCatScene *this, int index);

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
BOOL __thiscall kcCatScene_HasLineAt(kcCatScene *this, int index);

///FLAG: ALLOW_LINE_CONTINUE  0x1
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned int __thiscall kcCatScene_CopyLineAt(kcCatScene *this, OUT char *outBuffer, int bufferSize, IN OUT int *inoutIndex, unsigned int flags);

#endif

#endif /* end include guard */
