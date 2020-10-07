#pragma once

#ifndef KCLIB_SHIFTJIS_H
#define KCLIB_SHIFTJIS_H

///TODO: better alternative for BOOL (4-byte boolean)
#include <windows.h>


// This is a really lowsey catch-all header for random text functions
// Includes Shift JIS character identification (as per cp932)
// And IO/path functions, only SOME of which detect Shift JIS.
//   fix eventually...


///FID:cs2_full_v401/tool/ac.exe: FUN_00412200
BOOL __cdecl shiftjis_IsCharDoubleByte(IN const char *chr);


//BOOL __cdecl FUN_00412520(char *param_1, char *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00412520
BOOL __cdecl shiftjis_GetParentDirectory(IN const char *fullpath, OUT char *outParentdir);

///FID:cs2_full_v401/tool/ac.exe: FUN_00411ff0
BOOL __cdecl shiftjis_GetAbsolutePath(IN const char *filename, OUT char *outFullpath, OUT char **outBasename);

//undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004120e0
BOOL __cdecl shiftjis_ChangeExtension(IN OUT char *filename, IN const char *extension);


#endif /* end include guard */
