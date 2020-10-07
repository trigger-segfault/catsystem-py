#include "common.h"
#include "kclib_common.h"
#include "token_parse_types.h"
#include "kcAnmScript.h"
#include <windows.h>
#include <vector>
#include <list>


// This is very out-of-date from the rest of the files,
// it's sitting in wait of when the rest of the functions are better understood
// Sit tight!


////// WINAPI USED //////

// DWORD GetFullPathNameA(
//   LPCSTR lpFileName,
//   DWORD  nBufferLength,
//   LPSTR  lpBuffer,
//   LPSTR  *lpFilePart
// );
// <https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfullpathnamea>

///FID:cs2_full_v401/tool/ac.exe: FUN_00405bd0
void __cdecl ac_CoUninitializeCleanup(void)

{
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00405b10
BOOL __cdecl ac_CoInitializeSetup(unsigned int param_1, int param_2)

{
    // This likely sets up some globals of importance, like for detecting memory leaks.
    // 
    // We can see that absolute path with kcBaseClass.h in it, which is likely for an
    // STL std::list<kcBaseClass> or std::vector<kcBaseClass> (probably the first)


    // undefined4 *puVar1;
    // void *this;
    // int iVar2;
    
    // _guard_check_icall();
    // CoInitialize((LPVOID)0x0);
    // if (param_2 != 0) {
    //     puVar1 = (undefined4 *)
    //             FUN_00407810(0x18,"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
    //                         ,0x27);
    //     if (puVar1 != (undefined4 *)0x0) {
    //         FUN_0042efd0(puVar1);
    //     }
    // }
    // if (DAT_004c29dc == 0) {
    //     this = (void *)FUN_00407810(0x580,
    //                             "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
    //                             ,0x27);
    //     if (this != (void *)0x0) {
    //         FUN_004071d0(this,param_1,param_2);
    //     }
    // }
    // FUN_004229b0(0xa00000);
    // if (DAT_004c3430 == 0) {
    //     puVar1 = (undefined4 *)
    //          FUN_00407810(0x13524,
    //                       "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",0x27
    //                      );
    //     if (puVar1 != (undefined4 *)0x0) {
    //     FUN_0040ab40(puVar1);
    //     }
    // }
    // iVar2 = FUN_00407810(1,"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",0x27)
    // ;
    // if (iVar2 != 0) {
    //     FUN_0042e9e0(iVar2);
    // }
    // FUN_00405c90();
    return TRUE;
}



// typedef enum kclib_ANM_CMD
// {
//     CMD_ID    = 0, // [ID] [min] (max)
//     CMD_SET   = 1, // set [var] [min] (max)
//     CMD_LOOP  = 2, // loop [var] [label]
//     CMD_JUMP  = 3, // jump [label]
//     CMD_IF    = 4, // if [var] [label]
//     CMD_IFE   = 5, // ife [var] [value] [label]
//     CMD_IFN   = 6, // ifn [var] [value] [label]
//     CMD_IFG   = 7, // ifg [var] [value] [label]
//     CMD_IFS   = 8, // ifs [var] [value] [label]
//     CMD_IFGE  = 9, // ifge [var] [value] [label]
//     CMD_IFSE  = 10, // ifse [var] [value] [label]
//     CMD_MAX   = 11, // max [var]
//     CMD_BLEND = 12, // blend [param]
//     CMD_DISP  = 13, // disp [param]
//     CMD_POS   = 14, // pos [x] [y]
//     CMD_WAIT  = 15, // wait [min] [max]
//     CMD_ADD   = 16, // add [var] [value]
//     CMD_SUB   = 17  // sub [var] [value]
// } ANM_CMD;

// typedef enum kclib_ANM_VAR_TYPE
// {
//     TYPE_CONST    = 0, // 16
//     TYPE_VARIABLE = 1, // @16
//     TYPE_LABEL    = 2  // MyLabel
// } VAR_TYPE;


// // "ANM\0" (little endian)
// #define MAGIC_ANM 0x4d4e41



// // KCANMSCRIPT STRUCTS:

// #pragma pack(push, 1)

// // (field offset,size prefixes are in hex)

// typedef struct kclib_ANM_ARG
// {
//     /*$0,4*/   VAR_TYPE VarType;
//     /*$4,4*/   unsigned int Value;
//     /*$8*/
// } ANM_ARG;

// typedef struct kclib_ANM_TIMELINE
// {
//     /*$0,4*/   ANM_CMD CmdType;
//     /*$4,40*/  ANM_ARG Args[8];
//     /*$44*/
// } ANM_TIMELINE;


// typedef struct kclib_unk_ANM_UNK72  // likely STL class
// {
//     // /*$0,18*/  char UnkName[24]; // 0x0:0x16?
//     /*$0,18*/  unsigned int padding[6];
//     /*$18,1c*/ int **Field72Unk6ptr;
//     /*$1c,20*/ int Field72Unk7;
//     /*$20*/
// } ANM_UNK72;

// typedef struct kclib_ANM_SCRIPT
// {
//     /*$0,4*/   unsigned int TimelineCount; // Number of timelines
//     // /*$4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
//     /*$4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
//     /*$8,4*/   unsigned int Unk2; // (unknown)
//     /*$c,4*/   unsigned int Unk3; // (unknown)
//     /*$10,4*/  unsigned int Counter; // used in CMD_ID, CMD_WAIT, and during command loop
//     /*$14,4*/  int Instruction; // instruction ptr index
//     /*$18,4*/  unsigned int Wait; // used in CMD_ID, CMD_WAIT
//     /*$1c,4*/  unsigned int MaxFrame; // used in CMD_MAX
//     /*$20,ff*/ int Variables[64]; // live variables
//     /*$120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
//     /*$124,4*/ float Unk73; // (unknown)
//     /*$128,4*/ int FrameID; // current frame
//     /*$12c,4*/ int Blend; // used in CMD_BLEND
//     /*$130,4*/ BOOL Disp; // used in CMD_DISP
//     /*$134,4*/ int PosX; // used in CMD_POS
//     /*$138,4*/ int PosY; // used in CMD_POS
//     /*$140*/
// } ANM_SCRIPT;

// typedef struct kclib_ANM_FILEHEADER
// {
//     /*$0,4*/   unsigned int Magic; // "ANM\0" MAGIC_ANM
//     /*$4,4*/   unsigned int AnmUnk1;
//     /*$8,4*/   unsigned int TimelineCount;
//     /*$c,4*/   unsigned int AnmUnk3;
//     /*$10,4*/  unsigned int AnmUnk4;
//     /*$14,4*/  unsigned int AnmUnk5;
//     /*$18,4*/  unsigned int AnmUnk6;
//     /*$1c,4*/  unsigned int AnmUnk7;
//     /*$20*/
// } ANM_FILEHEADER;

// typedef struct kclib_ANM_FILEINFO
// {
//     /*$0,20*/  ANM_FILEHEADER Header;
//     /*$20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
// } ANM_FILEINFO;

// #pragma pack(pop)



// There's A LOT of nasty C++ STL (std::vector, std::list) in these functions.
// Recreating using the decompiled code as 'guide',
//  rather than as the desired result may work better

///FID:cs2_full_v401/tool/ac.exe: FUN_00402190
void __cdecl ac_parseLines(char *filename)

{


    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        local_604 += 1;
        ///JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        
        local_80c = local_604;
        local_8 = (uint)local_8._1_3_ << 8;
        ScriptDecoder_Close(decoder);
        local_8 = 0xffffffff;
        FUN_004112a0(local_600);
        return;
    }

    int iVar1;
    uint uVar2;
    int *piVar3;
    int iVar4;
    int **in_FS_OFFSET;
    undefined local_854 [8];
    int local_84c [2];
    undefined local_844 [8];
    int local_83c [2];
    undefined local_834 [8];
    undefined local_82c [8];
    int local_824 [2];
    undefined local_81c [8];
    int local_814 [2];
    int local_80c;
    undefined8 local_808 [64];
    int local_604;
    int local_600 [74];
    undefined4 local_4d8 [8];
    undefined4 local_4b8;
    int local_4b4;
    int local_4b0;
    int local_4ac;
    int local_4a8;
    int local_4a4;
    undefined4 local_4a0;
    undefined4 local_49c;
    int local_498;
    undefined4 local_494;
    int local_490;
    undefined4 local_48c;
    int local_488;
    undefined8 local_458;
    int local_450;
    char local_438 [1028];
    uint local_34;
    int local_30;
    int local_2c;
    undefined4 local_28;
    SCRIPT_DECODER decoder; //int local_24 [4];
    int local_14;
    int *local_10;
    undefined *puStack12;
    undefined4 local_8;
  
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_0044fd03;
    // local_10 = *in_FS_OFFSET;
    // local_34 = DAT_004817cc ^ (uint)&stack0xfffffffc;
    // *(int ***)in_FS_OFFSET = &local_10;
    FUN_00410780(local_600);
    local_8 = 0;
    local_28 = 0;
    FUN_00403cf0(&local_4b0);
    FUN_00403cf0(&local_4a8);
    ScriptDecoder_ctor(decoder);
    local_8._0_1_ = 1;
    local_604 = 0;
    FUN_00410870(local_600, 0);
    FUN_00403be0((int)&DAT_004a29bc);
    FUN_00403be0((int)&DAT_004a29a0);
    // open file
    iVar1 = ScriptDecoder_Open(decoder, filename);
    if (iVar1 == 0) {
        local_604 += 1;
        ///JP: printf("ファイルのロードに失敗しました : %s", filename);
        printf("Failed to load file : %s", filename);
        
        local_80c = local_604;
        local_8 = (uint)local_8._1_3_ << 8;
        ScriptDecoder_Close(decoder);
        local_8 = 0xffffffff;
        FUN_004112a0(local_600);
        return;
    }

    // Find all labels/lines
    local_30 = 0;
    local_14 = 1;
    while (uVar2 = ScriptDecoder_IsEOF(decoder), uVar2 == 0)
    {
        ScriptDecoder_NextLine(decoder, (int)local_808);// FUN_00411cf0(decoder, (int)local_808);
        FUN_004107e0(local_600, local_808);
        iVar1 = token_Next(local_600, &local_458);
        if ((iVar1 != 0) && (local_458._4_4_ == 1))
        {
            iVar1 = token_Next(local_600, &local_458);
            if (iVar1 == 0)
            {
                ///JP: printf("error (%d) : 不正なラベル名です。\n", local_14);
                printf("error (%d) : Illegal label name.\n", local_14);
                local_604 += 1;
                break;
            }         
            piVar3 = FUN_00403860(local_814, local_438);
            local_4a8 = *piVar3;
            local_4a4 = piVar3[1];
            piVar3 = (int *)FUN_00403b50(&DAT_004a29a0, local_81c);
            uVar2 = operator!=(&local_4a8, piVar3);
            if ((uVar2 & 0xff) != 0) {
                ///JP: printf("error (%d) : %s このラベル名は既に使用されています。\n", local_14, local_438);
                printf("error (%d) : %s Label name already in use.\n", local_14, local_438);
                local_604 += 1;
                break;
            }
            local_4b8 = 0xffffffff;
            local_4b4 = local_30;
            strcpy_s((char *)local_4d8, 0x20, local_438);
            FUN_00403cb0(&DAT_004a29a0, local_4d8);
            local_30 += 1;
        }
        local_14 += 1;
    }
    // parse commands
    ScriptDecoder_SetPosition(decoder, 0);
    local_30 = 0;
    local_14 = 1;
    while (uVar2 = ScriptDecoder_IsEOF(decoder), uVar2 == 0) {
        ScriptDecoder_NextLine(decoder, (int)local_808);
       
        memset(&local_4a0, 0, sizeof(ANM_TIMELINE)); // FUN_00412740(&local_4a0, 0x44);
        FUN_004107e0(local_600, local_808);
        iVar1 = token_Next(local_600, &local_458);
        if (iVar1 != 0) {
            if (local_458._4_4_ == 1) {
                iVar1 = token_Next(local_600,&local_458);
                if (iVar1 != 0) {
                    piVar3 = FUN_00403860(local_824,local_438);
                    local_4a8 = *piVar3;
                    local_4a4 = piVar3[1];
                    piVar3 = (int *)FUN_00403b50(&DAT_004a29a0,local_82c);
                    uVar2 = operator!=(&local_4a8,piVar3);
                    if ((uVar2 & 0xff) != 0) {
                    iVar1 = FUN_00403d10(&local_4a8);
                    *(int *)(iVar1 + 0x20) = local_30;
                    }
                }
            }
            else {
            if (local_458._4_4_ == 0xd) {
                local_4a0 = 0;
                local_49c = 0;
                local_498 = local_450;
                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                if (iVar1 == 0) {
                printf(s_error_(%d)_:_004513e4,local_14);
                local_604 += 1;
                }
                else {
                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                if (iVar1 == 0) {
                    local_48c = local_494;
                    local_488 = local_490;
                }
                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                local_30 += 1;
                }
            }
            else {
                if (local_458._4_4_ == 0x27) {
                local_4a0 = 0;
                local_49c = 1;
                iVar1 = token_Next(local_600,&local_458);
                if (iVar1 == 0) {
                    printf(s_error_(%d)_:_0045140c,local_14);
                    local_604 += 1;
                }
                else {
                    if ((local_450 < 0x40) && (-1 < local_450)) {
                    local_498 = local_450;
                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                    if (iVar1 == 0) {
                        printf(s_error_(%d)_:_0045146c,local_14);
                        local_604 += 1;
                    }
                    else {
                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                        if (iVar1 == 0) {
                        local_48c = local_494;
                        local_488 = local_490;
                        }
                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                        local_30 += 1;
                    }
                    }
                    else {
                    printf(s_error_(%d)_:_(0_63)_00451430,local_14);
                    local_604 += 1;
                    }
                }
                }
                else {
                iVar1 = stricmp(local_438,"jump");
                if (iVar1 == 0) {
                    local_4a0 = 3;
                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                    if (iVar1 == 0) {
                    printf(s_error_(%d)_:_0045149c,local_14);
                    local_604 += 1;
                    }
                    else {
                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                    local_30 += 1;
                    }
                }
                else {
                    iVar1 = stricmp(local_438,"loop");
                    if (iVar1 == 0) {
                    local_4a0 = 2;
                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                    if (iVar1 == 0) {
                        printf(s_error_(%d)_:_004514cc,local_14);
                        local_604 += 1;
                    }
                    else {
                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                        if (iVar1 == 0) {
                        printf(s_error_(%d)_:_004514f4,local_14);
                        local_604 += 1;
                        }
                        else {
                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                        local_30 += 1;
                        }
                    }
                    }
                    else {
                    iVar1 = stricmp(local_438,"set");
                    if (iVar1 == 0) {
                        local_4a0 = 1;
                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                        if (iVar1 == 0) {
                        printf(s_error_(%d)_:_00451520,local_14);
                        local_604 += 1;
                        }
                        else {
                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                        if (iVar1 == 0) {
                            printf(s_error_(%d)_:_00451548,local_14);
                            local_604 += 1;
                        }
                        else {
                            iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                            if (iVar1 == 0) {
                            local_48c = local_494;
                            local_488 = local_490;
                            }
                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                            local_30 += 1;
                        }
                        }
                    }
                    else {
                        iVar1 = stricmp(local_438,"if");
                        if (iVar1 == 0) {
                        local_4a0 = 4;
                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                        if (iVar1 == 0) {
                            printf(s_error_(%d)_:_00451574,local_14);
                            local_604 += 1;
                        }
                        else {
                            iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                            if (iVar1 == 0) {
                            printf(s_error_(%d)_:_0045159c,local_14);
                            local_604 += 1;
                            }
                            else {
                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                            local_30 += 1;
                            }
                        }
                        }
                        else {
                        iVar1 = stricmp(local_438,"ife");
                        if (iVar1 == 0) {
                            local_4a0 = 5;
                            iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                            if (iVar1 == 0) {
                            printf(s_error_(%d)_:_004515c8,local_14);
                            local_604 += 1;
                            }
                            else {
                            iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                            if (iVar1 == 0) {
                                printf(s_error_(%d)_:_004515f0,local_14);
                                local_604 += 1;
                            }
                            else {
                                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_00451618,local_14);
                                local_604 += 1;
                                }
                                else {
                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                local_30 += 1;
                                }
                            }
                            }
                        }
                        else {
                            iVar1 = stricmp(local_438,"ifn");
                            if (iVar1 == 0) {
                            local_4a0 = 6;
                            iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                            if (iVar1 == 0) {
                                printf(s_error_(%d)_:_00451644,local_14);
                                local_604 += 1;
                            }
                            else {
                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_0045166c,local_14);
                                local_604 += 1;
                                }
                                else {
                                iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451694,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                    local_30 += 1;
                                }
                                }
                            }
                            }
                            else {
                            iVar1 = stricmp(local_438,"ifg");
                            if (iVar1 == 0) {
                                local_4a0 = 7;
                                iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14);
                                if (iVar1 == 0) {
                                printf(s_error_(%d)_:_004516c0,local_14);
                                local_604 += 1;
                                }
                                else {
                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_004516e8,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451710,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                    local_30 += 1;
                                    }
                                }
                                }
                            }
                            else {
                                iVar1 = stricmp(local_438,"ifs");
                                if (iVar1 == 0) {
                                local_4a0 = 8;
                                iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),local_14
                                                    );
                                if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_0045173c,local_14);
                                    local_604 += 1;
                                }
                                else {
                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_00451764,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045178c,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                    }
                                    }
                                }
                                }
                                else {
                                iVar1 = stricmp(local_438,"ifge");
                                if (iVar1 == 0) {
                                    local_4a0 = 9;
                                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                        local_14);
                                    if (iVar1 == 0) {
                                    printf(s_error_(%d)_:_004517bc,local_14);
                                    local_604 += 1;
                                    }
                                    else {
                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_004517e4,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045180c,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                        }
                                    }
                                    }
                                }
                                else {
                                    iVar1 = stricmp(local_438,"ifse");
                                    if (iVar1 == 0) {
                                    local_4a0 = 10;
                                    iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                        local_14);
                                    if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_0045183c,local_14);
                                        local_604 += 1;
                                    }
                                    else {
                                        iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_00451864,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        iVar1 = FUN_004036f0(local_600,&local_494 + 2,local_14);
                                        if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_0045188c,local_14);
                                            local_604 += 1;
                                        }
                                        else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                        }
                                        }
                                    }
                                    }
                                    else {
                                    iVar1 = stricmp(local_438,"max");
                                    if (iVar1 == 0) {
                                        local_4a0 = 0xb;
                                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                            local_14);
                                        if (iVar1 == 0) {
                                        printf(s_error_(%d)_:_004518b8,local_14);
                                        local_604 += 1;
                                        }
                                        else {
                                        FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                        local_30 += 1;
                                        }
                                    }
                                    else {
                                        iVar1 = stricmp(local_438,"blend");
                                        if (iVar1 == 0) {
                                        local_4a0 = 0xc;
                                        iVar1 = FUN_004036f0(local_600,(undefined4 *)(&local_4a0 + 1),
                                                            local_14);
                                        if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_004518e8,local_14);
                                            local_604 += 1;
                                        }
                                        else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                        }
                                        }
                                        else {
                                        iVar1 = stricmp(local_438,"disp");
                                        if (iVar1 == 0) {
                                            local_4a0 = 0xd;
                                            iVar1 = FUN_004036f0(local_600,
                                                                (undefined4 *)(&local_4a0 + 1),local_14
                                                                );
                                            if (iVar1 == 0) {
                                            printf(s_error_(%d)_:_00451918,local_14);
                                            local_604 += 1;
                                            }
                                            else {
                                            FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                            local_30 += 1;
                                            }
                                        }
                                        else {
                                            iVar1 = stricmp(local_438,"pos");
                                            if (iVar1 == 0) {
                                            local_4a0 = 0xe;
                                            iVar1 = FUN_004036f0(local_600,
                                                                (undefined4 *)(&local_4a0 + 1),
                                                                local_14);
                                            if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_00451944,local_14);
                                                local_604 += 1;
                                            }
                                            else {
                                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_0045196c,local_14);
                                                local_604 += 1;
                                                }
                                                else {
                                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                                local_30 += 1;
                                                }
                                            }
                                            }
                                            else {
                                            iVar1 = stricmp(local_438,"wait");
                                            if (iVar1 == 0) {
                                                local_4a0 = 0xf;
                                                iVar1 = FUN_004036f0(local_600,
                                                                    (undefined4 *)(&local_4a0 + 1),
                                                                    local_14);
                                                if (iVar1 == 0) {
                                                printf(s_error_(%d)_:_0045199c,local_14);
                                                local_604 += 1;
                                                }
                                                else {
                                                iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                if (iVar1 == 0) {
                                                    local_494 = local_49c;
                                                    local_490 = local_498;
                                                }
                                                FUN_00403ba0(&DAT_004a29bc,(undefined4 *)&local_4a0);
                                                local_30 += 1;
                                                }
                                            }
                                            else {
                                                iVar1 = stricmp(local_438,"add");
                                                if (iVar1 == 0) {
                                                local_4a0 = 0x10;
                                                iVar1 = FUN_004036f0(local_600,
                                                                    (undefined4 *)(&local_4a0 + 1),
                                                                    local_14);
                                                if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_004519c8,local_14);
                                                    local_604 += 1;
                                                }
                                                else {
                                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14);
                                                    if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_004519f0,local_14);
                                                    local_604 += 1;
                                                    }
                                                    else {
                                                    FUN_00403ba0(&DAT_004a29bc,
                                                                (undefined4 *)&local_4a0);
                                                    local_30 += 1;
                                                    }
                                                }
                                                }
                                                else {
                                                iVar1 = stricmp(local_438,"sub");
                                                if (iVar1 == 0) {
                                                    local_4a0 = 0x11;
                                                    iVar1 = FUN_004036f0(local_600,
                                                                        (undefined4 *)(&local_4a0 + 1),
                                                                        local_14);
                                                    if (iVar1 == 0) {
                                                    printf(s_error_(%d)_:_00451a1c,local_14);
                                                    local_604 += 1;
                                                    }
                                                    else {
                                                    iVar1 = FUN_004036f0(local_600,&local_494,local_14
                                                                        );
                                                    if (iVar1 == 0) {
                                                        printf(s_error_(%d)_:_00451a44,local_14);
                                                        local_604 += 1;
                                                    }
                                                    else {
                                                        FUN_00403ba0(&DAT_004a29bc,
                                                                    (undefined4 *)&local_4a0);
                                                        local_30 += 1;
                                                    }
                                                    }
                                                }
                                                }
                                            }
                                            }
                                        }
                                        }
                                    }
                                    }
                                }
                                }
                            }
                            }
                        }
                        }
                    }
                    }
                }
                }
            }
            }
        }
        local_14 += 1;
        }
        piVar3 = (int *)FUN_00403c80(&DAT_004a29bc,local_834);
        local_4b0 = *piVar3;
        local_4ac = piVar3[1];
        while( true ) {
        piVar3 = (int *)FUN_00403b50(&DAT_004a29bc,local_844);
        uVar2 = operator!=(&local_4b0,piVar3);
        if ((uVar2 & 0xff) == 0) break;
        local_2c = 0;
        while (local_2c < 8) {
            iVar1 = FUN_00403d10(&local_4b0);
            if (*(int *)(iVar1 + 4 + local_2c * 8) == 2) {
            iVar1 = FUN_00403d10(&local_4b0);
            piVar3 = FUN_00403900(local_84c,*(int *)(iVar1 + 8 + local_2c * 8));
            local_4a8 = *piVar3;
            local_4a4 = piVar3[1];
            piVar3 = (int *)FUN_00403b50(&DAT_004a29a0,local_854);
            uVar2 = operator!=(&local_4a8,piVar3);
            if ((uVar2 & 0xff) == 0) {
                printf(s_error_:_00451a6c);
                break;
            }
            iVar1 = FUN_00403d10(&local_4b0);
            *(undefined4 *)(iVar1 + 4 + local_2c * 8) = 0;
            iVar1 = FUN_00403d10(&local_4a8);
            iVar4 = FUN_00403d10(&local_4b0);
            *(undefined4 *)(iVar4 + 8 + local_2c * 8) = *(undefined4 *)(iVar1 + 0x20);
            }
            local_2c += 1;
        }
        FUN_00403d30(&local_4b0,local_83c);
    }
    local_8 = (uint)local_8._1_3_ << 8;
    ScriptDecoder_Close(decoder);
    local_8 = 0xffffffff;
    FUN_004112a0(local_600);
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00403990
void __cdecl ac_WriteAnmFile(char *filename)

{
    FILE *file = fopen(filename, "wb+");
    if (file == NULL)
    {
        ///JP: printf("出力ファイル %s がオープンできません。\n", filename); //, uVar1
        printf("Output file %s cannot be opened.\n", filename);
        return;
    }
    std::vector<ANM_TIMELINE> timelines;
    ANM_FILEHEADER anmhdr = {0};
    anmhdr.Magic = MAGIC_ANM; // "ANM\0"
    anmhdr.AnmUnk1 = 0;
    anmhdr.TimelineCount = (unsigned int) timelines.size();
    fwrite((void *)&anmhdr, sizeof(anmhdr), 1, file);

    for (int i = 0; i < anmhdr.TimelineCount; i++)
    {
        const ANM_TIMELINE &anmline = timelines[i];

        fwrite((void *)&anmline, sizeof(ANM_TIMELINE), 1, file);
    }

    fclose(file);

    //unsigned int anmhdr[8];
    anmhdr[0] = MAGIC_ANM; // "ANM\0"
    anmhdr[1] = 0;
    anmhdr[2] = TimelineCount; //FUN_00412740((undefined8 *)&local_150,0x20);
    fwrite((void *)&anmhdr[0], sizeof(anmhdr), 1, file);
    // fwrite("ANM\0", 4, 1, file);
    // fwrite("ANM\0", 4, 1, file);



    fclose(file);
    uint uVar1;
    BOOL BVar2;
    int *piVar3;
    LPCVOID buffer;
    int iVar4;
    undefined4 *puVar5;
    undefined4 *puVar6;
    int **in_FS_OFFSET;
    DWORD bytesCount;
    undefined local_1b4 [8];
    int local_1ac [2];
    undefined local_1a4 [8];
    undefined4 local_19c;
    undefined4 local_198 [18];
    undefined4 local_150;
    undefined4 local_14c;
    undefined4 local_148;
    undefined4 kcfile [69];
    uint local_1c;
    int local_18;
    int local_14;
    int *local_10;
    undefined *puStack12;
    undefined4 local_8;
    
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_0044fd36;
    // local_10 = *in_FS_OFFSET;
    // uVar1 = DAT_004817cc ^ (uint)&stack0xfffffffc;
    // *(int ***)in_FS_OFFSET = &local_10;
    local_1c = uVar1;
    kcFile_init(kcfile);
    local_8 = 0;
    FUN_00403cf0(&local_18);
    BVar2 = kcFile_Open(kcfile,filename,0x402);
    if (BVar2 == 0) {
        ///JP: printf("出力ファイル %s がオープンできません。\n", filename, uVar1);
        printf("Output file %s cannot be opened.\n", filename, uVar1);
        local_19c = 0;
        local_8 = 0xffffffff;
        kcFile_dtor(kcfile);
    }
    else {
        unsigned int hdr[8];
        memset(&hdr[0], 0, sizeof(hdr));
        hdr[0] = MAGIC_ANM; // "ANM\0"
        hdr[1] = 0;
        hdr[2] = TimelineCount; //FUN_00412740((undefined8 *)&local_150,0x20);
        DWORD bytesWritten;
        WriteFile(hFile, &hdr[0], sizeof(hdr), &bytesWritten, NULL);
        //memset()
        FUN_00412740((undefined8 *)&local_150,0x20);
        local_150 = MAGIC_ANM; // "ANM\0"
        local_14c = 0;
        local_148 = FUN_00403b80((int)&DAT_004a29bc);
        kcFile_Write(kcfile,&local_150,0x20);
        piVar3 = (int *)FUN_00403c80(&DAT_004a29bc,local_1a4);
        local_18 = *piVar3;
        local_14 = piVar3[1];
        while( true ) {
            piVar3 = (int *)FUN_00403b50(&DAT_004a29bc,local_1b4);
            uVar1 = operator!=(&local_18,piVar3);
            if ((uVar1 & 0xff) == 0) break;
            puVar5 = (undefined4 *)FUN_00403fe0(&local_18);
            iVar4 = 0x11;
            puVar6 = local_198;
            while (iVar4 != 0) {
                iVar4 += -1;
                *puVar6 = *puVar5;
                puVar5 = puVar5 + 1;
                puVar6 = puVar6 + 1;
            }
            bytesCount = 0x44;
            buffer = (LPCVOID)FUN_00403fe0(&local_18);
            kcFile_Write(kcfile,buffer,bytesCount);
            FUN_00403d30(&local_18,local_1ac);
        }
        printf("anm out -> %s", filename);
        kcFile_Close((int)kcfile);
        local_8 = 0xffffffff;
        kcFile_dtor(kcfile);
    }
    // *in_FS_OFFSET = local_10;
    // FUN_00413008();
    return;
}

///FID:cs2_full_v401/tool/ac.exe: FUN_00401fe0
int main(int argc, char** argv)

{
    if (argc < 0)
    {
        ///JP: printf("アニメーションスクリプトコンバータ\n");
        printf("Animation script converter\n");
        printf("usage : ac [file] [file] ...\n");
        return 1;
    }

    ac_CoInitializeSetup(0x7f, 0); //FUN_00405b10(0x7f,0); CoInitialize and setup
    for (int i = 1; i < argc; i++)
    {
        char argFullpath[MAX_PATH + 4]; //char local_35c [264];
        char argParentdir[MAX_PATH + 4]; //char local_254 [264];
        shiftjis_GetAbsolutePath(argv[i], argFullpath, NULL); // FUN_00411ff0(argv[i], local_35c, (LPSTR *)0x0);
        shiftjis_GetParentDirectory(argFullpath, argParentdir); // FUN_00412520(local_35c, local_254);

        _WIN32_FIND_DATAA findData;
        HANDLE hFindFile = FindFirstFileA(argv[i], (LPWIN32_FIND_DATAA)&findData);
        if (hFindFile == INVALID_HANDLE_VALUE)
        {
            ///JP: printf("ファイルが見つかりません : %s\n", argv[i]);
            printf("File not found : %s\n", argv[i]);
            continue;
        }
        
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                int iVar1 = ac_parseLines(argFullpath);
                if (iVar1 == 0)
                {
                    sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                    shiftjis_ChangeExtension(argFullpath, "anm"); //FUN_004120e0(argFullpath,(undefined4 *)"anm");
                    ac_WriteAnmFile(argFullpath); //FUN_00403990(argFullpath);
                }
            }
        } while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&findData));

        FindClose(hFindFile);
    }
    ac_CoUninitializeCleanup();
    return 0;
}
