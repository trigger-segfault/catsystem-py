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
        // LINE_NONE = 0, // LINE_TYPE_0x00
        NO_LINE_TYPE = 0, // LINE_TYPE_0x00
        LINE_INPUT = 0x02, // LINE_TYPE_0x02
        LINE_PAGE = 0x03, // LINE_TYPE_0x03
        LINE_MESSAGE = 0x20, // LINE_TYPE_0x20
        LINE_NAME = 0x21, // LINE_TYPE_0x21
        LINE_COMMAND = 0x30, // LINE_TYPE_0x30
        LINE_DEBUG_FILENAME = 0xf0, // LINE_TYPE_0xf0 // used with /L option
        LINE_TYPE_0xf1 = 0xf1, // LINE_TYPE_0xf1 // used with /L option
    };

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // either the actual kcCatScene data, or more specifically,
    //   a simple collection of read lines from a file
    struct kcCatScene
    {
        /*$0,4*/    unsigned int MacUnk0; // 0, unknown, never observed outside constructor
        /*$4,4*/    unsigned int LineCount; // Number of lines in LineBuffer (LineCount * 4 == size of LineOffsets buffer)
        /*$8,4*/    char *LineBuffer; // Buffer of lines, separated by null-terminators
        /*$c,4*/    unsigned int *LineOffsets; // Offset table to each line in LineBuffer
        /*$10,4*/   BOOL LastLineMultibyteContinue; // set to TRUE after CopyLineAt (when a line continuation encountered, and allowed by flags), set to FALSE if no continuation in CopyLineAt
        /*$14,400*/ char Filename[0x400]; // Filename lines were read from
        /*$414,4*/  HGLOBAL MemoryLines; // HGLOBAL GlobalAlloc //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        /*$418,4*/  HGLOBAL MemoryOffsets; // HGLOBAL GlobalAlloc //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        /*$41c,4*/  unsigned int BufferSize; // Size of LineBuffer
        /*$420*/
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    #endif
    namespace kclib
    {
    // #else


    class kcFileLineBuffer
    {
        /*$0,4*/    unsigned int MacUnk0; // 0, unknown, never observed outside constructor
        /*$4,4*/    unsigned int LineCount; // Number of lines in LineBuffer (LineCount * 4 == size of LineOffsets buffer)
        /*$8,4*/    char *LineBuffer; // Buffer of lines, separated by null-terminators
        /*$c,4*/    unsigned int *LineOffsets; // Offset table to each line in LineBuffer
        /*$10,4*/   BOOL LastLineMultibyteContinue; // set to TRUE after CopyLineAt (when a line continuation encountered, and allowed by flags), set to FALSE if no continuation in CopyLineAt
        /*$14,400*/ char Filename[0x400]; // Filename lines were read from
        /*$414,4*/  HGLOBAL MemoryLines; // HGLOBAL GlobalAlloc //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        /*$418,4*/  HGLOBAL MemoryOffsets; // HGLOBAL GlobalAlloc //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        /*$41c,4*/  unsigned int BufferSize; // Size of LineBuffer
        /*$420*/
    public:
        //unsigned int * __thiscall kcMacroReader_initRun(void *this, char *param_1)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
        kcFileLineBuffer(IN const char *filename);
        ///CUSTOM: destructor not observed, but added anyway
        ~kcFileLineBuffer();

        // undefined4 __thiscall Read(int this,char *filename)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
        bool Read(IN const char *filename);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
        bool LockBuffers();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
        void UnlockBuffers();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
        bool IsLocked();

        // Returns null if !this->IsLocked() or index is out of range
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
        const char * GetLineAt(int index);
        // Returns false if !this->IsLocked() or index is out of range
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
        bool HasLineAt(int index);

        // Copy the next line (or continued lines) from the buffer at index
        //  writes to outBuffer, and updates inoutIndex (and +1 for each '\\' line continuation)
        ///FLAG: ALLOW_LINE_CONTINUE  0x1
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
        unsigned int CopyLineAt(OUT char *outBuffer, int bufferSize, IN OUT int *inoutIndex, unsigned int flags);

        ///CUSTOM: function for potentially inlined access to Filename member
        unsigned int GetLineCount() const;
        ///CUSTOM: function for potentially inlined access to Filename member
        const char * GetFilename() const;
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    } /* end namespace kclib */
    // #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
