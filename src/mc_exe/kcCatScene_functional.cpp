
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


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned short * __thiscall kcCatScene_FUN_00411a30(kcCatScene *this, unsigned short *shortTable, int param_3, IN OUT int *param_4, unsigned int param_5)
{
    undefined uVar1;
    bool bVar2;
    unsigned short *puVar3;
    unsigned short *local_28;
    int local_1c;
    size_t local_18;
    unsigned short *local_14;
    int local_10;
    byte *local_c;
    unsigned short local_8;
    
    local_1c = *param_4;
    local_28 = shortTable;
    bVar2 = false;
    while( true )
    {
        puVar3 = local_28;
        if ((false) || (local_c = (byte *)kcCatScene_GetLineAt(this, local_1c), local_c == (byte *)0x0))
            goto LAB_00411c0a;

        if (*(int *)(this + 0x10) != 0)
        {
            *(undefined4 *)(this + 0x10) = 0;
        }
        if (bVar2)
        {
            while ((*local_c != 0 && (*local_c < 0x21)))
            {
                local_c = local_c + 1;
            }
        }
        local_18 = std::strlen((char *)local_c);
        local_1c += 1;
        if (param_3 <= (int)local_18)
            break;
        std::memcpy(local_28,local_c,local_18);
        local_14 = local_28;
        puVar3 = (unsigned short *)((int)local_28 + local_18);
        param_3 -= local_18;
        if ((((param_5 & 1) == 0) || ((int)local_18 < 1)) || (*(char *)((int)puVar3 + -1) != '\\'))
            goto LAB_00411c0a;

        local_8 = *local_28;
        while (1 < (int)local_18)
        {
            if ((local_8 & 0xff) < 0x80)
            {
                local_10 = 1;
            }
            else
            {
                local_10 = 2;
            }
            local_18 -= local_10;
            local_14 = (unsigned short *)((int)local_14 + local_10);
            if ((int)local_18 < 2)
                break;
            local_8 = *local_14;
        }
        if (((local_18 == 0) && (local_10 == 2)) && (local_8 >> 8 == 0x5c))
        {
            uVar1 = *(undefined *)puVar3;
            *(undefined *)puVar3 = 0;
            *(undefined4 *)(this + 0x10) = 1;
            *(undefined *)puVar3 = uVar1;
            goto LAB_00411c0a;
        }
        bVar2 = true;
        local_28 = (unsigned short *)((int)puVar3 + -1);
    }
    std::memcpy(local_28,local_c,param_3 - 1U);
    puVar3 = (unsigned short *)((int)local_28 + (param_3 - 1U));
    LAB_00411c0a:
    local_28 = puVar3;
    *param_4 = local_1c;
    *(undefined *)local_28 = 0;
    return (unsigned short *)((int)local_28 - (int)shortTable);
}


#endif
