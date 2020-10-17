
#include "common_mc.h"
#ifdef KCLIB_OOP

#include "kcLargeBuffer.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
kclib::kcLargeBuffer::kcLargeBuffer()
{
    this->LrgUnk_0x10 = 0;
    this->LrgUnk_0x0 = 0;
    this->LrgUnk_0xc0014 = nullptr;
    this->Memory = nullptr;
    this->LrgUnk_0x8 = 0;
    this->LrgUnk_0xc = 0;
    // param_1[4] = 0;
    // *param_1 = 0;
    // param_1[0x30005] = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // return param_1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
kclib::kcLargeBuffer::~kcLargeBuffer()
{
    if (this->Memory != nullptr)
    {
        ::GlobalFree(this->Memory);
    }
}

#endif
