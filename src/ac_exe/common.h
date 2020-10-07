#pragma once

#ifndef KCLIB_COMMON_HEADER_H
#define KCLIB_COMMON_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Includes basic types and constants used like:
// * BOOL (32-bit boolean),
// * TRUE FALSE numeric constants (for returning BOOL)
//   So we don't have to drag windows.h around everywhere


//// WINAPI DEFINES ////

// MAX_PATH windows constant
#define MAX_PATH 260

// #ifndef NULL
// #ifdef __cplusplus
// #ifndef _WIN64
// #define NULL 0
// #else
// #define NULL 0LL
// #endif
// #else
// #define NULL ((void *)0)
// #endif
// #endif

#ifndef FALSE
// false keyword for BOOL (32-bit boolean)
#define FALSE 0
#endif
#ifndef TRUE
// true keyword for BOOL (32-bit boolean)
#define TRUE 1
#endif

#ifndef IN
// in specifier, for clarity only
#define IN
#endif
#ifndef OUT
// out specifier, for clarity only
#define OUT
#endif
#ifndef OPTIONAL
// optional specifier, for clarity only
#define OPTIONAL
#endif

typedef int BOOL; // 32-bit boolean (0 or 1, often checked for as such)


//// HELEPR DEFINES ////

// typedef int bool32_t; // 32-bit (4-byte) bool (0 or 1, often checked for as such)
// typedef int bool4_t; // 4-byte (32-bit) bool (0 or 1, often checked for as such)

// Confirm we have the right enum/name and value,
// there was a lot of room for copy&paste errors
#define assert_enum(value, TOKEN) TOKEN; static_assert(value == TOKEN)

// Lazy solution for C++ this keyword without implementing classes
#define this thisscr


#endif /* end include guard */
