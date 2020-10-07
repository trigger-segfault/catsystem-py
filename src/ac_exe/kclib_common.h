#pragma once

#ifndef KCLIB_KCLIB_COMMON_H
#define KCLIB_KCLIB_COMMON_H

#include "common.h"


// Generic or uncategorized kclib functions, or functions needed
// for decompiling.
// 
// Includes Shift JIS character identification (as per cp932)
// And IO/path functions, only SOME of which detect Shift JIS.
//   (even with shiftjis_ prefix) fix eventually...


// DAT_004c3430 is likely kcFileServer,
// or whatever id used to manage integration files
#define DAT_004c3430 ((void *)0)

// Used for kclib_HeapAlloc management, is this a WINAPI/MSVC thing?
// may be a kcMemFile thing
// may also be a C++/new operator/std::shared_ptr(with size) thing
#define DAT_004c3ef0 ((void *)0)



//undefined4 * __cdecl FUN_00422a60(undefined4 param_1,int param_2)
///WINAPI: HeapAlloc
///FID:cs2_full_v401/tool/ac.exe: FUN_00422a60
void * __cdecl kclib_HeapAlloc(unsigned int allocID, int allocSize);

//void __cdecl FUN_00422ab0(int param_1)
///WINAPI: HeapFree
///FID:cs2_full_v401/tool/ac.exe: FUN_00422ab0
void __cdecl kclib_HeapFree(void *kcHeap);


///FID:cs2_full_v401/tool/ac.exe: FUN_00407bc0
void kclib_LogSendMessageA(char *param_1, ...);

//undefined4 FUN_00407b50(void)
///FID:cs2_full_v401/tool/ac.exe: FUN_00407b50
BOOL kcFileBase_Close(void);


///FID:cs2_full_v401/tool/ac.exe: FUN_00412200
BOOL __cdecl shiftjis_IsCharDoubleByte(IN const char *chr);


//BOOL __cdecl FUN_00412520(char *param_1, char *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00412520
BOOL __cdecl shiftjis_GetParentDirectory(IN const char *fullpath, OUT char *outParentdir);

///WINAPI: GetFullPathNameA
///FID:cs2_full_v401/tool/ac.exe: FUN_00411ff0
BOOL __cdecl shiftjis_GetAbsolutePath(IN const char *filename, OUT char *outFullpath, OPTIONAL OUT char **outBasename);

//undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004120e0
BOOL __cdecl shiftjis_ChangeExtension(IN OUT char *filename, IN const char *extension);


#endif /* end include guard */
