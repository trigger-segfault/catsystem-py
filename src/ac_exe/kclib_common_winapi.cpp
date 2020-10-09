
// OOP + function definitions for kclib "common" functions using WINAPI
// 
// a separate cpp file for all functions requiring WINAPI,
//  to reduce windows.h bloat I guess

#include "kclib_common.h"
#include <windows.h>

#ifndef KCLIB_OOP

////// WINAPI USED //////

// HeapAlloc, HeapFree
// 
// DWORD GetFullPathNameA(
//   LPCSTR lpFileName,
//   DWORD  nBufferLength,
//   LPSTR  lpBuffer,
//   LPSTR  *lpFilePart
// );
// <https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfullpathnamea>


//undefined4 * __cdecl FUN_00422a60(undefined4 param_1,int param_2)
///WINAPI: HeapAlloc
///FID:cs2_full_v401/tool/ac.exe: FUN_00422a60
void * __cdecl kclib_HeapAlloc(unsigned int allocID, int allocSize)

{
    // first 4 bytes reserved for management
    int *lpMem = (int *)HeapAlloc(DAT_004c3ef0, 0, allocSize + 4);
    if (lpMem == NULL)
    {
        kcFileBase_Close();
        ///JP: kclib_LogSendMessageA("メモリが確保出来ませんでした  id = %d  size = %d(0x%x)", allocID, allocSize, allocSize); //FUN_00407bc0
        kclib_LogSendMessageA("Memory could not be allocated  id = %d  size = %d(0x%x)", allocID, allocSize, allocSize); //FUN_00407bc0
        return NULL;
    }
    *lpMem = 1; // ref count = 1
    return (void *)((char *)lpMem + 4); // return start of 'real' buffer data
}

//void __cdecl FUN_00422ab0(int param_1)
///WINAPI: HeapFree
///FID:cs2_full_v401/tool/ac.exe: FUN_00422ab0
void __cdecl kclib_HeapFree(void *kcHeap)

{
    if (kcHeap == NULL)
        return;

    int *lpMem = (int *)((char *)kcHeap - 4); // original start of heap (where ref count is)
    if (*lpMem < 2)
    {
        if (!HeapFree(DAT_004c3ef0, 0, lpMem))
        {
            kcFileBase_Close();
            return;
        }
    }
    else
    {
        *lpMem -= 1; // ref count--
    }
}


///WINAPI: GetFullPathNameA
///FID:cs2_full_v401/tool/ac.exe: FUN_00411ff0
BOOL __cdecl shiftjis_GetAbsolutePath(IN const char *filename, OUT char *outFullpath, OPTIONAL OUT char **outBasename)

{
    // wrapper call for WINAPI GetFullPathNameA
    char *basename; // this will just be a pointer offset from outFullpath,
                    // so no worries about cleanup
    
    if (!GetFullPathNameA(filename, MAX_PATH, outFullpath, &basename))
        return FALSE;
    
    if (outBasename != (char **)NULL)
    {
        *outBasename = basename;
    }
    return TRUE;
}

#else

///WINAPI: GetFullPathNameA
///FID:cs2_full_v401/tool/ac.exe: FUN_00411ff0
bool kclib::GetAbsolutePath(IN const char *filename, OUT char *outFullpath, OPTIONAL OUT char **outBasename)

{
    // wrapper call for WINAPI GetFullPathNameA
    char *basename; // this will just be a pointer offset from outFullpath,
                    // so no worries about cleanup
    
    if (!GetFullPathNameA(filename, MAX_PATH, outFullpath, &basename))
        return false;
    
    if (outBasename != nullptr)
    {
        *outBasename = basename;
    }
    return true;
}


///WINAPI: GetFullPathNameW
bool kclib::GetAbsolutePath(IN const wchar_t *filename, OUT wchar_t *outFullpath, OPTIONAL OUT wchar_t **outBasename)

{
    // wrapper call for WINAPI GetFullPathNameW
    wchar_t *basename; // this will just be a pointer offset from outFullpath,
                    // so no worries about cleanup
    
    if (!GetFullPathNameW(filename, MAX_PATH, outFullpath, &basename))
        return false;
    
    if (outBasename != nullptr)
    {
        *outBasename = basename;
    }
    return true;
}

#endif
