
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcDoubleBuffer_functional.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
UNK_SMALL_STRUCT * __fastcall kcSmallStruct_ctor(UNK_SMALL_STRUCT *this)
{
    this->StringBufferSize = 0U;
    this->OffsetsBufferCount = 0U;
    this->MemoryStringBuffer = nullptr;
    this->MemoryOffsetsBuffer = nullptr;
    this->SmlStringBuffer = nullptr;
    this->SmlOffsetsTable = nullptr;
    this->OffsetIndex = 0;
    this->BufferOffset = 0;
    // *param_1 = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // param_1[4] = 0;
    // param_1[5] = 0;
    // param_1[6] = 0;
    // param_1[7] = 0;
    kcSmallStruct_AllocBuffers(this, 0x1000, 0x100);
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
void __fastcall kcSmallStruct_dtor(UNK_SMALL_STRUCT *this)
{
    kcSmallStruct_FreeBuffers(this);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010d0
void * __thiscall kcSmallStruct_scalar_dtor(UNK_SMALL_STRUCT *this, int flags)
{
    kcSmallStruct_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void __thiscall kcSmallStruct_AllocBuffers(UNK_SMALL_STRUCT *this, unsigned int newStringBufferSize, unsigned int newOffsetsTableCount)
{
    kcSmallStruct_UnlockBuffers(this);
    if (newStringBufferSize != 0U)
    {
        this->StringBufferSize = newStringBufferSize;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        if (this->MemoryStringBuffer == nullptr)
        {
            this->MemoryStringBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->StringBufferSize);
            // *(SIZE_T *)this = newStringBufferSize;
            // pvVar1 = GlobalAlloc(0x42,*(SIZE_T *)this);
            // *(HGLOBAL *)((int)this + 8) = pvVar1;
        }
        else
        {
            this->MemoryStringBuffer = ::GlobalReAlloc(this->MemoryStringBuffer, this->StringBufferSize, GMEM_MOVEABLE | GMEM_ZEROINIT);
            // *(SIZE_T *)this = *(int *)this + newStringBufferSize;
            // pvVar1 = GlobalReAlloc(*(HGLOBAL *)((int)this + 8),*(SIZE_T *)this,0x42);
            // *(HGLOBAL *)((int)this + 8) = pvVar1;
        }
    }
    if (newOffsetsTableCount != 0U) 
    {
        this->OffsetsBufferCount = newOffsetsTableCount;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        if (this->MemoryOffsetsBuffer != nullptr)
        {
            this->MemoryOffsetsBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->OffsetsBufferCount);
            // *(SIZE_T *)((int)this + 4) = newOffsetsTableCount;
            // pvVar1 = GlobalAlloc(0x42,*(int *)((int)this + 4) << 2);
            // *(HGLOBAL *)((int)this + 0xc) = pvVar1;
        }
        else
        {
            this->MemoryOffsetsBuffer = ::GlobalReAlloc(this->MemoryOffsetsBuffer, this->OffsetsBufferCount, GMEM_MOVEABLE | GMEM_ZEROINIT);
            // *(int *)((int)this + 4) = *(int *)((int)this + 4) + newOffsetsTableCount;
            // pvVar1 = GlobalReAlloc(*(HGLOBAL *)((int)this + 0xc),*(int *)((int)this + 4) << 2,0x42);
            // *(HGLOBAL *)((int)this + 0xc) = pvVar1;
        }
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void __fastcall kcSmallStruct_FreeBuffers(UNK_SMALL_STRUCT *this)
{
    kcSmallStruct_UnlockBuffers(this);
    if (this->MemoryStringBuffer != nullptr)
    {
        ::GlobalFree(this->MemoryStringBuffer);
        this->MemoryStringBuffer = nullptr;
        this->StringBufferSize = 0U;
        // param_1[2] = 0;
        // *param_1 = 0;
    }
    if (this->MemoryOffsetsBuffer != nullptr)
    {
        ::GlobalFree(this->MemoryOffsetsBuffer);
        // param_1[3] = 0;
        // param_1[1] = 0;
        this->MemoryOffsetsBuffer = nullptr;
        this->OffsetsBufferCount = 0U;
    }
    // param_1[6] = 0;
    // param_1[7] = 0;
    this->OffsetIndex = 0;
    this->BufferOffset = 0;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
BOOL __fastcall kcSmallStruct_LockBuffers(UNK_SMALL_STRUCT *this)
{
    if (this->MemoryStringBuffer != nullptr && this->SmlStringBuffer == nullptr)
    {
        this->SmlStringBuffer = (char *)::GlobalLock(this->MemoryStringBuffer);
    }
    if (this->MemoryOffsetsBuffer != nullptr && this->SmlOffsetsTable == nullptr)
    {
        this->SmlOffsetsTable = (unsigned int *)::GlobalLock(this->MemoryOffsetsBuffer);
    }
    // if ((*(int *)(param_1 + 8) != 0) && (*(int *)(param_1 + 0x10) == 0)) {
    //     pvVar1 = GlobalLock(*(HGLOBAL *)(param_1 + 8));
    //     *(LPVOID *)(param_1 + 0x10) = pvVar1;
    // }
    // if ((*(int *)(param_1 + 0xc) != 0) && (*(int *)(param_1 + 0x14) == 0)) {
    //     pvVar1 = GlobalLock(*(HGLOBAL *)(param_1 + 0xc));
    //     *(LPVOID *)(param_1 + 0x14) = pvVar1;
    // }
    // if ((*(int *)(param_1 + 0x14) == 0) || (*(int *)(param_1 + 0x10) == 0))
    if (this->SmlStringBuffer == nullptr || this->SmlOffsetsTable == nullptr)
    {
        return FALSE;
    }
    return TRUE;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void __fastcall kcSmallStruct_UnlockBuffers(UNK_SMALL_STRUCT *this)
{
    if (this->MemoryStringBuffer != nullptr && this->SmlStringBuffer != nullptr)
    {
        ::GlobalUnlock(this->MemoryStringBuffer);
        this->SmlStringBuffer = nullptr;
    }
    if (this->MemoryOffsetsBuffer != nullptr && this->SmlOffsetsTable != nullptr)
    {
        ::GlobalUnlock(this->MemoryOffsetsBuffer);
        this->SmlOffsetsTable = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c30
int kcSmallStruct_TwoStringLength(IN const char *strBuffer)
{
    size_t sVar1;
    size_t sVar2;
    
    sVar1 = std::strlen(strBuffer);
    sVar2 = std::strlen(strBuffer + sVar1 + 1);
    return sVar1 + 2 + sVar2;
}


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void __thiscall kcSmallStruct_FUN_00412310(UNK_SMALL_STRUCT *this, IN const char *strA, IN const char *strB)
{
    size_t sVar1;
    size_t _Size;
    int iVar2;
    // char local_100c;
    char tmpBuffer [4096];
    int local_8;
    
    local_8 = 0x41231d; // might be part of removed stack guard
                        // (aka, the extra FS_IN_OFFSET, 0xffffffff starting a function, etc)
                        // ignore for now...
    std::strcpy(&tmpBuffer[0], strA);
    sVar1 = std::strlen(strA);
    std::strcpy(&tmpBuffer[sVar1 + 1], strB);
    _Size = std::strlen(strB);
    _Size = sVar1 + 2 + _Size;
    local_8 = kcSmallStruct_FUN_00411c70(this, strA);

    ///WARNING: POINTER MATH, ughhhh....

    // iVar2 = std::strcmp(strA, &this->SmlStringBuffer[this->SmlOffsetsTable]) // ...oh no
    iVar2 = std::strcmp(strA, &this->SmlStringBuffer[this->SmlOffsetsTable[local_8]]);
    // iVar2 = std::strcmp(strA, (char *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)));
    if (iVar2 == 0)
    {
        iVar2 = kcSmallStruct_TwoStringLength(&this->SmlStringBuffer[this->SmlOffsetsTable[local_8]]);
        kcSmallStruct_FUN_00412030(this, local_8, _Size - iVar2);
        std::memcpy(&this->SmlStringBuffer[this->SmlOffsetsTable[local_8]], &tmpBuffer[0], _Size);
        // iVar2 = kcSmallStruct_TwoStringLength((char *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)));
        // FUN_00412030(this, local_8, _Size - iVar2);
        // std::memcpy((void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)), &tmpBuffer[0], _Size);
    }
    else
    {
        // if (*(int *)((int)this + 4) <= *(int *)((int)this + 0x18))
        if (this->OffsetsBufferCount <= this->OffsetIndex)
        {
            kcSmallStruct_UnlockBuffers(this);
            kcSmallStruct_AllocBuffers(this, 0, 0x100);
            kcSmallStruct_LockBuffers(this);
        }
        // if (local_8 < *(int *)((int)this + 0x18))
        if (local_8 < this->OffsetIndex)
        {
            // std::memmove((void *)(*(int *)((int)this + 0x14) + 4 + local_8 * 4), (void *)(*(int *)((int)this + 0x14) + local_8 * 4), (*(int *)((int)this + 0x18) - local_8) * 4);
            std::memmove(&this->SmlOffsetsTable[local_8 + 1], &this->SmlOffsetsTable[local_8], (this->OffsetIndex - local_8) * sizeof(unsigned int));
            // *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
            this->OffsetIndex++;
            kcSmallStruct_FUN_00412030(this, local_8, _Size);
            this->SmlOffsetsTable[local_8] = this->SmlOffsetsTable[local_8 + 1] - _Size;
            // *(int *)(*(int *)((int)this + 0x14) + local_8 * 4) = *(int *)(*(int *)((int)this + 0x14) + 4 + local_8 * 4) - _Size;
        }
        else
        {
            while ((int)this->StringBufferSize < (int)(this->BufferOffset + _Size))
            // while (*(int *)this < (int)(*(int *)((int)this + 0x1c) + _Size))
            {
                kcSmallStruct_UnlockBuffers(this);
                kcSmallStruct_AllocBuffers(this, 0x1000, 0);
                kcSmallStruct_LockBuffers(this);
            }
            // *(undefined4 *)(*(int *)((int)this + 0x14) + local_8 * 4) = *(undefined4 *)((int)this + 0x1c);
            // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + _Size;
            // *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
            this->SmlOffsetsTable[local_8] = this->BufferOffset;
            this->BufferOffset += _Size;
            this->OffsetIndex++;
        }
        std::memcpy(&this->SmlStringBuffer[this->SmlOffsetsTable[local_8]], &tmpBuffer[0], _Size);
    }
}

//int __thiscall kcSmallStruct_FUN_00412240(int this,char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412240
int __thiscall kcSmallStruct_FUN_00412240(UNK_SMALL_STRUCT *this, IN const char *str)
{
    int iVar1;
    int iVar2;
    
    iVar1 = kcSmallStruct_FUN_00411c70(this, str);
    // if (iVar1 < 0 || *(int *)(this + 0x18) <= iVar1)
    if (iVar1 < 0 || this->OffsetIndex <= iVar1) // *(int *)0x18
    {
        iVar1 = -1;
    }
    else if (std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[iVar1]]) != 0)
    {
        iVar1 = -1;
    }
    // else
    // {
    //     // iVar2 = std::strcmp(str, (char *)(*(int *)(this + 0x10) + *(int *)(*(int *)(this + 0x14) + iVar1 * 4)));
    //     // if (iVar2 != 0)
    //     // iVar2 = std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[iVar1]]);
    //     if (std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[iVar1]]) != 0)
    //     {
    //         iVar1 = -1;
    //     }
    // }
    return iVar1;
}

//int __thiscall kcSmallStruct_FUN_00411c70(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c70
int __thiscall kcSmallStruct_FUN_00411c70(UNK_SMALL_STRUCT *this, IN const char *str)
{
    int iVar1;
    int iVar2;
    // int local_10;
    // int local_c;
    // int local_8;
    
    int local_c = 0;
    int local_10 = 0;
    // local_8 = *(int *)(this + 0x18) + -1;
    int local_8 = this->OffsetIndex - 1;
    iVar1 = std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[0]]);
    // iVar1 = std::strcmp(str, (char *)(*(int *)(this + 0x10) + **(int **)(this + 0x14)));
    if (iVar1 < 0)
    {
        return 0;
        // local_c = 0;
    }
    else if (local_8 < 0)
    {
        return 1;
        // local_c = 1;
    }
    else
    {
        // iVar2 = std::strcmp(str, (char *)(*(int *)(this + 0x10) + *(int *)(*(int *)(this + 0x14) + local_8 * 4)));
        iVar2 = std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[local_8]]);
        iVar1 = local_10;
        if (iVar2 < 1)
        {
            while (local_10 = iVar1, local_10 != local_8)
            {
                // local_c = local_10 + local_8 >> 1;
                // iVar2 = std::strcmp(str, (char *)(*(int *)(this + 0x10) + *(int *)(*(int *)(this + 0x14) + local_c * 4)));
                local_c = local_10 + local_8 / 2;
                iVar2 = std::strcmp(str, &this->SmlStringBuffer[this->SmlOffsetsTable[local_c]]);
                if (iVar2 == 0)
                {
                    return local_c;
                }
                if (local_8 - local_10 < 2)
                {
                    return local_8;
                }
                iVar1 = local_c;
                if (iVar2 < 0)
                {
                    iVar1 = local_10;
                    local_8 = local_c;
                }
            }
        }
        else
        {
            return this->OffsetIndex;
            // local_c = this->OffsetIndex;
            // local_c = *(int *)(this + 0x18);
        }
    }
    return local_c;
}


//void __thiscall kcSmallStruct_FUN_004122a0(int this, char *param_2, char *param_3)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004122a0
void __thiscall kcSmallStruct_FUN_004122a0(UNK_SMALL_STRUCT *this, OUT char *outBuffer, IN const char *str)
{
    int iVar1;
    size_t sVar2;
    char *_Str;
    
    iVar1 = kcSmallStruct_FUN_00412240(this, str);
    if (iVar1 < 0)
    {
        outBuffer[0] = '\0';
    }
    else
    {
        _Str = &this->SmlStringBuffer[this->SmlOffsetsTable[iVar1]];
        // _Str = (char *)(*(int *)(this + 0x10) + *(int *)(*(int *)(this + 0x14) + iVar1 * 4));
        sVar2 = std::strlen(_Str);
        std::strcpy(outBuffer, _Str + sVar2 + 1);
    }
    return;//return outBuffer; // (char *)
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412030
void __thiscall kcSmallStruct_FUN_00412030(UNK_SMALL_STRUCT *this, int param_1, int param_2)
{
    int iVar1;
    void *_Src;
    int local_18;
    
    // if (param_2 != 0 && param_1 > -1 && param_1 < *(int *)((int)this + 0x18))
    if (param_2 != 0 && param_1 >= 0 && param_1 < this->OffsetIndex)
    {
        iVar1 = kcSmallStruct_TwoStringLength(&this->SmlStringBuffer[this->SmlOffsetsTable[param_1]]);
        // iVar1 = kcSmallStruct_TwoStringLength((char *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + param_1 * 4)));
        if (param_2 < 0)
        {
            local_18 = iVar1;
/// SBORROW4(x,y) - true if subtracting the signed numbers would cause a borrow
/// The 4 is the size of both x and y in bytes
/// Returns "true" if there is an arithmetic overflow when subtracting "y" from "x" as signed integers. These are generated particularly by signed integer comparisons. There are rules in place for recovering the original comparison, but this is a missed one special case. These could also conceivably be generated in extended precision subtraction.
            ///FIXME: SBORROW4 macro is a bit hackish
            #define SBORROW4(x, y) ((((long long)(x) - (long long)(y)) & 0xffffffff00000000LLU) != 0LLU)
            // if (long long)iVar1
            if (SBORROW4(iVar1, -param_2) == (iVar1 + param_2 < 0))
            #undef SBORROW4
            {
                local_18 = -param_2;
            }
            // if (param_1 == *(int *)((int)this + 0x18) + -1)
            if (param_1 == this->OffsetIndex + -1)
            {
                this->BufferOffset -= local_18;
                // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) - local_18;
            }
            else
            {
                _Src = &this->SmlStringBuffer[this->SmlOffsetsTable[param_1] + iVar1];
                std::memcpy(_Src - local_18, _Src, this->BufferOffset - (this->SmlOffsetsTable[param_1] + iVar1));
                // _Src = (void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + param_1 * 4) + iVar1);
                // std::memcpy((void *)((int)_Src - local_18), _Src, (*(int *)((int)this + 0x10) + *(int *)((int)this + 0x1c)) - (int)_Src);
                // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) - local_18;
                this->BufferOffset -= local_18;
                param_1 += 1;
                // while (param_1 < *(int *)((int)this + 0x18))
                while (param_1 < this->OffsetIndex)
                {
                    this->SmlOffsetsTable[param_1] -= local_18;
                    // *(int *)(*(int *)((int)this + 0x14) + param_1 * 4) = *(int *)(*(int *)((int)this + 0x14) + param_1 * 4) - local_18;
                    param_1 += 1;
                }
            }
        }
        else
        {
            // while (*(int *)this < *(int *)((int)this + 0x1c) + param_2)
            while (this->StringBufferSize < this->BufferOffset + param_2)
            {
                kcSmallStruct_UnlockBuffers(this);
                kcSmallStruct_AllocBuffers(this, 0x1000, 0);
                kcSmallStruct_LockBuffers(this);
            }
            // if (param_1 == *(int *)((int)this + 0x18))
            if (param_1 == this->OffsetIndex)
            {
                this->BufferOffset += param_2;
                // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + param_2;
            }
            else
            {
                _Src = &this->SmlStringBuffer[this->SmlOffsetsTable[param_1]];
                std::memmove(_Src + param_2, _Src, this->SmlStringBuffer[this->BufferOffset] - this->SmlOffsetsTable[param_1]);
                this->BufferOffset += param_2;
                // _Src = (void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + param_1 * 4));
                // std::memmove((void *)((int)_Src + param_2), _Src, (*(int *)((int)this + 0x10) + *(int *)((int)this + 0x1c)) - (int)_Src);
                // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + param_2;
                param_1 += 1;
                // while (param_1 < *(int *)((int)this + 0x18))
                while (param_1 < this->OffsetIndex)
                {
                    this->SmlOffsetsTable[param_1] = this->SmlOffsetsTable[param_1 + 1] + param_2;
                    // *(int *)(*(int *)((int)this + 0x14) + param_1 * 4) = *(int *)(*(int *)((int)this + 0x14) + param_1 * 4) + param_2;
                    param_1 += 1;
                }
            }
        }
    }
    return;
}


#endif
