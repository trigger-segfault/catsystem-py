#pragma once

#ifndef KCLIB_MC_KCMESSAGECOMPILER_CLASS_H
#define KCLIB_MC_KCMESSAGECOMPILER_CLASS_H

#include "common_mc.h"
#include "kcCatScene.h"
#include "kcDoubleBuffer.h"
#include "kcLargeBuffer.h"


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    // for kcMessageCompiler fields $1c0cc4 and $1c0cc8, this is only a guess.
    //  Math is never done on the state, and only 0,1,2 is every assigned.
    enum THREE_STATE
    {
        STATE_0 = 0,
        STATE_1 = 1,
        STATE_2 = 2,
    };

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // The draugr deathlord of all these structures, it holds nearly EVERYTHING
    // - we have pointers to other areas stored in the struct for faster lookup
    // - extremely arbitrarily large buffers (assumed based on gap in field offsets, and even numbers)
    // - storage for some of the compiler flags
    // - AND MORE! \o/
    //
    // Asside from fields [$4-$3fc], and $404 all fields have been encountered
    //  (or assumed array on table sizes)
    // 
    // The defines below make it easier to transition after naming a field.
    //  When translating field offsets of a decompiled function, prefix "this->MsgUnk_"
    //  to the hex offset to get the define for the field name. Also tells us if the field
    //  hasn't been identified / named yet.
    struct kcMessageCompiler
    {
        /*$0,4*/       char MsgUnk_0x0[1]; // unk size (0x400?)
        //...
        /*$400,4*/     unsigned int InputLength; // MsgUnk_0x400
        /*$404,4*/     unsigned int MsgUnk_0x404; // not yet seen
        /*$408,4*/     unsigned int LineIndex; // MsgUnk_0x408
        /*$40c,4*/     unsigned int InputIndex; // MsgUnk_0x40c
        /*$410,40000*/ unsigned int OffsetsTable[0x10000]; // unk size // MsgUnk_0x410
        /*$40410,80000*/ unsigned int InputsTable[0x20000]; // unk size // MsgUnk_0x40410 (in actualaity, a two-DWORD struct)
        // /*$40414,4*/   unsigned int MsgUnk_0x40414[1]; // unk size
        /*$c0410,100000*/ char LargeBuffer[0x100000]; // unk size // MsgUnk_0xc0410
        /*$1c0410,4*/  unsigned int Position; // MsgUnk_0x1c0410
        /*$1c0414,4*/  BOOL CmdFlag_L; // MsgUnk_0x1c0414 = FALSE
        /*$1c0418,4*/  unsigned int MsgUnk_0x1c0418; // 0
        /*$1c041c,4*/  BOOL Flag_0x1c041c; // Flag_0x1c041c = FALSE
        /*$1c0420,4*/  BOOL CmdFlag_X; // MsgUnk_0x1c0420 = TRUE
        /*$1c0424,4*/  unsigned int ModifiedTimestamp; // param_4 (msdostime) // MsgUnk_0x1c0424
        /*$1c0428,4*/  int UpdateCount; //MsgUnk_0x1c0428
        /*$1c042c,4*/  UNK_SMALL_STRUCT *SmallStruct; // struct size 0x20 - g_UNK_SMALL_STRUCT // MsgUnk_0x1c042c
        /*$1c0430,400*/ char Filename[0x400]; // strcpy(MsgUnk_0x1c0430, filename)
        /*$1c0830,40*/ kcCatScene *SceneTable[0x10]; // MsgUnk_0x1c0830
        /*$1c0870,40*/ unsigned int MsgUnk_0x1c0870[0x10]; // unknown pointed-to objects
        /*$1c08b0,4*/  int SceneCount; // MsgUnk_0x1c08b0
        /*$1c08b4,400*/ char StringBuffer[0x400]; // MsgUnk_0x1c08b4 (unk size)
        /*$1c0cb4,4*/  char *CurrentString; // MsgUnk_0x1c0cb4
        /*$1c0cb8,4*/  kcCatScene *CurrentScene; // MsgUnk_0x1c0cb8
        /*$1c0cbc,4*/  int MsgUnk_0x1c0cbc; // int?
        /*$1c0cc0,4*/  int MsgUnk_0x1c0cc0; // unsigned int?
        /*$1c0cc4,4*/  THREE_STATE ThreeState_A; // MsgUnk_0x1c0cc4
        /*$1c0cc8,4*/  THREE_STATE ThreeState_B; // MsgUnk_0x1c0cc8
        /*$1c0ccc,4*/  KCLINE_TYPE MsgUnk_0x1c0ccc;
        /*$1c0cd0,4*/  unsigned int MsgUnk_0x1c0cd0; // 0xffffffff
        /*$1c0cd4,4*/  unsigned int MsgUnk_0x1c0cd4;
        /*$1c0cd8,4*/  UNK_LARGE_STRUCT *LargeStruct; // struct size 0xc11b0 - g_UNK_LARGE_STRUCT // MsgUnk_0x1c0cd8
        /*$1c0cdc,4*/  unsigned int MsgUnk_0x1c0cdc;
        /*$1c0ce0*/
        
        #define MsgUnk_0x0 MsgUnk_0x0
        #define MsgUnk_0x400 InputLength
        // #define MsgUnk_0x404 MsgUnk_0x404
        #define MsgUnk_0x408 LineIndex
        #define MsgUnk_0x40c InputIndex
        #define MsgUnk_0x410 OffsetsTable
        #define MsgUnk_0x40410 InputsTable
        // #define MsgUnk_0x40414 MsgUnk_0x40414
        #define MsgUnk_0xc0410 LargeBuffer
        #define MsgUnk_0x1c0410 Position
        #define MsgUnk_0x1c0414 CmdFlag_L
        #define MsgUnk_0x1c0418 MsgUnk_0x1c0418
        #define MsgUnk_0x1c041c Flag_0x1c041c
        #define MsgUnk_0x1c0420 CmdFlag_X
        #define MsgUnk_0x1c0424 ModifiedTimestamp
        #define MsgUnk_0x1c0428 UpdateCount
        #define MsgUnk_0x1c042c SmallStruct
        #define MsgUnk_0x1c0430 Filename
        #define MsgUnk_0x1c0830 SceneTable
        #define MsgUnk_0x1c0870 MsgUnk_0x1c0870
        #define MsgUnk_0x1c08b0 SceneCount
        #define MsgUnk_0x1c08b4 StringBuffer
        #define MsgUnk_0x1c0cb4 CurrentString
        #define MsgUnk_0x1c0cb8 CurrentScene
        #define MsgUnk_0x1c0cbc MsgUnk_0x1c0cbc
        #define MsgUnk_0x1c0cc0 MsgUnk_0x1c0cc0
        #define MsgUnk_0x1c0cc4 ThreeState_A
        #define MsgUnk_0x1c0cc8 ThreeState_B
        #define MsgUnk_0x1c0ccc MsgUnk_0x1c0ccc
        #define MsgUnk_0x1c0cd0 MsgUnk_0x1c0cd0
        #define MsgUnk_0x1c0cd4 MsgUnk_0x1c0cd4
        #define MsgUnk_0x1c0cd8 LargeStruct
        #define MsgUnk_0x1c0cdc MsgUnk_0x1c0cdc
    };

    #else


    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
