
#include "common_mc.h"
///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
#ifndef KCLIB_OOP
#undef this
// #ifdef KCLIB_OOP

#include "kcDoubleBuffer.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
kclib::kcStringDictionary::kcStringDictionary()
{
    this->BufferCapacity = 0U;
    this->PairCapacity = 0U;
    this->MemoryBuffer = nullptr;
    this->MemoryOffsets = nullptr;
    this->PairBuffer = nullptr;
    this->Offsets = nullptr;
    this->PairCount = 0;
    this->BufferSize = 0;

    this->AllocExtendBuffers(0x1000, 0x100);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
kclib::kcStringDictionary::~kcStringDictionary()
{
    this->FreeBuffers();
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void kclib::kcStringDictionary::AllocExtendBuffers(unsigned int addBufferSize, unsigned int addOffsetCount)
{
    this->UnlockBuffers();
    //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
    if (addBufferSize != 0U)
    {
        if (this->MemoryBuffer == nullptr)
        {
            this->BufferCapacity = addBufferSize;
            this->MemoryBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->BufferCapacity);
        }
        else
        {
            this->BufferCapacity += addBufferSize; // add to existing size
            this->MemoryBuffer = ::GlobalReAlloc(this->MemoryBuffer, this->BufferCapacity, GMEM_MOVEABLE | GMEM_ZEROINIT);
        }
    }
    if (addOffsetCount != 0U) 
    {
        if (this->MemoryOffsets != nullptr)
        {
            this->PairCapacity = addOffsetCount;
            this->MemoryOffsets = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, this->PairCapacity * sizeof(unsigned int));
        }
        else
        {
            this->PairCapacity += addOffsetCount; // add to existing size
            this->MemoryOffsets = ::GlobalReAlloc(this->MemoryOffsets, this->PairCapacity * sizeof(unsigned int), GMEM_MOVEABLE | GMEM_ZEROINIT);
        }
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void kclib::kcStringDictionary::FreeBuffers()
{
    this->UnlockBuffers();
    if (this->MemoryBuffer != nullptr)
    {
        ::GlobalFree(this->MemoryBuffer);
        this->MemoryBuffer = nullptr;
        this->BufferCapacity = 0U;
    }
    if (this->MemoryOffsets != nullptr)
    {
        ::GlobalFree(this->MemoryOffsets);
        this->MemoryOffsets = nullptr;
        this->PairCapacity = 0U;
    }
    this->PairCount = 0;
    this->BufferSize = 0;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
bool kclib::kcStringDictionary::LockBuffers()
{
    if (this->MemoryBuffer != nullptr && this->PairBuffer == nullptr)
    {
        this->PairBuffer = (char *)::GlobalLock(this->MemoryBuffer);
    }
    if (this->MemoryOffsets != nullptr && this->Offsets == nullptr)
    {
        this->Offsets = (unsigned int *)::GlobalLock(this->MemoryOffsets);
    }
    return (this->PairBuffer != nullptr && this->Offsets != nullptr);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void kclib::kcStringDictionary::UnlockBuffers()
{
    if (this->MemoryBuffer != nullptr && this->PairBuffer != nullptr)
    {
        ::GlobalUnlock(this->MemoryBuffer);
        this->PairBuffer = nullptr;
    }
    if (this->MemoryOffsets != nullptr && this->Offsets != nullptr)
    {
        ::GlobalUnlock(this->MemoryOffsets);
        this->Offsets = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c30
int kclib::kcStringDictionary::GetStringPairLength(IN const char *strBufferPair)
{
    int keyLength = (int)std::strlen(strBufferPair);
    int valueLength = (int)std::strlen(strBufferPair + keyLength + 1); // +1 for null-terminator
    return (keyLength + valueLength + 2); // +2 for both null-terminators
}


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void kclib::kcStringDictionary::SetValue(IN const char *strKey, IN const char *strValue)
{
    // unsigned int sVar1; //size_t sVar1;
    // // unsigned int pairLength; //size_t _Size;
    // int iVar2;
    // char local_100c;
    char tmpBuffer [4096];
    // int index; //int local_8;
    
    // local_8 = 0x41231d; // might be part of removed stack guard
    //                     // (aka, the extra FS_IN_OFFSET, 0xffffffff starting a function, etc)
    //                     // ignore for now...

    // copy strings into temp buffer (not required in recreation, but is optimization)
    std::strcpy(&tmpBuffer[0], strKey);
    unsigned int keyLength = (unsigned int)std::strlen(strKey);
    std::strcpy(&tmpBuffer[keyLength + 1], strValue);
    unsigned int valueLength = (unsigned int)std::strlen(strValue);
    unsigned int pairLength = (keyLength + valueLength + 2);

    int index = this->QuickSortIndex(strKey);

    if (std::strcmp(strKey, &this->PairBuffer[this->Offsets[index]]) == 0)
    {
        // This string key is already in the dicionary, replace it

        // resize existing string pair to fit new string pair
        int str_len = kclib::kcStringDictionary::GetStringPairLength(&this->PairBuffer[this->Offsets[index]]);
        this->ResizePairAt(index, pairLength - str_len);
        std::memcpy(&this->PairBuffer[this->Offsets[index]], &tmpBuffer[0], pairLength);
        // int str_len = kcSmallStruct_GetStringPairLength((char *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + index * 4)));
        // FUN_00412030(this, index, pairLength - str_len);
        // std::memcpy((void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + index * 4)), &tmpBuffer[0], pairLength);
    }
    else
    {
        // This string key is not in the dicionary, insert it (sorted)

        // if (*(int *)((int)this + 4) <= *(int *)((int)this + 0x18))
        if (this->PairCount >= this->PairCapacity)
        {
            this->UnlockBuffers();
            this->AllocExtendBuffers(0, 0x100); // increase capacity of offset table (WE NEED MORE LINES! MORE LINES!)
            this->LockBuffers();
        }

        if (index < this->PairCount)
        {
            // sorted index is not at end of list, we need to move memory to insert it
            std::memmove(&this->Offsets[index + 1], &this->Offsets[index], (this->PairCount - index) * sizeof(unsigned int));
            this->PairCount++; // this incremenent is probably essential before calling ResizePairAt
            this->ResizePairAt(index, pairLength);
            this->Offsets[index] = this->Offsets[index + 1] - pairLength;
            // *(int *)(*(int *)((int)this + 0x14) + index * 4) = *(int *)(*(int *)((int)this + 0x14) + 4 + index * 4) - pairLength;
        }
        else
        {
            while ((int)(this->BufferSize + pairLength) > (int)this->BufferCapacity)
            {
                this->UnlockBuffers();
                this->AllocExtendBuffers(0x1000, 0); // increase capacity of string buffer 
                this->LockBuffers();
            }
            this->Offsets[index] = this->BufferSize; // new entry at the end is at the original buffer size
            this->BufferSize += pairLength; // increase buffer size
            this->PairCount++; // increase entry count
        }
        // finally copy our new pair into the available space
        std::memcpy(&this->PairBuffer[this->Offsets[index]], &tmpBuffer[0], pairLength);
    }
}

//int __thiscall kcSmallStruct_IndexOf(int this,char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412240
int kclib::kcStringDictionary::IndexOf(IN const char *strKey)
{
    int index = this->QuickSortIndex(strKey);
    if (index < 0 || index >= this->PairCount) // *(int *)0x18
        return -1; // outside set, return not found (-1)
    
    if (std::strcmp(strKey, &this->PairBuffer[this->Offsets[index]]) != 0)
        return -1; // not a match, return not found (-1)
    
    return index; // found, return index
}

// function to find index of string by adding (with strcmp sorting)
//int __thiscall kcSmallStruct_QuickSortIndex(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411c70
int kclib::kcStringDictionary::QuickSortIndex(IN const char *strKey)
{
    if (std::strcmp(strKey, &this->PairBuffer[this->Offsets[0]]) < 0)
        return 0; // less than first line

    if (this->PairCount <= 0) // this->PairCount - 1 < 0
        return 1; // way of saying "I am outside the list, and the list is empty, insert away" ??
    
    if (std::strcmp(strKey, &this->PairBuffer[this->Offsets[this->PairCount - 1]]) > 0)
        return this->PairCount; // higher than last line

    // Quicksort with middle pivot:
    int lo = 0;
    int hi = this->PairCount - 1;
    while (lo != hi)
    {
        int mid = (lo + hi) / 2;
        int cmp = std::strcmp(strKey, &this->PairBuffer[this->Offsets[mid]]);
        if (cmp == 0)
            return mid;
        if (hi - lo < 2)
            return hi;
        
        if (cmp < 0)
            hi = mid;
        else
            lo = mid;
    }
    return lo; // equal to mid and hi
}


//void __thiscall kcSmallStruct_GetValue(int this, char *param_2, char *param_3)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004122a0
void kclib::kcStringDictionary::GetValue(OUT char *outBuffer, IN const char *strKey)
{
    int index = this->IndexOf(strKey);
    if (index < 0)
    {
        outBuffer[0] = '\0'; // not found, set outBuffer to empty string
    }
    else
    {
        const char *strBufferPair = &this->PairBuffer[this->Offsets[index]];
        // copy string after key in pair (value string) to outBuffer
        std::strcpy(outBuffer, strBufferPair + std::strlen(strBufferPair) + 1);
    }
    return;//return outBuffer; // (char *)
}


//void kclib::kcStringDictionary::ResizePairAt(int param_1, int param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412030
void kclib::kcStringDictionary::ResizePairAt(int index, int sizeDiff)
{
    // int pairLength; //int iVar1;
    void *_Src;
    // int diffLength;
    
    // if (sizeDiff != 0 && index > -1 && index < *(int *)((int)this + 0x18))
    if (sizeDiff != 0 && index >= 0 && index < this->PairCount)
    {
        // length of current pair at index
        int pairLength = kclib::kcStringDictionary::GetStringPairLength(&this->PairBuffer[this->Offsets[index]]);

        if (sizeDiff < 0) // shrink buffer
        {
            int diffLength = pairLength;
/// SBORROW4(x,y) - true if subtracting the signed numbers would cause a borrow
/// The 4 is the size of both x and y in bytes
/// Returns "true" if there is an arithmetic overflow when subtracting "y" from "x" as signed integers. These are generated particularly by signed integer comparisons. There are rules in place for recovering the original comparison, but this is a missed one special case. These could also conceivably be generated in extended precision subtraction.

            // honestly I have no idea how to handle this weird logic, for now I'll leave a WARNING mark :)
            //  BIG TODO :/

            ///WARNING: unimplemented logic for SBORROW4 in kcStringDictionary::ResizePairAt
            ///FIXME: SBORROW4 macro is a bit hackish
            // #define SBORROW4(x, y) ((((long long)(x) - (long long)(y)) & 0xffffffff00000000LLU) != 0LLU)
            // // if (long long)pairLength
            // if (SBORROW4(pairLength, -sizeDiff) == (pairLength + sizeDiff < 0))
            // #undef SBORROW4
            {
                diffLength = -sizeDiff;
            }

            if (index == this->PairCount - 1)
            {
                this->BufferSize -= diffLength; // no extra work needed, just shrunk capacity, we don't need to move anything
            }
            else
            {
                // TIL memcpy is legal within the same memory region when copying forward :P

                _Src = &this->PairBuffer[this->Offsets[index] + pairLength];
                std::memcpy(_Src - diffLength, _Src, this->BufferSize - (this->Offsets[index] + pairLength));
                // _Src = (void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + index * 4) + pairLength);
                // std::memcpy((void *)((int)_Src - diffLength), _Src, (*(int *)((int)this + 0x10) + *(int *)((int)this + 0x1c)) - (int)_Src);
                // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) - diffLength;
                this->BufferSize -= diffLength;
                
                // subtract (negated) sizeDiff being remove from each offset after index
                for (int i = index + 1; i < this->PairCount; i++)
                {
                    this->Offsets[i] -= diffLength;
                }
                
                // index += 1;
                // // while (index < *(int *)((int)this + 0x18))
                // while (index < this->PairCount)
                // {
                //     this->Offsets[index] -= diffLength;
                //     // *(int *)(*(int *)((int)this + 0x14) + index * 4) = *(int *)(*(int *)((int)this + 0x14) + index * 4) - diffLength;
                //     index += 1;
                // }
            }
        }
        else // expand buffer
        {
            while (this->BufferSize + sizeDiff > this->BufferCapacity)
            {
                this->UnlockBuffers();
                this->AllocExtendBuffers(0x1000, 0);
                this->LockBuffers();
            }

            if (index == this->PairCount)
            {
                this->BufferSize += sizeDiff; // no extra work needed, we have room, and don't need to move anything
            }
            else
            {
                _Src = &this->PairBuffer[this->Offsets[index]];
                std::memmove(_Src + sizeDiff, _Src, this->PairBuffer[this->BufferSize] - this->Offsets[index]);
                this->BufferSize += sizeDiff;
                
                // add sizeDiff being inserted to each offset after index
                for (int i = index + 1; i < this->PairCount; i++)
                {
                    this->Offsets[i] = this->Offsets[i + 1] + sizeDiff;
                }

                // // _Src = (void *)(*(int *)((int)this + 0x10) + *(int *)(*(int *)((int)this + 0x14) + index * 4));
                // // std::memmove((void *)((int)_Src + sizeDiff), _Src, (*(int *)((int)this + 0x10) + *(int *)((int)this + 0x1c)) - (int)_Src);
                // // *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x1c) + sizeDiff;
                // index += 1;
                // // while (index < *(int *)((int)this + 0x18))
                // while (index < this->PairCount)
                // {
                //     this->Offsets[index] = this->Offsets[index + 1] + sizeDiff;
                //     // *(int *)(*(int *)((int)this + 0x14) + index * 4) = *(int *)(*(int *)((int)this + 0x14) + index * 4) + sizeDiff;
                //     index += 1;
                // }
            }
        }
    }
}


#endif
