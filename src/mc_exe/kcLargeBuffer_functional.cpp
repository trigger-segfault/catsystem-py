
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcLargeBuffer_functional.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
UNK_LARGE_STRUCT * __fastcall kcLargeStruct_ctor(UNK_LARGE_STRUCT *this)
{
    this->LrgUnk_0x10 = 0;
    this->LrgUnk_0x0 = 0;
    this->LrgUnk_0xc0014 = nullptr;
    this->Memory = nullptr;
    this->LrgUnk_0x8 = 0;
    this->LrgUnk_0xc = 0;
    return this;
    // param_1[4] = 0;
    // *param_1 = 0;
    // param_1[0x30005] = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // return param_1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
void __fastcall kcLargeStruct_dtor(UNK_LARGE_STRUCT *this)
{
    if (this->Memory != nullptr)
    {
        ::GlobalFree(this->Memory);
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401100
void * __thiscall kcLargeStruct_scalar_dtor(UNK_LARGE_STRUCT *this, int flags)
{
    kcLargeStruct_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

#endif
