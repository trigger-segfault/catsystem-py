
#include "common_mc.h"
#ifdef KCLIB_OOP

#include "kcDoubleBuffer.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
kclib::kcDoubleBuffer::kcDoubleBuffer()
{
    this->SizeA = 0U;
    this->SizeB = 0U;
    this->MemoryA = nullptr;
    this->MemoryB = nullptr;
    this->BufferA = nullptr;
    this->BufferB = nullptr;
    this->SmlUnk6 = 0;
    this->SmlUnk7 = 0;
    // *param_1 = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // param_1[4] = 0;
    // param_1[5] = 0;
    // param_1[6] = 0;
    // param_1[7] = 0;
    this->AllocBuffers(0x1000, 0x100);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
kclib::kcDoubleBuffer::~kcDoubleBuffer()
{
    this->FreeBuffers();
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void kclib::kcDoubleBuffer::AllocBuffers(unsigned int sizeA, unsigned int sizeB)
{
    this->UnlockBuffers();
    if (sizeA != 0U)
    {
        this->SizeA = sizeA;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        if (this->MemoryA == nullptr)
        {
            this->MemoryA = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->SizeA);
            // *(SIZE_T *)this = sizeA;
            // pvVar1 = GlobalAlloc(0x42,*(SIZE_T *)this);
            // *(HGLOBAL *)((int)this + 8) = pvVar1;
        }
        else
        {
            this->MemoryA = ::GlobalReAlloc(this->MemoryA, this->SizeA, GMEM_MOVEABLE | GMEM_ZEROINIT);
            // *(SIZE_T *)this = *(int *)this + sizeA;
            // pvVar1 = GlobalReAlloc(*(HGLOBAL *)((int)this + 8),*(SIZE_T *)this,0x42);
            // *(HGLOBAL *)((int)this + 8) = pvVar1;
        }
    }
    if (sizeB != 0U) 
    {
        this->SizeB = sizeB;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        if (this->MemoryB != nullptr)
        {
            this->MemoryB = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->SizeB);
            // *(SIZE_T *)((int)this + 4) = sizeB;
            // pvVar1 = GlobalAlloc(0x42,*(int *)((int)this + 4) << 2);
            // *(HGLOBAL *)((int)this + 0xc) = pvVar1;
        }
        else
        {
            this->MemoryB = ::GlobalReAlloc(this->MemoryB, this->SizeB, GMEM_MOVEABLE | GMEM_ZEROINIT);
            // *(int *)((int)this + 4) = *(int *)((int)this + 4) + sizeB;
            // pvVar1 = GlobalReAlloc(*(HGLOBAL *)((int)this + 0xc),*(int *)((int)this + 4) << 2,0x42);
            // *(HGLOBAL *)((int)this + 0xc) = pvVar1;
        }
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void kclib::kcDoubleBuffer::FreeBuffers()
{
    this->UnlockBuffers();
    if (this->MemoryA != nullptr)
    {
        ::GlobalFree(this->MemoryA);
        this->MemoryA = nullptr;
        this->SizeA = 0U;
        // param_1[2] = 0;
        // *param_1 = 0;
    }
    if (this->MemoryB != nullptr)
    {
        ::GlobalFree(this->MemoryB);
        // param_1[3] = 0;
        // param_1[1] = 0;
        this->MemoryB = nullptr;
        this->SizeB = 0U;
    }
    // param_1[6] = 0;
    // param_1[7] = 0;
    this->SmlUnk6 = 0;
    this->SmlUnk7 = 0;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
bool kclib::kcDoubleBuffer::LockBuffers()
{
    if (this->MemoryA != nullptr && this->BufferA == nullptr)
    {
        this->BufferA = ::GlobalLock(this->MemoryA);
    }
    if (this->MemoryB != nullptr && this->BufferB == nullptr)
    {
        this->BufferB = ::GlobalLock(this->MemoryB);
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
    if (this->BufferA == nullptr || this->BufferB == nullptr)
    {
        return false;
    }
    return true;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void kclib::kcDoubleBuffer::UnlockBuffers()
{
    if (this->MemoryA != nullptr && this->BufferA != nullptr)
    {
        ::GlobalUnlock(this->MemoryA);
        this->BufferA = nullptr;
    }
    if (this->MemoryB != nullptr && this->BufferB != nullptr)
    {
        ::GlobalUnlock(this->MemoryB);
        this->BufferB = nullptr;
    }
}



/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void kclib::kcDoubleBuffer::kcDoubleBuffer_FUN_00412310(char *strA, char *strB)
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
    local_8 = FUN_00411c70(this, strA);

    ///WARNING: POINTER MATH, ughhhh....

    // iVar2 = std::strcmp(strA, &this->BufferA[this->BufferB]) // ...oh no
    iVar2 = std::strcmp(strA, (char *)(*(int *)((int)this + 0x10) +
                                    *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)));
    if (iVar2 == 0)
    {
        iVar2 = FUN_00411c30((char *)(*(int *)((int)this + 0x10) +
                                    *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)));
        FUN_00412030(this, local_8, _Size - iVar2);
        std::memcpy((void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)
                        ), &tmpBuffer[0], _Size);
    }
    else
    {
        if (*(int *)((int)this + 4) <= *(int *)((int)this + 0x18))
        {
            kcSmallStruct_UnlockBuffers(this);
            kcSmallStruct_AllocBuffers(this, 0, 0x100);
            kcSmallStruct_LockBuffers(this);
        }
        if (local_8 < *(int *)((int)this + 0x18))
        {
            std::memmove((void *)(*(int *)((int)this + 0x14) + 4 + local_8 * 4), 
                    (void *)(*(int *)((int)this + 0x14) + local_8 * 4), 
                    (*(int *)((int)this + 0x18) - local_8) * 4);
            *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
            FUN_00412030(this, local_8, _Size);
            *(int *)(*(int *)((int)this + 0x14) + local_8 * 4) =
                *(int *)(*(int *)((int)this + 0x14) + 4 + local_8 * 4) - _Size;
        }
        else
        {
            while (*(int *)this < (int)(*(int *)((int)this + 0x1c) + _Size))
            {
                kcSmallStruct_UnlockBuffers(this);
                kcSmallStruct_AllocBuffers(this, 0x1000, 0);
                kcSmallStruct_LockBuffers(this);
            }
            *(undefined4 *)(*(int *)((int)this + 0x14) + local_8 * 4) = *(undefined4 *)((int)this + 0x1c);
            *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + _Size;
            *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + 1;
        }
        std::memcpy((void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + local_8 * 4)
                        ), &tmpBuffer[0], _Size);
    }
}


#endif
