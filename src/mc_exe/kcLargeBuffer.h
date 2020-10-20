#pragma once

#ifndef KCLIB_MC_KCLARGEBUFFER_CLASS_H
#define KCLIB_MC_KCLARGEBUFFER_CLASS_H

#include "common_mc.h"

// This class is a large dynamic buffer that acts as a string table (set) to avoid string duplication
// Supports up to 0x10000 unique strings
// 
// Proper term is a StringPool I think, or something like with StringViews(?)
// 
// NEW INFO:
//  this class has relations to CST macros and macro expansion for macro function arguments (i.e. %1-%9)
//  so this may be either a CST scene buffer, and not kcCatScene like I figured, or specifically a macro database


#ifdef KCLIB_OOP
namespace kclib
{
#endif

    #pragma pack(push, 1)

    // sub-structure stored in table (count 0x10000) at UNK_LARGE_STRUCT field offset $14
    struct CST_MACRO_ENTRY
    {
        /*$0,4*/   unsigned int MacroBufferOffset; // position of string in UNK_LARGE_STRUCT::Memory // SubUnk0
        /*$4,4*/   int MacroNextLineOffset; // starts as -1 // SubUnk1
        /*$8,4*/   int MacroLineCount; // set to 0 then incremented // SubUnk2
        /*$c*/
    };

    #define SubUnk0 MacroBufferOffset
    #define SubUnk1 MacroNextLineOffset
    #define SubUnk2 MacroLineCount
    
    // for UNK_LARGE_STRUCT field $0, this is only a guess.
    //  Math is never done on the state, and only 0,1,2 is every assigned or compared.
    enum MACRO_THREE_STATE
    {
        MACRO_STATE_0 = 0,
        MACRO_STATE_1 = 1,
        MACRO_STATE_2 = 2,
    };

    #ifndef KCLIB_OOP

    // not understood yet. big boy buffer
    //  very likely that 0xc0000 gap is one large buffer, as is tradition with this program)
    struct UNK_LARGE_STRUCT
    {
        /*$0,4*/       MACRO_THREE_STATE MacroState; // some sort of state? Maybe THREE_STATE? // LrgUnk_0x0
        /*$4,4*/       HGLOBAL Memory; // (this is actually LPVOID or unsigned char*) allocated with GMEM_FIXED // LrgUnk_0x4
        /*$8,4*/       unsigned int MemoryCapacity; // allocates and reallocates in size units of 0x1000000 bytes. WOW // LrgUnk_0x8 (capacity)
        /*$c,4*/       unsigned int MemorySize; // position of next string to add in memory buffer // LrgUnk_0xc (size, current)
        /*$10,4*/      int CurrentMacroIndex; // index of CurrentMacroEntry in MacroTable // LrgUnk_0x10
        /*$14,c0000*/  CST_MACRO_ENTRY MacroTable[0x10000]; // 0xc0000 total bytes // LrgUnk_0x14
        /*$c0014,4*/   CST_MACRO_ENTRY *CurrentMacroEntry; // This points to somewhere in the MacroTable (LrgUnk_0x14) buffer // LrgUnk_0xc0014
        /*$c0018,4*/   int LrgUnk_0xc0018; // integer value // LrgUnk_0xc0018
        /*$c001c,4*/   int LrgUnk_0xc001c; // integer value // LrgUnk_0xc001c
        /*$c0020,40*/   char *ArgumentPointers[0x10]; // buffer string pointers, aka sizeof(int), may be some struct // LrgUnk_0xc0020
        //... (distance is $190, excluding field $c0020 size)
        /*$c01b0,1000*/ char ArgumentBuffer[0x1000]; // guessed size, Some byte* / char* buffer? // LrgUnk_0xc01b0
        /*$c11b0*/
    };

    #else

    class kcLargeBuffer
    {
        /*$0,4*/       MACRO_THREE_STATE MacroState; // some sort of state? Maybe THREE_STATE? // LrgUnk_0x0
        /*$4,4*/       HGLOBAL Memory; // (this is actually LPVOID or unsigned char*) allocated with GMEM_FIXED // LrgUnk_0x4
        /*$8,4*/       unsigned int MemoryCapacity; // allocates and reallocates in size units of 0x1000000 bytes. WOW // LrgUnk_0x8
        /*$c,4*/       unsigned int MemorySize; // position of next string to add in memory buffer // LrgUnk_0xc
        /*$10,4*/      int CurrentMacroIndex; // index of CurrentMacroEntry in MacroTable // LrgUnk_0x10
        /*$14,c0000*/  CST_MACRO_ENTRY MacroTable[0x10000]; // 0xc0000 total bytes // LrgUnk_0x14
        /*$c0014,4*/   CST_MACRO_ENTRY *CurrentMacroEntry; // This points to somewhere in the MacroTable (LrgUnk_0x14) buffer // LrgUnk_0xc0014
        /*$c0018,4*/   int LrgUnk_0xc0018; // integer value // LrgUnk_0xc0018
        /*$c001c,4*/   int LrgUnk_0xc001c; // integer value // LrgUnk_0xc001c
        /*$c0020,40*/   char *ArgumentPointers[0x10]; // buffer string pointers, aka sizeof(int), may be some struct // LrgUnk_0xc0020
        //... (distance is $190, excluding field $c0020 size)
        /*$c01b0,1000*/ char ArgumentBuffer[0x1000]; // guessed size, Some byte* / char* buffer? // LrgUnk_0xc01b0
        //... //?
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

    #define LrgUnk_0x0 MacroState
    #define LrgUnk_0x4 Memory
    #define LrgUnk_0x8 MemoryCapacity
    #define LrgUnk_0xc MemorySize
    #define LrgUnk_0x10 CurrentMacroIndex
    #define LrgUnk_0x14 MacroTable
    #define LrgUnk_0xc0014 CurrentMacroEntry
    #define LrgUnk_0xc0020 ArgumentPointers
    #define LrgUnk_0xc01b0 ArgumentBuffer

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
