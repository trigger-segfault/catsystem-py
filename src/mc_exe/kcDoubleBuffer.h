#pragma once

#ifndef KCLIB_MC_KCDOUBLEBUFFER_CLASS_H
#define KCLIB_MC_KCDOUBLEBUFFER_CLASS_H

#include "common_mc.h"


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // memory management of twin (double buffered?) Global allocations
    struct UNK_SMALL_STRUCT
    {
        /*$0,4*/    unsigned int SizeA; // 0x1000
        /*$4,4*/    unsigned int SizeB; //  0x100
        /*$8,4*/    HGLOBAL MemoryA;
        /*$c,4*/    HGLOBAL MemoryB;
        /*$10,4*/   void *BufferA;
        /*$14,4*/   void *BufferB;
        /*$18,4*/   unsigned int SmlUnk6;
        /*$1c,4*/   unsigned int SmlUnk7;
        /*$20*/
    };

    #else


    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
