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
        /*$0,4*/    unsigned int SizeA; // 0x1000 initial value
        /*$4,4*/    unsigned int SizeB; //  0x100 initial value
        /*$8,4*/    HGLOBAL MemoryA;
        /*$c,4*/    HGLOBAL MemoryB;
        /*$10,4*/   void *BufferA;
        /*$14,4*/   void *BufferB;
        /*$18,4*/   unsigned int SmlUnk6;
        /*$1c,4*/   unsigned int SmlUnk7;
        /*$20*/
    };

    #else

    // memory management of twin (double buffered?) Global allocations
    class kcDoubleBuffer
    {
        /*$0,4*/    unsigned int SizeA; // 0x1000 initial value
        /*$4,4*/    unsigned int SizeB; //  0x100 initial value
        /*$8,4*/    HGLOBAL MemoryA;
        /*$c,4*/    HGLOBAL MemoryB;
        /*$10,4*/   void *BufferA;
        /*$14,4*/   void *BufferB;
        /*$18,4*/   unsigned int SmlUnk6;
        /*$1c,4*/   unsigned int SmlUnk7;
        /*$20*/
    public:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
        kcDoubleBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
        ~kcDoubleBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
        // scalar_dtor(int flags); // (auto-generated in C++)

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
        void AllocBuffers(unsigned int sizeA, unsigned int sizeB);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
        void FreeBuffers();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
        bool LockBuffers();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
        void UnlockBuffers();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
        void kcDoubleBuffer_FUN_00412310(char *strA, char *strB);
    };

    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
