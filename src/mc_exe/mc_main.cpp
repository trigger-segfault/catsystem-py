#include "../ac_exe/common.h"
// #include "../ac_exe/TokenParser.h"
#include "../ac_exe/ghidra_types_min.h"
#include <windows.h>
#include <stdlib.h>

// Decompilation process for the cs2_full_v401/system/scene/mc.exe program
//  (AKA. CatSystem2 Message Compiler)
// 
// This is a lot more work than ac.exe reversing.
// There's a lot of memory management going on, string buffer movements, etc.
// 
// How much of this is the C++ standard library, STL, or C standard library? I have no idea...
// 
// The magic number for filename buffers is 0x400 (1024) bytes, oddly enough.
// In general there seems to be a lot of arbitrary number limits that don't fall
//  in-line with the rest of the CatSystem2 library.
// 
// Unlike the ac.exe tool, this program is much older in comparison. (or uses older tech?... I think)
//  like with the arbitrary numbers, it just feels very different from everything else
// 
//
//------------------------------------------------------------------
// CST Scene file format, for reference
// <https://github.com/trigger-segfault/TriggersTools.CatSystem2/wiki/CST-Scene>
// 
// it seems the format the data is stored in (in memory), is similar to that in the file
//  (like with the offsets and string table)
//------------------------------------------------------------------


/////////////////////////////////////////////////////
//// DEFINES


// workaround define until we add proper C++ class declarations
#define this thiscls



/////////////////////////////////////////////////////
//// ENUMS AND STRUCTURES

// kcFile open mode flags (must specify READ or WRITE, and one creation disposition)
enum KCFILE_MODE
{
    // file access (specify one, names have been confirmed)
    KCFILE_READ  = 0x1, //GENERIC_READ, FILE_SHARE_READ
    KCFILE_WRITE = 0x2, //GENERIC_WRITE, 0
    // _KCFILE_READWRITE = 0x3, //(helper enum, not used)

    // creation disposition (specify one, names are unconfirmed)
    KCFILE_OPEN     = 0x100, //OPEN_EXISTING
    KCFILE_NEW      = 0x200, //CREATE_NEW
    KCFILE_CREATE   = 0x400, //CREATE_NEW then TRUNCATE_EXISTING, requires KCFILE_WRITE
    KCFILE_ALWAYS   = 0x800, //OPEN_ALWAYS

    // This enum should include special mode flags for kcBigFile, kcMemFile doesn't use any... I think
};

// The HI byte of the CST CatScene line-type
//  (not sure what happens with the lower byte, but it is always 1 for CST)
//------------------------------------------------------------------
// <https://github.com/trigger-segfault/TriggersTools.CatSystem2/wiki/CST-Scene#linetypes>
//------------------------------------------------------------------
// Input       0x0201   Wait for input after message
// Page        0x0301   Novel page break and wait for input after message
// Message     0x2001   Display a message
// Name        0x2101   Set speaker of the message
// Command     0x3001   Perform any other command
// InputFlag   0x0200   Any type with this flag is treated as input
//------------------------------------------------------------------
enum KCLINE_TYPE
{
    LINE_INPUT = 0x02,
    LINE_PAGE = 0x03,
    LINE_MESSAGE = 0x20,
    LINE_NAME = 0x21,
    LINE_COMMAND = 0x30,
    // LINE_TYPE_0x20 = 0x20,
    // LINE_TYPE_0x21 = 0x21,
    LINE_TYPE_0xf0 = 0xf0,
    LINE_TYPE_0xf1 = 0xf1,
};

// for kcMessageCompiler fields $1c0cc4 and $1c0cc8, this is only a guess.
//  Math is never done on the state, and only 0,1,2 is every assigned.
enum THREE_STATE
{
    STATE_0 = 0,
    STATE_1 = 1,
    STATE_2 = 2,
};


// memory management of twin (double buffered?) Global allocations
struct UNK_SMALL_STRUCT
{
    /*$0,4*/    unsigned int SizeA; // 0x1000
    /*$4,4*/    unsigned int SizeB; //  0x100
    /*$8,4*/    HGLOBAL MemoryA;
    /*$c,4*/    HGLOBAL MemoryB;
    /*$10,4*/   void *BufferA;
    /*$14,4*/   void *BufferB;
    /*$18,4*/   unsigned int SmlUnk6;
    /*$1c,4*/   unsigned int SmlUnk7;
    /*$20*/
};

// not understood yet. big boy buffer
//  very likely that 0xc0000 gap is one large buffer, as is tradition with this program)
struct UNK_LARGE_STRUCT
{
    /*$0,4*/       unsigned int LrgUnk_0x0;
    /*$4,4*/       HGLOBAL Memory; //LrgUnk_0x4;
    /*$8,4*/       unsigned int LrgUnk_0x8;
    /*$c,4*/       unsigned int LrgUnk_0xc;
    //...
    /*$10,4*/      unsigned int LrgUnk_0x10;
    //...
    // /*$14,c0000*/  unsigned char LrgUnk_0x14[0xc0000];
    //...
    /*$c0014,4*/   int *LrgUnk_0xc0014;
    //...
    /*$c11b0*/
};

// either the actual kcCatScene data, or more specifically,
//   a simple collection of read lines from a file
struct kcCatScene
{
    /*$0,4*/    unsigned int MacUnk0; // 0
    /*$4,4*/    unsigned int LineCount; // 0
    /*$8,4*/    char *BufferLines; // 0
    /*$c,4*/    unsigned int *BufferOffsets; // 0
    /*$10,4*/   unsigned int MacUnk4; // 0
    /*$14,400*/ char Filename[0x400]; // param_1
    /*$414,4*/  HGLOBAL MemoryLines; // 0 (HGLOBAL GlobalAlloc)
    /*$418,4*/  HGLOBAL MemoryOffsets; // 0 (HGLOBAL GlobalAlloc)
    /*$41c,4*/  unsigned int FileSize; // 0
    /*$420*/
};

// a class for file management, that supports BOTH
// normal CreateFile operations and FindFile operations. neat!
struct FILE_READER
{
    /*$0,4*/    HANDLE Handle; // 0
    /*$4,4*/    HANDLE FindHandle; // 0
    /*$8,4*/    unsigned int DesiredAccess; // 0
    /*$c,4*/    unsigned int ShareMode; // 0x1 = FILE_SHARE_READ
    /*$10,4*/   unsigned int CreationDisposition; // 0
    /*$14,4*/   unsigned int FlagsAndAttributes; // 0
    /*$18,4*/   unsigned int RdrUnk6; // 0
    /*$1c,4*/   unsigned int RdrUnk7; // 0
    /*$20,4*/   unsigned int RdrUnk8; // 0
    /*$24,400*/ char Filename[0x400]; // MAX_PATH
    /*$424,4*/  unsigned int RdrUnk265; // 0
    /*$428,?*/  WIN32_FIND_DATAA FindData;
    /*$568,10*/ SYSTEMTIME ModifiedSystemTime; // stored as Local time
    /*$578,8*/  FILETIME ModifiedFileTime; // stored as Local time
    /*$598*/
};

// The draugr deathlord of all these structures, it holds nearly EVERYTHING
// - we have pointers to other areas stored in the struct for faster lookup
// - extremely arbitrarily large buffers (assumed based on gap in field offsets, and even numbers)
// - storage for some of the compiler flags
// - AND MORE! \o/
//
// Asside from fields [$4-$3fc], and $404 all fields have been encountered
//  (or assumed array on table sizes)
// 
// The defines below make it easier to transition after naming a field.
//  When translating field offsets of a decompiled function, prefix "this->MsgUnk_"
//  to the hex offset to get the define for the field name. Also tells us if the field
//  hasn't been identified / named yet.
struct kcMessageCompiler
{
    /*$0,4*/       char MsgUnk_0x0[1]; // unk size (0x400?)
    //...
    /*$400,4*/     unsigned int InputLength; // MsgUnk_0x400
    /*$404,4*/     unsigned int MsgUnk_0x404; // not yet seen
    /*$408,4*/     unsigned int LineIndex; // MsgUnk_0x408
    /*$40c,4*/     unsigned int InputIndex; // MsgUnk_0x40c
    /*$410,40000*/ unsigned int OffsetsTable[0x10000]; // unk size // MsgUnk_0x410
    /*$40410,80000*/ unsigned int InputsTable[0x20000]; // unk size // MsgUnk_0x40410 (in actualaity, a two-DWORD struct)
    // /*$40414,4*/   unsigned int MsgUnk_0x40414[1]; // unk size
    /*$c0410,100000*/ char LargeBuffer[0x100000]; // unk size // MsgUnk_0xc0410
    /*$1c0410,4*/  unsigned int Position; // MsgUnk_0x1c0410
    /*$1c0414,4*/  BOOL CmdFlag_L; // MsgUnk_0x1c0414 = FALSE
    /*$1c0418,4*/  unsigned int MsgUnk_0x1c0418; // 0
    /*$1c041c,4*/  BOOL Flag_0x1c041c; // Flag_0x1c041c = FALSE
    /*$1c0420,4*/  BOOL CmdFlag_X; // MsgUnk_0x1c0420 = TRUE
    /*$1c0424,4*/  unsigned int ModifiedTimestamp; // param_4 (msdostime) // MsgUnk_0x1c0424
    /*$1c0428,4*/  int UpdateCount; //MsgUnk_0x1c0428
    /*$1c042c,4*/  UNK_SMALL_STRUCT *SmallStruct; // struct size 0x20 - g_UNK_SMALL_STRUCT // MsgUnk_0x1c042c
    /*$1c0430,400*/ char Filename[0x400]; // strcpy(MsgUnk_0x1c0430, filename)
    /*$1c0830,40*/ kcCatScene *SceneTable[0x10]; // MsgUnk_0x1c0830
    /*$1c0870,40*/ unsigned int MsgUnk_0x1c0870[0x10]; // unknown pointed-to objects
    /*$1c08b0,4*/  int SceneCount; // MsgUnk_0x1c08b0
    /*$1c08b4,400*/ char StringBuffer[0x400]; // MsgUnk_0x1c08b4 (unk size)
    /*$1c0cb4,4*/  char *CurrentString; // MsgUnk_0x1c0cb4
    /*$1c0cb8,4*/  kcCatScene *CurrentScene; // MsgUnk_0x1c0cb8
    /*$1c0cbc,4*/  int MsgUnk_0x1c0cbc; // int?
    /*$1c0cc0,4*/  int MsgUnk_0x1c0cc0; // unsigned int?
    /*$1c0cc4,4*/  THREE_STATE ThreeState_A; // MsgUnk_0x1c0cc4
    /*$1c0cc8,4*/  THREE_STATE ThreeState_B; // MsgUnk_0x1c0cc8
    /*$1c0ccc,4*/  KCLINE_TYPE MsgUnk_0x1c0ccc;
    /*$1c0cd0,4*/  unsigned int MsgUnk_0x1c0cd0; // 0xffffffff
    /*$1c0cd4,4*/  unsigned int MsgUnk_0x1c0cd4;
    /*$1c0cd8,4*/  UNK_LARGE_STRUCT *LargeStruct; // struct size 0xc11b0 - g_UNK_LARGE_STRUCT // MsgUnk_0x1c0cd8
    /*$1c0cdc,4*/  unsigned int MsgUnk_0x1c0cdc;
    /*$1c0ce0*/
    
    #define MsgUnk_0x0 MsgUnk_0x0
    #define MsgUnk_0x400 InputLength
    // #define MsgUnk_0x404 MsgUnk_0x404
    #define MsgUnk_0x408 LineIndex
    #define MsgUnk_0x40c InputIndex
    #define MsgUnk_0x410 OffsetsTable
    #define MsgUnk_0x40410 InputsTable
    // #define MsgUnk_0x40414 MsgUnk_0x40414
    #define MsgUnk_0xc0410 LargeBuffer
    #define MsgUnk_0x1c0410 Position
    #define MsgUnk_0x1c0414 CmdFlag_L
    #define MsgUnk_0x1c0418 MsgUnk_0x1c0418
    #define MsgUnk_0x1c041c Flag_0x1c041c
    #define MsgUnk_0x1c0420 CmdFlag_X
    #define MsgUnk_0x1c0424 ModifiedTimestamp
    #define MsgUnk_0x1c0428 UpdateCount
    #define MsgUnk_0x1c042c SmallStruct
    #define MsgUnk_0x1c0430 Filename
    #define MsgUnk_0x1c0830 SceneTable
    #define MsgUnk_0x1c0870 MsgUnk_0x1c0870
    #define MsgUnk_0x1c08b0 SceneCount
    #define MsgUnk_0x1c08b4 StringBuffer
    #define MsgUnk_0x1c0cb4 CurrentString
    #define MsgUnk_0x1c0cb8 CurrentScene
    #define MsgUnk_0x1c0cbc MsgUnk_0x1c0cbc
    #define MsgUnk_0x1c0cc0 MsgUnk_0x1c0cc0
    #define MsgUnk_0x1c0cc4 ThreeState_A
    #define MsgUnk_0x1c0cc8 ThreeState_B
    #define MsgUnk_0x1c0ccc MsgUnk_0x1c0ccc
    #define MsgUnk_0x1c0cd0 MsgUnk_0x1c0cd0
    #define MsgUnk_0x1c0cd4 MsgUnk_0x1c0cd4
    #define MsgUnk_0x1c0cd8 LargeStruct
    #define MsgUnk_0x1c0cdc MsgUnk_0x1c0cdc
};

/////////////////////////////////////////////////////
//// GLOBALS

// Some of these globals, like g_FMODE, DAT_0041ffac, and DAT_0041ffe4 haven't seen use yet,
//  but they are next ot each other in terms of address (there's a 99% chance those two DAT fields are unimportant)

///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff90
static unsigned int g_FMODE = 0; // ::get_fmode (get's default )  _O_TEXT (0), _O_BINARY
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ffac (size=0x38)
static unsigned int DAT_0041ffac = 0;
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ffe4
static unsigned int DAT_0041ffe4 = 0;

// size of structure: 0x20, allocated in mc_main()
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff94
static UNK_SMALL_STRUCT * g_UNK_SMALL_STRUCT = nullptr;
// size of structure: 0xc11b0, allocated in mc_main()
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff98
static UNK_LARGE_STRUCT * g_UNK_LARGE_STRUCT = nullptr;
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
static unsigned int g_FILE_NUM_0041ff9c = 0;
// The only visible commandline option in the Usage section, only update modified files,
//  modified comparison is based on MSDOS-style timestamp,
//  so it'll break in 2043 or 2044 :)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ffa0
static BOOL g_CMD_FLAG_U = FALSE;
// Add special (debug?) lines to Scene script (line type 0xf0 and 0xf1, one is the name of the (input?) file)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ffa4
static BOOL g_CMD_FLAG_L = FALSE;
// Unused, likely would have been a decompile option (if ztpack.exe is anything to go by)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ffa8
static BOOL g_CMD_FLAG_D = FALSE;
// TRUE for zlib compression on CST SceneData
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041f088
static BOOL g_CMD_FLAG_X = TRUE;

// Following weird fields that are being referenced with an offset that's not a multiple of 4.
//  these could be bool (1-byte kind) or a single char. They are situated next to
//  ZLIB strings in the address space.

// (right next to zlib strings)
// used for "&macro.sts" (ampersand)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041c0ee
static unsigned char DAT_0041c0ee = 0;
// (right next to zlib strings)
// used for "%macro.sts" (percent)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041c0ef
static unsigned char DAT_0041c0ef = 0;



/////////////////////////////////////////////////////
//// ALL THE FUNCTIONS (all of them)


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413f10
kcMessageCompiler * __fastcall kcMessageCompiler_ctor(kcMessageCompiler *this)
{
    this->SmallStruct = nullptr; // 6
    this->SceneCount = 0; // this->MsgUnk_0x1c08b0 = 0; // 7
    this->MsgUnk_0x1c0cd0 = 0xffffffff; // 11
    this->CmdFlag_X = TRUE; // this->MsgUnk_0x1c0420 = 1; // 3
    this->MsgUnk_0x1c0cd4 = 0; // 12
    this->CurrentString = nullptr; // 8
    this->CurrentScene = nullptr; // this->MsgUnk_0x1c0cb8 = 0; // 9
    this->MsgUnk_0x1c0cbc = 0; // 10
    this->LargeStruct = nullptr; // 13
    this->MsgUnk_0x1c0cdc = 0; // 14
    this->MsgUnk_0x1c0418 = 0; // 1
    this->Flag_0x1c041c = FALSE; // 2
    this->CmdFlag_L = FALSE; // this->MsgUnk_0x1c0414 = 0; // 0
    this->ModifiedTimestamp = 0U; // 4
    this->UpdateCount = 0; // 5
    
    // *(undefined4 *)(param_1 + 0x1c042c) = 0; // 6
    // *(undefined4 *)(param_1 + 0x1c08b0) = 0; // 7
    // *(undefined4 *)(param_1 + 0x1c0cd0) = 0xffffffff; // 11
    // *(undefined4 *)(param_1 + 0x1c0420) = 1; // 3
    // *(undefined4 *)(param_1 + 0x1c0cd4) = 0; // 12
    // *(undefined4 *)(param_1 + 0x1c0cb4) = 0; // 8
    // *(undefined4 *)(param_1 + 0x1c0cb8) = 0; // 9
    // *(undefined4 *)(param_1 + 0x1c0cbc) = 0; // 10
    // *(undefined4 *)(param_1 + 0x1c0cd8) = 0; // 13
    // *(undefined4 *)(param_1 + 0x1c0cdc) = 0; // 14
    // *(undefined4 *)(param_1 + 0x1c0418) = 0; // 1
    // *(undefined4 *)(param_1 + 0x1c041c) = 0; // 2
    // *(undefined4 *)(param_1 + 0x1c0414) = 0; // 0
    // *(undefined4 *)(param_1 + 0x1c0424) = 0; // 4
    // *(undefined4 *)(param_1 + 0x1c0428) = 0; // 5
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413dc0
void __fastcall kcMessageCompiler_set_0x1c0cc4_to_0x1c0cc8(kcMessageCompiler *this)
{
    this->ThreeState_A = this->ThreeState_B;
    // *(undefined4 *)(param_1 + 0x1c0cc4) = *(undefined4 *)(param_1 + 0x1c0cc8);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ee0
void __thiscall kcMessageCompiler_set_0x1c0cd8(kcMessageCompiler *this, UNK_LARGE_STRUCT *newLargeStruct)
{
    this->LargeStruct = newLargeStruct;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ec0
void __thiscall kcMessageCompiler_set_0x1c042c(kcMessageCompiler *this, UNK_SMALL_STRUCT *newSmallStruct)
{
    this->SmallStruct = newSmallStruct;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413f00
void kcMessageCompiler_dtor(kcMessageCompiler *this)
{
    return; // surprisingly for overwhelming amount of fields it holds.
            // probably because it's mostly fixed-size buffers within the structure. *shrug*
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004010a0
void * __thiscall kcMessageCompiler_scalar_dtor(kcMessageCompiler *this, int flags)
{
    kcMessageCompiler_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004156e0
BOOL __thiscall kcMessageCompiler_Compile(kcMessageCompiler *this, IN const char *filename)
{
    kcCatScene *scene;
    BOOL BVar1;
    // int **in_FS_OFFSET;
    kcCatScene *local_20;
    // int *local_10;
    // undefined *puStack12;
    // undefined4 local_8;
    
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_00416971;
    // local_10 = *in_FS_OFFSET;
    // *(int ***)in_FS_OFFSET = &local_10;
    // std::strcpy((char *)((int)this + 0x1c0430), filename);
    std::strcpy(&this->Filename[0], filename); // 0x1c0430
    // *(undefined4 *)((int)this + 0x1c0418) = 1;
    this->MsgUnk_0x1c0418 = TRUE; // *0x1c0418 = 1
    scene = (kcCatScene *)std::malloc(0x420); // (void *)_newalloc(0x420);
    // local_8 = 0;
    if (scene != nullptr)
    {
        scene = kcCatScene_Open(scene, filename);
    }
    // local_8 = 0xffffffff;
    this->CurrentScene = scene; // *0x1c0cb8 = scene
    // *(undefined4 **)((int)this + 0x1c0cb8) = scene;
    // if ((*(int *)((int)this + 0x1c0cb8) == 0) || (*(int *)(*(int *)((int)this + 0x1c0cb8) + 4) == 0))
    if (this->CurrentScene == nullptr || this->CurrentScene->LineCount == 0) // *0x1c0cb8 == 0 || *0x1c0cb8->LineCount == 0
    {
        this->MsgUnk_0x1c0cd0 = 2;
        return FALSE;
        // *(undefined4 *)((int)this + 0x1c0cd0) = 2;
        // BVar1 = FALSE;
    }
    else
    {
        // *(undefined4 *)((int)this + 0x1c0cbc) = 0;
        this->MsgUnk_0x1c0cbc = 0;
        kcCatScene_LockBuffers(this->CurrentScene); // 0x1c0cb8
        kcMessageCompiler_ParseScript(this);
        if (this->CurrentScene != nullptr) // *0x1c0cb8 != nullptr
        {
            kcCatScene_UnlockBuffers(this->CurrentScene); // 0x1c0cb8
        }
        return TRUE;
        // BVar1 = TRUE;
    }
    // *in_FS_OFFSET = local_10;
    // return BVar1;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413520
BOOL __fastcall kcUnkFile_UpdateModifiedTime(FILE_READER *this)
{
    HANDLE hFile;
    if (this->Handle == nullptr)
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
    }
    else
    {
        hFile = this->Handle;
    }
    FILETIME creationTime, accessTime, modifiedTime;
    BOOL result = ::GetFileTime(hFile, &creationTime, &accessTime, &modifiedTime);
    if (this->Handle == nullptr)
    {
        ::CloseHandle(hFile);
    }
    if (result == TRUE) // == 1
    {
        ::FileTimeToLocalFileTime(&modifiedTime, &this->ModifiedFileTime); // 0x15e (0x578)
        ::FileTimeToSystemTime(&this->ModifiedFileTime, &this->ModifiedSystemTime); // 0x15e, 0x15a (0x578, 0x568)
    }
    return result;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413a50
unsigned int __fastcall kcUnkFile_GetMSDOSTime(FILE_READER *this)
{
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
    //https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
    // BOOL BVar1;
    // unsigned int uVar2;
    
    if (kcUnkFile_UpdateModifiedTime(this)) // != FALSE) // 0
    {
        // This will stop working in 2043 (or 2044?) ... yay~
        return ((((unsigned int)this->ModifiedSystemTime.wYear - 1980U) & 0x3f) << 26) | // (4 + 5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMonth  &  0xf) << 5) | // (5 + 5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wDay    & 0x1f) << 5) | // (5 + 6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wHour   & 0x1f) << 6) | // (6 + 6)
                (((unsigned int)this->ModifiedSystemTime.wMinute & 0x3f) << 6) | // (6)
                 ((unsigned int)this->ModifiedSystemTime.wSecond & 0x3f);        // (0)
        // return (((((
        //         (uint)*(ushort *)((int)param_1 + 0x568) - 0x7bc & 0x3f) << 4 | // 0x15a
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 22 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 17 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 12 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
        // return (((((
        //         (uint)*(ushort *)((int)param_1 + 0x568) - 0x7bc & 0x3f) << 4 | // 0x15a
        //         (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
        //         (uint)*(ushort *)((int)param_1 + 0x570) & 0x1f) << 6 | // 0x15c
        //         (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
        //         (uint)*(ushort *)((int)param_1 + 0x574) & 0x3f; // 0x15d
    }
    return 0U;
    // if (BVar1 == 0)
    // {
    //     uVar2 = 0;
    // }
    // else
    // {
    //     uVar2 = ((((((uint)*(ushort *)(param_1 + 0x15a) - 0x7bc & 0x3f) << 4 |
    //             (uint)*(ushort *)((int)param_1 + 0x56a) & 0xf) << 5 |
    //             (uint)*(ushort *)((int)param_1 + 0x56e) & 0x1f) << 5 |
    //             (uint)*(ushort *)(param_1 + 0x15c) & 0x1f) << 6 |
    //             (uint)*(ushort *)((int)param_1 + 0x572) & 0x3f) << 6 |
    //             (uint)*(ushort *)(param_1 + 0x15d) & 0x3f;
    // }
    // return uVar2;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412620
void __cdecl kcScriptCompiler_compileStart(IN const char *othername, IN const char *filename, IN OUT unsigned char *unused_bool, unsigned int modifiedTimestamp, bool verbose)
{
    int iVar1;
    kcMessageCompiler *compiler;
    
    std::printf("compile : %s\n", filename);
    compiler = (kcMessageCompiler *)std::malloc(0x1c0ce0); //_newalloc(0x1c0ce0);
    if (compiler != nullptr)
    {
        compiler = kcMessageCompiler_ctor(compiler);
    }
    kcMessageCompiler_set_0x1c0cd8(compiler, g_UNK_LARGE_STRUCT); // large struct
    kcMessageCompiler_set_0x1c042c(compiler, g_UNK_SMALL_STRUCT); // small struct
    compiler->CmdFlag_L = g_CMD_FLAG_L;
    compiler->CmdFlag_X = g_CMD_FLAG_X;
    compiler->ModifiedTimestamp = modifiedTimestamp;
    // *(BOOL *)((int)compiler + 0x1c0414) = g_CMD_FLAG_L;
    // *(BOOL *)((int)compiler + 0x1c0420) = g_CMD_FLAG_X;
    // *(undefined4 *)((int)compiler + 0x1c0424) = modifiedTimestamp;
    BOOL iVar1 = kcMessageCompiler_Compile(compiler, filename);
    if (iVar1 == 0)
    {
        std::printf("Warning!!!  Compile error!!!\n");
    }
    else
    {
        //if ((verbose != '\0') && (*(int *)((int)compiler + 0x1c0428) != 0))
        if (verbose && compiler->UpdateCount != 0)
        {
            //std::printf("  [%d] scenes update!\n",*(undefined4 *)((int)compiler + 0x1c0428));
            std::printf("  [%d] scenes update!\n", compiler->UpdateCount);
        }
    }
    if (compiler != nullptr)
    {
        kcMessageCompiler_scalar_dtor(compiler, 1);
    }
    return;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412d40
BOOL __fastcall kcCompiler_0x1c0cd8_FUN_00412d40(UNK_LARGE_STRUCT *this)
{
    if (this->LrgUnk_0x0 == 1)
    {
        if (this->LrgUnk_0xc0014[1] > -1)
        {
            this->LrgUnk_0x10++;
        }
    }
    else if (this->LrgUnk_0x0 != 2)
    {
        return FALSE;
    }
    this->LrgUnk_0xc0014 = nullptr;
    this->LrgUnk_0x0 = 0;
    // if (param_1[0] == 1) {
    //     if (*(int *)(param_1[0x30005] + 4) > -1) {
    //         param_1[4] = param_1[4] + 1;
    //     }
    // }
    // else if (param_1[0] != 2)
    // {
    //     return FALSE;
    // }

    // param_1[0x30005] = 0;
    // param_1[0] = 0;
    return TRUE;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned short * __thiscall kcCatScene_FUN_00411a30(kcCatScene *this, unsigned short *shortTable, int param_3, IN OUT int *param_4, unsigned int param_5)
{
    undefined uVar1;
    bool bVar2;
    unsigned short *puVar3;
    unsigned short *local_28;
    int local_1c;
    size_t local_18;
    unsigned short *local_14;
    int local_10;
    byte *local_c;
    unsigned short local_8;
    
    local_1c = *param_4;
    local_28 = shortTable;
    bVar2 = false;
    while( true )
    {
        puVar3 = local_28;
        if ((false) || (local_c = (byte *)kcCatScene_GetLineAt(this, local_1c), local_c == (byte *)0x0))
            goto LAB_00411c0a;

        if (*(int *)(this + 0x10) != 0)
        {
            *(undefined4 *)(this + 0x10) = 0;
        }
        if (bVar2)
        {
            while ((*local_c != 0 && (*local_c < 0x21)))
            {
                local_c = local_c + 1;
            }
        }
        local_18 = std::strlen((char *)local_c);
        local_1c += 1;
        if (param_3 <= (int)local_18)
            break;
        std::memcpy(local_28,local_c,local_18);
        local_14 = local_28;
        puVar3 = (unsigned short *)((int)local_28 + local_18);
        param_3 -= local_18;
        if ((((param_5 & 1) == 0) || ((int)local_18 < 1)) || (*(char *)((int)puVar3 + -1) != '\\'))
            goto LAB_00411c0a;

        local_8 = *local_28;
        while (1 < (int)local_18)
        {
            if ((local_8 & 0xff) < 0x80)
            {
                local_10 = 1;
            }
            else
            {
                local_10 = 2;
            }
            local_18 -= local_10;
            local_14 = (unsigned short *)((int)local_14 + local_10);
            if ((int)local_18 < 2)
                break;
            local_8 = *local_14;
        }
        if (((local_18 == 0) && (local_10 == 2)) && (local_8 >> 8 == 0x5c))
        {
            uVar1 = *(undefined *)puVar3;
            *(undefined *)puVar3 = 0;
            *(undefined4 *)(this + 0x10) = 1;
            *(undefined *)puVar3 = uVar1;
            goto LAB_00411c0a;
        }
        bVar2 = true;
        local_28 = (unsigned short *)((int)puVar3 + -1);
    }
    std::memcpy(local_28,local_c,param_3 - 1U);
    puVar3 = (unsigned short *)((int)local_28 + (param_3 - 1U));
    LAB_00411c0a:
    local_28 = puVar3;
    *param_4 = local_1c;
    *(undefined *)local_28 = 0;
    return (unsigned short *)((int)local_28 - (int)shortTable);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
BOOL __fastcall kcCatScene_IsLocked(kcCatScene *this)

{
    undefined4 uVar1;
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr)
    {
        uVar1 = 0;
    }
    else
    {
        uVar1 = 1;
    }
    return uVar1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
const char * __thiscall kcCatScene_GetLineAt(kcCatScene *this, int index)
{
    int iVar1;
    
    // iVar1 = ;
    if (!kcCatScene_IsLocked(this))
        return nullptr;

    // if ((index < 0) || (*(int *)(this + 4) <= index))
    if (index < 0 || index >= this->LineCount)
        return nullptr;
    
    return &this->BufferLines[this->BufferOffsets[index]];
    // iVar1 = *(int *)(this + 8) + *(int *)(*(int *)(this + 0xc) + index * 4);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
BOOL __thiscall kcCatScene_HasLineAt(kcCatScene *this, int index)
{
    // int iVar1;
    // iVar1 = kcCatScene_GetLineAt(this, index);
    // return (BOOL)(iVar1 != 0);
    return (kcCatScene_GetLineAt(this, index) != nullptr);
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414eb0
unsigned int __fastcall kcMessageCompiler_FUN_00414eb0(kcMessageCompiler *this)
{
    uint uVar1;
    size_t sVar2;
    
    this->CurrentString = nullptr;
    // *(undefined4 *)(param_1 + 0x1c0cb4) = 0;
    do
    {
        while( true )
        {
            if (this->CurrentScene == nullptr)
                goto LAB_00414ff6;

            this->MsgUnk_0x1c0cc0 = this->MsgUnk_0x1c0cbc;
            // uVar1 = ;
            if (!kcCatScene_HasLineAt(this->CurrentScene, this->MsgUnk_0x1c0cbc))
                break;
            
            // some form of line copy, 1 may be number of lines
            kcCatScene_FUN_00411a30(this->CurrentScene, (unsigned char *)&this->StringBuffer[0], 0x400, &this->MsgUnk_0x1c0cbc, 1);

            FUN_004118f0(&this->StringBuffer[0], &this->StringBuffer[0], 1); // 0x1c08b4, 0x1c08b4, 1
            
            // if (*(int *)(param_1 + 0x1c0cb8) == 0) goto LAB_00414ff6;
            // *(undefined4 *)(param_1 + 0x1c0cc0) = *(undefined4 *)(param_1 + 0x1c0cbc);
            // uVar1 = kcCatScene_HasLineAt(*(int *)(param_1 + 0x1c0cb8),*(int *)(param_1 + 0x1c0cbc));
            // if (uVar1 == 0) break;
            // kcCatScene_FUN_00411a30(*(int *)(param_1 + 0x1c0cb8),(unsigned short *)(param_1 + 0x1c08b4),0x400,
            //             (int *)(param_1 + 0x1c0cbc),1);
            // FUN_004118f0(param_1 + 0x1c08b4,param_1 + 0x1c08b4,1);
            if (((char *)this->StringBuffer)[0] != '\0' && ((char *)this->StringBuffer)[0] != ' ')
            {
                this->CurrentString = ((char *)&this->StringBuffer[0]);
                goto LAB_00414ff6;
            }
            // if ((*(char *)(param_1 + 0x1c08b4) != '\0') && (*(short *)(param_1 + 0x1c08b4) != 0x20))
            // {
            //     *(int *)(param_1 + 0x1c0cb4) = param_1 + 0x1c08b4;
            //     goto LAB_00414ff6;
            // }
            kcMessageCompiler_set_0x1c0cc4_to_0x1c0cc8(this);
        }
        if (this->MsgUnk_0x1c0cd4 != 0)
        {
            if (this->LargeStruct == nullptr)
            {
                // std::printf(LOG_COMMENT_BLOCK_NOT_CLOSED, *(int *)(param_1 + 0x1c0cb8) + 0x14);
                ///JP: std::printf("コメントアウトが閉じられていません：%s\n", &this->CurrentScene->Filename[0]);
                std::printf("Comment block not closed : %s\n", &this->CurrentScene->Filename[0]);
            }
            else
            {
                // std::printf(LOG_COMMENT_BLOCK_NOT_CLOSED_IN_MACRO);
                ///JP: std::printf("マクロ内でコメントアウトが閉じられていません\n");
                std::printf("Comment block not closed in macro\n");
            }
            this->MsgUnk_0x1c0cd4 = 0;
            // *(undefined4 *)(param_1 + 0x1c0cd4) = 0;
        }
        // uVar1 = kcMessageCompiler_FUN_00413de0(this);
    } while (kcMessageCompiler_FUN_00413de0(this) != FALSE); // != 0
    //     uVar1 = kcMessageCompiler_FUN_00413de0(this);
    // } while (uVar1 != 0); // != 0

LAB_00414ff6:
    // if (*(int *)(param_1 + 0x1c0cb4) == 0)
    if (this->CurrentString == nullptr)
    {
        return 0xffffffff;
    }
    else
    {
        return std::strlen(this->CurrentString);
        // sVar2 = std::strlen(*(char **)(param_1 + 0x1c0cb4));
    }
    // return sVar2;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413de0
unsigned int __fastcall kcMessageCompiler_FUN_00413de0(kcMessageCompiler *this)
{
    bool bVar1;
    
    if (this->LargeStruct != nullptr)
    {
        kcCompiler_0x1c0cd8_FUN_00412d40(this->LargeStruct);
        
        this->MsgUnk_0x1c0cdc = 0;
    }
    // bVar1 = (this->SceneCount > 0);
    if (this->SceneCount > 0) // 0x1c08b0 > 0
    {              
        // *(int *)(param_1 + 0x1c08b0) = *(int *)(param_1 + 0x1c08b0) + -1;
        // *(undefined4 *)(param_1 + 0x1c0cb8) =
        //     *(undefined4 *)(param_1 + 0x1c0830 + *(int *)(param_1 + 0x1c08b0) * 4);
        // *(undefined4 *)(param_1 + 0x1c0cbc) =
        //     *(undefined4 *)(param_1 + 0x1c0870 + *(int *)(param_1 + 0x1c08b0) * 4);
        // kcCatScene_LockBuffers(*(int *)(param_1 + 0x1c0cb8));
        // *(undefined4 *)(param_1 + 0x1c0cc0) = *(undefined4 *)(param_1 + 0x1c0cbc);
        this->SceneCount -= 1; // *0x1c08b0 -= 1;
        this->CurrentScene = this->SceneTable[this->SceneCount]; // *0x1c0cb8 = *(0x1c0830 + *0x1c08b0 * 4);
        this->MsgUnk_0x1c0cbc = this->MsgUnk_0x1c0870[this->SceneCount]; // *0x1c0cbc = *(0x1c0870 + *0x1c08b0 * 4);
        kcCatScene_LockBuffers(this->CurrentScene); // *0x1c0cb8
        this->MsgUnk_0x1c0cc0 = this->MsgUnk_0x1c0cbc; // *0x1c0cc0 = *0x1c0cbc;
        // *(undefined4 *)(param_1 + 0x1c0cc0) = *(undefined4 *)(param_1 + 0x1c0cbc);
        return TRUE;
    }
    else
    {
        this->CurrentString = nullptr;
        this->CurrentScene = nullptr; // this->MsgUnk_0x1c0cb8 = 0; // *0x1c0cb8 = 0
        this->MsgUnk_0x1c0cbc = 0;
        return FALSE;
    }
    // return (uint)bVar1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
void __fastcall kcCatScene_UnlockBuffers(kcCatScene *this)
{
    if (this->BufferLines != nullptr) // 0x8
    {
        ::GlobalUnlock(this->MemoryLines); // 0x414
        this->BufferLines = nullptr; // 0x8
    }
    if (this->BufferOffsets != nullptr) // 0xc
    {
        ::GlobalUnlock(this->MemoryOffsets); // 0x418
        this->BufferOffsets = nullptr; // 0xc
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
BOOL __fastcall kcCatScene_LockBuffers(kcCatScene *this)
{
    LPVOID pvVar1;
    undefined4 uVar2;
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr) // 0x8, 0xc
    {
        if (this->MemoryLines != nullptr) // 0x414
        {
            this->BufferLines = (char *)::GlobalLock(this->MemoryLines); // 0x8, 0x414
        }
        if (this->MemoryOffsets != nullptr) // 0x418
        {
            this->BufferOffsets = (unsigned int *)::GlobalLock(this->MemoryOffsets); // 0xc, 0x418
        }
    }
    
    if (this->BufferLines == nullptr || this->BufferOffsets == nullptr) // 0x8, 0xc
    {
        kcCatScene_UnlockBuffers(this);
        return FALSE;
    }
    return TRUE;
}


//unsigned int * __thiscall kcMacroReader_initRun(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
kcCatScene * __thiscall kcCatScene_Open(kcCatScene *this, IN const char *filename)
{
    this->BufferLines = nullptr;
    this->MemoryLines = nullptr; // HGLOBAL GlobalAlloc
    this->BufferOffsets = nullptr;
    this->MemoryOffsets = nullptr;
    this->FileSize = 0;
    this->LineCount = 0;
    this->MacUnk0 = 0;
    this->MacUnk4 = 0;
    std::strcpy(&this->Filename[0], filename);
    kcCatScene_Read(this, filename);
    return this;
    // *(undefined4 *)((int)this + 8) = 0;
    // *(undefined4 *)((int)this + 0x414) = 0;
    // *(undefined4 *)((int)this + 0xc) = 0;
    // *(undefined4 *)((int)this + 0x418) = 0;
    // *(undefined4 *)((int)this + 0x41c) = 0;
    // *(undefined4 *)((int)this + 4) = 0;
    // *(undefined4 *)this = 0;
    // *(undefined4 *)((int)this + 0x10) = 0;
    // _strcpy((char *)((int)this + 0x14),filename);
    // kcCatScene_Read((int)this,param_1);
    // return (undefined4 *)this;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004134f0
void __fastcall kcUnkFile_FindClose(FILE_READER *this)
{
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004138c0
unsigned int __fastcall kcUnkFile_FindNext(FILE_READER *this)
{
    if (this->FindHandle == nullptr)
    {
        this->FindHandle = ::FindFirstFileA(&this->Filename[0], &this->FindData);
        if (this->FindHandle == INVALID_HANDLE_VALUE)
        {
            kcUnkFile_FindClose(this);
            return INVALID_FILE_ATTRIBUTES;
        }
    }
    else
    {
        if (::FindNextFileA(this->FindHandle, &this->FindData) == FALSE)
        {
            kcUnkFile_FindClose(this);
            return INVALID_FILE_ATTRIBUTES;
        }
    }
    std::strcpy(&this->Filename[0], this->FindData.cFileName);
    return this->FindData.dwFileAttributes; //???
    // return *(undefined4 *)(param_1 + 0x428);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b40
unsigned int __fastcall kcUnkFile_FindLoop(FILE_READER *this)
{
    unsigned int result;
    
    do {
        result = kcUnkFile_FindNext(this);
        if ((int)result < 0) {
            return result;
        }
    } while ((result & 0x110) != 0);
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137f0
FILE_READER * __thiscall kcUnkFile_ctor(FILE_READER *this, IN const char *filename)
{
    // unsigned int name_len;
    // unsigned int tmp_len = ::strlen(filename);
    // if (tmp_len < 0x3ff)
    // {
    //     name_len = ::strlen(filename); // maximum over-redundancy! wooo~
    // }
    // else
    // {
    //     name_len = 0x3ff;
    // }
    unsigned int name_len = std::max(0x3ffU, (unsigned int)std::strlen(filename));
    std::memcpy(&this->Filename[0], filename, name_len);
    this->Filename[name_len] = '\0';
    this->Handle = nullptr; // 0
    this->FindHandle = nullptr; // 0
    this->DesiredAccess = GENERIC_READ | GENERIC_WRITE; // 0xc0000000
    this->ShareMode = (KCFILE_MODE)(FILE_SHARE_READ | FILE_SHARE_WRITE); // 3
    this->CreationDisposition = OPEN_EXISTING; // 3
    this->FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL; // 0x80
    this->RdrUnk6 = 0; // LPSECURITY_ATTRIBUTES?
    this->RdrUnk265 = 0; // HANDLE hTemplateFile? or Overlapped?
    g_FILE_NUM_0041ff9c += 1;
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004136b0
BOOL __fastcall kcUnkFile_Close(FILE_READER *this)
{
    BOOL result = TRUE;
    if (this->FindHandle != nullptr)
    {
        ::FindClose(this->FindHandle);
        this->FindHandle = nullptr;
    }
    if (this->Handle != nullptr)
    {
        result = ::CloseHandle(this->Handle);
        this->Handle = nullptr;
    }
    return result;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137d0
void __fastcall kcUnkFile_dtor(FILE_READER *this)
{
    kcUnkFile_Close(this);
    g_FILE_NUM_0041ff9c -= 1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401000
void * __thiscall kcUnkFile_scalar_dtor(FILE_READER *this, int flags)
{
    kcUnkFile_dtor(this);
    if ((flags & 1) != 0)
    {
        std::free(this);
    }
    return this;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413770
void __fastcall kcUnkFile_SetWriteMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_WRITE; // = 0x40000000;
    this->ShareMode &= FILE_SHARE_WRITE;// &= 2
    this->CreationDisposition = CREATE_ALWAYS; // = 2
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413b20
void __fastcall kcUnkFile_SetWriteMode_thunk(FILE_READER *this)
{
    kcUnkFile_SetWriteMode(this);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004137a0
void __fastcall kcUnkFile_SetReadMode(FILE_READER *this)
{
    this->DesiredAccess = GENERIC_READ; // = 0x80000000
    this->ShareMode &= FILE_SHARE_READ; // &= 1
    this->CreationDisposition = OPEN_EXISTING; // = 3
}

// DWORD __fastcall kcUnkFile_GetFileSize(HANDLE *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004135e0
unsigned int __fastcall kcUnkFile_GetFileSize(FILE_READER *this)
{
    HANDLE hFile;
    if (this->Handle == nullptr)
    {
        hFile = ::CreateFileA(&this->Filename[0], GENERIC_READ, this->ShareMode & FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) // 0xffffffff
        {
            return INVALID_FILE_SIZE; // 0xffffffff
        }
    }
    else
    {
        hFile = this->Handle;
    }

    DWORD fileSize;
    fileSize = ::GetFileSize(hFile, &fileSize);
    if (this->Handle == nullptr)
    {
        ::CloseHandle(hFile);
    }
    return (unsigned int)fileSize;
}


/* WARNING: Could not reconcile some variable overlaps */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413c30
void __fastcall kcMessageCompiler_FUN_00413c30(kcMessageCompiler *this)
{
    int local_8;
    
    // if (*(int *)(param_1 + 0x1c0cc4) != 0)
    if (this->ThreeState_A != STATE_0)
    {
        local_8 = -1;
        // if (*(int *)(param_1 + 0x1c0cc4) == 1)
        if (this->ThreeState_A == STATE_1)
        {
            local_8 = 2;
        }
        else
        {
            // if (*(int *)(param_1 + 0x1c0cc4) == 2)
            if (this->ThreeState_A == STATE_2)
            {
                local_8 = 3;
            }
        }
        if (local_8 > 0)
        {
            this->OffsetsTable[this->LineIndex] = this->Position;
            this->LineIndex++;
            this->InputLength++;
            this->LargeBuffer[this->Position] = 1; // TRUE?
            this->Position++;
            this->LargeBuffer[this->Position] = (char)local_8;
            this->Position++;
            this->LargeBuffer[this->Position] = 0; // FALSE?
            this->Position++;
            this->MsgUnk_0x1c0ccc = (KCLINE_TYPE)local_8;
            // *(undefined4 *)(param_1 + 0x410 + *(int *)(param_1 + 0x408) * 4) =
            //     *(undefined4 *)(param_1 + 0x1c0410);
            // *(int *)(param_1 + 0x408) = *(int *)(param_1 + 0x408) + 1;
            // *(int *)(param_1 + 0x400) = *(int *)(param_1 + 0x400) + 1;
            // *(undefined *)(param_1 + 0xc0410 + *(int *)(param_1 + 0x1c0410)) = 1;
            // *(int *)(param_1 + 0x1c0410) = *(int *)(param_1 + 0x1c0410) + 1;
            // *(undefined *)(param_1 + 0xc0410 + *(int *)(param_1 + 0x1c0410)) = (undefined)local_8;
            // *(int *)(param_1 + 0x1c0410) = *(int *)(param_1 + 0x1c0410) + 1;
            // *(undefined *)(param_1 + 0xc0410 + *(int *)(param_1 + 0x1c0410)) = 0;
            // *(int *)(param_1 + 0x1c0410) = *(int *)(param_1 + 0x1c0410) + 1;
            // *(int *)(param_1 + 0x1c0ccc) = local_8;
        }
    }
    this->ThreeState_A = STATE_0;
    this->ThreeState_B = STATE_0;
    // *(undefined4 *)(param_1 + 0x1c0cc4) = 0;
    // *(undefined4 *)(param_1 + 0x1c0cc8) = 0;
    if (this->LineIndex > 0)
    // if (*(int *)(param_1 + 0x408) > 0)
    {
        this->InputsTable[this->InputIndex * 2] = this->InputLength;
        this->InputIndex++;
        this->InputLength = 0;
        // *(undefined4 *)(param_1 + 0x40410 + *(int *)(param_1 + 0x40c) * 8) =
        //     *(undefined4 *)(param_1 + 0x400);
        // *(int *)(param_1 + 0x40c) = *(int *)(param_1 + 0x40c) + 1;
        // *(undefined4 *)(param_1 + 0x400) = 0;
    }
    return;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413ff0
void __thiscall kcMessageCompiler_AddLine(kcMessageCompiler *this, KCLINE_TYPE lineType, IN const char *str)
{
    // unsigned int newline_len; //size_t
    char local_114 [260];
    unsigned int local_10;
    bool lineContinue; // prepend newline
    // unsigned int str_len; //size_t
    
    // local_10 = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    if (this->MsgUnk_0x0[0] != '\0') // 0x000000
    {
        if (this->ThreeState_A != STATE_0) // 0x1c0cc4 != 0
        {
            kcMessageCompiler_FUN_00413c30(this);
        }
        // if (*(int *)((int)this + 0x1c0cc4) != STATE_0) {
        //     FUN_00413c30((int)this);
        // }
        lineContinue = false;
        if (lineType != LINE_TYPE_0xf1 && lineType != LINE_TYPE_0xf0)
        {
            if (lineType == LINE_MESSAGE && this->MsgUnk_0x1c0ccc == LINE_MESSAGE) // 0x1c0ccc
            {
                lineContinue = true; // We're still writing the same line
            }
            this->MsgUnk_0x1c0ccc = lineType;
            // *(int *)((int)this + 0x1c0ccc) = lineType;
        }
        // if ((*(int *)((int)this + 0x1c041c) != 0) && (*(int *)((int)this + 0x1c0414) != 0))
        if (this->Flag_0x1c041c != FALSE && this->CmdFlag_L != FALSE) // 0x1c041c, 0x1c0414
        {
            this->Flag_0x1c041c = FALSE;
            std::sprintf(local_114, "%d", this->MsgUnk_0x1c0cc0 + 1);
            kcMessageCompiler_AddLine(this, LINE_TYPE_0xf1, local_114);
            // *(undefined4 *)((int)this + 0x1c041c) = 0;
            // std::sprintf(local_114, "%d", *(int *)((int)this + 0x1c0cc0) + 1);
            // kcMessageCompiler_AddLine(this,0xf1,local_114);
        }
        // if ((*(int *)((int)this + 0x1c0418) != 0) && (*(int *)((int)this + 0x1c0414) != 0))
        if (this->MsgUnk_0x1c0418 != 0 && this->CmdFlag_L != 0) // 0x1c0418, 0x1c0414
        {
            this->MsgUnk_0x1c0418 = 0;
            kcMessageCompiler_AddLine(this, LINE_TYPE_0xf0, &this->CurrentScene->Filename[0]);
            // *(undefined4 *)((int)this + 0x1c0418) = 0;
            // kcMessageCompiler_AddLine(this, 0xf0, (char *)(*(int *)((int)this + 0x1c0cb8) + 0x14));
        }
        // if (*(int *)((int)this + 0x400) == 0) // 0x400
        if (this->InputLength == 0) // 0x400
        {
            this->InputsTable[this->InputIndex * 2 + 1] = this->LineIndex; //0x40414 or 0x40410 (offset 1)
            // *(undefined4 *)((int)this + *(int *)((int)this + 0x40c) * 8 + 0x40414) =
            //     *(undefined4 *)((int)this + 0x408);
        }
        this->OffsetsTable[this->LineIndex] = this->Position;
        // *(undefined4 *)((int)this + *(int *)((int)this + 0x408) * 4 + 0x410) =
        //     *(undefined4 *)((int)this + 0x1c0410);
        this->LineIndex++;
        this->InputLength++;
        // *(int *)((int)this + 0x408) = *(int *)((int)this + 0x408) + 1;
        // *(int *)((int)this + 0x400) = *(int *)((int)this + 0x400) + 1;
        this->LargeBuffer[this->Position] = 1;
        this->Position++;
        // *(undefined *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410) = 1;
        // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + 1;
        
        this->LargeBuffer[this->Position] = (char)lineType;
        this->Position++;
        // *(undefined *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410) = (undefined)lineType;
        // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + 1;

        unsigned int str_len = std::strlen(str);
        str_len += 1;

        // continue the current line content, by first prepending a newline
        if (lineContinue)
        {
            unsigned int newline_len = std::strlen("\\n");
            // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), "\\n", newline_len);
            std::memcpy(&this->LargeBuffer[this->Position], "\\n", newline_len);
            newline_len = std::strlen("\\n"); // redundant again...?

            this->Position += newline_len; // *0x1c0410 = newline_len + *0x1c0410
            // *(int *)((int)this + 0x1c0410) = newline_len + *(int *)((int)this + 0x1c0410);
        }

        // copy main content
        std::memcpy(&this->LargeBuffer[this->Position], str, str_len);
        this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;


        // if (!lineContinue)
        // {
        //     std::memcpy(&this->LargeBuffer[this->Position], str, str_len);
        //     this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        //     // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;
        // }
        // else
        // {
        //     unsigned int newline_len = std::strlen("\\n");
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), "\\n", newline_len);
        //     std::memcpy(&this->LargeBuffer[this->Position], "\\n", newline_len);
        //     newline_len = std::strlen("\\n"); // redundant again...?

        //     this->Position += newline_len; // *0x1c0410 = newline_len + *0x1c0410
        //     // *(int *)((int)this + 0x1c0410) = newline_len + *(int *)((int)this + 0x1c0410);
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        //     std::memcpy(&this->LargeBuffer[this->Position], str, str_len);
        //     // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;
        //     this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        // }
        if (lineType == LINE_MESSAGE)
        {
            this->ThreeState_B = STATE_1; // 0x1c0cc8 = 1
            // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
        }
    }
    // __end_security_critical();
    // return;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414b20
void __thiscall kcMessageCompiler_ParseName(kcMessageCompiler *this, IN const char *line)
{
    char buffer [1028];
    // uint local_10;
    // char *cptr;
    // char *lptr;
    
    // local_10 = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    const char *lptr = line;
    char *cptr = &buffer[0];
    // while not whitespace/control chars/null-terminator
    while (*((unsigned char *)lptr) > 0x20)
    {
        *cptr++ = *lptr++; // assigns value before incrementing ptr
        cptr++;
        lptr++;
    }
    *cptr = '\0'; // null-terminate buffer
    // trim whitespace (from end of line)
    while (--cptr >= &buffer[0] && (*cptr == ' ' || *cptr == '\t'))
    {
        if (*cptr != ' ' && *cptr != '\t')
            break;
        // cptr--;
    }
    // while ((buffer < cptr && ((cptr = cptr + -1, *cptr == ' ' || (*cptr == '\t'))))) {
    //     *cptr = 0;
    // }
    if (buffer[0] != '\0') // is our trimmed line not empty?
    {
        kcMessageCompiler_AddLine(this, LINE_NAME, &buffer[0]); // LINE_TYPE_0x21
    }
    // __end_security_critical();
    // return;
}

// undefined4 __thiscall kcCatScene_Read(int this,char *filename)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
BOOL __thiscall kcCatScene_Read(kcCatScene *this, IN const char *filename)
{
    // unsigned char bVar1;
    // // FILE_READER *this_00;
    // unsigned char *bufferLines;
    // //FILE *file;
    // int iVar3;
    // char *pcVar4;
    // undefined4 uVar5;
    // size_t sVar7;
    // FILE_READER *unkFile;
    // DWORD fileSize;
    // size_t local_1c;
    // int local_10;
    // int lineCount;
    
    // all this just to read fileSize... nice
    FILE_READER *unkFile = (FILE_READER *)std::malloc(0x598);//_newalloc(0x598);
    if (unkFile != nullptr)
    {
        unkFile = (FILE_READER *)kcUnkFile_ctor(unkFile, filename);
    }
    kcUnkFile_SetReadMode(unkFile);
    unsigned int fileSize = kcUnkFile_GetFileSize(unkFile);
    if (unkFile != nullptr)
    {
        kcUnkFile_scalar_dtor(unkFile, 1);
    }

    HGLOBAL hMemLines = nullptr;
    HGLOBAL hMemOffsets = nullptr;
    unsigned char *bufferLines = nullptr;
    unsigned int *bufferOffsets;
    unsigned int lineCount = 0;

    // allocate MemoryLines and copy lines into buffer,
    //  nullterminated with newlines and control chars removed
    if ((int)fileSize > 0)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        
        fileSize += 0x10; // extra space, for unknown reasons. Could be useful?
        FILE *file = nullptr;

        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemLines = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, fileSize);

        if (hMemLines != nullptr)
        {
            bufferLines = (unsigned char *)::GlobalLock(hMemLines);
        }

        if (bufferLines != nullptr)
        {
            file = std::fopen(filename, "rt");
        }

        // reading file (final 'if' in setup)
        if (file != nullptr)
        {
            for (int pos = 0; !std::feof(file) && std::fgets((char *)&bufferLines[pos], fileSize - pos, file); pos++)
            {
                // char *line = std::fgets(&bufferLines[i], fileSize - pos, file);
                // if (pcVar4 == nullptr)
                //     break;
                unsigned int len;// = std::strlen(&bufferLines[pos]); //&bufferLines[len + pos - 1]
                for (len = std::strlen((char *)&bufferLines[pos]); len > 0; len--)
                {
                    if (bufferLines[pos + len - 1] >= 0x20 || bufferLines[pos + len - 1] == '\t')
                        break;
                }
                bufferLines[pos + len] = '\0';
                pos += len;
                lineCount++;
                //*(char *)((int)bufferLines + pos + len) = '\0';
                // pos = pos + 1 + len
                //lineCount += 1;
            }
            std::fclose(file);
        }

        // cleanup
        if (bufferLines != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }
        // done
    }

    // No lines in the file? That's a paddlin' (return FALSE)
    if (lineCount < 1)
    {
        if (hMemLines != nullptr)
        {
            ::GlobalFree(hMemLines);
        }
        return FALSE;
    }
    // allocate MemoryOffsets and add byte offsets of each line in MemoryLines (return TRUE)
    else //if (lineCount >= 1)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        this->LineCount = lineCount;
        this->FileSize = fileSize;
        this->MemoryLines = hMemLines;
        bufferLines = nullptr;
        bufferOffsets = nullptr;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemOffsets = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lineCount * sizeof(unsigned int));
        if (hMemOffsets != nullptr)
        {
            bufferLines = (unsigned char *)::GlobalLock(hMemLines);
        }
        if (bufferLines != nullptr)
        {
            bufferOffsets = (unsigned int *)::GlobalLock(hMemOffsets);
        }
        
        // reading file (final 'if' in setup)
        if (bufferOffsets != nullptr)
        {
            // unsigned int offset = 0;
            for (unsigned int line = 0, offset = 0; line < this->LineCount; line++, offset++)
            {
                bufferOffsets[line] = offset;
                offset += (unsigned int)std::strlen((char *)&bufferLines[offset]);
            }
            ::GlobalUnlock(hMemOffsets);
        }

        // cleanup
        if (bufferLines != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }

        // done
        this->MemoryOffsets = hMemOffsets;
        return TRUE;
    }
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004125f0
const char * __cdecl mc_ParseCmdFlag(IN const char *str)
{
    if (str[0] == '-' || str[0] == '/')
    {
        return str + 1;
    }
    return nullptr;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004131f0
UNK_LARGE_STRUCT * __fastcall kcLargeStruct_ctor(UNK_LARGE_STRUCT *this)
{
    this->LrgUnk_0x10 = 0;
    this->LrgUnk_0x0 = 0;
    this->LrgUnk_0xc0014 = nullptr;
    this->Memory = nullptr;
    this->LrgUnk_0x8 = 0;
    this->LrgUnk_0xc = 0;
    return this;
    // param_1[4] = 0;
    // *param_1 = 0;
    // param_1[0x30005] = 0;
    // param_1[1] = 0;
    // param_1[2] = 0;
    // param_1[3] = 0;
    // return param_1;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fc0
UNK_SMALL_STRUCT * __fastcall kcSmallStruct_ctor(UNK_SMALL_STRUCT *this)
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
    kcSmallStruct_AllocBuffers(this, 0x1000, 0x100);
    return this;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411de0
BOOL __fastcall kcSmallStruct_LockBuffers(UNK_SMALL_STRUCT *this)
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
        return FALSE;
    }
    return TRUE;
}


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412310
void __thiscall kcSmallStruct_FUN_00412310(UNK_SMALL_STRUCT *this, char *strA, char *strB)
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

    // iVar2 = std::strcmp(strA, &this->BufferA[this->SmlUnk6])
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

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411ed0
void __thiscall kcSmallStruct_AllocBuffers(UNK_SMALL_STRUCT *this, unsigned int sizeA, unsigned int sizeB)
{
    kcSmallStruct_UnlockBuffers(this);
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



///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411d80
void __fastcall kcSmallStruct_UnlockBuffers(UNK_SMALL_STRUCT *this)
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


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411e50
void __fastcall kcSmallStruct_dtor_sub(UNK_SMALL_STRUCT *this)
{
    kcSmallStruct_UnlockBuffers(this);
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

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411fa0
void __fastcall kcSmallStruct_dtor(UNK_SMALL_STRUCT *this)
{
    kcSmallStruct_dtor_sub(this);
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

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004128e0
void __cdecl mc_main(int argc, char *argv[])
{
    bool macroFound;
    int *puVar2;
    void *this;
    uint uVar3;
    int *local_174;
    // FILE_READER *unkfile2;
    // FILE_READER *unkfile;
    UNK_LARGE_STRUCT *lrgStruct;
    char local_114 [MAX_PATH]; // 260, 0x104
    // uint local_10;
    int local_c;
    char *local_8;
    
    // local_10 = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    UNK_LARGE_STRUCT *lrgStruct = (UNK_LARGE_STRUCT *)std::malloc(0xc11b0); //_newalloc(0xc11b0);
    if (lrgStruct != nullptr)
    {
        lrgStruct = kcLargeStruct_ctor(lrgStruct);
    }
    g_UNK_LARGE_STRUCT = lrgStruct;
    if (true)
    {
        bool macrotxtFound = false;
        FILE_READER *unkfile = (FILE_READER *)std::malloc(0x598); //_newalloc(0x598);
        if (unkfile != nullptr)
        {
            unkfile = kcUnkFile_ctor(unkfile, "&macro.txt");
        }
        while ((int)kcUnkFile_FindLoop(unkfile) > -1)
        {
            kcScriptCompiler_compileStart("&macro.sts", (char *)(unkfile + 9), &DAT_0041c0ee, 0, false);
            macrotxtFound = true;
        }
        if (unkfile != nullptr)
        {
            kcUnkFile_scalar_dtor(unkfile, 1);
        }
        if (!macrotxtFound)
        {
            FILE_READER *unkfile2 = (FILE_READER *)std::malloc(0x598); //_newalloc(0x598);
            if (unkfile2 != nullptr)
            {
                unkfile2 = kcUnkFile_ctor(unkfile2, "%macro.txt");
            }
            while ((int)kcUnkFile_FindLoop(unkfile2) > -1)
            {
                kcScriptCompiler_compileStart("%macro.sts", &unkfile2->Filename[0], &DAT_0041c0ef, 0, false);
            }
            if (unkfile2 != nullptr)
            {
                kcUnkFile_scalar_dtor(unkfile2, 1);
            }
        }
    }
    g_UNK_SMALL_STRUCT = nullptr;
    if (argc == 1)
    {
        std::printf("Message Compiler for CatSystem2\t\tRelease 2010.2\n");
        std::printf("usage  : mc [/u] [file_name]\n");
        ///JP: std::printf("option : /u\tアップデート\n");
        std::printf("option : /u\tupdate\n");
    }
    else
    {
        local_114[0] = '\0';
        UNK_SMALL_STRUCT *smlStruct = (UNK_SMALL_STRUCT *)std::malloc(0x20); //_newalloc(0x20);
        if (smlStruct != nullptr)
        {
            smlStruct = kcSmallStruct_ctor(smlStruct);
        }
        g_UNK_SMALL_STRUCT = smlStruct;
        // local_c = 1;
        // int index = 1;
        for (int i = 1; i < argc; i++)
        // while (local_c < argc)
        {
            const char *opt = mc_ParseCmdFlag(argv[i]);
            if (opt == nullptr)
            {
                kcScriptCompiler_doFile(argv[i], local_114);
            }
            else
            {
                if (true)
                {
                    switch(opt[0])
                    {
                    case 'D':
                    case 'd':
                        g_CMD_FLAG_D = TRUE;
                        break;
                    case 'L':
                    case 'l':
                        g_CMD_FLAG_L = TRUE;
                        break;
                    case 'U':
                    case 'u':
                        g_CMD_FLAG_U = TRUE;
                        break;
                    case 'X':
                    case 'x':
                        g_CMD_FLAG_X = FALSE;
                        break;
                    }
                }
            }
            // local_c += 1;
        }
        if (g_UNK_SMALL_STRUCT != nullptr)
        {
            kcSmallStruct_scalar_dtor(g_UNK_SMALL_STRUCT, 1); // scalar_dtor
        }
        if (g_UNK_LARGE_STRUCT != nullptr)
        {
            kcLargeStruct_scalar_dtor(g_UNK_LARGE_STRUCT, 1); // scalar_dtor
        }
    }
    // __end_security_critical();
    // return;
}

