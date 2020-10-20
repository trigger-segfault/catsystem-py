#include "common_mc.h"
#include "../ac_exe/ghidra_types_min.h"
#include "kcFileInfo_functional.h"
#include "kcCatScene_functional.h"
#include "kcDoubleBuffer_functional.h"
#include "kcLargeBuffer_functional.h"
#include "shiftjis_table.h"
// #include <windows.h>
// #include <stdlib.h>
#include <zlib.h>

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

// // kcFile open mode flags (must specify READ or WRITE, and one creation disposition)
// enum KCFILE_MODE
// {
//     // file access (specify one, names have been confirmed)
//     KCFILE_READ  = 0x1, //GENERIC_READ, FILE_SHARE_READ
//     KCFILE_WRITE = 0x2, //GENERIC_WRITE, 0
//     // _KCFILE_READWRITE = 0x3, //(helper enum, not used)

//     // creation disposition (specify one, names are unconfirmed)
//     KCFILE_OPEN     = 0x100, //OPEN_EXISTING
//     KCFILE_NEW      = 0x200, //CREATE_NEW
//     KCFILE_CREATE   = 0x400, //CREATE_NEW then TRUNCATE_EXISTING, requires KCFILE_WRITE
//     KCFILE_ALWAYS   = 0x800, //OPEN_ALWAYS

//     // This enum should include special mode flags for kcBigFile, kcMemFile doesn't use any... I think
// };

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
// enum KCLINE_TYPE
// {
//     LINE_INPUT = 0x02,
//     LINE_PAGE = 0x03,
//     LINE_MESSAGE = 0x20,
//     LINE_NAME = 0x21,
//     LINE_COMMAND = 0x30,
//     LINE_DEBUG_FILENAME = 0xf0, // LINE_TYPE_0xf0 // used with /L option
//     LINE_TYPE_0xf1 = 0xf1, // used with /L option
// };

#pragma pack(push, 1)

// This may just be the standard <zlib.h> z_stream struct,
//  but for now we'll call it this. It's constructed, has Compress called, then destructed.
//  quick and simple
// 
// may be related to global address DAT_0041ffac (which has a "size" of $38 too)
struct kcZlibStream
{
    /*$0,38*/  z_stream ZStream;
    // /*$0,38*/  unsigned char _dummy[0x38]; // dummy padding (we don't need to really understand this struct)
    /*$38*/
};

// for kcMessageCompiler fields $1c0cc4 and $1c0cc8, this is only a guess.
//  Math is never done on the state, and only 0,1,2 is every assigned.
enum THREE_STATE
{
    STATE_0 = 0,
    STATE_1 = 1,
    STATE_2 = 2,
};

// input range in a CatScene, holds the ranges of when each input is triggered
//  (based on line indexes which -> offset table -> string table)
//  This is used by CatSystem 2 for skipping, saving, and loading, etc... probably
struct INPUT_RANGE
{
    /*$0,4*/    unsigned int Length; // number of lines this input range lasts for
    /*$4,4*/    unsigned int Index; // absolute index of this input range in scene lines
    /*$8*/
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
    /*$0,400*/     char SceneName[0x400]; // unk size (0x400?) // MsgUnk_0x0
    /*$400,4*/     unsigned int InputLength; // MsgUnk_0x400
    /*$404,4*/     unsigned int MsgUnk_0x404; // not yet seen // MsgUnk_0x404
    /*$408,4*/     unsigned int OutputLineIndex; // MsgUnk_0x408
    /*$40c,4*/     unsigned int InputIndex; // MsgUnk_0x40c
    /*$410,40000*/ unsigned int OffsetsTable[0x10000]; // MsgUnk_0x410
    /*$40410,80000*/ INPUT_RANGE InputsTable[0x10000]; // MsgUnk_0x40410 (in actualaity, a two-DWORD struct)
    // /*$40410,80000*/ unsigned int InputsTable[0x20000]; // unk size // MsgUnk_0x40410 (in actualaity, a two-DWORD struct)
    // /*$40414,4*/   unsigned int MsgUnk_0x40414[1]; // unk size
    /*$c0410,100000*/ char StringTable[0x100000]; // unk size // MsgUnk_0xc0410
    /*$1c0410,4*/  unsigned int Position; // position in StringTable buffer // MsgUnk_0x1c0410
    /*$1c0414,4*/  BOOL EnableDebugLines; // MsgUnk_0x1c0414 = FALSE (g_CMD_FLAG_L)
    /*$1c0418,4*/  unsigned int MsgUnk_0x1c0418; // 0
    /*$1c041c,4*/  BOOL Flag_0x1c041c; // Flag_0x1c041c = FALSE
    /*$1c0420,4*/  BOOL EnableCompression; // MsgUnk_0x1c0420 = TRUE (g_CMD_FLAG_X)
    /*$1c0424,4*/  unsigned int ModifiedTimestamp; // param_4 (msdostime) // MsgUnk_0x1c0424
    /*$1c0428,4*/  int UpdateCount; //MsgUnk_0x1c0428
    /*$1c042c,4*/  UNK_SMALL_STRUCT *SmallStruct; // struct size 0x20 - g_UNK_SMALL_STRUCT // MsgUnk_0x1c042c
    /*$1c0430,400*/ char Filename[0x400]; // strcpy(MsgUnk_0x1c0430, filename)
    /*$1c0830,40*/ kcCatScene *SceneTable[0x10]; // MsgUnk_0x1c0830
    /*$1c0870,40*/ unsigned int UnknownTable[0x10]; // unknown pointed-to objects // MsgUnk_0x1c0870
    /*$1c08b0,4*/  int SceneCount; // MsgUnk_0x1c08b0
    /*$1c08b4,400*/ char StringBuffer[0x400]; // MsgUnk_0x1c08b4
    /*$1c0cb4,4*/  char *CurrentString; // MsgUnk_0x1c0cb4
    /*$1c0cb8,4*/  kcCatScene *CurrentScene; // MsgUnk_0x1c0cb8
    /*$1c0cbc,4*/  int CurrentLineIndex; // int? // MsgUnk_0x1c0cbc
    /*$1c0cc0,4*/  int LastLineIndex; // unsigned int? // MsgUnk_0x1c0cc0
    /*$1c0cc4,4*/  THREE_STATE ThreeState_A; // MsgUnk_0x1c0cc4
    /*$1c0cc8,4*/  THREE_STATE ThreeState_B; // MsgUnk_0x1c0cc8
    /*$1c0ccc,4*/  KCLINE_TYPE LastLineType; // MsgUnk_0x1c0ccc
    /*$1c0cd0,4*/  unsigned int MsgUnk_0x1c0cd0; // 0xffffffff // MsgUnk_0x1c0cd0
    /*$1c0cd4,4*/  BOOL CommentBlock; // inside comment block? // MsgUnk_0x1c0cd4
    /*$1c0cd8,4*/  UNK_LARGE_STRUCT *LargeStruct; // struct size 0xc11b0 - g_UNK_LARGE_STRUCT // MsgUnk_0x1c0cd8
    /*$1c0cdc,4*/  BOOL MsgUnk_0x1c0cdc; // MsgUnk_0x1c0cdc
    /*$1c0ce0*/
    
    #define MsgUnk_0x0 SceneName
    #define MsgUnk_0x400 InputLength
    // #define MsgUnk_0x404 MsgUnk_0x404
    #define MsgUnk_0x408 OutputLineIndex
    #define MsgUnk_0x40c InputIndex
    #define MsgUnk_0x410 OffsetsTable
    #define MsgUnk_0x40410 InputsTable
    #define MsgUnk_0xc0410 StringTable
    #define MsgUnk_0x1c0410 Position
    #define MsgUnk_0x1c0414 EnableDebugLines
    #define MsgUnk_0x1c0418 MsgUnk_0x1c0418
    #define MsgUnk_0x1c041c Flag_0x1c041c
    #define MsgUnk_0x1c0420 EnableCompression
    #define MsgUnk_0x1c0424 ModifiedTimestamp
    #define MsgUnk_0x1c0428 UpdateCount
    #define MsgUnk_0x1c042c SmallStruct
    #define MsgUnk_0x1c0430 Filename
    #define MsgUnk_0x1c0830 SceneTable
    #define MsgUnk_0x1c0870 UnknownTable
    #define MsgUnk_0x1c08b0 SceneCount
    #define MsgUnk_0x1c08b4 StringBuffer
    #define MsgUnk_0x1c0cb4 CurrentString
    #define MsgUnk_0x1c0cb8 CurrentScene
    #define MsgUnk_0x1c0cbc CurrentLineIndex
    #define MsgUnk_0x1c0cc0 LastLineIndex
    #define MsgUnk_0x1c0cc4 ThreeState_A
    #define MsgUnk_0x1c0cc8 ThreeState_B
    #define MsgUnk_0x1c0ccc LastLineType
    #define MsgUnk_0x1c0cd0 MsgUnk_0x1c0cd0
    #define MsgUnk_0x1c0cd4 CommentBlock
    #define MsgUnk_0x1c0cd8 LargeStruct
    #define MsgUnk_0x1c0cdc MsgUnk_0x1c0cdc
};

struct CATSCENEHDR
{
    /*$0,8*/   char Signature[8]; // "CatScene"
    /*$8,4*/   unsigned int PackSize;
    /*$c,4*/   unsigned int UnpackSize;
    /*$10*/
};
struct SCRIPTHDR
{
    /*$0,4*/   unsigned int ScriptLength;
    /*$4,4*/   unsigned int InputCount;
    /*$8,4*/   unsigned int OffsetTable;
    /*$c,4*/   unsigned int StringTable;
    /*$10*/
};

#pragma pack(pop)

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

// reference counter for FILE_READER struct,
// signed since most reference counters are signed to check for underflow
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041ff9c
// static int g_FILE_READER_COUNT = 0;

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


// some inline global structure (not a pointer) lookup table?
///USES:
/// 
/// kcScriptUnk_sub_FUN_004111f0()
///============================================
/// if ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)*local_8 * 4) & 0x40000000) == 0)
/// 
/// if ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)*pbVar2 * 4) & 0x80000000) != 0)
/// 
/// kcScriptUnk_FUN_00411770()
///============================================
/// if ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)*local_8 * 4) & 0x40000000) == 0)
/// 
/// if ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)*pbVar1 * 4) & 0x80000000) != 0)
/// 
/// kcMessageCompiler_ParseScript()
///============================================
/// if (*local_8 == '$' || ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)(unsigned char)*local_8 * 4) & 0x40000000) != 0))
// (TABLE_SHIFTJIS[(unsigned char)*local_8] & 0x40000000) != 0
/// 
/// 
/// if ((*local_8 == '$' || *local_8 == '[' || *local_8 == '<' || *local_8 == '\"') || ((unsigned char)*local_8 > 0x9f && (unsigned char)*local_8 < 0xe0) || (*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)(unsigned char)*local_8 * 4) & 0x40000000) != 0 || (bVar2 || bVar3))
// (TABLE_SHIFTJIS[(unsigned char)*local_8] & 0x40000000) != 0
/// 
/// if (((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)(unsigned char)*local_8 * 4) & 0x40000000) == 0) || ((*(unsigned int *)(&TABLE_SHIFTJIS + (unsigned int)(unsigned char)local_8[1] * 4) & 0x80000000) == 0))


///REGEX: VSCode extension CST message regex for comparison:
/// 
///SPEAKER: "^([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED][^ \t]*)[ \t]*$"
///CLEANED: ^([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED][^ \t]*)[ \t]*$
/// 
///DIALOGUE: "^([^ \t#][^ \t]*)[ \t]+([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED].*)[ \t]*$"
///CLEANED: ^([^ \t#][^ \t]*)[ \t]+([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED].*)[ \t]*$
///
///PART:1 - don't start with chars: ' ', '\t', '#'
///            and while not chars: ' ', '\t'
///PART:2 - at least one chars: ' ', '\t'
///PART:3 - none of the following chars: ' ', '\t', '\x21'-'\x5a', '\x5e'-'\xff', '\uff61'-'\uffdc', '\uffe8'-'\uffed'
///
/// 0x5b | '['
/// 0x5c | '\\'
/// 0x5d | ']'


/// 
///MONOLOGUE: "^()[ \t]+([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED].*)[ \t]*$"
///CLEANED: ^()[ \t]+([^ \t\u0021-\u005A\u005E-\u00FF\uFF61-\uFFDC\uFFE8-\uFFED].*)[ \t]*$
/// 
///MONOESCAPE: "^(\\\\)[ \t]+([^ \t].*)[ \t]*$"
///CLEANED: ^(\\)[ \t]+([^ \t].*)[ \t]*$
/// 

// May be used as a SHIFT_JIS conversion lookup table? Or table to validate message/command type? (no wait, that's exactly what it is, all of these characters also present signify just that)
// Uses constants 0x40000000 and 0x80000000 (GENERIC_WRITE and GENERIC_READ??)
// theoretically maximum size is 256 since its used with unsigned char (byte)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041f0a0
// defined in "shiftjis_table.h"
// static unsigned int TABLE_SHIFTJIS[256] = 0; // unk size //DAT_0041f0a0

// (right next to zlib strings)
// used for "&macro.sts" (ampersand)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041c0ee
static unsigned char DAT_0041c0ee = 0;
// (right next to zlib strings)
// used for "%macro.sts" (percent)
///FID:cs2_full_v401/system/scene/mc.exe: DAT_0041c0ef
static unsigned char DAT_0041c0ef = 0;



/////////////////////////////////////////////////////
//// SOME OF THE FUNCTIONS (some of them)

///GITHUB: <https://github.com/madler/zlib/blob/master/zconf.h>
//         <https://github.com/madler/zlib/blob/master/zlib.h>

// typedef struct z_stream_s {
//     /*$0,4*/   const unsigned char *next_in;     /* next input byte */
//     /*$4,4*/   unsigned int     avail_in;  /* number of bytes available at next_in */
//     /*$8,4*/   unsigned int    total_in;  /* total number of input bytes read so far */

//     /*$c,4*/   unsigned char    *next_out; /* next output byte will go here */
//     /*$10,4*/  unsigned int     avail_out; /* remaining free space at next_out */
//     /*$14,4*/  unsigned int    total_out; /* total number of bytes output so far */

//     /*$18,4*/  const char *msg;  /* last error message, NULL if no error */
//     /*$1c,4*/  struct internal_state FAR *state; /* not visible by applications */

//     /*$20,4*/  void     *zalloc;  /* used to allocate the internal state */
//     /*$24,4*/  void     *zfree;   /* used to free the internal state */
//     /*$28,4*/  void     *opaque;  /* private data object passed to zalloc and zfree */

//     /*$2c,4*/  int     data_type;  /* best guess about the data type: binary or text
//                            for deflate, or the decoding state for inflate */
//     /*$30,4*/  unsigned int   adler;      /* Adler-32 or CRC-32 value of the uncompressed data */
//     /*$34,4*/  unsigned int   reserved;   /* reserved for future use */
//     /*$38*/
// } z_stream;

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004111e0
kcZlibStream * __fastcall kcZlibStream_ctor(kcZlibStream *this)
{
    return this; // nothing here :)
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004111d0
void kcZlibStream_dtor(kcZlibStream *this)
{
    return; // nothing here :)
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00401030
void * __thiscall kcZlibStream_scalar_dtor(kcZlibStream *this, int flags)
{
    kcZlibStream_dtor(this);
    if ((flags & 1) != 0)
    {
        free(this);
    }
    return this;
}

// int __thiscall kcZlibStream_Compress(kcZlibStream *this, OUT unsigned char *dest, int destLen, IN const unsigned char *source, int sourceLen)
int __thiscall kcZlibStream_Compress(kcZlibStream *this, OUT unsigned char *dest, int destLen, IN const unsigned char *source, int sourceLen)
{
    // int iVar1;
    // int outputLen;
    unsigned char tmpBuffer[0x1000]; // local_1014 [4100];
    // uint local_10;
    // size_t local_c;
    // int zresult;
    
    this->ZStream.zalloc = nullptr;
    this->ZStream.zfree = nullptr;
    this->ZStream.opaque = nullptr;
    // *(undefined4 *)((int)this + 0x20) = 0;
    // *(undefined4 *)((int)this + 0x24) = 0;
    // *(undefined4 *)((int)this + 0x28) = 0;
    int outputLen = 0;
    int zresult = deflateInit((z_streamp)&this->ZStream, Z_BEST_COMPRESSION); // 9
    // zresult = deflateInit_((z_streamp)&this->ZStream, Z_BEST_COMPRESSION, "1.1.3", 0x38);
    // iVar1 = kcZlibStream_DeflateInit(this,9,"1.1.3",0x38);
    if (zresult == Z_OK) // == 0
    {
        this->ZStream.next_in = (Bytef *)source;
        this->ZStream.avail_in = sourceLen;
        // *(int *)this = source;
        // *(int *)((int)this + 4) = sourceLen;
        // while(*(int *)((int)this + 4) != 0)
        while (this->ZStream.avail_in != 0U)
        {
            this->ZStream.next_out = tmpBuffer;
            this->ZStream.avail_out = (unsigned int)sizeof(tmpBuffer); // 0x1000
            zresult = deflate((z_streamp)&this->ZStream, Z_NO_FLUSH); // 0
            // *(undefined **)((int)this + 0xc) = local_1014;
            // *(undefined4 *)((int)this + 0x10) = 0x1000;
            // zresult = kcZlibStream_Deflate((int *)this,0);
            if (zresult != Z_OK) // != 0
                break;
            unsigned int bytesWritten = (unsigned int)sizeof(tmpBuffer) - this->ZStream.avail_out; // 0x1000
            // local_c = 0x1000 - *(int *)((int)this + 0x10);
            // if ((int)(local_1018 + local_c) <= destLen)
            if ((int)(outputLen + bytesWritten) <= destLen)
            {
                std::memcpy(dest + outputLen, tmpBuffer, bytesWritten);
                // std::memcpy((void *)(dest + local_1018), local_1014, local_c);
            }
            outputLen += bytesWritten;
            // local_1018 += local_c;
        }
        do
        {
            if (false)
                break;

            this->ZStream.next_out = tmpBuffer;
            this->ZStream.avail_out = (unsigned int)sizeof(tmpBuffer); // 0x1000
            zresult = deflate((z_streamp)&this->ZStream, Z_FINISH); // 4
            // *(undefined **)((int)this + 0xc) = local_1014;
            // *(undefined4 *)((int)this + 0x10) = 0x1000;
            // zresult = kcZlibStream_Deflate((int *)this,4);
            unsigned int bytesWritten = (unsigned int)sizeof(tmpBuffer) - this->ZStream.avail_out; // 0x1000
            // local_c = 0x1000 - *(int *)((int)this + 0x10);
            // if ((int)(local_1018 + local_c) <= destLen)
            if ((int)(outputLen + bytesWritten) <= destLen)
            {
                std::memcpy(dest + outputLen, tmpBuffer, bytesWritten);
                // std::memcpy((void *)(dest + local_1018), local_1014, local_c);
            }
            outputLen += bytesWritten;
            // local_1018 += local_c;
        } while (zresult != Z_STREAM_END); // != 1

        deflateEnd((z_streamp)&this->ZStream);
        // kcZlibStream_DeflateEnd((int)this);
    }
    return outputLen;
    // return local_1018;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_004111f0
unsigned int kcScriptUnk_sub_FUN_004111f0(OUT char **outBuffer, IN OUT const char **str, BOOL allowEscapes)
{
    char quoteChar; //char cVar1;
    // const char *pcVar2;
    // const char *pbVar3; //byte *pbVar3;
    char *out_ptr;
    const char *in_ptr;

    unsigned int outLength = 0;
    
    in_ptr = *str; //pcVar2 = *str;

    // pcVar2 = *str;
    out_ptr = *outBuffer;
    // quoteChar = pcVar2[0];

    quoteChar = in_ptr[0]; //quoteChar = pcVar2[0];

    // check for quotes to start string
    if (quoteChar == '\"' || quoteChar == '\'')
    {
        out_ptr[0] = in_ptr[0]; //out_ptr[0] = pcVar2[0];

        // out_ptr[0] = pcVar2[0];
        out_ptr++; //out_ptr = out_ptr + 1;
        // pbVar3 = pcVar2 + 1;
        
        in_ptr++; //in_ptr = pcVar2 + 1;//pbVar3 = pcVar2 + 1;
        
        // while (in_ptr = (char *)pbVar3, pcVar2 = in_ptr, in_ptr[0] != '\0')
        while (in_ptr[0] != '\0')
        {
            // check for end of quoted string
            if (in_ptr[0] == quoteChar)
            {
                out_ptr[0] = in_ptr[0]; //*out_ptr = *in_ptr;
                out_ptr++; //out_ptr = out_ptr + 1;
                in_ptr++; //in_ptr = in_ptr + 1;
                break;
            }
            if (in_ptr[0] == '\\')
            {
                if (allowEscapes == FALSE) // no escape flag / BOOL ??
                {
                    out_ptr[0] = in_ptr[0];
                    out_ptr++; //out_ptr = out_ptr + 1;
                }
                in_ptr++; //in_ptr = in_ptr + 1;
                if (in_ptr[0] == '\0')
                    break;
                out_ptr[0] = in_ptr[0];
                out_ptr++; //out_ptr = out_ptr + 1;
                // pbVar3 = pcVar2 + 2;

                // only += 1 because of the previous increment above
                in_ptr++; //pbVar3 = pcVar2 + 2;
            }
            else
            {
                if ((TABLE_SHIFTJIS[(unsigned char)in_ptr[0]] & SJS_LEADBYTE) == 0) // & 0x40000000
                {
                    out_ptr[0] = in_ptr[0];
                    out_ptr++; //out_ptr = out_ptr + 1;
                    // pbVar3 = in_ptr + 1;

                    in_ptr++; //pbVar3 = in_ptr + 1;
                }
                else
                {
                    out_ptr[0] = in_ptr[0];
                    // pbVar3 = in_ptr + 1;

                    // in_ptr++; //pbVar3 = in_ptr + 1;

                    if ((TABLE_SHIFTJIS[(unsigned char)in_ptr[1]] & SJS_SECONDBYTE) != 0) // & 0x80000000
                    // if ((TABLE_SHIFTJIS[(unsigned char)in_ptr[0]] & 0x80000000) != 0)
                    // if ((TABLE_SHIFTJIS[(unsigned char)pbVar3[0]] & 0x80000000) != 0)
                    {
                        out_ptr[1] = in_ptr[1]; // out_ptr[1] = pbVar3[0];
                        // out_ptr[1] = in_ptr[0]; // out_ptr[1] = pbVar3[0];

                        // out_ptr[1] = pbVar3[0];
                        out_ptr += 2; //out_ptr = out_ptr + 2;
                        // pbVar3 = in_ptr + 2;
                        
                        // // only += 1 because of the previous increment above
                        // in_ptr++; // pbVar3 = in_ptr + 2;
                        
                        in_ptr += 2; // pbVar3 = in_ptr + 2;
                    }
                    else //NOTE: added as replacement for increment beforehand
                    {
                        in_ptr++;
                    }
                }
            }
        }
        // length?
        outLength = (unsigned int)(out_ptr + -(std::size_t)*outBuffer);
        *str = in_ptr;
        *outBuffer = out_ptr;
    }
    else
    {
        outLength = 0U;
        // pcVar2 = nullptr;
    }
    return outLength;
}

///FLAGS: 0x1 = allow line comments ( \\ )
// char * kcScriptUnk_FUN_00411770(IN OUT char **outBuffer, IN OUT const char **str, unsigned int flags)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_FUN_00411770
unsigned int kcScriptUnk_FUN_00411770(IN OUT char **outBuffer, IN OUT const char **str, unsigned int flags)
{
    // char *pcVar1;
    // const char *local_EAX_139;
    // Why is this local an array? Why you do this Ghidra?
    // char *local_10 [2]; //char *local_10 [2];
    // char *out_ptr; //char *local_10 [2]; //char *local_10 [2];
    // const char *in_ptr; //char *local_8;
    
    char *out_ptr = *outBuffer; //local_10[0] = *outBuffer;
    const char *in_ptr = *str; //local_EAX_139 = *str;
    while (in_ptr[0] != '\0')
    {
        // in_ptr = local_EAX_139;
        // if (in_ptr[0] == '\0')
        // {
        // LAB_004118c3:
        //     out_ptr[0] = '\0'; //local_10[0][0] = '\0'; // null-terminate output string
        //     // unsigned int outLength = (unsigned int)(local_10[0] - (int)outBuffer);
        //     unsigned int outLength = (unsigned int)(out_ptr - (int)outBuffer);
        //     // pcVar1 = *outBuffer;
        //     *str = in_ptr;
        //     *outBuffer = out_ptr; //*outBuffer = local_10[0];
        //     // return local_10[0] + -(int)pcVar1;
        //     return outLength;
        // }
        if ((unsigned char)in_ptr[0] < 0x21) // <= (unsigned char)' ' | <= 0x20 | < 0x21
        {
            out_ptr[0] = ' '; //local_10[0][0] = ' ';
            out_ptr++; //local_10[0] = local_10[0] + 1;
            do
            {
                in_ptr++; //in_ptr = in_ptr + 1;
                // local_EAX_139 = in_ptr;
                // if (in_ptr[0] == '\0')
                //     break;
            } while (in_ptr[0] != '\0' && (unsigned char)in_ptr[0] < 0x21);
        }
        else if (in_ptr[0] == '\"' || in_ptr[0] == '\'')
        {
            kcScriptUnk_sub_FUN_004111f0(&out_ptr, &in_ptr, 0);
            // kcScriptUnk_sub_FUN_004111f0(local_10, &in_ptr, 0);
            // local_EAX_139 = in_ptr;
        }
        ///SHIFTJIS: Conversion table lookup for lead byte
        else if ((TABLE_SHIFTJIS[(unsigned char)in_ptr[0]] & SJS_LEADBYTE) == 0) // & 0x40000000
        {
            // note a lead byte

            // flag involving line comments?
            if ((flags & 1) != 0 && in_ptr[0] == '/' && in_ptr[1] == '/')
            {
                // goto LAB_004118c3;
                break;
            }
            out_ptr[0] = in_ptr[0]; //local_10[0][0] = in_ptr[0];
            out_ptr++; //local_10[0]++;
            in_ptr++; // local_EAX_139 = in_ptr + 1;
        }
        else
        {
            out_ptr[0] = in_ptr[0]; //local_10[0][0] = in_ptr[0];
            // local_EAX_139 = in_ptr + 1;
            // if ((TABLE_SHIFTJIS[(unsigned char)local_EAX_139[0]] & 0x80000000) != 0)
            if ((TABLE_SHIFTJIS[(unsigned char)in_ptr[1]] & SJS_SECONDBYTE) != 0) // & 0x80000000
            {
                out_ptr[1] = in_ptr[1]; //local_10[0][1] = in_ptr[1]; // local_10[0][1] = *local_EAX_139;
                out_ptr += 2; //local_10[0] += 2; //local_10[0] = local_10[0] + 2;
                in_ptr += 2; // local_EAX_139 = in_ptr + 2;
            }
            else //NOTE: added as replacement for increment beforehand
            {
                in_ptr++;
            }
        }
    }
    //} while( true );
    
    out_ptr[0] = '\0'; //local_10[0][0] = '\0'; // null-terminate output string
    // unsigned int outLength = (unsigned int)(local_10[0] - (int)outBuffer);
    unsigned int outLength = (unsigned int)(out_ptr - (int)outBuffer);
    // pcVar1 = *outBuffer;
    *str = in_ptr;
    *outBuffer = out_ptr; //*outBuffer = local_10[0];
    // return local_10[0] + -(int)pcVar1;
    return outLength;
}

///FLAGS: 0x1 = allow line comments ( \\ )
// wrapper function for kcScriptUnk_FUN_00411770
//  (so no need to pass address of string pointer, and thus it won't be modified)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004118f0
unsigned int kcScriptUnk_FUN_004118f0_thunk_FUN_00411770(OUT char *outBuffer, IN const char *str, unsigned int flags)
{
    unsigned int uVar1;
    uVar1 = kcScriptUnk_FUN_00411770(&outBuffer, &str, flags);
    return uVar1;
}



///WINAPI: HGLOBAL GlobalAlloc (UINT, SIZE_T)
///MSDOCS: <https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globalalloc>
// GMEM_FIXED     0x0000  Allocates fixed memory. The return value is a pointer.
// GMEM_MOVEABLE  0x0002  Allocates movable memory. Memory blocks are never moved in physical memory, but they can be moved within the default heap.
//                        The return value is a handle to the memory object. To translate the handle into a pointer, use the GlobalLock function.
//                        This value cannot be combined with GMEM_FIXED.
// GMEM_ZEROINIT  0x0040  Initializes memory contents to zero.
// GPTR           0x0040  Combines GMEM_FIXED and GMEM_ZEROINIT.
// GHND           0x0042  Combines GMEM_MOVEABLE and GMEM_ZEROINIT.

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00413f10
kcMessageCompiler * __fastcall kcMessageCompiler_ctor(kcMessageCompiler *this)
{
    this->SmallStruct = nullptr; // 6
    this->SceneCount = 0; // this->MsgUnk_0x1c08b0 = 0; // 7
    this->MsgUnk_0x1c0cd0 = 0xffffffff; // 11
    this->EnableCompression = TRUE; // this->MsgUnk_0x1c0420 = 1; // 3
    this->CommentBlock = FALSE; // 12
    this->CurrentString = nullptr; // 8
    this->CurrentScene = nullptr; // this->MsgUnk_0x1c0cb8 = 0; // 9
    this->CurrentLineIndex = 0; // 10
    this->LargeStruct = nullptr; // 13
    this->MsgUnk_0x1c0cdc = FALSE; // 14
    this->MsgUnk_0x1c0418 = 0; // 1
    this->Flag_0x1c041c = FALSE; // 2
    this->EnableDebugLines = FALSE; // this->MsgUnk_0x1c0414 = 0; // 0
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
void __fastcall kcMessageCompiler_SetThreeState_A_to_B(kcMessageCompiler *this)
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
    kcCatScene *local_20;
    // int *local_10;
    
    // std::strcpy((char *)((int)this + 0x1c0430), filename);
    std::strcpy(&this->Filename[0], filename); // 0x1c0430
    // *(undefined4 *)((int)this + 0x1c0418) = 1;
    this->MsgUnk_0x1c0418 = TRUE; // *0x1c0418 = 1
    scene = (kcCatScene *)std::malloc(0x420); // (void *)_newalloc(0x420);
    if (scene != nullptr)
    {
        scene = kcCatScene_ctor(scene, filename);
    }
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
        this->CurrentLineIndex = 0;
        kcCatScene_LockBuffers(this->CurrentScene); // 0x1c0cb8
        kcMessageCompiler_ParseScript(this);
        if (this->CurrentScene != nullptr) // *0x1c0cb8 != nullptr
        {
            kcCatScene_UnlockBuffers(this->CurrentScene); // 0x1c0cb8
        }
        return TRUE;
        // BVar1 = TRUE;
    }
    // return BVar1;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412620
void __cdecl kcScriptCompiler_compileStart(IN const char *othername, IN const char *filename, IN OUT unsigned char *unused_bool, unsigned int timestamp, bool verbose)
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
    compiler->EnableDebugLines = g_CMD_FLAG_L;
    compiler->EnableCompression = g_CMD_FLAG_X;
    compiler->ModifiedTimestamp = timestamp;
    // *(BOOL *)((int)compiler + 0x1c0414) = g_CMD_FLAG_L;
    // *(BOOL *)((int)compiler + 0x1c0420) = g_CMD_FLAG_X;
    // *(undefined4 *)((int)compiler + 0x1c0424) = timestamp;
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


//unsigned int __fastcall kcMessageCompiler_FUN_00414eb0(kcMessageCompiler *this)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414eb0
int __fastcall kcMessageCompiler_FUN_00414eb0(kcMessageCompiler *this)
{
    // unsigned int uVar1;
    // size_t sVar2;
    
    this->CurrentString = nullptr;
    // *(undefined4 *)(param_1 + 0x1c0cb4) = 0;
    do
    {
        while( true )
        {
            if (this->CurrentScene == nullptr)
                goto LAB_00414ff6;

            this->LastLineIndex = this->CurrentLineIndex;
            // uVar1 = ;
            if (!kcCatScene_HasLineAt(this->CurrentScene, this->CurrentLineIndex))
                break;
            
            // some form of line copy, 1 may be number of lines
            kcCatScene_CopyLineAt(this->CurrentScene, &this->StringBuffer[0], 0x400, &this->CurrentLineIndex, 1);

            ///FLAGS: 1 == allow line comments (or is it line continuation?)
            kcScriptUnk_FUN_004118f0_thunk_FUN_00411770(&this->StringBuffer[0], &this->StringBuffer[0], 1); // 0x1c08b4, 0x1c08b4, 1
            
            // if (*(int *)(param_1 + 0x1c0cb8) == 0) goto LAB_00414ff6;
            // *(undefined4 *)(param_1 + 0x1c0cc0) = *(undefined4 *)(param_1 + 0x1c0cbc);
            // uVar1 = kcCatScene_HasLineAt(*(int *)(param_1 + 0x1c0cb8),*(int *)(param_1 + 0x1c0cbc));
            // if (uVar1 == 0) break;
            // kcCatScene_CopyLineAt(*(int *)(param_1 + 0x1c0cb8),(unsigned short *)(param_1 + 0x1c08b4),0x400,
            //             (int *)(param_1 + 0x1c0cbc),1);
            // kcScriptUnk_FUN_004118f0_thunk_FUN_00411770(param_1 + 0x1c08b4,param_1 + 0x1c08b4,1);
            if (this->StringBuffer[0] != '\0' && this->StringBuffer[0] != ' ')
            {
                this->CurrentString = &this->StringBuffer[0];
                goto LAB_00414ff6;
            }
            // if ((*(char *)(param_1 + 0x1c08b4) != '\0') && (*(short *)(param_1 + 0x1c08b4) != 0x20))
            // {
            //     *(int *)(param_1 + 0x1c0cb4) = param_1 + 0x1c08b4;
            //     goto LAB_00414ff6;
            // }
            kcMessageCompiler_SetThreeState_A_to_B(this);
        }
        if (this->CommentBlock != FALSE) // *0x1c0cd4 != 0
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
            this->CommentBlock = FALSE; // *(unk4 *)0x1c0cd4 = 0
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
        return -1; //return 0xffffffff;
    }
    else
    {
        return (int)std::strlen(this->CurrentString);
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
        kcLargeStruct_EndMacroExpansion(this->LargeStruct);
        
        this->MsgUnk_0x1c0cdc = FALSE;
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
        this->SceneCount--; // *0x1c08b0 -= 1;
        this->CurrentScene = this->SceneTable[this->SceneCount]; // *0x1c0cb8 = *(0x1c0830 + *0x1c08b0 * 4);
        this->CurrentLineIndex = this->UnknownTable[this->SceneCount]; // *0x1c0cbc = *(0x1c0870 + *0x1c08b0 * 4);
        kcCatScene_LockBuffers(this->CurrentScene); // *0x1c0cb8
        this->LastLineIndex = this->CurrentLineIndex; // *0x1c0cc0 = *0x1c0cbc;
        // *(undefined4 *)(param_1 + 0x1c0cc0) = *(undefined4 *)(param_1 + 0x1c0cbc);
        return TRUE;
    }
    else
    {
        this->CurrentString = nullptr;
        this->CurrentScene = nullptr; // this->MsgUnk_0x1c0cb8 = 0; // *0x1c0cb8 = 0
        this->CurrentLineIndex = 0;
        return FALSE;
    }
    // return (uint)bVar1;
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
            this->OffsetsTable[this->OutputLineIndex] = this->Position;
            this->OutputLineIndex++;
            this->InputLength++;
            this->StringTable[this->Position] = 1; // TRUE?
            this->Position++;
            this->StringTable[this->Position] = (char)local_8;
            this->Position++;
            this->StringTable[this->Position] = 0; // FALSE?
            this->Position++;
            this->LastLineType = (KCLINE_TYPE)local_8;
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
    if (this->OutputLineIndex > 0)
    // if (*(int *)(param_1 + 0x408) > 0)
    {
        this->InputsTable[this->InputIndex].Length = this->InputLength;
        // this->InputsTable[this->InputIndex * 2] = this->InputLength;
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
    
    if (this->SceneName[0] != '\0') // 0x0
    {
        if (this->ThreeState_A != STATE_0) // 0x1c0cc4 != 0
        {
            kcMessageCompiler_FUN_00413c30(this);
        }
        // if (*(int *)((int)this + 0x1c0cc4) != STATE_0) {
        //     FUN_00413c30((int)this);
        // }
        lineContinue = false;
        if (lineType != LINE_TYPE_0xf1 && lineType != LINE_DEBUG_FILENAME) // 0xf1, 0xf0
        {
            if (lineType == LINE_MESSAGE && this->LastLineType == LINE_MESSAGE) // 0x1c0ccc
            {
                lineContinue = true; // We're still writing the same line
            }
            this->LastLineType = lineType;
            // *(int *)((int)this + 0x1c0ccc) = lineType;
        }
        // if ((*(int *)((int)this + 0x1c041c) != 0) && (*(int *)((int)this + 0x1c0414) != 0))
        if (this->Flag_0x1c041c != FALSE && this->EnableDebugLines != FALSE) // 0x1c041c, 0x1c0414
        {
            this->Flag_0x1c041c = FALSE;
            std::sprintf(local_114, "%d", this->LastLineIndex + 1);
            kcMessageCompiler_AddLine(this, LINE_TYPE_0xf1, local_114);
            // *(undefined4 *)((int)this + 0x1c041c) = 0;
            // std::sprintf(local_114, "%d", *(int *)((int)this + 0x1c0cc0) + 1);
            // kcMessageCompiler_AddLine(this,0xf1,local_114);
        }
        // if ((*(int *)((int)this + 0x1c0418) != 0) && (*(int *)((int)this + 0x1c0414) != 0))
        if (this->MsgUnk_0x1c0418 != 0 && this->EnableDebugLines != 0) // 0x1c0418, 0x1c0414
        {
            this->MsgUnk_0x1c0418 = 0;
            kcMessageCompiler_AddLine(this, LINE_DEBUG_FILENAME, &this->CurrentScene->Filename[0]);
            // *(undefined4 *)((int)this + 0x1c0418) = 0;
            // kcMessageCompiler_AddLine(this, 0xf0, (char *)(*(int *)((int)this + 0x1c0cb8) + 0x14));
        }
        // if (*(int *)((int)this + 0x400) == 0) // 0x400
        if (this->InputLength == 0) // 0x400
        {
            this->InputsTable[this->InputIndex].Index = this->OutputLineIndex; //0x40414 or 0x40410 (offset 1)
            // this->InputsTable[this->InputIndex * 2 + 1] = this->OutputLineIndex; //0x40414 or 0x40410 (offset 1)
            // *(undefined4 *)((int)this + *(int *)((int)this + 0x40c) * 8 + 0x40414) =
            //     *(undefined4 *)((int)this + 0x408);
        }
        this->OffsetsTable[this->OutputLineIndex] = this->Position;
        // *(undefined4 *)((int)this + *(int *)((int)this + 0x408) * 4 + 0x410) =
        //     *(undefined4 *)((int)this + 0x1c0410);
        this->OutputLineIndex++;
        this->InputLength++;
        // *(int *)((int)this + 0x408) = *(int *)((int)this + 0x408) + 1;
        // *(int *)((int)this + 0x400) = *(int *)((int)this + 0x400) + 1;
        this->StringTable[this->Position] = 1;
        this->Position++;
        // *(undefined *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410) = 1;
        // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + 1;
        
        this->StringTable[this->Position] = (char)lineType;
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
            std::memcpy(&this->StringTable[this->Position], "\\n", newline_len);
            newline_len = std::strlen("\\n"); // redundant again...?

            this->Position += newline_len; // *0x1c0410 = newline_len + *0x1c0410
            // *(int *)((int)this + 0x1c0410) = newline_len + *(int *)((int)this + 0x1c0410);
        }

        // copy main content
        std::memcpy(&this->StringTable[this->Position], str, str_len);
        this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;


        // if (!lineContinue)
        // {
        //     std::memcpy(&this->StringTable[this->Position], str, str_len);
        //     this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        //     // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;
        // }
        // else
        // {
        //     unsigned int newline_len = std::strlen("\\n");
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), "\\n", newline_len);
        //     std::memcpy(&this->StringTable[this->Position], "\\n", newline_len);
        //     newline_len = std::strlen("\\n"); // redundant again...?

        //     this->Position += newline_len; // *0x1c0410 = newline_len + *0x1c0410
        //     // *(int *)((int)this + 0x1c0410) = newline_len + *(int *)((int)this + 0x1c0410);
        //     // std::memcpy((void *)((int)this + *(int *)((int)this + 0x1c0410) + 0xc0410), str, str_len);
        //     std::memcpy(&this->StringTable[this->Position], str, str_len);
        //     // *(int *)((int)this + 0x1c0410) = *(int *)((int)this + 0x1c0410) + str_len;
        //     this->Position += str_len; // *0x1c0410 = *0x1c0410 + str_len
        // }
        if (lineType == LINE_MESSAGE)
        {
            this->ThreeState_B = STATE_1; // 0x1c0cc8 = 1
            // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
        }
    }
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414be0
char * __thiscall kcMessageCompiler_call_handleLine_0x21(kcMessageCompiler *this, IN char *line)
{
    // char *pcVar1;
    char buffer [1028];
    // uint local_10;
    // char *local_c;
    // char *local_8;
    
    // local_8 = line;
    // local_c = &buffer[0];
    char *lptr = line;
    char *cptr = &buffer[0];
    // while (((unsigned char)local_8[0] > 0x20 && ((local_8[0] != -0x7f || (local_8[1] != 'u')))))
    while ((unsigned char)lptr[0] > 0x20 && (lptr[0] != '\x81' || lptr[1] != 'u')) //0x81, 0x75
    {
        cptr[0] = lptr[0];
        cptr++;
        lptr++;
    }
    lptr[0] = '\0';
    if (buffer[0] != '\0')
    {
        kcMessageCompiler_AddLine(this, LINE_NAME, &buffer[0]); // LINE_TYPE_0x21
    }
    return lptr;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414b20
char * __thiscall kcMessageCompiler_ParseName(kcMessageCompiler *this, IN char *line)
{
    char buffer [1028];
    // uint local_10;
    // char *cptr;
    // char *lptr;
    
    char *lptr = line;
    char *cptr = &buffer[0];
    // while not whitespace/control chars/null-terminator
    while ((unsigned char)lptr[0] > 0x20)
    {
        cptr[0] = lptr[0];
        cptr++;
        lptr++;
    }
    cptr[0] = '\0'; // null-terminate buffer
    // trim whitespace (from end of line)
    while (--cptr >= &buffer[0] && (cptr[0] == ' ' || cptr[0] == '\t'))
    {
        cptr[0] = '\0';
        // if (*cptr != ' ' && *cptr != '\t')
        //     break;
        // cptr--;
    }
    // while ((buffer < cptr && ((cptr = cptr + -1, *cptr == ' ' || (*cptr == '\t'))))) {
    //     *cptr = 0;
    // }
    if (buffer[0] != '\0') // is our trimmed line not empty?
    {
        kcMessageCompiler_AddLine(this, LINE_NAME, &buffer[0]); // LINE_TYPE_0x21
    }
    return lptr;
}



// void __fastcall kcScriptCompiler_writeCSTfile(char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414360
void __fastcall kcScriptCompiler_writeCSTfile(kcMessageCompiler *this)
{
    // int iVar1;
    // size_t _Size;
    // unsigned int uVar2;
    // size_t _Size_00;
    // int iVar3;
    unsigned char *scriptBuffer; // int *hMem;
    unsigned char *packBuffer; // unsigned char *inBuffer;
    // int iVar4;
    // size_t _Size_01;
    // void *local_47c;
    FILE_READER *unkfile;
    // unsigned int packLen; // unsigned int local_428;
    unsigned char local_40c [8];
    // unsigned int local_404;
    // int iStack1024;
    // unsigned int local_c;
    // int *local_8;
    
    // local_c = DAT_0041e044 ^ (unsigned int)&stack0xfffffffc;
    if (this->LargeStruct != nullptr) // 0x1c0cd8
    {
        kcLargeStruct_EndMacroExpansion(this->LargeStruct); // 0x1c0cd8
        this->MsgUnk_0x1c0cdc = FALSE;
        // *(undefined4 *)(param_1 + 0x1c0cdc) = 0;
    }
    // if (*param_1 != '\0')
    if (this->SceneName[0] != '\0')
    {
        kcMessageCompiler_FUN_00413c30(this);
        std::sprintf((char *)local_40c, "%s.cst", &this->SceneName[0]); // this (aka buffer at &this[0])
        unkfile = (FILE_READER *)std::malloc(0x598); //_newalloc(0x598);
        if (unkfile != nullptr)
        {
            unkfile = kcFileInfo_ctor(unkfile, (char *)local_40c);
        }
        // if ((*(int *)(param_1 + 0x1c0424) == 0) ||
        //     (uVar2 = kcFileInfo_GetTimestamp(unkfile), uVar2 < *(unsigned int *)(param_1 + 0x1c0424)))
        if (this->ModifiedTimestamp == 0 || // *0x1c0424
            (kcFileInfo_GetTimestamp(unkfile) < this->ModifiedTimestamp)) // < *(unsigned int *)0x1c0424
        {
            // if (*(int *)(param_1 + 0x408) == 0)
            if (this->OutputLineIndex == 0) // *0x408 == 0
            {
                kcFileInfo_Delete(unkfile); // FUN_00413b00(unkfile);
                if (unkfile != nullptr)
                {
                    kcFileInfo_scalar_dtor(unkfile, 1);
                }
            }
            else
            {
                // iVar4 = this->InputIndex; // *0x40c
                // _Size_01 = iVar4 * 8;
                // iVar1 = this->OutputLineIndex; // *0x408
                // _Size_00 = iVar1 * 4;
                // _Size = this->Position; // *0x1c0410
                // unsigned int scriptLen = _Size_01 + _Size_00 + _Size;
                unsigned int inputsLen = this->InputIndex * sizeof(INPUT_RANGE); // *0x40c
                unsigned int offsetsLen = this->OutputLineIndex * sizeof(unsigned int); // *0x408
                unsigned int stringsLen = this->Position; // *0x1c0410
                unsigned int scriptLen = inputsLen + offsetsLen + stringsLen;

                // iVar4 = *(int *)(param_1 + 0x40c);
                // _Size_01 = iVar4 * 8;
                // iVar1 = *(int *)(param_1 + 0x408);
                // _Size_00 = iVar1 * 4;
                // _Size = *(size_t *)(param_1 + 0x1c0410);
                // iVar3 = _Size_01 + _Size_00 + _Size;
                
                // Is writing directly to these HGLOBAL's even legal!??
                //   "The answer might shock you. Next on 60 minutes."
                // 
                // The answer is YES.
                // GMEM_FIXED : Allocates fixed memory. The return value is a pointer.
                //0x40 (GPTR, GMEM_FIXED | GMEM_ZEROINIT)
                scriptBuffer = (unsigned char *)::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, scriptLen + sizeof(SCRIPTHDR));
                packBuffer = (unsigned char *)::GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, scriptLen + sizeof(SCRIPTHDR));
                if (scriptBuffer == nullptr || packBuffer == nullptr)
                {
                    if (scriptBuffer != nullptr)
                    {
                        ///FIXME: Either ghidra decompiler error, or mc.exe bug
                        ::GlobalFree(packBuffer);
                        ///CORRECT: ::GlobalFree(scriptBuffer);
                    }
                    if (packBuffer != nullptr)
                    {
                        ::GlobalFree(packBuffer);
                    }
                }
                else
                {
                    ((SCRIPTHDR *)scriptBuffer)->ScriptLength = scriptLen; // (without 0x10 sizeof(SCRIPTHDR))
                    ((SCRIPTHDR *)scriptBuffer)->InputCount = this->InputIndex; // *0x40c
                    ((SCRIPTHDR *)scriptBuffer)->OffsetTable = offsetsLen;
                    ((SCRIPTHDR *)scriptBuffer)->StringTable = offsetsLen + stringsLen;
                    unsigned char *local_8 = scriptBuffer + sizeof(SCRIPTHDR); // 0x10
                    std::memcpy(local_8, &this->InputsTable[0], inputsLen);
                    local_8 += inputsLen;
                    std::memcpy(local_8, &this->OffsetsTable[0], offsetsLen);
                    local_8 += offsetsLen;
                    std::memcpy(local_8, &this->StringTable[0], stringsLen);
                    unsigned int packLen = 0U;


                    // *scriptBuffer = scriptLen;
                    // scriptBuffer[1] = this->InputIndex; // *0x40c
                    // scriptBuffer[2] = _Size_01;
                    // scriptBuffer[3] = _Size_01 + _Size_00;
                    // local_8 = scriptBuffer + 4;
                    // std::memcpy(local_8, &this->InputsTable[0], _Size_01);
                    // local_8 = local_8 + iVar4 * 2;
                    // std::memcpy(local_8, &this->OffsetsTable[0], _Size_00);
                    // local_8 = local_8 + iVar1;
                    // std::memcpy(local_8, &this->StringTable[0], _Size);
                    // local_428 = 0;

                    // *scriptBuffer = iVar3;
                    // scriptBuffer[1] = *(int *)(param_1 + 0x40c);
                    // scriptBuffer[2] = _Size_01;
                    // scriptBuffer[3] = _Size_01 + _Size_00;
                    // local_8 = scriptBuffer + 4;
                    // std::memcpy(local_8, param_1 + 0x40410, _Size_01);
                    // local_8 = local_8 + iVar4 * 2;
                    // std::memcpy(local_8, param_1 + 0x410, _Size_00);
                    // local_8 = local_8 + iVar1;
                    // std::memcpy(local_8, param_1 + 0xc0410, _Size);
                    // local_428 = 0;
                    // if (*(int *)(param_1 + 0x1c0420) != 0)
                    if (this->EnableCompression) // *0x1c0420
                    {
                        kcZlibStream *zstrm = (kcZlibStream *)std::malloc(0x38);//_newalloc(0x38);
                        if (zstrm != nullptr)
                        {
                            zstrm = kcZlibStream_ctor(zstrm);
                        }
                        ///FIXME: These parameters look a little incorrect, confirm in assembly with ghidra
                        packLen = (unsigned int)kcZlibStream_Compress(zstrm, packBuffer, scriptLen + sizeof(SCRIPTHDR), scriptBuffer, scriptLen + sizeof(SCRIPTHDR));
                        if (zstrm != nullptr)
                        {
                            kcZlibStream_scalar_dtor(zstrm, 1);
                        }
                        if ((int)packLen >= scriptLen + sizeof(SCRIPTHDR))
                        {
                            packLen = 0;
                        }
                    }
                    kcFileInfo_SetWriteMode_thunk(unkfile);
                    kcFileInfo_Open(unkfile);
                    CATSCENEHDR scenehdr;
                    std::sprintf(&scenehdr.Signature[0], "CatScene");
                    scenehdr.PackSize = packLen;
                    scenehdr.UnpackSize = scriptLen + sizeof(SCRIPTHDR);
                    kcFileInfo_Write(unkfile, local_40c, sizeof(CATSCENEHDR)); // 0x10
                    // std::sprintf((char *)local_40c, "CatScene");
                    // local_404 = packLen;
                    // iStack1024 = scriptLen + sizeof(SCRIPTHDR);
                    // kcFileInfo_Write(unkfile, local_40c, sizeof(CATSCENEHDR)); // 0x10

                    // zero compression means we write the data uncompressed
                    //  I guess this means failure *is* an option (for zlib compression)
                    if (packLen == 0U)
                    {
                        kcFileInfo_Write(unkfile, scriptBuffer, scriptLen + sizeof(SCRIPTHDR));
                    }
                    else
                    {
                        kcFileInfo_Write(unkfile, packBuffer, packLen);
                    }

                    kcFileInfo_Close(unkfile);
                    if (unkfile != nullptr)
                    {
                        kcFileInfo_scalar_dtor(unkfile, 1);
                    }
                    ::GlobalFree(packBuffer);
                    ::GlobalFree(scriptBuffer);

                    // I'm starting to think this is a bool, of all the things...
                    this->SceneName[0] = '\0'; // *(char *)this
                    this->UpdateCount++; // *0x1c0428 = *0x1c0428 + 1
                    // *param_1 = '\0';
                    // *(int *)(param_1 + 0x1c0428) = *(int *)(param_1 + 0x1c0428) + 1;
                }
            }
        }
        else
        {
            if (unkfile != nullptr)
            {
                kcFileInfo_scalar_dtor(unkfile, 1);
            }
        }
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00412730
void __cdecl kcScriptCompiler_doFile(IN const char *filename, unsigned char *unused_bool)
{
    // These are probably actually only 256 bytes in length, but MAX_PATH won't hurt
    char cst_filename[MAX_PATH]; //char local_214 [256];
    char txt_filename[MAX_PATH]; //char local_10c [260]; // MAX_PATH?
    
    // local_8 = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    FILE_READER *file = (FILE_READER *)std::malloc(0x598);
    if (file != nullptr)
    {
        file = kcFileInfo_ctor(file, filename);
    }
    
    ///FIXME: This is where .txt input extensions are enforced (we can change this later)
    kcFileInfo_ChangeExtension(file, "txt"); // change extension?

    while (kcFileInfo_FindNextValid(file) > -1) // find all files matching the pattern (or only file)
    {
        // this will force the filename to only be the basename,
        //  with possibly, some exceptions based on pattern issues with FindFirstFileA

        // exclude &macro.txt and %macro.txt files (in this case any file with those prefixes)
        if (file->Filename[0] != '%' && file->Filename[0] != '&')
        {
            unsigned int timestamp = kcFileInfo_GetTimestamp(file);
            std::strcpy(txt_filename, &file->Filename[0]);
            // And this is where .cst output extensions are enforced
            kcFileInfo_ChangeExtension(file, "cst");
            // the returned timestamp for the ".cst" file here is never used,
            //  but kcFileInfo_GetTimestamp is called *again* in kcScriptCompiler_compileStart
            kcFileInfo_GetTimestamp(file); 
            std::strcpy(cst_filename, &file->Filename[0]);
            if (g_CMD_FLAG_U == FALSE) // note update mode?
            {
                kcScriptCompiler_compileStart(cst_filename, txt_filename, unused_bool, 0, true);
            }
            else // in update mode, pass the timestamp to check if we need to update
            {
                kcScriptCompiler_compileStart(cst_filename, txt_filename, unused_bool, timestamp, true);
            }
        }
    }

    // cleanup file class
    if (file != nullptr)
    {
        kcFileInfo_scalar_dtor(file, 1);
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414940
void __thiscall kcMessageCompiler_DeclareMacroName(kcMessageCompiler *this, char *macroName)
{
    kcScriptCompiler_writeCSTfile(this);
    if (this->LargeStruct != nullptr) // *0x1c0cd8 != 0
    {
        kcLargeStruct_DeclareMacroName(this->LargeStruct, macroName); // *(void **)0x1c0cd8
        this->MsgUnk_0x1c0cdc = TRUE; // *(unk4 *)0x1c0cdc = 1
        // *(undefined4 *)((int)this + 0x1c0cdc) = 1;
    }
    // return;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414c90
void __thiscall kcMessageCompiler_SetSceneName(kcMessageCompiler *this, IN const char *newSceneName)
{
    int iVar1;
    char *local_410;
    char local_40c [1024];
    // uint local_c;
    const char *local_8;
    
    // local_c = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    kcScriptCompiler_writeCSTfile(this);
    if (this->SmallStruct != nullptr) // *0x1c042c != 0
    {
        kcSmallStruct_LockBuffers(this->SmallStruct); // *0x1c042c
        iVar1 = kcSmallStruct_IndexOf(this->SmallStruct, newSceneName); // *0x1c042c
        if (iVar1 < 0)
        {
            std::sprintf(local_40c, "%s(%d)", &this->Filename[0], this->LastLineIndex + 1); // 0x1c0430, (*(int *)0x1c0cc0) 
            kcSmallStruct_SetValue(this->SmallStruct, newSceneName, local_40c); // *0x1c042c
        }
        else
        {
            ///LOG_fmt_s_IS_DUPLICATED_IN_THE_FOLLOWING_PLACES
            ///JP: std::printf("#%s が以下の箇所で重複定義されています\n", newSceneName);
            std::printf("#%s is duplicated in the following places\n", newSceneName);
            kcSmallStruct_GetValue(this->SmallStruct, local_40c, newSceneName); // *0x1c042c
            std::printf("%s\n", local_40c);
            // this->MsgUnk_0x1c0cc0
            std::sprintf(local_40c, "%s(%d)", &this->Filename[0], this->LastLineIndex + 1); // 0x1c0430, (*(int *)0x1c0cc0) + 1
            std::printf("%s\n", local_40c);
        }
        kcSmallStruct_UnlockBuffers(this->SmallStruct); // *0x1c042c
    }
    local_8 = newSceneName;
    // What's this?? WE're FINALLY writing to other fields in the 0x0 buffer. It's not a bool!
    local_410 = &this->SceneName[0];
    while (local_8[0] > ' ' && local_8[0] < '\x7f')
    {
        local_410[0] = local_8[0];
        local_410++; //local_410 = local_410 + 1;
        local_8++; //local_8 = local_8 + 1;
    }
    local_410[0] = '\0';
    this->InputLength = 0; // *0x400 = 0
    this->OutputLineIndex = 0; // *0x408 = 0
    this->InputIndex = 0; // *0x40c = 0
    this->MsgUnk_0x404 = 0; // *0x404 = 0
    this->Position = 0; // *0x1c0410 = 0
    this->ThreeState_A = STATE_0; // *0x1c0cc4 = 0
    this->ThreeState_B = STATE_0; // *0x1c0cc8 = 0
    // *(undefined4 *)((int)this + 0x400) = 0;
    // *(undefined4 *)((int)this + 0x408) = 0;
    // *(undefined4 *)((int)this + 0x40c) = 0;
    // *(undefined4 *)((int)this + 0x404) = 0;
    // *(undefined4 *)((int)this + 0x1c0410) = 0;
    // *(undefined4 *)((int)this + 0x1c0cc4) = 0;
    // *(undefined4 *)((int)this + 0x1c0cc8) = 0;
    // __end_security_critical();
    // return;
}

//undefined4 __thiscall kcMessageCompiler_Include(int this, char *param_2)

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00414990
BOOL __thiscall kcMessageCompiler_Include(kcMessageCompiler *this, IN const char *filename)
{
    unsigned int uVar1;
    BOOL uVar2; //unsigned int uVar2; //undefined4 uVar2;
    // void *this_00;
    // int **in_FS_OFFSET;
    // unsigned int *local_24; //undefined4 *local_24;
    int *local_10;
    // undefined *puStack12;
    // undefined4 local_8;
    
    // local_8 = 0xffffffff;
    // puStack12 = &LAB_0041694b;
    // local_10 = *in_FS_OFFSET;
    // uVar1 = DAT_0041e044 ^ (uint)&stack0xfffffffc;
    // *(int ***)in_FS_OFFSET = &local_10;
    // if (*(int *)(this + 0x1c08b0) < 0x10)
    if (this->SceneCount < 0x10) // *(int *)0x1c08b0 < 0x10
    {
        // kcCatScene *this_00 = (void *)std::malloc(0x420);//_newalloc(0x420);
        // // local_8 = 0;
        // if (this_00 == nullptr)
        // {
        //     local_24 = (undefined4 *)0x0;
        // }
        // else
        // {
        //     local_24 = kcCatScene_ctor(this_00, filename);
        // }
        kcCatScene *local_24 = (kcCatScene *)std::malloc(0x420);//_newalloc(0x420);
        if (local_24 != nullptr)
        {
            local_24 = kcCatScene_ctor(local_24, filename);
        }
        // local_8 = 0xffffffff;
        if (local_24 == nullptr || local_24->LineCount == 0) // local_24[1] == 0)
        {
            ///LOG_CANNOT_INCLUDE_%s
            ///JP: std::printf("Cannot include : %s\n", filename);
            std::printf("Cannot include : %s\n", filename);
            // *(undefined4 *)(this + 0x1c0cd0) = 2;
            this->MsgUnk_0x1c0cd0 = 2; // *(unk4 *)0x1c0cd0 = 2
            uVar2 = FALSE;
        }
        else
        {
            kcCatScene_UnlockBuffers(this->CurrentScene); // *0x1c0cb8
            this->SceneTable[this->SceneCount] = this->CurrentScene;
            this->UnknownTable[this->SceneCount] = this->CurrentLineIndex;
            // *(undefined4 *)(this + 0x1c0830 + *(int *)(this + 0x1c08b0) * 4) = *(undefined4 *)(this + 0x1c0cb8);
            // *(undefined4 *)(this + 0x1c0870 + *(int *)(this + 0x1c08b0) * 4) = *(undefined4 *)(this + 0x1c0cbc);
            // *(int *)(this + 0x1c08b0) = *(int *)(this + 0x1c08b0) + 1;
            // *(undefined4 **)(this + 0x1c0cb8) = local_24;
            // *(undefined4 *)(this + 0x1c0cbc) = 0;
            this->SceneCount++;
            this->CurrentScene = local_24; // *(unk4 **)0x1c0cb8 = local_24
            this->CurrentLineIndex = 0; // *(unk4 *)0x1c0cbc = 0
            kcCatScene_LockBuffers(this->CurrentScene); // *0x1c0cb8
            // *(undefined4 *)(this + 0x1c0cc0) = *(undefined4 *)(this + 0x1c0cbc);
            // *(undefined4 *)(this + 0x1c0418) = 1;
            this->LastLineIndex = this->CurrentLineIndex; // *(unk4 *)0x1c0cc0 = *(unk4 *)0x1c0cbc
            this->MsgUnk_0x1c0418 = 1; // *(unk4 *)0x1c0418 = 1
            uVar2 = TRUE;
        }
    }
    else
    {
        ///LOG_INCLUDE_NESTING_TOO_DEEP_MULTIPLE_TOO_MANY
        ///JP: std::printf("インクルードのネストが深すぎます（多重インクルードし過ぎです）\n");
        std::printf("Includes are too deeply nested (multiple includes)\n");
        // *(undefined4 *)(this + 0x1c0cd0) = 1;
        this->MsgUnk_0x1c0cd0 = 1; // *(unk4 *)0x1c0cd0 = 1
        uVar2 = FALSE;
    }
    // *in_FS_OFFSET = local_10;
    return uVar2;
}


///FID:cs2_full_v401/system/scene/mc.exe: FUN_00415020
BOOL __fastcall kcMessageCompiler_ParseScript(kcMessageCompiler *this)
{
    char bVar1; //byte bVar1;
    bool bVar2;
    bool bVar3;
    size_t sVar4;
    int iVar5;
    BOOL BVar6;
    char *local_8; //byte *local_8;
    
    this->LastLineType = NO_LINE_TYPE; //LINE_NONE; //(KCLINE_TYPE)0; // *(unk4 *)0x1c0ccc = 0
    this->ThreeState_A = STATE_0; // *(unk4 *)0x1c0cc4 = 0
    this->ThreeState_B = STATE_0; // *(unk4 *)0x1c0cc8 = 0
    this->SceneName[0] = '\0'; // *(unk1 *)0x0 = 0
    // *(undefined4 *)((int)this + 0x1c0ccc) = 0;
    // *(undefined4 *)((int)this + 0x1c0cc4) = 0;
    // *(undefined4 *)((int)this + 0x1c0cc8) = 0;
    // *(undefined *)this = 0;
    // while (*(int *)((int)this + 0x1c0cb8) != 0)
    while (this->CurrentScene != 0) // *(int *)0x1c0cb8 != 0
    {
        sVar4 = kcMessageCompiler_FUN_00414eb0(this);
        if ((int)sVar4 <= 0)
            continue;
        // if ((int)sVar4 > 0)
        // {
        bVar2 = false;
        local_8 = &this->CurrentString[0]; // *(char **)0x1c0cb4
        // local_8 = *(byte **)((int)this + 0x1c0cb4);
        // if (local_8[0] == 0x2f && local_8[1] == 0x2a)
        
        ///// HANDLE COMMENTS /////
        if (local_8[0] == '/' && local_8[1] == '*') // 0x2f, 0x2a
        {
            // if (*(int *)((int)this + 0x1c0cd4) == 0)
            if (this->CommentBlock == FALSE) // *(int *)0x1c0cd4 == 0
            {
                // comment block state?
                this->CommentBlock = TRUE; // *(unk4 *)0x1c0cd4 = 1
                // *(undefined4 *)((int)this + 0x1c0cd4) = 1;
            }
            else
            {
                ///LOG_COMMENT_BLOCK_ALREADY_STARTED
                ///JP: std::printf("２重にコメントアウトされています：%s\n", this->CurrentScene->Filename);
                std::printf("Comment block already started : %s\n", this->CurrentScene->Filename);
            }
        }
        // if (local_8[0] == 0x2a && local_8[1] == 0x2f)
        else if (local_8[0] == '*' && local_8[1] == '/') // 0x2a, 0x2f
        {
            // if (*(int *)((int)this + 0x1c0cd4) == 0)
            if (this->CommentBlock == FALSE) // *(int *)0x1c0cd4 == 0
            {
                ///LOG_COMMENT_BLOCK_EXTRA_CLOSURE
                ///JP: std::printf("コメントアウトの閉じが余分です：%s\n", this->CurrentScene->Filename);
                std::printf("Comment block extra closure : %s\n", this->CurrentScene->Filename);
            }
            else
            {
                // comment block state?
                this->CommentBlock = TRUE; // *(unk4 *)0x1c0cd4 = 1
            }
        }
        else if (this->CommentBlock != FALSE) // *(int *)0x1c0cd4 == 0
            continue;
        
        // // if (*(int *)((int)this + 0x1c0cd4) == 0) // *(int *)0x1c0cd4 == 0
        // else if (this->CommentBlock == FALSE) // *(int *)0x1c0cd4 == 0
        // {
        
        // if (local_8[0] == 0x23)
        if (local_8[0] == '#') // 0x23
        {
            // iVar5 = std::memcmp(local_8 + 1, "include", 7);
            // if (iVar5 == 0)
            // + 1 for: // std::strlen("#")
            if (std::memcmp(local_8 + 1, "include", 7) == 0)
            {
                // whitspace check
                // if (local_8[8] == 0x20 || local_8[8] == 9)
                if (local_8[8] == ' ' || local_8[8] == '\t') // 0x20, 0x9
                {
                    // + 9 for: // std::strlen("#") + std::strlen("include") + std::strlen(" ")
                    kcMessageCompiler_Include(this, local_8 + 9);
                }
            }
            else
            {
                // + 1 for: // std::strlen("#")
                kcMessageCompiler_SetSceneName(this, local_8 + 1);
            }
        }
        // if (local_8[0] == 0x25)
        else if (local_8[0] == '%') // 0x25
        {
            // Hardcoded %echo function during compile
            if (std::memcmp(local_8 + 1, "echo", 4) == 0)
            {
                if (local_8[5] == ' ' || local_8[5] == '\t')
                {
                    std::printf("%echo : %s\n", local_8 + 6);
                }
            }
            else
            {
                // first char in line is '%', macro definition!
                kcMessageCompiler_DeclareMacroName(this, local_8 + 1);
            }
        }
        else if (this->MsgUnk_0x1c0cdc != FALSE)
        {
            kcLargeStruct_FUN_004133a0(this->LargeStruct, local_8);
            // kcLargeStruct_FUN_004133a0(*(void **)((int)this + 0x1c0cd8), (char *)local_8);
        }
        else //if (this->MsgUnk_0x1c0cdc == FALSE) // *(int *)0x1c0cdc == 0
        {
            this->MsgUnk_0x1c0cdc = TRUE; //*(unk4 *)0x1c041c = 1
            // *(undefined4 *)((int)this + 0x1c041c) = 1;
            // if ((local_8[0] != 0x20) && (true)) // 0x20 // && true is possibly some preprocessor flag?
            if (local_8[0] != ' ' && (true)) // 0x20 // && true is possibly some preprocessor flag?
            {
                // if ((local_8[0] == 0x24) ||
                //    ((TABLE_SHIFTJIS[local_8[0]] & 0x40000000) != 0))
                if (local_8[0] == '$' || // 0x24
                    ((TABLE_SHIFTJIS[local_8[0]] & SJS_LEADBYTE) != 0)) // & 0x40000000
                {
                    local_8 = kcMessageCompiler_call_handleLine_0x21(this, local_8);
                }
                else if (local_8[0] == '\\') // 0x5c
                {
                    local_8++; //local_8 = local_8 + 1;
                    if (local_8[0] == ' ' || local_8[0] == '\t') // 0x20, 0x9
                    {
                        bVar2 = true;
                    }
                }
                else
                {
                    local_8 = kcMessageCompiler_ParseName(this, local_8);
                    bVar2 = true;
                }
            }

            while ((char)local_8[0] > '\0' && (char)local_8[0] < '!')
            {
                local_8++; //local_8 = local_8 + 1;
            }
            if (local_8[0] == '\0') // 0x0
                continue;

            // if (local_8[0] != '\0') // 0x0
            // {
            bVar3 = false;
            if (local_8[0] == '\\') // 0x5c
            {
                bVar1 = local_8[1];
                // if ((((char)bVar1 < '0') || ((char)bVar1 > '9')) && (bVar1 != 0x28))
                // The '(' check here is because variables can be
                //   referenced with extra math, like pointer math :D
                // EXAMPLE: \(10+#7) // this accesses localvar[10 + globalvar[7]]
                if (((char)bVar1 < '0' || (char)bVar1 > '9') && bVar1 != '(') // < '0', > '9', != 0x28
                {
                    bVar3 = true; // escape next char
                }
                else
                {
                    bVar3 = false; // no escape, we doing variables!
                }
            }

            ///SHIFTJIS: Change when upgrading to UTF-8? (this may actually be specifically for message detection)
            if ((local_8[0] == '$' || local_8[0] == '[' || // 0x24, 0x5b
                local_8[0] == '<' || local_8[0] == '\"') || // 0x3c, 0x22
                ((((unsigned char)local_8[0] > 0x9f && (unsigned char)local_8[0] < 0xe0)) || // > 0x9f, < 0xe0 (SHIFT JIS)
                (((TABLE_SHIFTJIS[local_8[0]] & SJS_LEADBYTE) != 0 || (bVar2 || bVar3))))) // & 0x40000000
            {
                // if (local_8[0] == 0x5c && local_8[1] == 0x40 && local_8[2] != 0)
                if (local_8[0] == '\\' && local_8[1] == '@' && local_8[2] != '\0') // == 0x5c, == 0x40, != 0
                {
                    // keep going, I think \@ escapes are ignored if not the end of the message ??
                    //  (or is this the beginning?)
                    local_8 += 2;
                }
                // iVar5 = std::strcmp(local_8, "\\p");
                ///SHIFTJIS: These CJK ansi characters *may* need to be changed before compiling
                if (std::strcmp(local_8, "\\p") == 0 ||
                    std::strcmp(local_8, "\\P") == 0 ||
                    std::strcmp(local_8, "￥ｐ") == 0 || // "\x81\x8f\x82\x90"
                    std::strcmp(local_8, "￥Ｐ") == 0 || // "\x81\x8f\x82\x6f"
                    std::strcmp(local_8, "￥せ") == 0)   // "\x81\x8f\x82\xb9"
            //   iVar5 = std::strcmp((char *)local_8, "\\p");
            //   if (((iVar5 == 0 || (iVar5 = std::strcmp((char *)local_8, "\\P"), iVar5 == 0)) ||
            //       (iVar5 = std::strcmp((char *)local_8, "￥ｐ"), iVar5 == 0)) ||
            //      ((iVar5 = std::strcmp((char *)local_8, "￥Ｐ"), iVar5 == 0 ||
            //       (iVar5 = std::strcmp((char *)local_8, "￥せ"), iVar5 == 0))))
            //   if ((((iVar5 == 0) || (iVar5 = std::strcmp((char *)local_8, "\\P"), iVar5 == 0)) ||
            //       (iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_p), iVar5 == 0)) ||
            //      ((iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_P), iVar5 == 0 ||
            //       (iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_UNKP), iVar5 == 0))))
                {
                    // if (*(int *)((int)this + 0x1c0cc8) != 0)
                    if (this->ThreeState_B != STATE_0) // *(int *)0x1c0cc8 != 0
                    {
                        // STATE_2 likely has relations to PAGE INPUT (paragraph, used in NVL (novel) mode)
                        this->ThreeState_B = STATE_2; // *(unk4 *)0x1c0cc8 = 2
                        // *(undefined4 *)((int)this + 0x1c0cc8) = 2;
                        kcMessageCompiler_SetThreeState_A_to_B(this);
                    }
                }
                // iVar5 = std::strcmp(local_8, "\\n");
                ///SHIFTJIS: These CJK ansi characters *may* need to be changed before compiling
                else if (std::strcmp(local_8, "\\n") == 0 ||
                    std::strcmp(local_8, "\\N") == 0 ||
                    std::strcmp(local_8, "￥ｎ") == 0 || // "\x81\x8f\x82\x8e"
                    std::strcmp(local_8, "￥Ｎ") == 0 || // "\x81\x8f\x82\x6d"
                    std::strcmp(local_8, "￥み") == 0)   // "\x81\x8f\x82\xdd"
                // iVar5 = std::strcmp((char *)local_8, "\\n");
                // if (((iVar5 == 0 ||
                //      ((iVar5 = std::strcmp((char *)local_8, "\\N"), iVar5 == 0 ||
                //       (iVar5 = std::strcmp((char *)local_8, "￥ｎ"), iVar5 == 0)))
                //      ) || (iVar5 = std::strcmp((char *)local_8, "￥Ｎ"),
                //           iVar5 == 0)) ||
                //    (iVar5 = std::strcmp((char *)local_8, "￥み"), iVar5 == 0))
                // if ((((iVar5 == 0) ||
                //      ((iVar5 = std::strcmp((char *)local_8, "\\N"), iVar5 == 0 ||
                //       (iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_n), iVar5 == 0)))
                //      ) || (iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_N),
                //           iVar5 == 0)) ||
                //    (iVar5 = std::strcmp((char *)local_8, TOKEN_FULLWIDTH_YEN_UNKN), iVar5 == 0))
                {
                    // Add empty message line (used to force input, I think?)
                    kcMessageCompiler_AddLine(this, LINE_MESSAGE, ""); // 0x20
                    this->ThreeState_B = STATE_1; // *(unk4 *)0x1c0cc8 = 1
                    // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
                }
                else
                {
                    kcMessageCompiler_AddLine(this, LINE_MESSAGE, local_8); // 0x20
                    while (local_8[0] != '\0')
                    {
                        if ((TABLE_SHIFTJIS[local_8[0]] & SJS_LEADBYTE) == 0 || // & 0x40000000
                            (TABLE_SHIFTJIS[local_8[1]] & SJS_SECONDBYTE) == 0)   // & 0x80000000
                        {
                            if (local_8[0] == '\\' && local_8[1] != '\0') // 0x5c, 0x0
                            {
                                if (local_8[1] == '\\' || local_8[1] == '@') // 0x5c, 0x40
                                {
                                    this->ThreeState_B = STATE_0; // *(unk4 *)0x1c0cc8 = 0
                                    // *(undefined4 *)((int)this + 0x1c0cc8) = 0;
                                }
                                else
                                {
                                    this->ThreeState_B = STATE_1; // *(unk4 *)0x1c0cc8 = 1
                                    // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
                                }
                                local_8 += 2; //local_8 = local_8 + 2;
                            }
                            else
                            {
                                // if (local_8[0] > 0x20)
                                if ((unsigned char)local_8[0] > (unsigned char)' ') // 0x20
                                {
                                    this->ThreeState_B = STATE_1; // *(unk4 *)0x1c0cc8 = 1
                                    // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
                                }
                                local_8++; //local_8 = local_8 + 1;
                            }
                        }
                        else
                        {
                            this->ThreeState_B = STATE_1; // *(unk4 *)0x1c0cc8 = 1
                            // *(undefined4 *)((int)this + 0x1c0cc8) = 1;
                            local_8 += 2; //local_8 = local_8 + 2;
                        }
                    }
                }
            }
            else if (local_8[0] == '%') // 0x25
            {
                // if (*(int *)((int)this + 0x1c0cd8) != 0)
                if (this->LargeStruct != nullptr) // *0x1c0cd8 != 0
                {
                    // BVar6 = kcLargeStruct_BeginMacroExpansion(*(void **)((int)this + 0x1c0cd8), (char *)(local_8 + 1));
                    if (kcLargeStruct_BeginMacroExpansion(this->LargeStruct, local_8 + 1) == FALSE)
                    {
                        // LOG_MACRO_EXPANSION_ERROR_fmt_s
                        ///JP: std::printf("マクロ展開エラー：%s\n", local_8 + 1);
                        std::printf("Macro expansion error : %s\n", local_8 + 1);
                    }
                    else
                    {
                        // while (sVar4 = kcLargeStruct_ExpandMacro(*(void **)((int)this + 0x1c0cd8), *(char **)((int)this + 0x1c0cb4)), (int)sVar4 > -1)
                        while (kcLargeStruct_ExpandMacro(this->LargeStruct, this->CurrentString) > -1) // *(void **)0x1c0cd8, *(char **)0x1c0cb4
                        {
                            local_8 = this->CurrentString; //*(byte **)0x1c0cb4
                            // local_8 = *(byte **)((int)this + 0x1c0cb4);
                            while ((char)local_8[0] > '\0' && (char)local_8[0] < '!')
                            {
                                local_8++; //local_8 = (byte *)((char *)local_8 + 1);
                            }
                            if (local_8[0] != '\0')
                            {
                                // can macros only expand to normal commands? (not messages/names/input?)
                                kcMessageCompiler_AddLine(this, LINE_COMMAND, local_8); // 0x30
                            }
                        }
                        kcLargeStruct_EndMacroExpansion(this->LargeStruct); // *(int **)0x1c0cd8
                    }
                }
            }
            else
            {
                // catch-all, if it ain't a Name, Message, Input, or Paragraph, it's a Command! :3
                kcMessageCompiler_AddLine(this, LINE_COMMAND, local_8); // 0x30
            }
        }
    }
    kcMessageCompiler_SetThreeState_A_to_B(this);
    kcScriptCompiler_writeCSTfile(this);
    //return 1; // TRUE?
    return TRUE;
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

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004128e0
void __cdecl mc_main(int argc, char *argv[])
{
    bool macroFound;
    int *puVar2;
    void *this;
    unsigned int uVar3;
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
            unkfile = kcFileInfo_ctor(unkfile, "&macro.txt");
        }
        while ((int)kcFileInfo_FindNextValid(unkfile) > -1)
        {
            kcScriptCompiler_compileStart("&macro.sts", (char *)(unkfile + 9), &DAT_0041c0ee, 0, false);
            macrotxtFound = true;
        }
        if (unkfile != nullptr)
        {
            kcFileInfo_scalar_dtor(unkfile, 1);
        }
        if (!macrotxtFound)
        {
            FILE_READER *unkfile2 = (FILE_READER *)std::malloc(0x598); //_newalloc(0x598);
            if (unkfile2 != nullptr)
            {
                unkfile2 = kcFileInfo_ctor(unkfile2, "%macro.txt");
            }
            while ((int)kcFileInfo_FindNextValid(unkfile2) > -1)
            {
                kcScriptCompiler_compileStart("%macro.sts", &unkfile2->Filename[0], &DAT_0041c0ef, 0, false);
            }
            if (unkfile2 != nullptr)
            {
                kcFileInfo_scalar_dtor(unkfile2, 1);
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
                ///TODO: re-evaluate the unused_bool thing
                kcScriptCompiler_doFile(argv[i], (unsigned char *)&local_114[0]);
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

