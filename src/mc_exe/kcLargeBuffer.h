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
    struct STRING_POOL_ENTRY
    {
        /*$0,4*/   unsigned int StringOffset; // position of string in UNK_LARGE_STRUCT::Memory // SubUnk0
        /*$4,4*/   int NextStringOffset; // starts as -1 // SubUnk1
        /*$8,4*/   int StringRefCount; // set to 0 then incremented // SubUnk2
        /*$c*/
    };

    #define SubUnk0 StringOffset
    #define SubUnk1 NextStringOffset
    #define SubUnk2 StringRefCount
    
    // for UNK_LARGE_STRUCT field $0, this is only a guess.
    //  Math is never done on the state, and only 0,1,2 is every assigned or compared.
    enum POOL_THREE_STATE
    {
        POOL_STATE_0 = 0,
        POOL_STATE_1 = 1,
        POOL_STATE_2 = 2,
    };

    #ifndef KCLIB_OOP

    // not understood yet. big boy buffer
    //  very likely that 0xc0000 gap is one large buffer, as is tradition with this program)
    struct UNK_LARGE_STRUCT
    {
        /*$0,4*/       POOL_THREE_STATE PoolState; // some sort of state? Maybe THREE_STATE? // LrgUnk_0x0
        /*$4,4*/       HGLOBAL Memory; // (this is actually LPVOID or unsigned char*) allocated with GMEM_FIXED // LrgUnk_0x4
        /*$8,4*/       unsigned int MemorySize; // allocates and reallocates in size units of 0x1000000 bytes. WOW // LrgUnk_0x8
        /*$c,4*/       unsigned int MemoryPosition; // position of next string to add in memory buffer // LrgUnk_0xc
        /*$10,4*/      int StringPoolIndex; // index of CurrentStringEntry in StringPoolTable // LrgUnk_0x10
        /*$14,c0000*/  STRING_POOL_ENTRY StringPoolTable[0x10000]; // 0xc0000 total bytes // LrgUnk_0x14
        /*$c0014,4*/   STRING_POOL_ENTRY *CurrentStringEntry; // This points to somewhere in the StringPoolTable (LrgUnk_0x14) buffer // LrgUnk_0xc0014
        /*$c0018,4*/   int LrgUnk_0xc0018; // integer value // LrgUnk_0xc0018
        /*$c001c,4*/   int LrgUnk_0xc001c; // integer value // LrgUnk_0xc001c
        /*$c0020,40*/   char *StringPointers[0x10]; // buffer string pointers, aka sizeof(int), may be some struct // LrgUnk_0xc0020
        //... (distance is $190, excluding field $c0020 size)
        /*$c01b0,1000*/ char TempBuffer[0x1000]; // guessed size, Some byte* / char* buffer? // LrgUnk_0xc01b0
        //... //?
        /*$c11b0*/
    };

    #else

    class kcLargeBuffer
    {
        /*$0,4*/       POOL_THREE_STATE PoolState; // some sort of state? Maybe THREE_STATE? // LrgUnk_0x0
        /*$4,4*/       HGLOBAL Memory; // (this is actually LPVOID or unsigned char*) allocated with GMEM_FIXED // LrgUnk_0x4
        /*$8,4*/       unsigned int MemorySize; // allocates and reallocates in size units of 0x1000000 bytes. WOW // LrgUnk_0x8
        /*$c,4*/       unsigned int MemoryPosition; // position of next string to add in memory buffer // LrgUnk_0xc
        /*$10,4*/      int StringPoolIndex; // index of CurrentStringEntry in StringPoolTable // LrgUnk_0x10
        /*$14,c0000*/  STRING_POOL_ENTRY StringPoolTable[0x10000]; // 0xc0000 total bytes // LrgUnk_0x14
        /*$c0014,4*/   STRING_POOL_ENTRY *CurrentStringEntry; // This points to somewhere in the StringPoolTable (LrgUnk_0x14) buffer // LrgUnk_0xc0014
        /*$c0018,4*/   int LrgUnk_0xc0018; // integer value // LrgUnk_0xc0018
        /*$c001c,4*/   int LrgUnk_0xc001c; // integer value // LrgUnk_0xc001c
        /*$c0020,40*/   char *StringPointers[0x10]; // buffer string pointers, aka sizeof(int), may be some struct // LrgUnk_0xc0020
        //... (distance is $190, excluding field $c0020 size)
        /*$c01b0,1000*/ char TempBuffer[0x1000]; // guessed size, Some byte* / char* buffer? // LrgUnk_0xc01b0
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

    #define LrgUnk_0x0 PoolState
    #define LrgUnk_0x4 Memory
    #define LrgUnk_0x8 MemorySize
    #define LrgUnk_0xc MemoryPosition
    #define LrgUnk_0x10 StringPoolIndex
    #define LrgUnk_0x14 StringPoolTable
    #define LrgUnk_0xc0014 CurrentStringEntry
    #define LrgUnk_0xc0020 StringPointers
    #define LrgUnk_0xc01b0 TempBuffer

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
