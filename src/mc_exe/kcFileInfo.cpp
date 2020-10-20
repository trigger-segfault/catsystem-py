
#include "common_mc.h"
///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
#ifndef KCLIB_OOP
#undef this
// #ifdef KCLIB_OOP

#include "kcFileInfo.h"


///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
int kclib::kcFileInfo::g_FILEINFO_COUNT;


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
kclib::kcFileInfo::kcFileInfo(IN const char *filename)
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

    kclib::kcFileInfo::g_FILEINFO_COUNT += 1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
kclib::kcFileInfo::~kcFileInfo()
{
    this->Close();
    
    kclib::kcFileInfo::g_FILEINFO_COUNT -= 1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
bool kclib::kcFileInfo::Open()
{
    HANDLE hFile = ::CreateFileA(&this->Filename[0], this->DesiredAccess, this->ShareMode,
                        NULL, this->CreationDisposition, this->FlagsAndAttributes, this->TemplateHandle);

    this->Handle = hFile;
    //hFile = this->Handle; // what on Earth is going on here? Hot Potato??

    if (hFile == INVALID_HANDLE_VALUE)
    {
        this->Handle == nullptr;
    }
    return (hFile != INVALID_HANDLE_VALUE);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
bool kclib::kcFileInfo::Close()
{
    bool result = true;
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
    if (this->Handle != nullptr)
    {
        result = (bool)::CloseHandle(this->Handle);
        this->Handle = nullptr;
    }
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b00
bool kclib::kcFileInfo::Delete()
{
    this->Close();
    return (bool)::DeleteFileA(&this->Filename[0]);
}

///CUSTOM: probably exists in co-operation with Write function
unsigned int kclib::kcFileInfo::Read(OUT unsigned char *outBuffer, unsigned int numBytes)
{
    if (this->Handle == nullptr)
        return 0U;

    ///FIXME: Temp workaround for x64 build environment, making DWORD (unsigned long) sizeof 8
    DWORD bytesRead = this->LastBytesRead;
    BOOL result = ::ReadFile(this->Handle, outBuffer, numBytes, &bytesRead, NULL);
    this->LastBytesRead = (unsigned int)bytesRead;
    // BOOL result = ::ReadFile(this->Handle, outBuffer, numBytes, (LPDWORD)&this->LastBytesRead, NULL);
    if (result == FALSE)
        return 0U;

    return this->LastBytesRead;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
unsigned int kclib::kcFileInfo::Write(IN const unsigned char *inBuffer, unsigned int numBytes)
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
void kclib::kcFileInfo::SetReadMode()
{
    this->DesiredAccess = GENERIC_READ; // = 0x80000000
    this->ShareMode &= FILE_SHARE_READ; // &= 1
    this->CreationDisposition = OPEN_EXISTING; // = 3
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20 (thunk)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void kclib::kcFileInfo::SetWriteMode()
{
    this->DesiredAccess = GENERIC_WRITE; // = 0x40000000;
    this->ShareMode &= FILE_SHARE_WRITE;// &= 2
    this->CreationDisposition = CREATE_ALWAYS; // = 2
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int kclib::kcFileInfo::FindNextValid()
{
    unsigned int attributes;
    do
    {
        attributes = this->FindNext();
        if ((int)attributes < 0) // comparison for INVALID_FILE_ATTRIBUTES (0xffffffff)
            break; // next file is invalid, break

    } while ((attributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_TEMPORARY)) != 0); // skip directories/temp files

    return attributes; // returns either next valid file attributes, or INVALID_FILE_ATTRIBUTES
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int kclib::kcFileInfo::FindNext()
{
    // Open find handle if first time calling
    if (this->FindHandle == nullptr)
    {
        this->FindHandle = ::FindFirstFileA(&this->Filename[0], &this->FindData);
        if (this->FindHandle == INVALID_HANDLE_VALUE)
        {
            this->FindClose();
            return INVALID_FILE_ATTRIBUTES; // 0xffffffff
        }
    }
    else if (::FindNextFileA(this->FindHandle, &this->FindData) == FALSE)
    {
        this->FindClose();
        return INVALID_FILE_ATTRIBUTES; // 0xffffffff
    }

    std::strcpy(&this->Filename[0], this->FindData.cFileName); // Update to found filename (for iteration)
    return this->FindData.dwFileAttributes; // *(unk4 *)0x428
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void kclib::kcFileInfo::FindClose()
{
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
bool kclib::kcFileInfo::UpdateModifiedTime()
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
unsigned int kclib::kcFileInfo::GetMSDOSTimestamp()
{
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime

    if (this->UpdateModifiedTime()) // != FALSE)
    {
        // This will stop working in 2043 (or 2044?) ... yay~
        return ((((unsigned int)this->ModifiedSystemTime.wYear - 1980U) & 0x3f) << 26) | // (4 + 5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMonth  &  0xf) << 5) | // (5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wDay    & 0x1f) << 5) | // (5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wHour   & 0x1f) << 6) | // (6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMinute & 0x3f) << 6) | // (6)
                 ((unsigned int)this->ModifiedSystemTime.wSecond & 0x3f);        // (0)

        //         (uint)*(ushort *)((int)param_1 + 0x568) - 0x7bc & 0x3f) << 4 | // 0x15a
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 6 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
    }
    return 0U;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int kclib::kcFileInfo::GetSize() const
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

///CUSTOM: function for potentially inlined access to Filename member
const char * kclib::kcFileInfo::GetFilename() const
{
    return &this->Filename[0];
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
char * kclib::kcFileInfo::GetBasename()
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
char * kclib::kcFileInfo::GetExtension()
{
    char *ext_ptr = std::strrchr(this->GetBasename(), '.'); //0x2e);
    if (ext_ptr != nullptr)
    {
        return ext_ptr + 1; // +1 to get extension *after* '.'
    }
    return nullptr; // no extension (or failure with invalid space ' ' in basename func)
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
void kclib::kcFileInfo::ChangeExtension(IN const char *newExtension)
{
    char *ext_ptr = this->GetExtension(); // will never return index &this->Filename[0]
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
