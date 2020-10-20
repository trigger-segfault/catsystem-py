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
    extern int g_FILE_READER_COUNT;

    // similar in nature to C#'s FileInfo class,
    //  this is for info on a file, but not reading or writing
    struct FILE_READER
    {
        // reference counter for kcFileInfo's
        // signed since most reference counters are signed to check for underflow
        ///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
        static int g_FILEINFO_COUNT;

        /*$0,4*/    HANDLE Handle; // Handle to open file
        /*$4,4*/    HANDLE FindHandle; // Handle to open file find operation
        /*$8,4*/    unsigned int DesiredAccess; // GENERIC_READ | GENERIC_WRITE (0xc0000000)
        /*$c,4*/    unsigned int ShareMode; // FILE_SHARE_READ | FILE_SHARE_WRITE (0x3)
        /*$10,4*/   unsigned int CreationDisposition; // OPEN_EXISTING (0x3)
        /*$14,4*/   unsigned int FlagsAndAttributes; // FILE_ATTRIBUTE_NORMAL (0x80)
        /*$18,4*/   HANDLE TemplateHandle; // Handle to a template file (optional, for use when opening in create mode)
        /*$1c,4*/   unsigned int LastBytesRead; // Possibly LastBytesRead (similar to LastBytesWritten) // LastBytesRead
        /*$20,4*/   unsigned int LastBytesWritten; // Set to bytesWritten count after Write()
        /*$24,400*/ char Filename[0x400]; // copy of filepath passed in from ctor
        /*$424,4*/  unsigned int RdrReserved; // Set to zero in ctor, never observed (may be reserved field as failsafe for Filename size limit)
        /*$428,14c*/ WIN32_FIND_DATAA FindData;
        /*$568,10*/ SYSTEMTIME ModifiedSystemTime; // stored as Local time
        /*$578,8*/  FILETIME ModifiedFileTime; // stored as Local time
        // ... /*$580,18*/
        /*$598*/
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    #endif
    namespace kclib
    {
    // #else

    // similar in nature to C#'s FileInfo class,
    //  combined with read/write capabilities it seems...
    //  this really does it all!
    class kcFileInfo
    {
        // reference counter for kcFileInfo's
        // signed since most reference counters are signed to check for underflow
        ///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
        static int g_FILEINFO_COUNT;

        /*$0,4*/    HANDLE Handle; // Handle to open file
        /*$4,4*/    HANDLE FindHandle; // Handle to open file find operation
        /*$8,4*/    unsigned int DesiredAccess; // GENERIC_READ | GENERIC_WRITE (0xc0000000)
        /*$c,4*/    unsigned int ShareMode; // FILE_SHARE_READ | FILE_SHARE_WRITE (0x3)
        /*$10,4*/   unsigned int CreationDisposition; // OPEN_EXISTING (0x3)
        /*$14,4*/   unsigned int FlagsAndAttributes; // FILE_ATTRIBUTE_NORMAL (0x80)
        /*$18,4*/   HANDLE TemplateHandle; // Handle to a template file (optional, for use when opening in create mode)
        /*$1c,4*/   unsigned int LastBytesRead; // Possibly LastBytesRead (similar to LastBytesWritten) // LastBytesRead
        /*$20,4*/   unsigned int LastBytesWritten; // Set to bytesWritten count after Write()
        /*$24,400*/ char Filename[0x400]; // copy of filepath passed in from ctor
        /*$424,4*/  unsigned int RdrReserved; // Set to zero in ctor, never observed (may be reserved field as failsafe for Filename size limit)
        /*$428,14c*/ WIN32_FIND_DATAA FindData;
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

        // Open the current file name for reading or writing (based on mode set by SetReadMode/SetWriteMode)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
        bool Open();
        // Close any open files (including any find handles)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
        bool Close();
        // Deletes the current file name being pointed to
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
        bool Delete();

        // read data from a file opened with Open()
        ///CUSTOM: probably exists in co-operation with Write function
        unsigned int Read(OUT unsigned char *outBuffer, unsigned int numBytes);
        // write data to a file opened with Open()
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
        unsigned int Write(IN const unsigned char *inBuffer, unsigned int numBytes);

        // Set the file info to read only mode (no longer usable to write files)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
        void SetReadMode();
        // Set the file info to write only mode (no longer usable to read files)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20 (thunk)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
        void SetWriteMode();

        // Iterate through all files in the pattern (or only file if not a pattern)
        // Files with attributes (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_TEMPORARY) are skipped
        //   returns the file attributes,
        //   or INVALID_FILE_ATTRIBUTES (0xffffffff) if no more files were found
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
        unsigned int FindNextValid();
        // Internal call of FindNextValid
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
        unsigned int FindNext();
        // Closes the find handle, if open
        //  (automatically called when no more files are found by FindNext*() functions)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
        void FindClose();

        // Returns the MSDOS-style timestamp of the file's last modified time
        //  0 is returned on failure
        // This will fail after the year 2043 (this is an old timestamp format)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
        unsigned int GetMSDOSTimestamp();
        // Gets the size of the file (up to 4 GB)
        //  returns INVALID_FILE_SIZE (0xffffffff) on failure
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
        unsigned int GetSize() const;

        ///CUSTOM: function for potentially inlined access to Filename member
        const char * GetFilename() const;

        // Always returns a pointer from within this->Filename field.
        // Any spaces in the filepath (but not filename) will return unexpected results
        // If the first char in this->Filename is a space, the Filename field will be cleared
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
        char * GetBasename();
        // Returns position after '.' char, or null if no '.' char
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
        char * GetExtension();
        // Extension should NOT include "."
        //  empty string OR null will remove extension (and '.')
        // does not handle spaces in filepaths well (due to GetBasename func)
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
        void ChangeExtension(IN const char *newExtension);

    private:
        // internal?
        ///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
        bool UpdateModifiedTime();
    };

    ///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
    } /* end namespace kclib */
    // #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
