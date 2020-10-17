
#include "common_mc.h"
#ifdef KCLIB_OOP

#include "kcCatScene.h"
#include "kcFileInfo.h"


//unsigned int * kcMacroReader_initRun(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
kclib::kcCatScene::kcCatScene(IN const char *filename)
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
    this->Read(filename);
    // return this;
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

// undefined4 kclib::kcCatScene::Read(int this,char *filename)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
bool kclib::kcCatScene::Read(IN const char *filename)
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
    kcFileInfo *fileinfo = new kcFileInfo(filename);
    fileinfo->SetReadMode();
    unsigned int fileSize = fileinfo->GetFileSize();
    delete fileinfo;
    // FILE_READER *unkFile = (FILE_READER *)std::malloc(0x598);//_newalloc(0x598);
    // if (unkFile != nullptr)
    // {
    //     unkFile = (FILE_READER *)kcUnkFile_ctor(unkFile, filename);
    // }
    // kcUnkFile_SetReadMode(unkFile);
    // unsigned int fileSize = kcUnkFile_GetFileSize(unkFile);
    // if (unkFile != nullptr)
    // {
    //     kcUnkFile_scalar_dtor(unkFile, 1);
    // }

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
        return false;
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
        return true;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
bool kclib::kcCatScene::LockBuffers()
{
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
        this->UnlockBuffers();
        return false;
    }
    return true;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
void kclib::kcCatScene::UnlockBuffers()
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
bool kclib::kcCatScene::IsLocked()
{
    return (this->BufferLines != nullptr && this->BufferOffsets != nullptr);
    // undefined4 uVar1;
    // if (this->BufferLines == nullptr || this->BufferOffsets == nullptr)
    // {
    //     uVar1 = 0;
    // }
    // else
    // {
    //     uVar1 = 1;
    // }
    // return uVar1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
const char * kclib::kcCatScene::GetLineAt(int index)
{
    // int iVar1 = ;
    if (!this->IsLocked())
        return nullptr;

    // if ((index < 0) || (*(int *)(this + 4) <= index))
    if (index < 0 || index >= this->LineCount)
        return nullptr;
    
    return &this->BufferLines[this->BufferOffsets[index]];
    // iVar1 = *(int *)(this + 8) + *(int *)(*(int *)(this + 0xc) + index * 4);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
bool kclib::kcCatScene::HasLineAt(int index)
{
    // int iVar1;
    // iVar1 = kcCatScene_GetLineAt(this, index);
    // return (BOOL)(iVar1 != 0);
    return (this->GetLineAt(index) != nullptr);
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned short * kclib::kcCatScene::kcCatScene_FUN_00411a30(unsigned short *shortTable, int param_3, IN OUT int *param_4, unsigned int param_5)
{

}


#endif
