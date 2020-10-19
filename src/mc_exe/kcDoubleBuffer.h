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
        /*$0,4*/    unsigned int StringBufferSize; // 0x1000 initial value // SmlUnk_0x0
        /*$4,4*/    unsigned int OffsetsBufferCount; // 0x100 initial value // SmlUnk_0x4
        /*$8,4*/    HGLOBAL MemoryStringBuffer; // SmlUnk_0x8
        /*$c,4*/    HGLOBAL MemoryOffsetsBuffer; // SmlUnk_0xc
        /*$10,4*/   char *SmlStringBuffer; // SmlUnk_0x10
        /*$14,4*/   unsigned int *SmlOffsetsTable; // SmlUnk_0x14
        /*$18,4*/   int OffsetIndex; // SmlUnk_0x18
        /*$1c,4*/   int BufferOffset; // SmlUnk_0x1c
        /*$20*/
    };

    #else

    // memory management of twin (double buffered?) Global allocations
    class kcDoubleBuffer
    {
        /*$0,4*/    unsigned int StringBufferSize; // 0x1000 initial value
        /*$4,4*/    unsigned int OffsetsBufferCount; //  0x100 initial value
        /*$8,4*/    HGLOBAL MemoryStringBuffer;
        /*$c,4*/    HGLOBAL MemoryOffsetsBuffer;
        /*$10,4*/   char *SmlStringBuffer;
        /*$14,4*/   unsigned int *SmlOffsetsTable;
        /*$18,4*/   int OffsetIndex; // SmlUnk_0x18
        /*$1c,4*/   int BufferOffset; // SmlUnk_0x1c
        /*$20*/
    public:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
        kcDoubleBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
        ~kcDoubleBuffer();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
        // scalar_dtor(int flags); // (auto-generated in C++)

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
        void AllocBuffers(unsigned int newSizeA, unsigned int newSizeB);

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
