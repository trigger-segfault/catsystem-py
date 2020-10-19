
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcCatScene_functional.h"
#include "../ac_exe/ghidra_types_min.h"
#include "kcFileInfo_functional.h"


//unsigned int * __thiscall kcMacroReader_initRun(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
kcCatScene * __thiscall kcCatScene_ctor(kcCatScene *this, IN const char *filename)
{
    this->BufferLines = nullptr;
    this->MemoryLines = nullptr; // HGLOBAL GlobalAlloc
    this->BufferOffsets = nullptr;
    this->MemoryOffsets = nullptr;
    this->FileSize = 0;
    this->LineCount = 0;
    this->MacUnk0 = 0;
    this->MacUnk4 = 0;
    std::strcpy(&this->Filename[0], filename);
    kcCatScene_Read(this, filename);
    return this;
    // *(undefined4 *)((int)this + 8) = 0;
    // *(undefined4 *)((int)this + 0x414) = 0;
    // *(undefined4 *)((int)this + 0xc) = 0;
    // *(undefined4 *)((int)this + 0x418) = 0;
    // *(undefined4 *)((int)this + 0x41c) = 0;
    // *(undefined4 *)((int)this + 4) = 0;
    // *(undefined4 *)this = 0;
    // *(undefined4 *)((int)this + 0x10) = 0;
    // _strcpy((char *)((int)this + 0x14),filename);
    // kcCatScene_Read((int)this,param_1);
    // return (undefined4 *)this;
}

// undefined4 __thiscall kcCatScene_Read(int this,char *filename)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
BOOL __thiscall kcCatScene_Read(kcCatScene *this, IN const char *filename)
{
    // unsigned char bVar1;
    // // FILE_READER *this_00;
    // unsigned char *bufferLines;
    // //FILE *file;
    // int iVar3;
    // char *pcVar4;
    // undefined4 uVar5;
    // size_t sVar7;
    // FILE_READER *unkFile;
    // DWORD fileSize;
    // size_t local_1c;
    // int local_10;
    // int lineCount;
    
    // all this just to read fileSize... nice
    FILE_READER *unkFile = (FILE_READER *)std::malloc(0x598);//_newalloc(0x598);
    if (unkFile != nullptr)
    {
        unkFile = (FILE_READER *)kcUnkFile_ctor(unkFile, filename);
    }
    kcUnkFile_SetReadMode(unkFile);
    unsigned int fileSize = kcUnkFile_GetFileSize(unkFile);
    if (unkFile != nullptr)
    {
        kcUnkFile_scalar_dtor(unkFile, 1);
    }

    HGLOBAL hMemLines = nullptr;
    HGLOBAL hMemOffsets = nullptr;
    unsigned char *bufferLines = nullptr;
    unsigned int *bufferOffsets;
    unsigned int lineCount = 0;

    // allocate MemoryLines and copy lines into buffer,
    //  nullterminated with newlines and control chars removed
    if ((int)fileSize > 0)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        
        fileSize += 0x10; // extra space, for unknown reasons. Could be useful?
        FILE *file = nullptr;

        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemLines = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, fileSize);

        if (hMemLines != nullptr)
        {
            bufferLines = (unsigned char *)::GlobalLock(hMemLines);
        }

        if (bufferLines != nullptr)
        {
            file = std::fopen(filename, "rt");
        }

        // reading file (final 'if' in setup)
        if (file != nullptr)
        {
            for (int pos = 0; !std::feof(file) && std::fgets((char *)&bufferLines[pos], fileSize - pos, file); pos++)
            {
                // char *line = std::fgets(&bufferLines[i], fileSize - pos, file);
                // if (pcVar4 == nullptr)
                //     break;
                unsigned int len;// = std::strlen(&bufferLines[pos]); //&bufferLines[len + pos - 1]
                for (len = std::strlen((char *)&bufferLines[pos]); len > 0; len--)
                {
                    if (bufferLines[pos + len - 1] >= 0x20 || bufferLines[pos + len - 1] == '\t')
                        break;
                }
                bufferLines[pos + len] = '\0';
                pos += len;
                lineCount++;
                //*(char *)((int)bufferLines + pos + len) = '\0';
                // pos = pos + 1 + len
                //lineCount += 1;
            }
            std::fclose(file);
        }

        // cleanup
        if (bufferLines != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }
        // done
    }

    // No lines in the file? That's a paddlin' (return FALSE)
    if (lineCount < 1)
    {
        if (hMemLines != nullptr)
        {
            ::GlobalFree(hMemLines);
        }
        return FALSE;
    }
    // allocate MemoryOffsets and add byte offsets of each line in MemoryLines (return TRUE)
    else //if (lineCount >= 1)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        this->LineCount = lineCount;
        this->FileSize = fileSize;
        this->MemoryLines = hMemLines;
        bufferLines = nullptr;
        bufferOffsets = nullptr;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemOffsets = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lineCount * sizeof(unsigned int));
        if (hMemOffsets != nullptr)
        {
            bufferLines = (unsigned char *)::GlobalLock(hMemLines);
        }
        if (bufferLines != nullptr)
        {
            bufferOffsets = (unsigned int *)::GlobalLock(hMemOffsets);
        }
        
        // reading file (final 'if' in setup)
        if (bufferOffsets != nullptr)
        {
            // unsigned int offset = 0;
            for (unsigned int line = 0, offset = 0; line < this->LineCount; line++, offset++)
            {
                bufferOffsets[line] = offset;
                offset += (unsigned int)std::strlen((char *)&bufferLines[offset]);
            }
            ::GlobalUnlock(hMemOffsets);
        }

        // cleanup
        if (bufferLines != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }

        // done
        this->MemoryOffsets = hMemOffsets;
        return TRUE;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
BOOL __fastcall kcCatScene_LockBuffers(kcCatScene *this)
{
    LPVOID pvVar1;
    undefined4 uVar2;
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr) // 0x8, 0xc
    {
        if (this->MemoryLines != nullptr) // 0x414
        {
            this->BufferLines = (char *)::GlobalLock(this->MemoryLines); // 0x8, 0x414
        }
        if (this->MemoryOffsets != nullptr) // 0x418
        {
            this->BufferOffsets = (unsigned int *)::GlobalLock(this->MemoryOffsets); // 0xc, 0x418
        }
    }
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr) // 0x8, 0xc
    {
        kcCatScene_UnlockBuffers(this);
        return FALSE;
    }
    return TRUE;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
void __fastcall kcCatScene_UnlockBuffers(kcCatScene *this)
{
    if (this->BufferLines != nullptr) // 0x8
    {
        ::GlobalUnlock(this->MemoryLines); // 0x414
        this->BufferLines = nullptr; // 0x8
    }
    if (this->BufferOffsets != nullptr) // 0xc
    {
        ::GlobalUnlock(this->MemoryOffsets); // 0x418
        this->BufferOffsets = nullptr; // 0xc
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
BOOL __fastcall kcCatScene_IsLocked(kcCatScene *this)
{
    undefined4 uVar1;
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr)
    {
        uVar1 = 0;
    }
    else
    {
        uVar1 = 1;
    }
    return uVar1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
const char * __thiscall kcCatScene_GetLineAt(kcCatScene *this, int index)
{
    int iVar1;
    
    // iVar1 = ;
    if (!kcCatScene_IsLocked(this))
        return nullptr;

    // if ((index < 0) || (*(int *)(this + 4) <= index))
    if (index < 0 || index >= this->LineCount)
        return nullptr;
    
    return &this->BufferLines[this->BufferOffsets[index]];
    // iVar1 = *(int *)(this + 8) + *(int *)(*(int *)(this + 0xc) + index * 4);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
BOOL __thiscall kcCatScene_HasLineAt(kcCatScene *this, int index)
{
    // int iVar1;
    // iVar1 = kcCatScene_GetLineAt(this, index);
    // return (BOOL)(iVar1 != 0);
    return (BOOL)(kcCatScene_GetLineAt(this, index) != nullptr);
}

// I think this function checks for line continuations while copying?
//const char * __thiscall kcCatScene_FUN_00411a30(kcCatScene *this, unsigned short *shortTable, int param_3, IN OUT int *param_4, unsigned int param_5)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned int __thiscall kcCatScene_FUN_00411a30(kcCatScene *this, IN OUT char *inoutBuffer, int bufferAvail, IN OUT int *inoutIndex, unsigned int flags)
{
    // char cVar1;
    //bool notFirstLoop; //bool bVar2;
    char *lineEnd; //char *pcVar3;
    // char *lineStart; //char *local_28;
    // int index; //int local_1c;
    // unsigned int line_len; size_t local_18;
    // unsigned short *wchar_ptr; //char *local_14;
    // int wchar_width; //int local_10;
    const char *line_str; //char *local_c;
    // unsigned short tmp_wchar; //unsigned short local_8;
    
    int index = *inoutIndex;
    char *lineStart = inoutBuffer; //local_28 = inoutBuffer;
    bool notFirstLoop = false; //bVar2 = false;
    while( true )
    {
        lineEnd = lineStart; // local_28 may be "line start"
        // false is probably a preprocessor flag
        // if ((false) || (local_c = kcCatScene_GetLineAt(this, index), local_c == nullptr))
        //     goto LAB_00411c0a; // end func
        if ((false))
            break; //goto LAB_00411c0a; // end func
        line_str = kcCatScene_GetLineAt(this, index);
        if (line_str == nullptr)
            break; //goto LAB_00411c0a; // end func

        // if (*(int *)(this + 0x10) != 0)
        if (this->MacUnk4 != 0)
        {
            this->MacUnk4 = 0;
            // *(undefined4 *)(this + 0x10) = 0;
        }
        if (notFirstLoop)
        {
            while (line_str[0] != '\0' && (unsigned char)line_str[0] < 0x21) // '\0', 0x21
            {
                line_str++; //line_str = line_str + 1;
            }
        }
        int line_len = (int)std::strlen(line_str);
        index++;
        if ((int)line_len >= bufferAvail)
        {
            // these 2 lines removes need for goto statements elsewhere in loop
            std::memcpy(lineStart, line_str, bufferAvail - 1U);
            lineEnd = lineStart + (bufferAvail - 1U);
            break;
        }

        std::memcpy(lineStart, line_str, line_len);
        // local_14 = lineStart;
        lineEnd = lineStart + line_len;
        bufferAvail -= line_len;
        
        // some flags (it's that "allow line comment" flag again!)
        // if ((flags & 1) == 0 || (int)line_len < 1 || lineStart[line_len - 1] != '\\') // no line continuation?
        if ((flags & 1) == 0 || (int)line_len < 1 || lineEnd[-1] != '\\') // no line continuation?
            break; //goto LAB_00411c0a;

        // unsigned short tmp_wchar = *(unsigned short *)lineStart;
        int wchar_width = 0; // this initial value is never used, but the compiler may not realize that
        unsigned short *wchar_ptr = (unsigned short *)lineStart;
        while ((int)line_len > 1)
        {
            // if ((tmp_wchar & 0xff) < 0x80)
            if ((*wchar_ptr & 0xff) < 0x80)
            {
                wchar_width = 1;
            }
            else
            {
                wchar_width = 2;
            }
            line_len -= wchar_width;
            // local_14 += wchar_width;
            if ((int)line_len < 2)
                break;

            wchar_ptr = (unsigned short *)((char *)wchar_ptr + wchar_width);
            // tmp_wchar = *(unsigned short *)wchar_ptr;
        }
        // if (line_len == 0 && wchar_width == 2 && (tmp_wchar >> 8) == (unsigned short)'\\') // 0x5c
        if (line_len == 0 && wchar_width == 2 && (*wchar_ptr >> 8) == (unsigned short)'\\') // 0x5c
        {
            char cVar1 = lineEnd[0];
            lineEnd[0] = '\0';
            // This may be some inlined function, it could explain the random null-termination behavior before and after
            this->MacUnk4 = 1; //*(undefined4 *)(this + 0x10) = 1;
            lineEnd[0] = cVar1;
            break; //goto LAB_00411c0a;
        }
        notFirstLoop = true;
        lineStart = lineEnd - 1; // -1 to skip null-termination
    }

    /// moved to only break in loop
    // std::memcpy(lineStart, line_str, param_3 - 1U);
    // lineEnd = lineStart + (param_3 - 1U);
LAB_00411c0a:
    lineStart = lineEnd;
    *inoutIndex = index;
    lineStart[0] = '\0'; // null-terminate
    ///PTRMATH: string distance
    return (unsigned int)((std::size_t)lineStart - (std::size_t)inoutBuffer); // std::size_t for pointer math
}


#endif
