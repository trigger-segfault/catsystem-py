
// OOP + function definitions for kclib "common" functions. (those not using WINAPI)

#include "kclib_common.h"

#ifndef KCLIB_OOP

///FID:cs2_full_v401/tool/ac.exe: FUN_00407bc0
void kclib_LogSendMessageA(char *param_1, ...)
{
    // dummy func
}

//undefined4 FUN_00407b50(void)
///FID:cs2_full_v401/tool/ac.exe: FUN_00407b50
BOOL kcFileBase_Close(void)

{
    return FALSE; // actual implementation (for polymorphism)
}


///FID:cs2_full_v401/tool/ac.exe: FUN_00412200
BOOL __cdecl shiftjis_IsCharDoubleByte(IN const char *chr)

{
    // This function is EVERYWHERE in Cs2 character parsing!
    // it's used in-place of parsing text per-character,
    //  and detects if we should ignore (and group) the following byte
    //  this is important because many ascii-range values can appear after the first byte
    //  and these may be a character we're detecting for
    //  see: shiftjis_GetParentDirectory (below), to see a use case example
    const unsigned char *ch = (const unsigned char *)chr; // easier char math with unsigned
    if (ch == NULL || ch[0] == 0x00)
        return FALSE;
    // ch[0] in range [$81-$9f, $e0-$ef]
    // ch[1] in range [$40-$7e, $80-$fc]
    return ((ch[0] >= 0x81 && ch[0] < 0xa0) || (ch[0] >= 0xe0 && ch[0] < 0xf0)) &&
           ((ch[1] >= 0x40 && ch[1] < 0x7f) || (ch[1] >= 0x80 && ch[1] < 0xfd));
}


//BOOL __cdecl FUN_00412520(char *param_1, char *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00412520
BOOL __cdecl shiftjis_GetParentDirectory(IN const char *fullpath, OUT char *outParentdir)

{
    // This expects a full path... I think...

    int seppos = 0;
    // get index of last path separator
    for (int i = 0; fullpath[i] != '\0'; i++)
    {
        if (shiftjis_IsCharDoubleByte(&fullpath[i]))
        {
            i++;
        }
        else if (fullpath[i] == '\\' || fullpath[i] == '/')
        {
            seppos = i;
        }
    }
    // copy basepath
    //memcpy(outParentdir, fullpath, seppos);
    for (int i = 0; i < seppos; i++)
    {
        outParentdir[i] = fullpath[i];
    }
    outParentdir[seppos] = '\0';

    return seppos != 0;
}   


//undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004120e0
BOOL __cdecl shiftjis_ChangeExtension(IN OUT char *filename, IN const char *extension)

{
    // extension does NOT include '.'
    // NULL treats as remove extension
    // empty string results with just a '.'

    if (filename == NULL)
        return FALSE;
    
    int extpos = -1;
    int namelen = strlen(filename);
    for (int i = namelen - 1; i >= 0; i--)
    {
        if (filename[i] == '\\' || filename[i] == '/')
        {
            extpos = namelen;
            break;
        }
        if (filename[i] == '.')
        {
            extpos = i;
            break;
        }
    }
    if (extension != NULL)
    {
        filename[extpos++] = '.';
        for (int i = 0; extension[i] != '\0'; i++)
        {
            filename[extpos++] = extension[i];
        }
    }
    filename[extpos] = '\0';
    return TRUE;
}

#else


///FID:cs2_full_v401/tool/ac.exe: FUN_00412200
bool kclib::IsCharDoubleByte(IN const char *chr)

{
    // This function is EVERYWHERE in Cs2 character parsing!
    // it's used in-place of parsing text per-character,
    //  and detects if we should ignore (and group) the following byte
    //  this is important because many ascii-range values can appear after the first byte
    //  and these may be a character we're detecting for
    //  see: shiftjis_GetParentDirectory (below), to see a use case example
    const unsigned char *ch = (const unsigned char *)chr; // easier char math with unsigned
    if (ch == nullptr || ch[0] == 0x00)
        return false;
    // ch[0] in range [$81-$9f, $e0-$ef]
    // ch[1] in range [$40-$7e, $80-$fc]
    return ((ch[0] >= 0x81 && ch[0] < 0xa0) || (ch[0] >= 0xe0 && ch[0] < 0xf0)) &&
           ((ch[1] >= 0x40 && ch[1] < 0x7f) || (ch[1] >= 0x80 && ch[1] < 0xfd));
}

//BOOL __cdecl FUN_00412520(char *param_1, char *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_00412520
bool kclib::GetParentDirectory(IN const char *fullpath, OUT char *outParentdir)

{
    // This expects a full path... I think...

    int seppos = 0;
    // get index of last path separator
    for (int i = 0; fullpath[i] != '\0'; i++)
    {
        if (kclib::IsCharDoubleByte(&fullpath[i]))
        {
            i++;
        }
        else if (fullpath[i] == '\\' || fullpath[i] == '/')
        {
            seppos = i;
        }
    }
    // copy basepath
    std::memcpy(outParentdir, fullpath, seppos);
    // for (int i = 0; i < seppos; i++)
    // {
    //     outParentdir[i] = fullpath[i];
    // }
    outParentdir[seppos] = '\0';

    return seppos != 0;
}   


//undefined4 __cdecl FUN_004120e0(char *param_1,undefined4 *param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_004120e0
bool kclib::ChangeExtension(IN OUT char *filename, IN const char *extension)

{
    // extension does NOT include '.'
    // NULL treats as remove extension
    // empty string results with just a '.'

    if (filename == nullptr)
        return false;
    
    int extpos = -1;
    int namelen = std::strlen(filename);
    for (int i = namelen - 1; i >= 0; i--)
    {
        if (filename[i] == '\\' || filename[i] == '/')
        {
            extpos = namelen;
            break;
        }
        if (filename[i] == '.')
        {
            extpos = i;
            break;
        }
    }
    if (extension != nullptr)
    {
        filename[extpos++] = '.';
        for (int i = 0; extension[i] != '\0'; i++)
        {
            filename[extpos++] = extension[i];
        }
    }
    filename[extpos] = '\0';
    return true;
}

// see: <https://stackoverflow.com/a/150616>
void _kclib_LogError(const char *format, std::va_list argp)
{
//     __va_list_tag args;
//     va_start
//     va_start(args, format);
    std::vfprintf(stderr, format, argp);
//     va_end(args);
}

void kclib::LogError(char *format, ...)
{
    std::va_list args;
    va_start(args, format);
    _kclib_LogError(format, args);
    va_end(args);
}

#endif
