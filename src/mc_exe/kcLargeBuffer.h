#pragma once

#ifndef KCLIB_MC_KCLARGEBUFFER_CLASS_H
#define KCLIB_MC_KCLARGEBUFFER_CLASS_H

#include "common_mc.h"


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // not understood yet. big boy buffer
    //  very likely that 0xc0000 gap is one large buffer, as is tradition with this program)
    struct UNK_LARGE_STRUCT
    {
        /*$0,4*/       unsigned int LrgUnk_0x0;
        /*$4,4*/       HGLOBAL Memory; //LrgUnk_0x4;
        /*$8,4*/       unsigned int LrgUnk_0x8;
        /*$c,4*/       unsigned int LrgUnk_0xc;
        //...
        /*$10,4*/      unsigned int LrgUnk_0x10;
        //...
        // /*$14,c0000*/  unsigned char LrgUnk_0x14[0xc0000];
        //...
        /*$c0014,4*/   int *LrgUnk_0xc0014;
        //...
        /*$c11b0*/
    };

    #else

    class kcLargeBuffer
    {
        /*$0,4*/       unsigned int LrgUnk_0x0;
        /*$4,4*/       HGLOBAL Memory; //LrgUnk_0x4;
        /*$8,4*/       unsigned int LrgUnk_0x8;
        /*$c,4*/       unsigned int LrgUnk_0xc;
        //...
        /*$10,4*/      unsigned int LrgUnk_0x10;
        //...
        // /*$14,c0000*/  unsigned char LrgUnk_0x14[0xc0000];
        //...
        /*$c0014,4*/   int *LrgUnk_0xc0014;
        //...
        /*$c11b0*/
    public:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
        kcLargeBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
        ~kcLargeBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401100
        // scalar_dtor(int flags); // (auto-generated in C++)

    };

    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
