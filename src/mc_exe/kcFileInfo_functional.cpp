
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcFileInfo_functional.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
int g_FILE_NUM_0041ff9c;


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
FILE_READER * __thiscall kcUnkFile_ctor(FILE_READER *this, IN const char *filename)
{
    // unsigned int name_len;
    // unsigned int tmp_len = ::strlen(filename);
    // if (tmp_len < 0x3ff)
    // {
    //     name_len = ::strlen(filename); // maximum over-redundancy! wooo~
    // }
    // else
    // {
    //     name_len = 0x3ff;
    // }
    unsigned int name_len = std::max(0x3ffU, (unsigned int)std::strlen(filename));
    std::memcpy(&this->Filename[0], filename, name_len);
    this->Filename[name_len] = '\0';
    this->Handle = nullptr; // 0
    this->FindHandle = nullptr; // 0
    this->DesiredAccess = GENERIC_READ | GENERIC_WRITE; // 0xc0000000
    this->ShareMode = (FILE_SHARE_READ | FILE_SHARE_WRITE); // 3
    this->CreationDisposition = OPEN_EXISTING; // 3
    this->FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL; // 0x80
    this->TemplateHandle = nullptr; // CreateFileA HANDLE hTemplateFile / Overlapped?
    this->RdrUnk265 = 0; // ???
    g_FILE_NUM_0041ff9c += 1;
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
void __fastcall kcUnkFile_dtor(FILE_READER *this)
{
    kcUnkFile_Close(this);
    g_FILE_NUM_0041ff9c -= 1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
void * __thiscall kcUnkFile_scalar_dtor(FILE_READER *this, int flags)
{
    kcUnkFile_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413710
BOOL __fastcall kcUnkFile_Open(FILE_READER *this)
{
    HANDLE hFile;
    // hFile = ::CreateFileA((LPCSTR)(param_1 + 9),(DWORD)param_1[2],(DWORD)param_1[3],
    //                     (LPSECURITY_ATTRIBUTES)0x0,(DWORD)param_1[4],(DWORD)param_1[5],param_1[6]);
    hFile = ::CreateFileA(&this->Filename[0], this->DesiredAccess, this->ShareMode,
                        NULL, this->CreationDisposition, this->FlagsAndAttributes, this->TemplateHandle);

    this->Handle = hFile; // what on Earth is going on here? Hot Potato?
    hFile = this->Handle;

    if (hFile == INVALID_HANDLE_VALUE)
    {
        this->Handle == nullptr;
    }
    return (BOOL)(hFile != INVALID_HANDLE_VALUE);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
BOOL __fastcall kcUnkFile_Close(FILE_READER *this)
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
BOOL __fastcall kcUnkFile_Delete(FILE_READER *this)
{
    kcUnkFile_Close(this);
    return DeleteFileA(&this->Filename[0]);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413660
unsigned int __thiscall kcUnkFile_Write(FILE_READER *this, IN const unsigned char *inBuffer, unsigned int numBytes)

{
    // unsigned int bytesWritten;
    // BOOL BVar2;
    
    if (this->Handle == nullptr) // == 0
        return 0U;

    ///FIXME: Temp workaround for x64 build environment, making DWORD (unsigned long) sizeof 8
    DWORD bytesWritten = this->LastBytesWritten;
    BOOL result = ::WriteFile(this->Handle, inBuffer, numBytes, &bytesWritten, NULL);
    this->LastBytesWritten = bytesWritten;
    // BOOL result = ::WriteFile(this->Handle, inBuffer, numBytes, (LPDWORD)&this->LastBytesWritten, NULL);
    if (result == FALSE) // == 0
    {
        return 0U;
    }
    return this->LastBytesWritten;
    // if (result == 0)
    // {
    //     return 0U;
    //     bytesWritten = 0U;
    // }
    // else
    // {
    //     bytesWritten = this->LastBytesWritten;
    // }
    // return bytesWritten;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
void __fastcall kcUnkFile_SetReadMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_READ; // = 0x80000000
    this->ShareMode &= FILE_SHARE_READ; // &= 1
    this->CreationDisposition = OPEN_EXISTING; // = 3
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void __fastcall kcUnkFile_SetWriteMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_WRITE; // = 0x40000000;
    this->ShareMode &= FILE_SHARE_WRITE;// &= 2
    this->CreationDisposition = CREATE_ALWAYS; // = 2
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20
void __fastcall kcUnkFile_SetWriteMode_thunk(FILE_READER *this)
{
    kcUnkFile_SetWriteMode(this);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int __fastcall kcUnkFile_FindLoop(FILE_READER *this)
{
    unsigned int result;
    do
    {
        result = kcUnkFile_FindNext(this);
        if ((int)result < 0) // I have NO IDEA what this comparison is trying to achieve
        {
            return result; // maybe... break on no more files?
        }
    } while ((result & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_TEMPORARY)) != 0); // this is the real comparison I guess
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int __fastcall kcUnkFile_FindNext(FILE_READER *this)
{
    if (this->FindHandle == nullptr)
    {
        this->FindHandle = ::FindFirstFileA(&this->Filename[0], &this->FindData);
        if (this->FindHandle == INVALID_HANDLE_VALUE)
        {
            kcUnkFile_FindClose(this);
            return INVALID_FILE_ATTRIBUTES; // 0xffffffff
        }
    }
    else
    {
        if (::FindNextFileA(this->FindHandle, &this->FindData) == FALSE)
        {
            kcUnkFile_FindClose(this);
            return INVALID_FILE_ATTRIBUTES; // 0xffffffff
        }
    }
    std::strcpy(&this->Filename[0], this->FindData.cFileName);
    return this->FindData.dwFileAttributes; //???
    // return *(undefined4 *)(param_1 + 0x428);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void __fastcall kcUnkFile_FindClose(FILE_READER *this)
{
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
BOOL __fastcall kcUnkFile_UpdateModifiedTime(FILE_READER *this)
{
    HANDLE hFile;
    if (this->Handle == nullptr)
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
    }
    else
    {
        hFile = this->Handle;
    }
    FILETIME creationTime, accessTime, modifiedTime;
    BOOL result = ::GetFileTime(hFile, &creationTime, &accessTime, &modifiedTime);
    if (this->Handle == nullptr)
    {
        ::CloseHandle(hFile);
    }
    if (result == TRUE) // == 1
    {
        ::FileTimeToLocalFileTime(&modifiedTime, &this->ModifiedFileTime); // 0x15e (0x578)
        ::FileTimeToSystemTime(&this->ModifiedFileTime, &this->ModifiedSystemTime); // 0x15e, 0x15a (0x578, 0x568)
    }
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
unsigned int __fastcall kcUnkFile_GetMSDOSTimestamp(FILE_READER *this)
{
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
    // BOOL BVar1;
    // unsigned int uVar2;
    
    if (kcUnkFile_UpdateModifiedTime(this)) // != FALSE) // 0
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
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 22 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 17 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 12 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
        // return (((((
        //         (uint)*(ushort *)((int)param_1 + 0x568) - 0x7bc & 0x3f) << 4 | // 0x15a
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 6 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
    }
    return 0U;
    // if (BVar1 == 0)
    // {
    //     uVar2 = 0;
    // }
    // else
    // {
    //     uVar2 = ((((((uint)*(ushort *)(param_1 + 0x15a) - 0x7bc & 0x3f) << 4 |
    //             (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
    //             (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
    //             (uint)*(ushort *)(param_1 + 0x15c) & 0x1f) << 6 |
    //             (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
    //             (uint)*(ushort *)(param_1 + 0x15d) & 0x3f;
    // }
    // return uVar2;
}

// DWORD __fastcall kcUnkFile_GetFileSize(HANDLE *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int __fastcall kcUnkFile_GetFileSize(FILE_READER *this)
{
    HANDLE hFile;
    if (this->Handle == nullptr)
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) // 0xffffffff
        {
            return INVALID_FILE_SIZE; // 0xffffffff
        }
    }
    else
    {
        hFile = this->Handle;
    }

    DWORD fileSize;
    fileSize = ::GetFileSize(hFile, &fileSize);
    if (this->Handle == nullptr)
    {
        ::CloseHandle(hFile);
    }
    return (unsigned int)fileSize;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413960
char * __fastcall kcUnkFile_FUN_00413960(FILE_READER *this)
{
    char *pcVar1;
    char *pcVar2;
    char *pcVar3;
    char *local_14;
    char *local_8;

    ///NOTE:
    /// strchr = find FIRST (and return char pointer from)
    /// strrchr = find LAST (and return char pointer from), aka "reverse", or "right"
    
    pcVar1 = std::strchr(&this->Filename[0], ' ');//0x20);
    if (pcVar1 != nullptr)
    {
        *pcVar1 = '\0';
    }
    if (this->Filename[0] == '\0')
    {
        local_8 = &this->Filename[0];
    }
    else
    {
        pcVar2 = std::strrchr(&this->Filename[0], '/'); //0x2f);
        pcVar3 = std::strrchr(&this->Filename[0], '\\'); //0x5c);
        if (pcVar3 < pcVar2)
        {
            local_14 = std::strrchr(&this->Filename[0], '/'); //0x2f);
        }
        else
        {
            local_14 = std::strrchr(&this->Filename[0], '\\'); //0x5c);
        }
        local_8 = local_14;
        if ((local_14 == nullptr) &&
            (local_8 = std::strchr(&this->Filename[0], ':' /*0x3a*/), local_8 == nullptr)) // 0x3a
        {
            if (pcVar1 != nullptr)
            {
                *pcVar1 = ' ';
            }
            return &this->Filename[0];
        }
        if (pcVar1 != nullptr)
        {
            *pcVar1 = ' ';
        }
        local_8 = local_8 + 1;
    }
    return local_8;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b70
char * __fastcall kcUnkFile_GetExtension(FILE_READER *this)
{
    char *_Str;
    _Str = kcUnkFile_FUN_00413960(this);
    _Str = std::strrchr(_Str, '.'); //0x2e);
    if (_Str == nullptr)
        return nullptr;
    return _Str + 1;
    // if (_Str == (char *)0x0)
    // {
    //     _Str = (char *)0x0;
    // }
    // else
    // {
    //     _Str = _Str + 1;
    // }
    // return _Str;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413bb0
void __thiscall kcUnkFile_ChangeExtension(FILE_READER *this, IN const char *ext)
{
    char *puVar1;
    size_t sVar2;
    char *local_8;
    
    local_8 = kcUnkFile_GetExtension(this);
    if (local_8 == (char *)0x0)
    {
        sVar2 = std::strlen(&this->Filename[0]); //(char *)(this + 0x24));
        puVar1 = &this->Filename[sVar2]; //(char *)(this + 0x24 + sVar2);
        *puVar1 = '.'; //0x2e;
        local_8 = puVar1 + 1;
    }
    if (ext == nullptr || ext[0] == '\0')
    {
        local_8[-1] = '\0';
    }
    else
    {
        ///FIXME: Does this properly null-terminate?, if not it should be fixed for mc_tool release
        std::strcpy(local_8, ext);
    }
    return;
}

#endif
