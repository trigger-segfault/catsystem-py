#include <windows.h>

#define LOG_A_DIFFERENT_FILE_IS_ALREADY_OPEN "既に違うファイルがオープンされています"
#define LOG_PLEASE_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE "KCFILE_READ 又は KCFILE_WRITE を指定してください"
#define LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ "KCFILE_READ ではアクセス出来ません"
#define LOG_KCFILE_INVALID_HANDLE "kcFile : ハンドルが無効です"
#define LOG_NOT_OPEN_IN_READ_MODE_fmt_s "読み込みモードでオープンされていません : %s"
#define LOG_READ_FILE_FAILED_fmt_d "Read file failed : %d"
#define LOG_NOT_OPEN_IN_WRITE_MODE_fmt_s "書き込みモードでオープンされていません : %s"
#define LOG_WRITE_FILE_FAILED_fmt_d "Write file failed : %d"

typedef int bool32;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
// typedef unsigned long long ulonglong;
typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

//cs2_log_sendmessage_004b57b0
void kclib_logError(const char *cmsg, ...)

{
  // (dummy func)
}



//void __fastcall FUN_00413c80(undefined4 param_1,int param_2,char *param_1_00)
void kcFileBase_logVsprintf(void* param_1, int param_2, char *param_1_00)

{
  _vsprintf((char *)(param_2 + 0xc), param_1_00, &stack0x00000008);
  return;
}



// Used to force crashes?
#define DAT_00000000 *((unsigned char *)0)

typedef struct {


} kcFile;

// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
// HANDLE CreateFileA(
//   LPCSTR                lpFileName,
//   DWORD                 dwDesiredAccess,
//   DWORD                 dwShareMode,
//   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
//   DWORD                 dwCreationDisposition,
//   DWORD                 dwFlagsAndAttributes,
//   HANDLE                hTemplateFile
// );



//void __fastcall FUN_00413430(int param_1)
void FUN_00413430(int param_1)

{
    uint uVar1;
    int iVar2;
    undefined4 uVar3;
    int iVar4;
    undefined4 unaff_EDI;
    int **in_FS_OFFSET;
    undefined auStack292 [276];
    uint local_10;
    int *local_c;
    undefined *puStack8;
    undefined4 local_4;
    
    local_4 = 0xffffffff;
    puStack8 = &LAB_005da33b;
    local_c = *in_FS_OFFSET;
    local_10 = DAT_00640160 ^ (uint)auStack292;
    uVar1 = DAT_00640160 ^ (uint)&stack0xfffffed0;
    *(int ***)in_FS_OFFSET = &local_c;
    kcFile_alloc_unk(uVar1);
    local_4 = 0;
    FUN_00415970(*(undefined4 *)(param_1 + 4));
    iVar2 = new_kcFile(unaff_EDI,0x101);
    if (iVar2 == 0) {
        FUN_00413c80(LOG_FILE_OPEN_FAILED_fmt_s);
        *(undefined4 *)(param_1 + 8) = 1;
        ~kcFile();
    }
    else {
        iVar2 = kcFile_GetSize();
        uVar3 = FUN_00516b1f(iVar2);
        iVar4 = kcFile_Read(uVar3,iVar2);
        if (iVar4 == iVar2) {
            FUN_00413560(iVar2);
            FUN_00517e67(uVar3);
            ~kcFile();
        }
        else {
            FUN_00413c80(LOG_FAILED_TO_READ_FILE_fmt_s);
            *(undefined4 *)(param_1 + 8) = 2;
            ~kcFile();
        }
    }
        *in_FS_OFFSET = local_c;
    __security_check_cookie();
    return;
}


//new_kcFile
//FUN_004b5c80
//undefined4 __thiscall new_kcFile(int param_1,char *param_1_00,unsigned int param_2)
undefined4 new_kcFile(kcFile *this, char *param_1_00,unsigned int param_2)

{
    char cVar1;
    DWORD dwCreationDisposition;
    HANDLE pvVar2;
    DWORD dwDesiredAccess;
    int iVar3;
    unsigned int dwShareMode;
    unsigned int uVar4;
    DWORD dwFlagsAndAttributes;
  
    if (*(int *)(this + 8) != -1) {
        kclib_logError(LOG_A_DIFFERENT_FILE_IS_ALREADY_OPEN);
        DAT_00000000 = 0;
    }
    if ((param_2 & 3) == 0) {
        kclib_logError(LOG_PLEASE_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE);
        DAT_00000000 = 0;
    }
    uVar4 = param_2 & 1;
    if (uVar4 == 0) {
        dwDesiredAccess = 0x40000000;
    }
    else {
        dwDesiredAccess = 0x80000000;
    }
    dwShareMode = (unsigned int)(uVar4 != 0);
    dwCreationDisposition = 0;
    if ((param_2 & 0x100) != 0) {
        dwCreationDisposition = 3;
    }
    if ((param_2 & 0x200) != 0) {
        dwCreationDisposition |= 1;
    }
    if ((param_2 & 0x800) != 0) {
        dwCreationDisposition |= 4;
    }
    if ((param_2 & 0x400) == 0) {
        if (uVar4 == 0) {
            dwFlagsAndAttributes = 0x80;
        }
        else {
            dwFlagsAndAttributes = 0;
        }
        pvVar2 = CreateFileA(param_1_00,dwDesiredAccess,dwShareMode,(LPSECURITY_ATTRIBUTES)0x0,
                             dwCreationDisposition,dwFlagsAndAttributes,(HANDLE)0x0);
    }
    else {
        if (dwDesiredAccess != 0x40000000) {
            kclib_logError(LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ);
            DAT_00000000 = 0;
        }
        pvVar2 = CreateFileA(param_1_00,0x40000000,0,(LPSECURITY_ATTRIBUTES)0x0,1,0x80,(HANDLE)0x0);
        *(HANDLE *)(this + 8) = pvVar2;
        if (pvVar2 != (HANDLE)0xffffffff) goto LAB_004b5d87;
        pvVar2 = CreateFileA(param_1_00,0x40000000,dwShareMode,(LPSECURITY_ATTRIBUTES)0x0,5,0x80,
                             (HANDLE)0x0);
    }
    *(HANDLE *)(this + 8) = pvVar2;
LAB_004b5d87:
    if (*(int *)(this + 8) != -1) {
        iVar3 = (this + 0xc) - (int)param_1_00;
        do {
            cVar1 = *param_1_00;
            param_1_00[iVar3] = cVar1;
            param_1_00 = param_1_00 + 1;
        } while (cVar1 != '\0');
        *(undefined4 *)(this + 0x10c) = 0;
        *(unsigned int *)(this + 4) = param_2;
        return 1;
    }
    return 0;
}

