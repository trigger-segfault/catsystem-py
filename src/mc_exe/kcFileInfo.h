#pragma once

#ifndef KCLIB_MC_KCFILEINFO_CLASS_H
#define KCLIB_MC_KCFILEINFO_CLASS_H

#include "common_mc.h"

// a class for file management, that supports BOTH
// normal CreateFile operations and FindFile operations. neat!

///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime>
// typedef struct _FILETIME {
//   /*$0,4*/    DWORD dwLowDateTime;
//   /*$4,4*/    DWORD dwHighDateTime;
//   /*$8*/
// } FILETIME,*PFILETIME,*LPFILETIME;

///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime>
// typedef struct _SYSTEMTIME {
//   /*$0,2*/    WORD wYear;
//   /*$2,2*/    WORD wMonth;
//   /*$4,2*/    WORD wDayOfWeek;
//   /*$6,2*/    WORD wDay;
//   /*$8,2*/    WORD wHour;
//   /*$a,2*/    WORD wMinute;
//   /*$c,2*/    WORD wSecond;
//   /*$e,2*/    WORD wMilliseconds;
//   /*$10*/
// } SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataa>
// typedef struct _WIN32_FIND_DATAA {
//   /*$0,4*/    DWORD    dwFileAttributes;
//   /*$4,8*/    FILETIME ftCreationTime;
//   /*$c,8*/    FILETIME ftLastAccessTime;
//   /*$14,8*/   FILETIME ftLastWriteTime;
//   /*$1c,4*/   DWORD    nFileSizeHigh;
//   /*$20,4*/   DWORD    nFileSizeLow;
//   /*$24,4*/   DWORD    dwReserved0;
//   /*$28,4*/   DWORD    dwReserved1;
//   /*$2c,104*/ CHAR     cFileName[MAX_PATH];
//   /*$130,e*/  CHAR     cAlternateFileName[14];
//   /*$140*/
//   // these last 3 fields are excluded in some builds based on a define
//   //   We know these are NOT present in kcFileInfo because of the
//   //   field offsets differing by $140 bytes
//   /*$140,4*/  DWORD    dwFileType;
//   /*$144,4*/  DWORD    dwCreatorType;
//   /*$148,2*/  WORD     wFinderFlags;
//   /*$14c*/
// } WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

#ifdef KCLIB_OOP
namespace kclib
{
#endif

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // reference counter for kcFileInfo's
    // signed since most reference counters are signed to check for underflow
    ///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
    extern int g_FILE_NUM_0041ff9c;

    // similar in nature to C#'s FileInfo class,
    //  this is for info on a file, but not reading or writing
    struct FILE_READER
    {
        /*$0,4*/    HANDLE Handle; // 0
        /*$4,4*/    HANDLE FindHandle; // 0
        /*$8,4*/    unsigned int DesiredAccess; // 0
        /*$c,4*/    unsigned int ShareMode; // 0x1 = FILE_SHARE_READ
        /*$10,4*/   unsigned int CreationDisposition; // 0
        /*$14,4*/   unsigned int FlagsAndAttributes; // 0
        /*$18,4*/   HANDLE TemplateHandle; // 0
        /*$1c,4*/   unsigned int RdrUnk7; // 0
        /*$20,4*/   unsigned int LastBytesWritten; // 0
        /*$24,400*/ char Filename[0x400];
        /*$424,4*/  unsigned int RdrUnk265; // 0
        /*$428,?*/  WIN32_FIND_DATAA FindData;
        /*$568,10*/ SYSTEMTIME ModifiedSystemTime; // stored as Local time
        /*$578,8*/  FILETIME ModifiedFileTime; // stored as Local time
        // ... /*$580,18*/
        /*$598*/
    };

    #else

    // similar in nature to C#'s FileInfo class,
    //  combined with read/write capabilities it seems...
    class kcFileInfo
    {
        // reference counter for kcFileInfo's
        // signed since most reference counters are signed to check for underflow
        ///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
        static int g_FILE_NUM_0041ff9c;

        /*$0,4*/    HANDLE Handle; // 0
        /*$4,4*/    HANDLE FindHandle; // 0
        /*$8,4*/    unsigned int DesiredAccess; // 0
        /*$c,4*/    unsigned int ShareMode; // 0x1 = FILE_SHARE_READ
        /*$10,4*/   unsigned int CreationDisposition; // 0
        /*$14,4*/   unsigned int FlagsAndAttributes; // 0
        /*$18,4*/   HANDLE TemplateHandle; // 0
        /*$1c,4*/   unsigned int RdrUnk7; // 0
        /*$20,4*/   unsigned int LastBytesWritten; // 0
        /*$24,400*/ char Filename[0x400];
        /*$424,4*/  unsigned int RdrUnk265; // 0
        /*$428,?*/  WIN32_FIND_DATAA FindData;
        /*$568,10*/ SYSTEMTIME ModifiedSystemTime; // stored as Local time
        /*$578,8*/  FILETIME ModifiedFileTime; // stored as Local time
        // ... /*$580,18*/
        /*$598*/
    public:
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
        kcFileInfo(IN const char *filename);
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
        ~kcFileInfo();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
        // scalar_dtor(int flags); // (auto-generated in C++)

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
        bool Open();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
        bool Close();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
        bool Delete();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
        unsigned int Write(IN const unsigned char *inBuffer, unsigned int numBytes);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
        void SetReadMode();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
        void SetWriteMode();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20
        void SetWriteMode_thunk();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
        unsigned int FindLoop();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
        unsigned int FindNext();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
        void FindClose();

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
        bool UpdateModifiedTime();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
        unsigned int GetMSDOSTimestamp();
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
        unsigned int GetFileSize();


        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
        char * kcFileInfo_FUN_00413960(FILE_READER *this);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
        char * GetExtension(FILE_READER *this);

        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
        void ChangeExtension(FILE_READER *this, IN const char *ext);
        
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
        // void FindClose();

        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
        // bool UpdateModifiedTime();
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
        // unsigned int GetFileSize();
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
        // bool Close();

        
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
        // void SetWriteMode();
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
        // void SetReadMode();

        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
        // unsigned int FindNext();
        
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
        // unsigned int GetMSDOSTimestamp();

        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20
        // void SetWriteMode_thunk();
        // ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
        // unsigned int FindLoop();


    };

    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
