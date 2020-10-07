#pragma once

#ifndef KCLIB_KCMEMFILE_H
#define KCLIB_KCMEMFILE_H

#include "ghidra_types_min.h"
#include <stdio.h>
#include <windows.h>


// Attempted quick reverse to start with the script reader text buffering
// but... never finished...


typedef struct kclib_kcMemFile
{
    /*$0,4*/    void      *vftable;
//public:
    /*$10c,4*/  undefined4 Position; //MemUnk67
    /*$110,4*/  void      *MemoryBuffer; //MemUnk68
    /*$114,4*/  undefined4 Size; //MemUnk69
    /*$118,?*/  undefined4 MemUnk70;
    /*$11c,4*/  undefined4 MemUnk71;
} kcMemFile;

typedef struct kclib_unk_SHAREDBUFFER
{
    /*$-4,4*/  undefined4 LpMemUnk_4;

} SHAREDBUFFER;

//void __cdecl kclib_HeapFree(int param_1)
void __cdecl kclib_HeapFree(void* param_1) // lpMem

{
    BOOL BVar1;
    
    if (param_1 != NULL) {
        if (*(int *)(param_1 + -4) < 2) {
            BVar1 = HeapFree(DAT_004c3ef0,0,(int *)(param_1 + -4));
            if (BVar1 == 0) {
                FUN_00407b50();
                return;
            }
        }
            else {
            *(int *)(param_1 + -4) = *(int *)(param_1 + -4) + -1;
        }
    }
    return;

    BOOL BVar1;
    
    if (param_1 != 0) {
        if (*(int *)(param_1 + -4) < 2) {
            BVar1 = HeapFree(DAT_004c3ef0,0,(int *)(param_1 + -4));
            if (BVar1 == 0) {
                FUN_00407b50();
                return;
            }
        }
            else {
            *(int *)(param_1 + -4) = *(int *)(param_1 + -4) + -1;
        }
    }
    return;
}

//uint __thiscall FUN_0040c4b0(void *this, OUT undefined8 *param_1, uint param_2)
unsigned int __thiscall kcMemFile_Read(kcMemFile *this, OUT void *outBuffer, unsigned int bytesCount)

{
    uint uVar1;
    uint uVar2;
    
    if (this->MemoryBuffer == NULL)
    {
        //FUN_00407bc0("kcMemFile : ファイルがオープンされていません");
        ///JP: printf("kcMemFile : ファイルがオープンされていません");
        printf("kcMemFile : File is not open");
    }
    else if (this->Position < this->Size)
    {
        if (this->Position + bytesCount > this->Size)
        {
            bytesCount = this->Size - this->Position;
        }
        memcpy(outBuffer, this->MemoryBuffer + this->Position, bytesCount);
        this->Position += bytesCount;
        return bytesCount;
    }
    return 0xffffffff;
}

int __fastcall FUN_0040c470(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x110);
  if (iVar1 == 0) {
    return iVar1;
  }
  if (*(int *)(param_1 + 0x11c) == 0) {
    kclib_HeapFree(iVar1);
  }
  *(undefined4 *)(param_1 + 0x110) = 0;
  *(undefined4 *)(param_1 + 0x114) = 0;
  return 1;
}

//uint __thiscall FUN_0040c7b0(void *this,undefined8 *param_1,uint param_2)

// undefined4 __thiscall kcMemFile_Write_Resize(void *this,int param_1)
BOOL __thiscall kcMemFile_Write_Resize(kcMemFile *this, unsigned int bytesCount)

{
    unsigned int origSize;
    void *newBuffer;
    

    if (this->MemoryBuffer == NULL)
    {
        return FALSE;
    }
    if (this->Position + bytesCount < this->Size)
    {
        return FALSE;
    }

    origSize = this->Size;
    do
    {
        this->Size = this->Size + this->MemUnk70; // Chunk Size?/ Capacity of sorts?
    } while (this->Position + bytesCount >= this->Size);

    // Allocate new
    newBuffer = FUN_00422a60(0, this->Position); //*(uint *)((int)this + 0x114));

    // Extend to new size?
    FUN_00412740(newBuffer, this->Size); //*(uint *)((int)this + 0x114));
    // Copy original data
    memcpy(newBuffer, this->MemoryBuffer, origSize);
    // Free old
    kclib_HeapFree(this->MemoryBuffer);
    this->MemoryBuffer = newBuffer;
    return TRUE;


    memcpy(puVar2,*(undefined8 **)((int)this + 0x110),uVar1);
    kclib_HeapFree(*(int *)((int)this + 0x110));
    *(undefined8 **)((int)this + 0x110) = puVar2;
    return TRUE;
    
    uint uVar1;
    undefined8 *puVar2;
    
    if (*(int *)((int)this + 0x110) == 0) {
        return FALSE;
    }
    uVar1 = *(uint *)((int)this + 0x114);
    if ((uint)(*(int *)((int)this + 0x10c) + param_1) < uVar1) {
        return FALSE;
    }
    do {
        *(int *)((int)this + 0x114) = *(int *)((int)this + 0x114) + *(int *)((int)this + 0x118);
    } while (*(uint *)((int)this + 0x114) <= (uint)(*(int *)((int)this + 0x10c) + param_1));
    puVar2 = (undefined8 *)FUN_00422a60(0,*(uint *)((int)this + 0x114));
    FUN_00412740(puVar2,*(uint *)((int)this + 0x114));
    memcpy(puVar2,*(undefined8 **)((int)this + 0x110),uVar1);
    kclib_HeapFree(*(int *)((int)this + 0x110));
    *(undefined8 **)((int)this + 0x110) = puVar2;
    return TRUE;
}


unsigned int __thiscall kcMemFile_Write(kcMemFile *this, IN void *inBuffer, unsigned int bytesCount)

{
    int iVar1;
    
    if (this->MemoryBuffer == NULL)
    {
        //FUN_00407bc0("kcMemFile : ファイルがオープンされていません");
        ///JP: printf("kcMemFile : ファイルがオープンされていません");
        printf("kcMemFile : File is not open");
        return 0xffffffff;
    }
    else
    {
        kcMemFile_Write_Resize(this, bytesCount);
        iVar1 = this->Position;
        if (this->Position + bytesCount > this->Size)
        {
            bytesCount = this->Size - this->Position;
        }
        memcpy(this->MemoryBuffer + this->Position, inBuffer, bytesCount);
        this->Position += bytesCount;
        return bytesCount;
    }
    // kcMemFile_Write_Resize(this,param_2);
    // iVar1 = *(int *)((int)this + 0x10c);
    // if (*(uint *)((int)this + 0x114) < iVar1 + param_2) {
    //     param_2 = *(uint *)((int)this + 0x114) - iVar1;
    // }
    // memcpy((undefined8 *)(*(int *)((int)this + 0x110) + iVar1),param_1,param_2);
    // *(int *)((int)this + 0x10c) = *(int *)((int)this + 0x10c) + param_2;
    // return param_2;
}




//int __fastcall FUN_0040c470(int param_1)
BOOL __fastcall kcMemFile_Close(kcMemFile *this)

{
    if (this->MemoryBuffer == NULL)
    {
        return FALSE;
    }
    if (this->MemUnk71 == 0)
    {
        kclib_HeapFree(this->MemoryBuffer);
    }
    // if (*(int *)(param_1 + 0x11c) == 0) {
    //     kclib_HeapFree(iVar1);
    // }
    this->MemoryBuffer = NULL;
    this->Size = 0;
    *(undefined4 *)(param_1 + 0x110) = 0;
    *(undefined4 *)(param_1 + 0x114) = 0;
    return TRUE;

    int iVar1;
    
    iVar1 = *(int *)(param_1 + 0x110);
    if (iVar1 == 0) {
        return FALSE;
    }
    if (*(int *)(param_1 + 0x11c) == 0) {
        kclib_HeapFree(iVar1);
    }
    *(undefined4 *)(param_1 + 0x110) = 0;
    *(undefined4 *)(param_1 + 0x114) = 0;
    return TRUE;
}


#endif /* end include guard */
