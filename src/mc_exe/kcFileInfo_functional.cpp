
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcFileInfo_functional.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
int g_FILE_READER_COUNT;


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
FILE_READER * __thiscall kcFileInfo_ctor(FILE_READER *this, IN const char *filename)
{
    unsigned int name_len = std::max(0x3ffU, (unsigned int)std::strlen(filename));
    std::memcpy(&this->Filename[0], filename, name_len);
    this->Filename[name_len] = '\0';

    this->Handle     = nullptr;
    this->FindHandle = nullptr;
    this->DesiredAccess = GENERIC_READ | GENERIC_WRITE;       // 0xc0000000
    this->ShareMode     = FILE_SHARE_READ | FILE_SHARE_WRITE; // 0x3
    this->CreationDisposition = OPEN_EXISTING;                // 0x3
    this->FlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL;        // 0x80
    this->TemplateHandle      = nullptr; // CreateFileA HANDLE hTemplateFile

    this->RdrReserved = 0; // ???

    g_FILE_READER_COUNT += 1;
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
void __fastcall kcFileInfo_dtor(FILE_READER *this)
{
    kcFileInfo_Close(this);

    g_FILE_READER_COUNT -= 1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
void * __thiscall kcFileInfo_scalar_dtor(FILE_READER *this, int flags)
{
    kcFileInfo_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
BOOL __fastcall kcFileInfo_Open(FILE_READER *this)
{
    HANDLE hFile = ::CreateFileA(&this->Filename[0], this->DesiredAccess, this->ShareMode,
                        NULL, this->CreationDisposition, this->FlagsAndAttributes, this->TemplateHandle);

    this->Handle = hFile;
    //hFile = this->Handle; // what on Earth is going on here? Hot Potato??

    if (hFile == INVALID_HANDLE_VALUE)
    {
        this->Handle == nullptr;
    }
    return (BOOL)(hFile != INVALID_HANDLE_VALUE);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
BOOL __fastcall kcFileInfo_Close(FILE_READER *this)
{
    BOOL result = TRUE;
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
    if (this->Handle != nullptr)
    {
        result = ::CloseHandle(this->Handle);
        this->Handle = nullptr;
    }
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
BOOL __fastcall kcFileInfo_Delete(FILE_READER *this)
{
    kcFileInfo_Close(this);
    return DeleteFileA(&this->Filename[0]);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
unsigned int __thiscall kcFileInfo_Write(FILE_READER *this, IN const unsigned char *inBuffer, unsigned int numBytes)
{
    if (this->Handle == nullptr)
        return 0U;

    ///FIXME: Temp workaround for x64 build environment, making DWORD (unsigned long) sizeof 8
    DWORD bytesWritten = this->LastBytesWritten;
    BOOL result = ::WriteFile(this->Handle, inBuffer, numBytes, &bytesWritten, NULL);
    this->LastBytesWritten = (unsigned int)bytesWritten;
    // BOOL result = ::WriteFile(this->Handle, inBuffer, numBytes, (LPDWORD)&this->LastBytesWritten, NULL);
    if (result == FALSE)
        return 0U;

    return this->LastBytesWritten;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
void __fastcall kcFileInfo_SetReadMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_READ; // = 0x80000000
    this->ShareMode &= FILE_SHARE_READ; // &= 1
    this->CreationDisposition = OPEN_EXISTING; // = 3
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void __fastcall kcFileInfo_SetWriteMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_WRITE; // = 0x40000000;
    this->ShareMode &= FILE_SHARE_WRITE;// &= 2
    this->CreationDisposition = CREATE_ALWAYS; // = 2
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20 (thunk: FUN_00413770)
void __fastcall kcFileInfo_SetWriteMode_thunk(FILE_READER *this)
{
    kcFileInfo_SetWriteMode(this);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int __fastcall kcFileInfo_FindNextValid(FILE_READER *this)
{
    unsigned int attributes;
    do
    {
        attributes = kcFileInfo_FindNext(this);
        if ((int)attributes < 0) // comparison for INVALID_FILE_ATTRIBUTES (0xffffffff)
            break; // next file is invalid, break

    } while ((attributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_TEMPORARY)) != 0); // skip directories/temp files

    return attributes; // returns either next valid file attributes, or INVALID_FILE_ATTRIBUTES
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int __fastcall kcFileInfo_FindNext(FILE_READER *this)
{
    // Open find handle if first time calling
    if (this->FindHandle == nullptr)
    {
        this->FindHandle = ::FindFirstFileA(&this->Filename[0], &this->FindData);
        if (this->FindHandle == INVALID_HANDLE_VALUE)
        {
            kcFileInfo_FindClose(this);
            return INVALID_FILE_ATTRIBUTES; // 0xffffffff
        }
    }
    else if (::FindNextFileA(this->FindHandle, &this->FindData) == FALSE)
    {
        kcFileInfo_FindClose(this);
        return INVALID_FILE_ATTRIBUTES; // 0xffffffff
    }

    std::strcpy(&this->Filename[0], this->FindData.cFileName); // Update to found filename (for iteration)
    return this->FindData.dwFileAttributes; // *(unk4 *)0x428
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void __fastcall kcFileInfo_FindClose(FILE_READER *this)
{
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
BOOL __fastcall kcFileInfo_UpdateModifiedTime(FILE_READER *this)
{
    HANDLE hFile = this->Handle;
    if (hFile == nullptr) // open new handle if our's is not already open
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            return false;
        }
    }

    FILETIME creationTime, accessTime, modifiedTime;
    BOOL result = ::GetFileTime(hFile, &creationTime, &accessTime, &modifiedTime);

    if (this->Handle == nullptr) // we opened a new handle, close it
    {
        ::CloseHandle(hFile);
    }

    if (result == TRUE)
    {
        ::FileTimeToLocalFileTime(&modifiedTime, &this->ModifiedFileTime); // 0x15e (0x578)
        ::FileTimeToSystemTime(&this->ModifiedFileTime, &this->ModifiedSystemTime); // 0x15e, 0x15a (0x578, 0x568)
    }
    return (bool)result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
unsigned int __fastcall kcFileInfo_GetTimestamp(FILE_READER *this)
{
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime

    if (kcFileInfo_UpdateModifiedTime(this) != FALSE)
    {
        // This will stop working in 2043 (or 2044?) ... yay~
        return ((((unsigned int)this->ModifiedSystemTime.wYear - 1980U) & 0x3f) << 26) | // (4 + 5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMonth  &  0xf) << 5) | // (5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wDay    & 0x1f) << 5) | // (5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wHour   & 0x1f) << 6) | // (6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMinute & 0x3f) << 6) | // (6)
                 ((unsigned int)this->ModifiedSystemTime.wSecond & 0x3f);        // (0)

        // return (((((
        //         (uint)*(ushort *)((int)param_1 + 0x568) - 0x7bc & 0x3f) << 4 | // 0x15a
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 6 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
    }
    return 0U;
}

// DWORD __fastcall kcFileInfo_GetSize(HANDLE *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int __fastcall kcFileInfo_GetSize(FILE_READER *this)
{
    HANDLE hFile = this->Handle;
    if (hFile == nullptr) // open new handle if our's is not already open
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            return INVALID_FILE_SIZE; // 0xffffffff
        }
    }

    DWORD fileSize; // this function only gets the LODWORD size (up to 4 GB)
    fileSize = ::GetFileSize(hFile, &fileSize);

    if (this->Handle == nullptr) // we opened a new handle, close it
    {
        ::CloseHandle(hFile);
    }
    return (unsigned int)fileSize;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
char * __fastcall kcFileInfo_GetBasename(FILE_READER *this)
{
    ///NOTE: strchr = find FIRST (and return char pointer from)
    ///      strrchr = find LAST (and return char pointer from), aka "reverse", or "right"
    
    // disables the usage of ' ' spaces in filepaths
    //  (only names would allow for a space without being butchered, which seems counterintuitive)
    ///FIXME: Space behavior should only care about spaces in basenames
    char *firstSpace = std::strchr(&this->Filename[0], ' ');//0x20);
    if (firstSpace != nullptr)
    {
        *firstSpace = '\0'; // first instance of space is null-terminated?
    }

    // if the filepath length is non-zero (and doesn't start with a space, making it valid)
    //   find path separators in filepath (if one exists)
    if (this->Filename[0] != '\0')
    {
        char *separator;

        // find last slash in filepath ('/' or '\\')
        char *lastFwdslash = std::strrchr(&this->Filename[0], '/'); //0x2f
        char *lastBackslash = std::strrchr(&this->Filename[0], '\\'); //0x5c
        if (lastBackslash < lastFwdslash) // last separator is '/'
        {
            separator = lastFwdslash;
        }
        else if (lastBackslash != nullptr) // last separator is '\\'
        {
            separator = lastBackslash;
        }
        else // no slash separators, check for first colon separator
        {
            separator = std::strchr(&this->Filename[0], ':'); //firstColon;
        }
        
        // restore the null-terminated space from earlier
        ///FIXME: Space behavior should only care about spaces in basenames
        if (firstSpace != nullptr)
        {
            *firstSpace = ' '; // note how space is only restored if not first char in filepath
                               //  it's wonkey behavior, but spaces as first chars are indeed invalid
        }
        
        // if we found a separator, filepath field contains more than just basename,
        //   return "start of name" (not fully accurate because space handling is problematic)
        if (separator != nullptr)
        {
            return separator + 1; // +1 to start name after matched ('/', '\\', or ':') char
        }
    }

    // no separators found, return start of filename
    return &this->Filename[0];
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
char * __fastcall kcFileInfo_GetExtension(FILE_READER *this)
{
    char *ext_ptr = std::strrchr(kcFileInfo_GetBasename(this), '.'); //0x2e);
    if (ext_ptr != nullptr)
    {
        return ext_ptr + 1; // +1 to get extension *after* '.'
    }
    return nullptr; // no extension (or failure with invalid space ' ' in basename func)
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
void __thiscall kcFileInfo_ChangeExtension(FILE_READER *this, IN const char *newExtension)
{
    char *ext_ptr = kcFileInfo_GetExtension(this); // will never return index &this->Filename[0]
    if (ext_ptr == nullptr)
    {
        // no extension, let's prepare to add one by appending '.'
        unsigned int str_len = (unsigned int)std::strlen(&this->Filename[0]);
        ext_ptr = &this->Filename[str_len];
        ext_ptr[0] = '.';
        ext_ptr++; // +1 for position right after extension
    }

    // remove extension (null or empty string)
    if (newExtension == nullptr || newExtension[0] == '\0')
    {
        ext_ptr[-1] = '\0'; // -1 for '.' position (ext_ptr can never be index &this->Filename[0])
    }
    else // change extension (correctly overwrites longer/shorter extensions)
    {
        std::strcpy(ext_ptr, newExtension); // strcpy includes copying null-terminator byte - all is well!
    }
}

#endif
