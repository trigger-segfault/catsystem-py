#pragma once

#ifndef KCLIB_KCMEMFILE_CLASS_H
#define KCLIB_KCMEMFILE_CLASS_H

#include "common.h"
#include "kclib_common.h"
#include "ghidra_types_min.h"
#include <windows.h>


// Attempted quick reverse to start with the script reader text buffering
// but... never finished...


#pragma pack(push, 1)

typedef struct kclib_kcMemFile
{
    /*$0,4*/    void *vftable;
//public:
    /*$4,4*/    unsigned int MemUnk1; // flags? (at least for kcFile)
    /*$8,4*/    unsigned int MemUnk2; // handle? (at least for kcFile)
    /*$c,100*/  char Filename[256]; // <unused> (at least for kcFile)
    /*$10c,4*/  unsigned int Position; //MemUnk67
    /*$110,4*/  void      *MemoryBuffer; //lpMem *(int *)((char *)lpMem - 4) is ref count
    /*$114,4*/  unsigned int Size; //MemUnk69
    /*$118,4*/  undefined4 MemUnk70;
    /*$11c,4*/  int MemUnk71; // reference count?
    /*$120??*/
} kcMemFile;

typedef struct kclib_KCMEM_REF
{
    /*$0,4*/   int RefCount;
    /*$4,0+*/  unsigned char MemData[1];
    /*4+*/
} KCMEM_REF;

#pragma pack(pop)


//uint __thiscall FUN_0040c4b0(void *this, OUT undefined8 *param_1, uint param_2)
///FID:cs2_full_v401/tool/ac.exe: 
unsigned int __thiscall kcMemFile_Read(kcMemFile *this, OUT void *outBuffer, unsigned int bytesCount)

{
    if (this->MemoryBuffer == NULL)
    {
        ///JP: printf("kcMemFile : ファイルがオープンされていません"); //FUN_00407bc0
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


// ///FID:cs2_full_v401/tool/ac.exe: FUN_004141f0
// int * __cdecl memset(int *param_1,uint param_2,uint param_3);
// 
// //void __cdecl kclib_MemZero(undefined8 *param_1,uint param_2)
// ///FID:cs2_full_v401/tool/ac.exe: FUN_00412740
// void __cdecl kclib_MemZero(undefined8 *param_1,uint param_2)
// {
//     int iVar1;
//     if (param_2 != 0) {
//         if (DAT_004c29dc == (void *)0x0) {
//             memset((int *)param_1, 0, param_2);
//             return;
//         }
//         iVar1 = FUN_00405cf0(DAT_004c29dc, (undefined4 *)0x0);
//         if (*(int *)(iVar1 + 0x28) == 0) {
//             if ((param_2 & 3) == 0) {
//                 param_2 >>= 2;
//                 do {
//                     *(undefined4 *)param_1 = 0;
//                     param_1 = (undefined8 *)((int)param_1 + 4);
//                     param_2 -= 1;
//                 } while (param_2 != 0);
//                 return;
//             }
//         }
//         else {
//             if ((param_2 & 7) == 0) {
//                 param_2 >>= 3;
//                 do {
//                     *param_1 = 0;
//                     param_1 = param_1 + 1;
//                     param_2 -= 1;
//                 } while (param_2 != 0);
//                 return;
//             }
//             if ((param_2 & 3) == 0) {
//                 FUN_00412500((undefined4 *)param_1, param_2);
//                 return;
//             }
//         }
//         memset((int *)param_1, 0, param_2);
//     }
//     return;
// }

// undefined4 __thiscall FUN_0040c6f0(void *this,int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_0040c6f0
BOOL __thiscall kcMemFile_Write_Resize(kcMemFile *this, unsigned int bytesCount)

{
    if (this->MemoryBuffer == NULL)
        return FALSE;
    if (this->Position + bytesCount < this->Size)
        return FALSE;

    unsigned int origSize = this->Size;
    do
    {
        this->Size += this->MemUnk70; // Chunk Size?/ Capacity of sorts?
    } while (this->Position + bytesCount >= this->Size);

    void *newBuffer = kclib_HeapAlloc(0, this->Size); // allocate new
    memset(newBuffer, 0, this->Size); // zero new data
    memcpy(newBuffer, this->MemoryBuffer, origSize); // copy original data
    kclib_HeapFree(this->MemoryBuffer); // free old
    this->MemoryBuffer = newBuffer;
    return TRUE;
}

//uint __thiscall FUN_0040c7b0(void *this,undefined8 *param_1,uint param_2)
///FID:cs2_full_v401/tool/ac.exe: FUN_0040c7b0
unsigned int __thiscall kcMemFile_Write(kcMemFile *this, IN void *inBuffer, unsigned int bytesCount)

{
    if (this->MemoryBuffer == NULL)
    {
        ///JP: printf("kcMemFile : ファイルがオープンされていません"); //FUN_00407bc0
        printf("kcMemFile : File is not open");
        return 0xffffffff;
    }

    kcMemFile_Write_Resize(this, bytesCount);
    if (this->Position + bytesCount > this->Size)
    {
        bytesCount = this->Size - this->Position;
    }
    memcpy(this->MemoryBuffer + this->Position, inBuffer, bytesCount);
    this->Position += bytesCount;
    return bytesCount;
}



//int __fastcall FUN_0040c470(int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_0040c470
BOOL __fastcall kcMemFile_Close(kcMemFile *this)

{
    if (this->MemoryBuffer == NULL)
        return FALSE;

    if (this->MemUnk71 == 0)
    {
        kclib_HeapFree(this->MemoryBuffer);
    }
    this->MemoryBuffer = NULL;
    this->Size = 0;
    return TRUE;
}


#endif /* end include guard */
