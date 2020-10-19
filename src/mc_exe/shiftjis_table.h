#pragma once

#ifndef KCLIB_MC_SHIFTJIS_TABLE_H
#define KCLIB_MC_SHIFTJIS_TABLE_H

#include "common_mc.h"


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    // flags in the TABLE_SHIFTJIS
    // enum SHIFTJIS_FLAG
    // {
    //     // SJS_SINGLEBYTE = 0x00000000,
    //     SJS_LEADBYTE   = 0x40000000,
    //     SJS_SECONDBYTE = 0x80000000,
    //     // SJS_MULTIBYTE  = 0xc0000000,
    // };

    // character is a valid lead byte (meaning the second byte is part of the codepoint)
    #define SJS_LEADBYTE 0x40000000
    // character is a valid second byte that may appear after a lead byte
    #define SJS_SECONDBYTE 0x80000000

    // attributes for a single SHIFT_JIS byte
    ///FLAG: SJS_LEADBYTE    0x40000000
    ///FLAG: SJS_SECONDBYTE  0x80000000
    ///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041f0a0
    extern const unsigned int TABLE_SHIFTJIS[256];

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
