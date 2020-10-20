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
        /*$0,4*/    unsigned int BufferCapacity; // 0x1000 initial value // SmlUnk_0x0
        /*$4,4*/    unsigned int PairCapacity; // 0x100 initial value // SmlUnk_0x4
        /*$8,4*/    HGLOBAL MemoryBuffer; // SmlUnk_0x8
        /*$c,4*/    HGLOBAL MemoryOffsets; // SmlUnk_0xc
        /*$10,4*/   char *PairBuffer; // SmlUnk_0x10
        /*$14,4*/   unsigned int *Offsets; // SmlUnk_0x14
        /*$18,4*/   int PairCount; // SmlUnk_0x18
        /*$1c,4*/   int BufferSize; // SmlUnk_0x1c
        /*$20*/
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    #endif
    namespace kclib
    {
    // #else

    // memory management of twin (double buffered?) Global allocations
    class kcStringDictionary
    {
        /*$0,4*/    unsigned int BufferCapacity; // 0x1000 initial value // SmlUnk_0x0
        /*$4,4*/    unsigned int PairCapacity; // 0x100 initial value // SmlUnk_0x4
        /*$8,4*/    HGLOBAL MemoryBuffer; // SmlUnk_0x8
        /*$c,4*/    HGLOBAL MemoryOffsets; // SmlUnk_0xc
        /*$10,4*/   char *PairBuffer; // SmlUnk_0x10
        /*$14,4*/   unsigned int *Offsets; // SmlUnk_0x14
        /*$18,4*/   int PairCount; // SmlUnk_0x18
        /*$1c,4*/   int BufferSize; // SmlUnk_0x1c
        /*$20*/
    public:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
        kcStringDictionary();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
        ~kcStringDictionary();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
        // scalar_dtor(int flags); // (auto-generated in C++)

        //void __thiscall kcSmallStruct_GetValue(int this, char *param_2, char *param_3)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004122a0
        void GetValue(OUT char *outBuffer, IN const char *strBufferKey);
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
        void SetValue(IN const char *strKey, IN const char *strValue);

        //int __thiscall kcSmallStruct_IndexOf(int this,char *param_2)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412240
        int IndexOf(IN const char *strKey);
        //int __thiscall kcSmallStruct_QuickSortIndex(int this, char *param_2)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c70
        int QuickSortIndex(IN const char *strKey);

    private:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412030
        void ResizePairAt(int index, int size);

        // internal helper to measure size of two lines within a null-terminated line buffer
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c30
        static int GetStringPairLength(IN const char *strKey);


        // re-allocate buffers with added sizes. Sizes of zero will not have their buffer changed.
        //  if this is called after buffers have already been allocated, then the sizes will be ADDED
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
        void AllocExtendBuffers(unsigned int addBufferCapacity, unsigned int addOffsetCapacity);

        // free allocated buffers
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
        void FreeBuffers();
        // lock buffers to access the line contents
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
        bool LockBuffers();
        // unlock buffers when not using them, or re-allocating
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
        void UnlockBuffers();
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    } /* end namespace kclib */
    // #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
