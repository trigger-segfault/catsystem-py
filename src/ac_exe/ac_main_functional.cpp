
// functional definition of ac.exe main
// 
// This is very out-of-date from the rest of the files,
// it's sitting in wait of when the rest of the functions are better understood
// Sit tight!

#include "common.h"
#ifndef KCLIB_OOP

#include "TokenParser_functional.h"
#include "kclib_common.h"
#include "kcAnmScript.h"
#include "ghidra_types_min.h"
#include <windows.h>
#include <vector>


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


// There's A LOT of nasty C++ STL (std::vector, std::list) in these functions.
// Recreating using the decompiled code as 'guide',
//  rather than as the desired result may work better


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

#endif
