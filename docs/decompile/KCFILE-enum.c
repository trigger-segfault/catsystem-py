#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// typedef enum {
//     CREATE_NEW = 1,
//     CREATE_ALWAYS = 2,
//     OPEN_EXISTING = 3,
//     OPEN_ALWAYS = 4,
//     TRUNCATE_EXISTING = 5,
// } CREATE_FILE; //dwCreationDisposition

// typedef enum {
//     GENERIC_READ = 0x80000000,
//     GENERIC_WRITE = 0x40000000,
// } FILE_ACCESS; // ACCESS_MASK


// typedef enum {
//     FILE_SHARE_READ = 0x00000001,
//     FILE_SHARE_WRITE = 0x00000002,
//     FILE_SHARE_DELETE = 0x00000004,
// } FILE_SHARE;

// typedef enum {
//     FILE_ATTRIBUTE_READONLY = 0x1,
//     FILE_ATTRIBUTE_DIRECTORY = 0x10,
//     FILE_ATTRIBUTE_NORMAL = 0x80,
//     FILE_ATTRIBUTE_TEMPORARY = 0x100,
// } FILE_ATTRIBUTES;


typedef enum {
    // file access (specify one, names have been confirmed)
    KCFILE_READ  = 0x1, //GENERIC_READ, FILE_SHARE_READ
    KCFILE_WRITE = 0x2, //GENERIC_WRITE, 0
    _KCFILE_READWRITE = 0x3, //(helper enum, not used)

    // creation disposition (specify one, names are unconfirmed)
    KCFILE_OPEN     = 0x100, //OPEN_EXISTING
    KCFILE_NEW      = 0x200, //CREATE_NEW
    KCFILE_TRUNCATE = 0x400, //CREATE_NEW then TRUNCATE_EXISTING, requires KCFILE_WRITE
    KCFILE_CREATE   = 0x800, //OPEN_ALWAYS

    // common flag usages
    // _KCFILE_OPEN_READ = KCFILE_READ | KCFILE_OPEN,
    // _KCFILE_OVERWRITE_WRITE = KCFILE_WRITE | KCFILE_OVERWRITE,
} KCFILE_MODE;



#define DAT_00000000 *((unsigned char *)0)
typedef unsigned int undefined4;
typedef unsigned int uint;

// "A different file is already open"
#define LOG_A_DIFFERENT_FILE_IS_ALREADY_OPEN "既に違うファイルがオープンされています"

// "Specify KCFILE_READ or KCFILE_WRITE"
#define LOG_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE "KCFILE_READ 又は KCFILE_WRITE を指定してください"

// "Not accessible with KCFILE_READ"
#define LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ "KCFILE_READ ではアクセス出来ません"


// KCLIB FUNCTIONS:

void kclib_logError(LPCSTR message, ...)

{
  // (dummy func)
}


//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
// HANDLE CreateFileA(
//   LPCSTR                lpFileName,
//   DWORD                 dwDesiredAccess,
//   DWORD                 dwShareMode,
//   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
//   DWORD                 dwCreationDisposition,
//   DWORD                 dwFlagsAndAttributes,
//   HANDLE                hTemplateFile
// );

typedef struct {
    /*0,4*/   undefined4 reserved0; //vftable?
    /*4,4*/   unsigned int Mode; //KCFILE_MODE
    /*8,4*/   HANDLE Handle;
    /*c,100*/ char Name[256];
    /*10c,4*/ unsigned int Position;
    //unsigned int Size;
} kcFile;

// cs2_full_v401:MakeInt.exe (kcFile vftable[1])
//undefined4 __thiscall FUN_00402050(kcFile *this,char *param_1,uint param_2)
BOOL __thiscall kcFile_Open(kcFile *this, char *filename, unsigned int mode)

{
    DWORD dwDesiredAccess;
    DWORD dwShareMode;
    DWORD dwCreationDisposition;
    DWORD dwFlagsAndAttributes;
    HANDLE hFile;
    
    if (this->Handle != INVALID_HANDLE_VALUE) {
        kclib_logError(LOG_A_DIFFERENT_FILE_IS_ALREADY_OPEN);
        DAT_00000000 = 0; //crash self?
    }
    
    if (mode & KCFILE_READ) {
        dwDesiredAccess = GENERIC_READ; //0x80000000;
        dwShareMode = FILE_SHARE_READ;
    }
    else if (mode & KCFILE_WRITE) {
        dwDesiredAccess = GENERIC_WRITE; //0x40000000;
        dwShareMode = 0;
    }
    else {
        kclib_logError(LOG_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE);
        DAT_00000000 = 0; //crash self?
    }

    dwCreationDisposition = 0; //0;
    if ((mode & KCFILE_OPEN) != 0) {
        dwCreationDisposition |= OPEN_EXISTING; // actually =
    }
    if ((mode & KCFILE_NEW) != 0) {
        dwCreationDisposition |= CREATE_NEW;
    }
    if ((mode & KCFILE_CREATE) != 0) {
        dwCreationDisposition |= OPEN_ALWAYS;
    }
    
    dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    if (mode & KCFILE_TRUNCATE) {
        if (dwDesiredAccess != GENERIC_WRITE) {
            kclib_logError(LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ);
            DAT_00000000 = 0; //crash self?
        }
        hFile = CreateFileA(filename, dwDesiredAccess, 0, (LPSECURITY_ATTRIBUTES)NULL,
                            CREATE_NEW, dwFlagsAndAttributes, (HANDLE)NULL);
        
        if (hFile == INVALID_HANDLE_VALUE) {
            hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, (LPSECURITY_ATTRIBUTES)NULL,
                                TRUNCATE_EXISTING, dwFlagsAndAttributes, (HANDLE)NULL);
        }
    }
    else {
        if (mode & KCFILE_READ) {
            dwFlagsAndAttributes = 0;
        }
        hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, (LPSECURITY_ATTRIBUTES)NULL,
                            dwCreationDisposition, dwFlagsAndAttributes, (HANDLE)NULL);
    }
    this->Handle = hFile;
    if (this->Handle != INVALID_HANDLE_VALUE) {
        strcpy(&this->Name[0], &filename[0]);
        this->Position = 0U;
        this->Mode = mode;
        return TRUE;
    }
    return FALSE;

    // if ((mode & (KCFILE_READ | KCFILE_WRITE)) == 0) {
    //     kclib_logError(LOG_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE);
    //     DAT_00000000 = 0;
    // }
    //uVar4 = mode & 1;
    // //if (uVar4 == 0) {
    // if ((mode & KCFILE_READ) == 0) {
    //     dwDesiredAccess = GENERIC_WRITE; //0x40000000;
    // }
    // else {
    //     dwDesiredAccess = GENERIC_READ; //0x80000000;
    // }
    // //dwShareMode = (uint)(uVar4 != 0);
    // if ((mode & KCFILE_READ) != 0) {
    //     dwShareMode = FILE_SHARE_READ;
    // }
    // else {
    //     dwShareMode = 0;
    // }
    dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL; //0x80;
    dwCreationDisposition = 0; //0;
    if (mode & KCFILE_OPEN)
        dwCreationDisposition |= OPEN_EXISTING; // actually =
    if (mode & KCFILE_NEW)
        dwCreationDisposition |= CREATE_NEW;
    if (mode & KCFILE_CREATE)
        dwCreationDisposition |= OPEN_ALWAYS;

    if ((mode & KCFILE_OPEN) != 0) {
        dwCreationDisposition |= OPEN_EXISTING; // actually =
    }
    if ((mode & KCFILE_NEW) != 0) {
        dwCreationDisposition |= CREATE_NEW;
    }
    if ((mode & KCFILE_CREATE) != 0) {
        dwCreationDisposition |= OPEN_ALWAYS;
    }

    dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL; //0x80;
    dwCreationDisposition = 0; //0;
    //if ((mode & 0x100) != 0) { // = 3;
    //if ((mode & 0x200) != 0) { // |= 1;
    //if ((mode & 0x800) != 0) { // |= 4;
    //if ((mode & 0x400) == 0) {
    if ((mode & KCFILE_OPEN) != 0) {
        dwCreationDisposition = OPEN_EXISTING; //3;
    }
    if ((mode & KCFILE_NEW) != 0) {
        dwCreationDisposition |= CREATE_NEW; //1;
    }
    if ((mode & KCFILE_CREATE) != 0) {
        dwCreationDisposition |= OPEN_ALWAYS; //4;
    }
    if ((mode & KCFILE_TRUNCATE) != 0) {
        if ((mode & KCFILE_WRITE) != 0) {
            kclib_logError(LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ);
            DAT_00000000 = 0;
        }
        hFile = CreateFileA(filename, GENERIC_WRITE, 0, (LPSECURITY_ATTRIBUTES)NULL,
                            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
        
        if (hFile == INVALID_HANDLE_VALUE) {
            hFile = CreateFileA(filename, GENERIC_WRITE, dwShareMode, (LPSECURITY_ATTRIBUTES)NULL,
                                TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
        }
    }
    else {
        if ((mode & KCFILE_READ) != 0) {
            dwFlagsAndAttributes = 0;
        }
        hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, (LPSECURITY_ATTRIBUTES)NULL,
                            dwCreationDisposition, dwFlagsAndAttributes, (HANDLE)NULL);
    }
    this->Handle = hFile;
    
    if (this->Handle != INVALID_HANDLE_VALUE) {
        strcpy(&this->Name[0], &filename[0]);//, 256);
        this->Position = 0U;
        this->Mode = mode;
        return TRUE;
    }
    return FALSE;
}


// cs2_full_v401:MakeInt.exe (kcFile vftable[1])
// undefined4 __thiscall FUN_00402050(void *this,char *param_1,uint param_2)

// {
//     char cVar1;
//     int iVar2;
//     DWORD dwCreationDisposition;
//     HANDLE pvVar3;
//     DWORD dwDesiredAccess;
//     uint dwShareMode;
//     uint uVar4;
//     DWORD dwFlagsAndAttributes;
    
//     if (*(int *)((int)this + 8) != -1) {
//         kclib_logError(LOG_A_DIFFERENT_FILE_IS_ALREADY_OPEN);
//         DAT_00000000 = 0;
//     }
//     if ((param_2 & 3) == 0) {
//         kclib_logError(LOG_SPECIFY_KCFILE_READ_OR_KCFILE_WRITE);
//         DAT_00000000 = 0;
//     }
//     uVar4 = param_2 & 1;
//     if (uVar4 == 0) {
//         dwDesiredAccess = 0x40000000;
//     }
//     else {
//         dwDesiredAccess = 0x80000000;
//     }
//     dwShareMode = (uint)(uVar4 != 0);
//     dwCreationDisposition = 0;
//     if ((param_2 & 0x100) != 0) {
//         dwCreationDisposition = 3;
//     }
//     if ((param_2 & 0x200) != 0) {
//         dwCreationDisposition |= 1;
//     }
//     if ((param_2 & 0x800) != 0) {
//         dwCreationDisposition |= 4;
//     }
//     if ((param_2 & 0x400) == 0) {
//         if (uVar4 == 0) {
//             dwFlagsAndAttributes = 0x80;
//         }
//         else {
//             dwFlagsAndAttributes = 0;
//         }
//         pvVar3 = CreateFileA(param_1,dwDesiredAccess,dwShareMode,(LPSECURITY_ATTRIBUTES)0x0,
//                              dwCreationDisposition,dwFlagsAndAttributes,(HANDLE)0x0);
//     }
//     else {
//         if (dwDesiredAccess != 0x40000000) {
//             kclib_logError(LOG_NOT_ACCESSIBLE_WITH_KCFILE_READ);
//             DAT_00000000 = 0;
//         }
//         pvVar3 = CreateFileA(param_1,0x40000000,0,(LPSECURITY_ATTRIBUTES)0x0,1,0x80,(HANDLE)0x0);
//         *(HANDLE *)((int)this + 8) = pvVar3;
//         if (pvVar3 != (HANDLE)0xffffffff) goto LAB_00402157;
//         pvVar3 = CreateFileA(param_1,0x40000000,dwShareMode,(LPSECURITY_ATTRIBUTES)0x0,5,0x80,
//                              (HANDLE)0x0);
//     }
//     *(HANDLE *)((int)this + 8) = pvVar3;
// LAB_00402157:
//     if (*(int *)((int)this + 8) != -1) {
//         iVar2 = 0xc - (int)param_1;
//         do {
//             cVar1 = *param_1;
//             param_1[(int)this + iVar2] = cVar1;
//             param_1 = param_1 + 1;
//         } while (cVar1 != '\0');
//         *(undefined4 *)((int)this + 0x10c) = 0;
//         *(uint *)((int)this + 4) = param_2;
//         return 1;
//     }
//     return 0;
// }