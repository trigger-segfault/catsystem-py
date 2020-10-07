#pragma once

#ifndef GHIDRA_BUILT_IN_TYPES_H
#define GHIDRA_BUILT_IN_TYPES_H


// Include me to stop the linter/IDE from exploding
//  when reversing decompiled Ghidra functions.
// 
// Also useful for types we really aren't sure about.
// It could be a pointer, a signed int, an unsigned int, a char[4], WHO KNOWS! :D


//// NOTES ON TYPES ////

///NOTES: undefined4
// * Very often a function returning undefined4 may be returning BOOL (4-byte 1 or 0).
//   Check returns to confirm

///NOTES: undefined8
// * Ghidra likes to use undefined8* for data that gets memcpy'ed a lot,
//   rarely are we actually dealing with undefined8*, just be careful to pay
//   attention to pointer offsets, like (param_1 + 1) 0x8 vs. ((int)param_1 + 4) 0x4

///NOTES: ulong
// * Current environment is MinGW64 on Windows, we don't want unexpected 64-byte INTs,
//   we already have to be aware of possible differences in pointer size


// Ghidra undefined types:
typedef unsigned char           undefined;
typedef unsigned char           undefined1;
typedef unsigned short          undefined2;
typedef unsigned int            undefined4;
typedef unsigned long long      undefined8;

// Ghidra short-hand types:
typedef unsigned char           byte;
typedef unsigned short          ushort;
typedef unsigned int            uint;
//typedef unsigned long           ulong; // Current environment is MinGW64 on Windows
                                         // we don't want unexpected 64-byte INT types
typedef long long               longlong;
typedef unsigned long long      ulonglong;


#endif /* end include guard */
