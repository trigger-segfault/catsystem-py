#pragma once

#ifndef KCLIB_MC_KCCATSCENE_CLASS_H
#define KCLIB_MC_KCCATSCENE_CLASS_H

#include "common_mc.h"


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    // The HI byte of the CST CatScene line-type
    //  (not sure what happens with the lower byte, but it is always 1 for CST)
    //------------------------------------------------------------------
    // <https://github.com/trigger-segfault/TriggersTools.CatSystem2/wiki/CST-Scene#linetypes>
    //------------------------------------------------------------------
    // Input       0x0201   Wait for input after message
    // Page        0x0301   Novel page break and wait for input after message
    // Message     0x2001   Display a message
    // Name        0x2101   Set speaker of the message
    // Command     0x3001   Perform any other command
    // InputFlag   0x0200   Any type with this flag is treated as input
    //------------------------------------------------------------------
    enum KCLINE_TYPE
    {
        LINE_INPUT = 0x02,
        LINE_PAGE = 0x03,
        LINE_MESSAGE = 0x20,
        LINE_NAME = 0x21,
        LINE_COMMAND = 0x30,
        LINE_DEBUG_FILENAME = 0xf0, // LINE_TYPE_0xf0 // used with /L option
        LINE_TYPE_0xf1 = 0xf1, // used with /L option
    };

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // either the actual kcCatScene data, or more specifically,
    //   a simple collection of read lines from a file
    struct kcCatScene
    {
        /*$0,4*/    unsigned int MacUnk0; // 0
        /*$4,4*/    unsigned int LineCount; // 0
        /*$8,4*/    char *BufferLines; // 0
        /*$c,4*/    unsigned int *BufferOffsets; // 0
        /*$10,4*/   unsigned int MacUnk4; // 0
        /*$14,400*/ char Filename[0x400]; // param_1
        /*$414,4*/  HGLOBAL MemoryLines; // 0 (HGLOBAL GlobalAlloc)
        /*$418,4*/  HGLOBAL MemoryOffsets; // 0 (HGLOBAL GlobalAlloc)
        /*$41c,4*/  unsigned int FileSize; // 0 (or lines buffer size, which is +0x10 more)
        /*$420*/
    };

    #else

    class kcCatScene
    {
        /*$0,4*/    unsigned int MacUnk0; // 0
        /*$4,4*/    unsigned int LineCount; // 0
        /*$8,4*/    char *BufferLines; // 0
        /*$c,4*/    unsigned int *BufferOffsets; // 0
        /*$10,4*/   unsigned int MacUnk4; // 0
        /*$14,400*/ char Filename[0x400]; // param_1
        /*$414,4*/  HGLOBAL MemoryLines; // 0 (HGLOBAL GlobalAlloc)
        /*$418,4*/  HGLOBAL MemoryOffsets; // 0 (HGLOBAL GlobalAlloc)
        /*$41c,4*/  unsigned int FileSize; // 0 (or lines buffer size, which is +0x10 more)
        /*$420*/
    public:
        //unsigned int * __thiscall kcMacroReader_initRun(void *this, char *param_1)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
        kcCatScene(IN const char *filename);

        // undefined4 __thiscall Read(int this,char *filename)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
        bool Read(IN const char *filename);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
        bool LockBuffers();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
        void UnlockBuffers();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
        bool IsLocked();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
        const char * GetLineAt(int index);
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
        bool HasLineAt(int index);


        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
        unsigned short * kcCatScene_FUN_00411a30(unsigned short *shortTable, int param_3, IN OUT int *param_4, unsigned int param_5);
    };

    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
