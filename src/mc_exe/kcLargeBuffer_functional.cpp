
#include "common_mc.h"
#ifndef KCLIB_OOP

#include "kcLargeBuffer_functional.h"
#include "../ac_exe/ghidra_types_min.h"


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
UNK_LARGE_STRUCT * __fastcall kcLargeStruct_ctor(UNK_LARGE_STRUCT *this)
{
    this->StringPoolIndex = 0; //this->LrgUnk_0x10 = 0;
    this->PoolState = POOL_STATE_0; //this->LrgUnk_0x0 = 0;
    this->CurrentStringEntry = nullptr; //this->LrgUnk_0xc0014 = nullptr;
    this->Memory = nullptr;
    this->MemorySize = 0; //this->LrgUnk_0x8 = 0;
    this->MemoryPosition = 0; //this->LrgUnk_0xc = 0;
    return this;
    // param_1[4] = 0;
    // *param_1 = 0;
    // param_1[0x30005] = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // return param_1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131c0
void __fastcall kcLargeStruct_dtor(UNK_LARGE_STRUCT *this)
{
    if (this->Memory != nullptr)
    {
        ::GlobalFree(this->Memory);
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401100
void * __thiscall kcLargeStruct_scalar_dtor(UNK_LARGE_STRUCT *this, int flags)
{
    kcLargeStruct_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

// BOOL __thiscall kcLargeStruct_FUN_00413240(void *this, byte *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413240
BOOL __thiscall kcLargeStruct_FUN_00413240(UNK_LARGE_STRUCT *this, IN const char *str)
{
    BOOL uVar1;
    // int iVar2;
    char *tmp_ptr; //char *local_10;
    // int i; // int local_c;
    const char *str_ptr; //char *local_8;
    
    // if (*(int *)this == 0)
    if (this->PoolState == POOL_STATE_0) // == 0
    {
        int i = 0; // counter index (for loop?) // int local_c;
        str_ptr = str;
        // local_10 = (byte *)((int)this + 0xc01b0);
        tmp_ptr = &this->TempBuffer[0]; //&this->LrgUnk_0xc01b0[0]; // likely a temporary / intermediate buffer
        while (*str_ptr != '\0')
        {
            // if (('\0' < (char)*local_8) && ((char)*local_8 < '!'))
            if (*str_ptr > '\0' && *str_ptr <= ' ') // '!' == 0x21 (so *str_ptr <= ' ', *str_ptr <= 0x20)
            {
                str_ptr++; // ptr++
            }
            if (*str_ptr == '\0')
                break;

            this->StringPointers[i] = tmp_ptr;
            // *(char **)((int)this + local_c * 4 + 0xc0020) = local_10;
            // stop at control char or whitespace
            while ((unsigned char)*str_ptr > (unsigned char)' ') // (unsigned char) cast added for signed char (such a pain in the ass)
            {
                *tmp_ptr = *str_ptr;
                tmp_ptr++;
                str_ptr++;
            }
            *tmp_ptr = '\0';
            tmp_ptr++;
            i++;
        }
        this->LrgUnk_0xc001c = i;
        // *(int *)((int)this + 0xc001c) = local_c;
        while (i < 0x10)
        {
            this->StringPointers[i] = nullptr;
            // *(undefined4 *)((int)this + local_c * 4 + 0xc0020) = 0;
            i++;
        }
        // if (*(int *)((int)this + 0xc001c) < 1)
        if (this->LrgUnk_0xc001c < 1)
        {
            uVar1 = FALSE;
        }
        else
        {
            // int iVar2 = kcLargeStruct_IndexOf(this, *(char **)((int)this + 0xc0020));
            // if (iVar2 < 0) // string not in table
            int str_idx = kcLargeStruct_IndexOf(this, (char *)this->StringPointers[0]);// *(char **)((int)this + 0xc0020));
            if (str_idx < 0) // string not in table
            {
                uVar1 = FALSE;
            }
            else
            {
                // note, pointer not dereferenced, keep as address of
                this->CurrentStringEntry = &this->StringPoolTable[str_idx];
                // this->LrgUnk_0xc0014 = &this->StringPoolTable[str_idx];
                this->LrgUnk_0xc0018 = 0;
                this->PoolState = POOL_STATE_2; // 2

                // *(int *)((int)this + 0xc0014) = (int)this + iVar2 * 0xc + 0x14;
                // *(undefined4 *)((int)this + 0xc0018) = 0;
                // *(undefined4 *)this = 2;

                uVar1 = TRUE;
            }
        }
    }
    else
    {
        uVar1 = FALSE;
    }
    return uVar1;
}

// int __thiscall kcLargeStruct_Add(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004130b0
int __thiscall kcLargeStruct_Add(UNK_LARGE_STRUCT *this, IN const char *str)
{
    // append string function?
    // int iVar1;
    // unsigned int _Size; // size_t
    HGLOBAL hMem;
    
    unsigned int _Size = (unsigned int)std::strlen(str);
    _Size += 1; // +1 for null-termination

    ///TODO: This is a very wonkey control flow (probably courtesy of ghidra, we only need one loop)
    while( true ) // loop for allocating the buffer and then continuing to fit the string in
    {
        while( true ) // keep incrementing buffer size by 0x1000000 until string fits
        {
            // if ((int)(*(int *)(this + 0xc) + _Size) <= *(int *)(this + 8))
            // if ((int)(this->LrgUnk_0xc + _Size) <= this->LrgUnk_0x8)
            if ((int)(this->MemoryPosition + _Size) <= (int)this->MemorySize)
            {
                // This will be buffer position of added string
                int currentPosition = this->MemoryPosition; //iVar1 = this->LrgUnk_0xc;
                std::memcpy(this->Memory + currentPosition, str, _Size); // memcpy, so the null-terminator is included too
                this->MemoryPosition += _Size; //this->LrgUnk_0xc += _Size;
                return currentPosition; // return start position of the added string

                // int iVar1 = *(int *)(this + 0xc);
                // std::memcpy((void *)(*(int *)(this + 4) + iVar1), str, _Size);
                // *(int *)(this + 0xc) = *(int *)(this + 0xc) + _Size;
                // return iVar1;
            }

            // if (*(int *)(this + 4) == 0)
            if (this->Memory == nullptr)
                break; // no memory allocated, break and call GlobalAlloc

            this->MemorySize += 0x1000000; //this->LrgUnk_0x8 += 0x1000000;
            //0x40 (GPTR, GMEM_FIXED | GMEM_ZEROINIT)
            hMem = ::GlobalReAlloc(this->Memory, (SIZE_T)this->MemorySize, GMEM_FIXED | GMEM_ZEROINIT);
            // hMem = ::GlobalReAlloc(this->Memory, (SIZE_T)this->LrgUnk_0x8, GMEM_FIXED | GMEM_ZEROINIT);
            // *(int *)(this + 8) = *(int *)(this + 8) + 0x1000000;
            // hMem = ::GlobalReAlloc(*(HGLOBAL *)(this + 4), *(SIZE_T *)(this + 8), 0x40);
            if (hMem == nullptr)
            {
                this->MemorySize -= 0x1000000; //this->LrgUnk_0x8 -= 0x1000000;
                // *(int *)(this + 8) = *(int *)(this + 8) + -0x1000000;
                return -1;
            }
            this->Memory = hMem;
            // *(HGLOBAL *)(this + 4) = hMem;
        }
        //0x40 (GPTR, GMEM_FIXED | GMEM_ZEROINIT)
        hMem = ::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, 0x1000000); // that's quite a lot of bytes... 0x1000000
        if (hMem == nullptr)
            break; // allocation failed, break and return -1

        this->MemorySize = 0x1000000; //this->LrgUnk_0x8 = 0x1000000;
        this->Memory = hMem;
        this->MemoryPosition = 0; //this->LrgUnk_0xc = 0;
        // *(undefined4 *)(this + 8) = 0x1000000;
        // *(HGLOBAL *)(this + 4) = hMem;
        // *(undefined4 *)(this + 0xc) = 0;
        // this is an infinite while loop, so after allocating a buffer
        //  for the first time, we continue trying to add the string
    }
    return -1;
}

// int __thiscall kcLargeStruct_IndexOf(int this, char *param_2)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413050
int __thiscall kcLargeStruct_IndexOf(UNK_LARGE_STRUCT *this, IN const char *str)
{
    // int i = 0;
    for (int i = 0; i < this->StringPoolIndex; i++)
    {
        // Because the pointer is resolved ==> *(int *)(this + 0x14 + i * 0xc)) and this table is local, we're accessing first field
        // (char *)this->Memory + this->StringPoolTable[i].StringOffset; // 
        // int iVar1 = std::strcmp(((char *)this->Memory + this->StringPoolTable[i].StringOffset), str);
        // int iVar1 = std::strcmp((char *)(*(int *)(this + 4) + *(int *)(this + 0x14 + i * 0xc)), str);
        // if (iVar1 == 0) // strings are equal
        if (std::strcmp(((char *)this->Memory + this->StringPoolTable[i].StringOffset), str) == 0)
        {
            // break;
            return i; // string found
        }
    }
    return -1; // string not found
    
    // int iVar1;
    // int local_c;

    // int local_c = 0;
    // while( true )
    // {
    //     // if (*(int *)(this + 0x10) <= local_c)
    //     // if (this->LrgUnk_0x10 <= local_c)
    //     if (local_c >= this->StringPoolIndex)
    //     {
    //         return -1;
    //     }
    //     // Because the pointer is resolved ==> *(int *)(this + 0x14 + local_c * 0xc)) and this table is local, we're accessing first field
    //     // (char *)this->Memory + this->StringPoolTable[local_c].StringOffset; //
    //     iVar1 = std::strcmp(((char *)this->Memory + this->StringPoolTable[local_c].StringOffset), str);
    //     // iVar1 = std::strcmp((char *)(*(int *)(this + 4) + *(int *)(this + 0x14 + local_c * 0xc)), str);
    //     if (iVar1 == 0) // strings are equal
    //         break;
    //     local_c += 1;
    // }
    // return local_c;
}

// undefined4 __thiscall kcLargeStruct_FUN_00413430(void *this,char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413430
BOOL __thiscall kcLargeStruct_FUN_00413430(UNK_LARGE_STRUCT *this, IN const char *str)
{
    BOOL uVar1;
    int iVar2;
    
    // if (*(int *)this == 0)
    if (this->PoolState == POOL_STATE_0) // == 0
    {
        // if (*(int *)((int)this + 0x10) < 0x10000)
        // if (this->LrgUnk_0x10 < 0x10000)
        if (this->StringPoolIndex < 0x10000) // 0x10000 is count of StringPoolTable
        {
            // iVar2 = kcLargeStruct_IndexOf(this, str);
            // if (iVar2 < 0) // string not in table
            // int str_idx = kcLargeStruct_IndexOf(this, str);
            // if (str_idx < 0) // string not in table
            if (kcLargeStruct_IndexOf(this, str) < 0) // string not in table
            {
                // iVar2 = kcLargeStruct_Add(this, str);
                // if (iVar2 < 0) // failed to add string
                int str_pos = kcLargeStruct_Add(this, str);
                if (str_pos < 0) // failed to add string
                {
                    uVar1 = FALSE;
                }
                else
                {
                    // Assign the string info to the next sub-struct?
                    this->CurrentStringEntry = &this->StringPoolTable[this->StringPoolIndex];
                    this->CurrentStringEntry->StringOffset = str_pos; // SubUnk0 = iVar2;
                    this->CurrentStringEntry->NextStringOffset = -1; //0xffffffff;
                    this->CurrentStringEntry->StringRefCount = 0; //SubUnk2 = 0;
                    this->PoolState = POOL_STATE_1; // 1 // this->LrgUnk_0x0 = 1;

                    // this->LrgUnk_0xc0014 = &this->PoolState + 
                    // *(int *)((int)this + 0xc0014) = (int)this + *(int *)((int)this + 0x10) * 0xc + 0x14;
                    // this->LrgUnk_0xc0014[0] = iVar2; // **(int **)((int)this + 0xc0014)
                    // this->LrgUnk_0xc0014[1] = 0xffffffff; // *(undefined4 *)(*(int *)((int)this + 0xc0014) + 4) = 0xffffffff;
                    // this->LrgUnk_0xc0014[2] = 0; // *(undefined4 *)(*(int *)((int)this + 0xc0014) + 8) = 0;
                    // this->PoolState = 1; // *(undefined4 *)this = 1;

                    // *(int *)((int)this + 0xc0014) = (int)this + *(int *)((int)this + 0x10) * 0xc + 0x14;
                    // **(int **)((int)this + 0xc0014) = iVar2;
                    // *(undefined4 *)(*(int *)((int)this + 0xc0014) + 4) = 0xffffffff;
                    // *(undefined4 *)(*(int *)((int)this + 0xc0014) + 8) = 0;
                    // *(undefined4 *)this = 1;
                    
                    uVar1 = TRUE;
                }
            }
            else
            {
                uVar1 = FALSE;
            }
        }
        else
        {
            uVar1 = FALSE;
        }
    }
    else
    {
        uVar1 = FALSE;
    }
    return uVar1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004133a0
void __thiscall kcLargeStruct_FUN_004133a0(UNK_LARGE_STRUCT *this, IN const char *str)
{
    // no return confirmed
    
    unsigned int str_len = std::strlen(str);
    if (std::strlen(str) != 0 && this->PoolState == POOL_STATE_1 && this->CurrentStringEntry != nullptr)
    {
        int str_pos = kcLargeStruct_Add(this, str);
        if (str_pos >= 0) // > -1
        {
            // oh no... it's pointers all the way down...
            // if (*(int *)(*(int *)((int)this + 0xc0014) + 4) < 0)
            if (this->CurrentStringEntry->NextStringOffset < 0) // unassigned?
            {
                this->CurrentStringEntry->NextStringOffset = str_pos;
                // *(int *)(*(int *)((int)this + 0xc0014) + 4) = iVar2;
            }
            this->CurrentStringEntry->StringRefCount++; //SubUnk2++;
            // this->LrgUnk_0xc0014[2] += 1;
            // this->LrgUnk_0xc0014[2] = this->LrgUnk_0xc0014[2] + 1;
            // *(int *)(*(int *)((int)this + 0xc0014) + 8) = *(int *)(*(int *)((int)this + 0xc0014) + 8) + 1;
        }
    }
    // if ((((sVar1 != 0) && (*(int *)this == 1)) && (*(int *)((int)this + 0xc0014) != 0)) &&
        // (iVar2 = kcLargeStruct_Add(this, str), iVar2 > -1)) // iVar2 is actually used in this case
    // if ((std::strlen(str) != 0 && this->PoolState == POOL_STATE_1 && this->CurrentStringEntry != nullptr) && // *0x0 == 1
    //     (iVar2 = kcLargeStruct_Add(this, str), iVar2 > -1)) // iVar2 is actually used in this case
    // {
    //     // oh no... it's pointers all the way down...
    //     // if (*(int *)(*(int *)((int)this + 0xc0014) + 4) < 0)
    //     if (this->CurrentStringEntry->SubUnk1 < 0)
    //     {
    //         this->CurrentStringEntry->SubUnk1 = iVar2;
    //         // *(int *)(*(int *)((int)this + 0xc0014) + 4) = iVar2;
    //     }
    //     this->CurrentStringEntry->StringRefCount++; //SubUnk2++;
    //     // this->LrgUnk_0xc0014[2] += 1;
    //     // this->LrgUnk_0xc0014[2] = this->LrgUnk_0xc0014[2] + 1;
    //     // *(int *)(*(int *)((int)this + 0xc0014) + 8) = *(int *)(*(int *)((int)this + 0xc0014) + 8) + 1;
    // }

    // // unsigned int sVar1; //size_t sVar1;
    // int iVar2;

    // unsigned int sVar1 = std::strlen(str);
    // // if ((((sVar1 != 0) && (*(int *)this == 1)) && (*(int *)((int)this + 0xc0014) != 0)) &&
    //     // (iVar2 = kcLargeStruct_Add(this, str), iVar2 > -1)) // iVar2 is actually used in this case
    // if ((std::strlen(str) != 0 && this->PoolState == POOL_STATE_1 && this->CurrentStringEntry != nullptr) && // *0x0 == 1
    //     (iVar2 = kcLargeStruct_Add(this, str), iVar2 > -1)) // iVar2 is actually used in this case
    // {
    //     // oh no... it's pointers all the way down...
    //     // if (*(int *)(*(int *)((int)this + 0xc0014) + 4) < 0)
    //     if (this->CurrentStringEntry->SubUnk1 < 0)
    //     {
    //         this->CurrentStringEntry->SubUnk1 = iVar2;
    //         // *(int *)(*(int *)((int)this + 0xc0014) + 4) = iVar2;
    //     }
    //     this->CurrentStringEntry->StringRefCount++; //SubUnk2++;
    //     // this->LrgUnk_0xc0014[2] += 1;
    //     // this->LrgUnk_0xc0014[2] = this->LrgUnk_0xc0014[2] + 1;
    //     // *(int *)(*(int *)((int)this + 0xc0014) + 8) = *(int *)(*(int *)((int)this + 0xc0014) + 8) + 1;
    // }
    // return;
}

// BOOL __fastcall kcLargeStruct_FUN_00412d40(int *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412d40
BOOL __fastcall kcLargeStruct_FUN_00412d40(UNK_LARGE_STRUCT *this)
{
    // is this using THREE_STATE enum?

    // if (*param_1 == 1)
    if (this->PoolState == POOL_STATE_1) // == 1
    {
        // if (*(int *)(param_1[0x30005] + 4) > -1)
        // if (*(int *)((int)this->LrgUnk_0xc0014 + 4) > -1)
        // if (this->LrgUnk_0xc0014[1] > -1)
        if (this->CurrentStringEntry->NextStringOffset >= 0) // > -1
        {
            // This is the only condition observed to change StringPoolIndex
            this->StringPoolIndex++; // this->LrgUnk_0x10++;
            // param_1[4] = param_1[4] + 1;
        }
    }
    else
    {
        // if (*param_1 != 2)
        if (this->PoolState != POOL_STATE_2) // != 2
        {
            return FALSE;
        }
    }
    this->CurrentStringEntry = nullptr; // this->LrgUnk_0xc0014 = nullptr;
    this->PoolState = POOL_STATE_0; // 0 //this->LrgUnk_0x0 = 0;
    // param_1[0x30005] = 0;
    // *param_1 = 0;
    return TRUE;
}

///FIXME:  This function's first parameter is the codepage, this is where we'll add 932, to fix certain locale issues
///WINAPI: BOOL IsDBCSLeadByteEx (UINT, BYTE)
///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-isdbcsleadbyteex>

//size_t __thiscall kcLargeStruct_FUN_00412db0(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412db0
int __thiscall kcLargeStruct_FUN_00412db0(UNK_LARGE_STRUCT *this, char *str)
{
    unsigned int _Size; //size_t _Size;
    // BOOL BVar1;
    // int arg_idx; //int iVar2;
    char *tmp_mem_ptr; //char *local_EDX_312;
    char *str_ptr; //char *local_14;
    int local_c;
    char *mem_ptr; //char *local_8;
    
    // if (*(int *)this == 2 && *(int *)((int)this + 0xc0014) != 0)
    if (this->PoolState == POOL_STATE_2 && this->CurrentStringEntry != nullptr) // == 2 (pool state)
    {
        // if (*(int *)((int)this + 0xc0018) < *(int *)(*(int *)((int)this + 0xc0014) + 8))
        if (this->LrgUnk_0xc0018 < this->CurrentStringEntry->StringRefCount) // *(int *)(*(int *)((int)this + 0xc0014) + 8))
        {
            str_ptr = str;

            mem_ptr = (char *)this->Memory + this->CurrentStringEntry->NextStringOffset;
            local_c = this->LrgUnk_0xc0018;

            // mem_ptr = (char *)(*(int *)((int)this + 4) + *(int *)(*(int *)((int)this + 0xc0014) + 4));
            // local_c = *(int *)((int)this + 0xc0018);
            
            // tmp_mem_ptr = mem_ptr;
            for (; local_c > 0; local_c--)
            {
                _Size = std::strlen(mem_ptr);
                mem_ptr += _Size + 1;
            }
            tmp_mem_ptr = mem_ptr;

            // while (tmp_mem_ptr = mem_ptr, local_c != 0)
            // {
            //     _Size = std::strlen(mem_ptr);
            //     mem_ptr += _Size + 1;
            //     local_c--;
            // }

            while (mem_ptr[0] != '\0')
            {
                if (::IsDBCSLeadByteEx(0, (unsigned char)mem_ptr[0]) && mem_ptr[1] != '\0') // BVar1 == 1, mem_ptr[1] != '\0'
                {
                    // *str_ptr = *mem_ptr;
                    str_ptr[0] = mem_ptr[0];
                    str_ptr[1] = mem_ptr[1];
                    str_ptr += 2;
                    mem_ptr += 2;
                    // str_ptr = str_ptr + 2;
                    // tmp_mem_ptr = mem_ptr + 2;
                }
                // if (*mem_ptr == '%' && mem_ptr[1] > '0' && mem_ptr[1] < ':') // '0','1' in ascii (0x30, 0x31) // ':','9' in ascii (0x39, 0x3a)
                // '%' == 0x25
                // 
                // these have got to be CST macro arguments (0 is just illegal for argument use)
                // macro expansion?
                // 
                // else if (mem_ptr[0] == '%' && mem_ptr[1] > '0' && mem_ptr[1] <= '9')
                else if (mem_ptr[0] == '%' && mem_ptr[1] >= '1' && mem_ptr[1] <= '9')
                {
                    int arg_idx = (int)mem_ptr[1] - (int)'0'; // - 0x30 (value of digit)
                    mem_ptr += 2;
                    // int iVar2 = (int)mem_ptr[1] + -0x30;
                    // tmp_mem_ptr = mem_ptr + 2;
                    // if (iVar2 > 0)
                    if (arg_idx > 0)
                    {
                        // some sort of copying of text inside [] brackets

                        // if (iVar2 < *(int *)((int)this + 0xc001c))
                        if (arg_idx < this->LrgUnk_0xc001c) // possibly macro argument count
                        {
                            // if (*(int *)((int)this + iVar2 * 4 + 0xc0020) != 0)
                            if (this->StringPointers[arg_idx] != nullptr)
                            {
                                // copy macro argument into stringpointer for expansion??

                                _Size = std::strlen(this->StringPointers[arg_idx]);
                                std::memcpy(str_ptr, this->StringPointers[arg_idx], _Size);

                                // _Size = std::strlen(*(char **)((int)this + iVar2 * 4 + 0xc0020));
                                // std::memcpy(str_ptr, *(void **)((int)this + iVar2 * 4 + 0xc0020), _Size);

                                // str_ptr = str_ptr + _Size;
                                str_ptr += _Size;
                            }
                            // if (tmp_mem_ptr[0] == '[')
                            if (mem_ptr[0] == '[')
                            {
                                for (mem_ptr = mem_ptr + 1; mem_ptr[0] != '\0'; mem_ptr++)
                                {
                                    if (mem_ptr[0] == ']')
                                    {
                                        mem_ptr++;
                                        // tmp_mem_ptr = mem_ptr + 1;
                                        break;
                                    }
                                }
                                // // mem_ptr = mem_ptr + 3;
                                // // mem_ptr += 3;
                                // mem_ptr += 1; // because tmp_mem_ptr += 2
                                // // while (tmp_mem_ptr = mem_ptr, mem_ptr[0] != '\0')
                                // while (mem_ptr[0] != '\0')
                                // {
                                //     if (mem_ptr[0] == ']')
                                //     {
                                //         mem_ptr++;
                                //         // tmp_mem_ptr = mem_ptr + 1;
                                //         break;
                                //     }
                                //     mem_ptr++;
                                //     // mem_ptr = mem_ptr + 1;
                                // }
                            }
                        }
                        else
                        {
                            // if (tmp_mem_ptr[0] == '[')
                            if (mem_ptr[0] == '[')
                            {
                                for (mem_ptr = mem_ptr + 1; mem_ptr[0] != '\0'; mem_ptr++, str_ptr++)
                                {
                                    if (mem_ptr[0] == ']')
                                    {
                                        mem_ptr++;
                                        // tmp_mem_ptr = mem_ptr + 1;
                                        break;
                                    }
                                    str_ptr[0] = mem_ptr[0];
                                }
                                // // mem_ptr = mem_ptr + 3;
                                // // mem_ptr += 3;
                                // mem_ptr += 1; // because tmp_mem_ptr += 2
                                // // while (tmp_mem_ptr = mem_ptr, mem_ptr[0] != '\0')
                                // while (mem_ptr[0] != '\0')
                                // {
                                //     if (mem_ptr[0] == ']')
                                //     {
                                //         mem_ptr++;
                                //         // tmp_mem_ptr = mem_ptr + 1;
                                //         break;
                                //     }
                                //     str_ptr[0] = mem_ptr[0];
                                //     str_ptr++;
                                //     mem_ptr++;
                                //     // str_ptr = str_ptr + 1;
                                //     // mem_ptr = mem_ptr + 1;
                                // }
                            }
                        }
                    }
                }
                else
                {
                    str_ptr[0] = mem_ptr[0];
                    str_ptr++;
                    mem_ptr++;
                    // str_ptr = str_ptr + 1;
                    // tmp_mem_ptr = mem_ptr + 1;
                }
            }
            *str_ptr = '\0';

            this->LrgUnk_0xc0018++;
            // *(int *)((int)this + 0xc0018) = *(int *)((int)this + 0xc0018) + 1;

            return (int)std::strlen(str); //_Size = std::strlen(str);
        }
        // else
        // {
        //     return -1; //_Size = 0xffffffff;
        // }
    }
    // else
    // {
    //     return -1; //_Size = 0xffffffff;
    // }
    return -1; //return _Size;
}



#endif
