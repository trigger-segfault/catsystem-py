#include <cassert>
#include <cstdio>
#include <memory>
//#include <afxmt.h>
//#include <atlstr.h>

//Temporary defines, used during unobfuscation
#define BYTE unsigned char
#define BOOL int
#define INT int
#define UINT unsigned int
#define LONG long
#define ULONG unsigned long
#define VOID void
#define CHAR char
#define FLOAT float
#define LPCSTR const char*
#define FALSE 0
#define TRUE 1
// undetermined signedness
#define undefined4 unsigned int

// Google Translste: "kcAnmScript : Command error"
#define LOG_kcAnmScript_command_error "kcAnmScript : コマンドエラー"
// Google Translate: "kcAnmScript : Data format is different"
#define LOG_kcAnmScript_different_data_format "kcAnmScript : データ形式が違います"
// Google Translate: "kcAnmScript : Timeline count is invalid"
#define LOG_kcAnmScript_invalid_timeline_count "kcAnmScript : タイムライン数が不正です"
// Google Translate: "kcAnmScript : Data size is invalid"
#define LOG_kcAnmScript_invalid_data_size "kcAnmScript : データサイズが不正です"

// "ANM" (little endian)
#define MAGIC_ANM 0x4d4e41

//undefined4 DAT_007a0c44    ??
#define DAT_007a0c44  (unsigned int *)0x007a0c44

//Used to translate CMD #defines to an enum
//REGEX: ^#define (CMD_([^ ]*)) (\d+)$
//REPLACE:   $1 = $3,

//Used to prefix struct fields with offsets
//REGEX: ^  ([A-Za-z_][A-Za-z_0-9]*)( +)(\*?[A-Za-z_][A-Za-z_0-9]*(?:\[[0-9]+\])?); \/\/ \[([0-9]+:[0-9]+)\](.*)$
//REPLACE:   /* [$4] */ $1$2$3; //$5
//REGEX: \/\*([0-9]+):([0-9]+)\*\/
//REPLACE: /* $1:$2 */
//REPLACE: /*$1,4*/

// COMMAND/ARG ENUMS:

typedef enum {
  CMD_ID    = 0, // [ID] [min] (max)
  CMD_SET   = 1, // set [var] [min] (max)
  CMD_LOOP  = 2, // loop [var] [label]
  CMD_JUMP  = 3, // jump [label]
  CMD_IF    = 4, // if [var] [label]
  CMD_IFE   = 5, // ife [var] [value] [label]
  CMD_IFN   = 6, // ifn [var] [value] [label]
  CMD_IFG   = 7, // ifg [var] [value] [label]
  CMD_IFS   = 8, // ifs [var] [value] [label]
  CMD_IFGE  = 9, // ifge [var] [value] [label]
  CMD_IFSE  = 10, // ifse [var] [value] [label]
  CMD_MAX   = 11, // max [var]
  CMD_BLEND = 12, // blend [param]
  CMD_DISP  = 13, // disp [param]
  CMD_POS   = 14, // pos [x] [y]
  CMD_WAIT  = 15, // wait [min] [max]
  CMD_ADD   = 16, // add [var] [value]
  CMD_SUB   = 17  // sub [var] [value]
} ANM_CMD;

typedef enum {
  TYPE_CONST    = 0, // 16
  TYPE_VARIABLE = 1, // @16
  TYPE_LABEL    = 2  // MyLabel
} VAR_TYPE;


// KCANMSCRIPT STRUCTS:

#pragma pack(push, 1)

// (field offset,size prefixes are in hex)

typedef struct {
  /*0,4*/   VAR_TYPE VarType;
  /*4,4*/   UINT Value;
  /*8*/
} ANM_ARG;

typedef struct {
  /*0,4*/   ANM_CMD CmdType;
  /*4,40*/  ANM_ARG Args[8];
  /*44*/
} ANM_TIMELINE;

typedef struct {
  // /*0,18*/  CHAR UnkName[24]; // 0x0:0x16?
  /*0,18*/  UINT padding[6];
  /*18,1c*/ INT **Field72Unk6ptr;
  /*1c,20*/ INT Field72Unk7;
  /*20*/
} ANM_UNK72;

typedef struct {
  /*0,4*/   UINT TimelineCount; // Number of timelines
  // /*4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
  /*4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
  /*8,4*/   UINT Unk2; // (unknown)
  /*c,4*/   UINT Unk3; // (unknown)
  /*10,4*/  UINT Counter; // used in CMD_ID, CMD_WAIT, and during command loop
  /*14,4*/  INT Instruction; // instruction ptr index
  /*18,4*/  UINT Wait; // used in CMD_ID, CMD_WAIT
  /*1c,4*/  UINT MaxFrame; // used in CMD_MAX
  /*20,ff*/ INT Variables[64]; // live variables
  /*120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
  /*124,4*/ FLOAT Unk73; // (unknown)
  /*128,4*/ INT FrameID; // current frame
  /*12c,4*/ INT Blend; // used in CMD_BLEND
  /*130,4*/ BOOL Disp; // used in CMD_DISP
  /*134,4*/ INT PosX; // used in CMD_POS
  /*138,4*/ INT PosY; // used in CMD_POS
  /*140*/
} ANM_SCRIPT;

typedef struct {
  /*0,4*/   UINT Magic; // "ANM" MAGIC_ANM
  /*4,4*/   UINT AnmUnk1;
  /*8,4*/   UINT TimelineCount;
  /*c,4*/   UINT AnmUnk3;
  /*10,4*/  UINT AnmUnk4;
  /*14,4*/  UINT AnmUnk5;
  /*18,4*/  UINT AnmUnk6;
  /*1c,4*/  UINT AnmUnk7;
  /*20*/
} ANM_FILEHEADER;

typedef struct {
  /*0,20*/  ANM_FILEHEADER Header;
  /*20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
} ANM_FILEINFO;

// static_assert(sizeof(ANM_ARG) == 0x8, "ANM_ARG struct size is not 8 bytes");
// static_assert(sizeof(ANM_TIMELINE) == 0x44, "ANM_TIMELINE struct size is not 68 bytes");
// static_assert(sizeof(ANM_FILEHEADER) == 0x20, "ANM_FILEHEADER struct size is not 32 bytes");

class kcAnmScript {
private:
  /*0,4*/   UINT TimelineCount; // Number of timelines
  // /*4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
  /*4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
  /*8,4*/   UINT Unk2; // (unknown)
  /*c,4*/   UINT Unk3; // (unknown)
  /*10,4*/  UINT Counter; // used in CMD_ID, CMD_WAIT, and during command loop
  /*14,4*/  INT Instruction; // instruction ptr index
  /*18,4*/  UINT Wait; // used in CMD_ID, CMD_WAIT
  /*1c,4*/  UINT MaxFrame; // used in CMD_MAX
  /*20,ff*/ INT Variables[64]; // live variables
  /*120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
  /*124,4*/ FLOAT Unk73; // (unknown)
  /*128,4*/ INT FrameID; // current frame
  /*12c,4*/ INT Blend; // used in CMD_BLEND
  /*130,4*/ BOOL Disp; // used in CMD_DISP
  /*134,4*/ INT PosX; // used in CMD_POS
  /*138,4*/ INT PosY; // used in CMD_POS

//void __fastcall
//FUN_00597870(undefined4 param_1,int param_2,int *param_1_00,byte *param_2_00,undefined4 param_3,
//            undefined4 param_4,undefined4 param_5,undefined4 param_6,undefined4 param_7)

  //[004c0c90] kcAnmScript_dtor
  //[00597500] FUN_00597500
  //[005975c0] FUN_005975c0
  //[00597780] FUN_00597780
  //[00597870] FUN_00597870
  //[00597ba0] FUN_00597ba0
  //[00597cf0] kcAnmScript_execute
  //[005984d0] FUN_005984d0
  //[00598500] kcAnmScript_reset
  //[00598540] kcAnmScript_dtor_sub
  //[005985e0] kcAnmScript_readFile
  //[005986f0] kcAnmScript_new
  //[00598860] FUN_00598860
  //[005992a0] kcAnmScript_ParseFile
  
  //[005c0970] cs2_script_lookup_initfunc_005c0970


public:
  //kcAnmScript_new [005986f0]
  kcAnmScript() {
    this->TimelineCount = 0; // *unaff_ESI = 0;
    this->Timelines = NULL; // unaff_ESI[1] = 0;
    this->Unk2 = 0; // unaff_ESI[2] = 0;
    this->Unk3 = 0; // unaff_ESI[3] = 0;
    this->Counter = 0; // unaff_ESI[4] = 0;
    this->Instruction = 0; // unaff_ESI[5] = 0;
    this->Wait = 0; // unaff_ESI[6] = 0;
    this->MaxFrame = 0; // unaff_ESI[7] = 0;
    for (INT i = 0; i < 64; i++) {
      this->Variables[i] = 0;
    }

    this->Unk73 = 1.00000000; //0x3f800000; // unaff_ESI[0x49] = 0x3f800000;
    this->FrameID = 0; // unaff_ESI[0x4a] = 0;
    this->Blend = 255; // unaff_ESI[0x4b] = 0xff;
    this->Disp = TRUE; // unaff_ESI[0x4c] = 1;
    this->PosX = 0; // unaff_ESI[0x4d] = 0;
    this->PosY = 0; // unaff_ESI[0x4e] = 0;

    // stl allocation
    this->Unk72stl = new ANM_UNK72(); //unaff_ESI[0x48] = iVar2;
    // this->Unk72stl = NULL; // unaff_ESI[0x48] = 0;
  }

  //kcAnmScript_ParseFile [005992a0]
  //kcAnmScript_dtor [004c0c90]
  
  //kcAnmScript_dtor_sub [00598540]
  ~kcAnmScript() {
    INT *pUnkStruct;
    this->TimelineCount = 0; // *this = 0;
    if (this->Timelines != NULL) {
      delete[] this->Timelines;
      this->Timelines = NULL;
    }
    this->Unk2 = 0;          // this[2] = 0;
    this->Unk3 = 0;          // this[3] = 0;
    this->Counter = 0;       // this[4] = 0;
    this->Wait = 0;          // this[6] = 0;
    for (INT i = 0; i < 64; i++) {
      this->Variables[i] = 0;
    }
    
    // Some stl deallocation
    delete this->Unk72stl;
    // this->Unk72stl = NULL;
    this->Unk72stl->Field72Unk6ptr[1] = (INT *) this->Unk72stl->Field72Unk6ptr;
    this->Unk72stl->Field72Unk7 = 0;
    this->Unk72stl->Field72Unk6ptr[0] = (INT *) this->Unk72stl->Field72Unk6ptr;
    this->Unk72stl->Field72Unk6ptr[2] = (INT *) this->Unk72stl->Field72Unk6ptr;
    // *(INT *)(*(INT *)(iVar1 + 0x18) + 4) = *(INT *)(iVar1 + 0x18);
    // *(undefined4 *)(iVar1 + 0x1c) = 0;
    // *(undefined4 *)*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(iVar1 + 0x18);
    // *(INT *)(*(INT *)(iVar1 + 0x18) + 8) = *(INT *)(iVar1 + 0x18);

    this->FrameID = 0; // this[0x4a] = 0;
    this->Blend = 255; // this[0x4b] = 0xff;
    this->Disp = TRUE; // this[0x4c] = 1;
    this->PosX = 0; // this[0x4d] = 0;
    this->PosY = 0; // this[0x4e] = 0;
  }

  //kcAnmScript_readFile [005985e0]
  BOOL readFile(ANM_FILEINFO *fileData, UINT fileSize) {
    ANM_FILEHEADER *fileHeader = &fileData->Header;
    
    //kcAnmScript_dtor_sub();
    kcAnmScript_dtor_sub(this);
    if (fileHeader->Magic != MAGIC_ANM) { // "ANM"
      kclib_logError(LOG_kcAnmScript_different_data_format);
      return FALSE;
    }
    if (fileHeader->TimelineCount == 0) {
      kclib_logError(LOG_kcAnmScript_invalid_timeline_count);
      return FALSE;
    }
    //if (fileSize < fileHeader->TimelineCount * 0x44 + 0x20U)
    if (fileSize < fileHeader->TimelineCount * sizeof(ANM_TIMELINE) + sizeof(ANM_FILEHEADER)) {
      kclib_logError(LOG_kcAnmScript_invalid_data_size);
      return FALSE;
    }
    this->Unk2 = fileHeader->AnmUnk1; // param_1[2] = in_EAX[1]; ANM.[....]tcnt .... ....
    this->TimelineCount = fileHeader->TimelineCount; // in_EAX[2]; ANM. ....[tcnt].... ....
    //iVar1 = (ANM_TIMELINE *)cs2_HeapAlloc_logged_004b02b0(0, fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    //this->Timelines = iVar1;
    //cs2_strmemcpy_004b1600(iVar1, &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
    this->Timelines = new ANM_TIMELINE[fileHeader->TimelineCount]; // param_1[1] = iVar1;
    memcpy(&this->Timelines[0], &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE));
    this->resetScript();
    for (INT i = 0; i < 64; i++) {
      this->Variables[i] = 0;
    }
    return FALSE;
  }
  //kcAnmScript_reset [00598500]
  BOOL resetScript() {
    this->Instruction = -1; // *(undefined4 *)(in_EAX + 0x14) = 0xffffffff;
    this->FrameID = 0; // *(undefined4 *)(in_EAX + 0x128) = 0;
    this->Blend = 255; // *(undefined4 *)(in_EAX + 300) = 0xff;
    this->Disp = TRUE; // *(undefined4 *)(in_EAX + 0x130) = 1;
    this->PosX = 0; // *(undefined4 *)(in_EAX + 0x134) = 0;
    this->PosY = 0; // *(undefined4 *)(in_EAX + 0x138) = 0;
    return TRUE;
  }


  // Evaluate the value of an argument.
  INT evalArg(ANM_ARG arg) {
    switch (arg.VarType) {
    case TYPE_CONST:
    case TYPE_LABEL:
      return arg.Value;
    
    case TYPE_VARIABLE:
      return this->Variables[arg.Value];

    default:
      return 0; // invalid variable type defaults to 0
    }
  }

  //kcAnmScript_execute [00597cf0]
  UINT executeScript() {


  }

  INT FUN_005984d0(undefined4 param_1_00)

  {
    int iVar1;
    
    iVar1 = kcAnmScript_execute();
    if (iVar1 != 0) {
      //cs2_strmemcpy_004b1600(param_1_00, param_1 + 0x128, 0x14);
      cs2_strmemcpy_004b1600(param_1_00, this->FrameID, 0x14);
    }
    return iVar1;
  }  

  
  VOID kcAnmScript_new() {
    this->Unk73 = 1.00000000; //0x3f800000; // unaff_ESI[0x49] = 0x3f800000;
    this->Timelines = NULL; // unaff_ESI[1] = 0;
    this->TimelineCount = 0; // *unaff_ESI = 0;
    this->Unk2 = 0; // unaff_ESI[2] = 0;
    this->Unk3 = 0; // unaff_ESI[3] = 0;
    this->Counter = 0; // unaff_ESI[4] = 0;
    this->Instruction = 0; // unaff_ESI[5] = 0;
    this->Wait = 0; // unaff_ESI[6] = 0;
    this->MaxFrame = 0; // unaff_ESI[7] = 0;

    for (INT i = 0; i < 64; i++) {
      this->Variables[i] = 0;
    }

    this->FrameID = 0; // unaff_ESI[0x4a] = 0;
    this->Blend = 255; // unaff_ESI[0x4b] = 0xff;
    this->Disp = TRUE; // unaff_ESI[0x4c] = 1;
    this->PosX = 0; // unaff_ESI[0x4d] = 0;
    this->PosY = 0; // unaff_ESI[0x4e] = 0;
    // stl allocation
    this->Unk72stl = new ANM_UNK72(); //unaff_ESI[0x48] = iVar2;
    // this->Unk72stl = NULL; // unaff_ESI[0x48] = 0;
    return;
  }


};

#pragma pack(pop)


// KCLIB FUNCTIONS:

VOID kclib_logError(LPCSTR cmsg)

{
  // (dummy func)
}

// MersenneTwister PRNG
UINT mt_genrand(VOID)

{
  return 4; // A fair roll of the dice (dummy func)
}


VOID unk_recursive_dtor_004c9240(INT *param_1)

{
  if (((CHAR *)param_1)[21] == '\0') {
    unk_recursive_dtor_004c9240(((INT **)param_1)[2]);
                    /* WARNING: Subroutine does not return */
    free(param_1);
    //std_free_00517994(param_1);
  }
  return;
}
// VOID unk_recursive_dtor_004c9240(INT param_1)

// {
//   if (*(CHAR *)(param_1 + 0x15) == '\0') {
//     unk_recursive_dtor_004c9240(*(undefined4 *)(param_1 + 8));
//                     /* WARNING: Subroutine does not return */
//     free(param_1);
//     //std_free_00517994(param_1);
//   }
//   return;
// }

// KCANMSCRIPT FUNCTIONS:


VOID kcAnmScript_new(ANM_SCRIPT *anmScr)

{
  INT i;

  anmScr->Unk73 = 1.00000000; //0x3f800000; // unaff_ESI[0x49] = 0x3f800000;
  anmScr->Timelines = NULL; // unaff_ESI[1] = 0;
  anmScr->TimelineCount = 0; // *unaff_ESI = 0;
  anmScr->Unk2 = 0; // unaff_ESI[2] = 0;
  anmScr->Unk3 = 0; // unaff_ESI[3] = 0;
  anmScr->Counter = 0; // unaff_ESI[4] = 0;
  anmScr->Instruction = 0; // unaff_ESI[5] = 0;
  anmScr->Wait = 0; // unaff_ESI[6] = 0;
  anmScr->MaxFrame = 0; // unaff_ESI[7] = 0;

  for (i = 0; i < 64; i++) {
    anmScr->Variables[i] = 0;
  }

  anmScr->FrameID = 0; // unaff_ESI[0x4a] = 0;
  anmScr->Blend = 255; // unaff_ESI[0x4b] = 0xff;
  anmScr->Disp = TRUE; // unaff_ESI[0x4c] = 1;
  anmScr->PosX = 0; // unaff_ESI[0x4d] = 0;
  anmScr->PosY = 0; // unaff_ESI[0x4e] = 0;
  // stl allocation
  anmScr->Unk72stl = new ANM_UNK72(); //unaff_ESI[0x48] = iVar2;
  // anmScr->Unk72stl = NULL; // unaff_ESI[0x48] = 0;
  return;
}



// VOID kcAnmScript_new(VOID)

// {
//   UINT uVar1;
//   INT iVar2;
//   undefined4 *unaff_ESI;
//   undefined4 *puVar3;
//   INT **in_FS_OFFSET;
//   INT local_10;
//   INT *local_c;
//   undefined *puStack8;
//   undefined4 local_4;
  
//   local_4 = 0xffffffff;
//   puStack8 = &LAB_005d4077;
//   local_c = *in_FS_OFFSET;
//   uVar1 = DAT_00640160 ^ (uint)&stack0xffffffe8;
//   *(INT ***)in_FS_OFFSET = &local_c;
//   unaff_ESI[0x49] = 0x3f800000;
//   unaff_ESI[1] = 0;
//   *unaff_ESI = 0;
//   unaff_ESI[2] = 0;
//   unaff_ESI[3] = 0;
//   unaff_ESI[4] = 0;
//   unaff_ESI[5] = 0;
//   unaff_ESI[6] = 0;
//   unaff_ESI[7] = 0;
//   iVar2 = 0x40;
//   puVar3 = unaff_ESI + 8;
//   while (iVar2 != 0) {
//     iVar2 += -1;
//     *puVar3 = 0;
//     puVar3 = puVar3 + 1;
//   }
//   unaff_ESI[0x4a] = 0;
//   unaff_ESI[0x4b] = 0xff;
//   unaff_ESI[0x4c] = 1;
//   unaff_ESI[0x4d] = 0;
//   unaff_ESI[0x4e] = 0;
//   iVar2 = alloc_func_005179ba(0x20,uVar1);
//   local_4 = 0;
//   if (iVar2 != 0) {
//     local_10 = iVar2;
//     FUN_004deb00(iVar2);
//     unaff_ESI[0x48] = iVar2;
//     *in_FS_OFFSET = local_c;
//     return;
//   }
//   unaff_ESI[0x48] = 0;
//   *in_FS_OFFSET = local_c;
//   return;
// }

// kcAnmScript_reset

BOOL kcAnmScript_reset(ANM_SCRIPT *anmScr)

{
  anmScr->Instruction = -1; // *(undefined4 *)(in_EAX + 0x14) = 0xffffffff;
  anmScr->FrameID = 0; // *(undefined4 *)(in_EAX + 0x128) = 0;
  anmScr->Blend = 255; // *(undefined4 *)(in_EAX + 300) = 0xff;
  anmScr->Disp = TRUE; // *(undefined4 *)(in_EAX + 0x130) = 1;
  anmScr->PosX = 0; // *(undefined4 *)(in_EAX + 0x134) = 0;
  anmScr->PosY = 0; // *(undefined4 *)(in_EAX + 0x138) = 0;
  return TRUE;
}

// undefined4 kcAnmScript_reset(VOID)

// {
//   INT in_EAX;
  
//   *(undefined4 *)(in_EAX + 0x14) = 0xffffffff;
//   *(undefined4 *)(in_EAX + 0x128) = 0;
//   *(undefined4 *)(in_EAX + 300) = 0xff;
//   *(undefined4 *)(in_EAX + 0x130) = 1;
//   *(undefined4 *)(in_EAX + 0x134) = 0;
//   *(undefined4 *)(in_EAX + 0x138) = 0;
//   return 1;
// }


//undefined4 kcAnmScript_dtor_sub(VOID)
BOOL kcAnmScript_dtor_sub(ANM_SCRIPT *anmScr)

{
  // INT iVar1;
  // undefined4 *unaff_ESI;
  // undefined4 *puVar2;
  //INT iVar1;
  //ANM_TIMELINE *pTimelines;
  //INT *pVariables;
  //ANM_SCRIPT *anmScr; //undefined4 *unaff_ESI; // ANM_SCRIPT*
  //undefined4 *puVar2;
  INT i;
  INT *pUnkStruct;

  //pTimelines = anmScr->Timelines; // iVar1 = anmScr[1];
  if (anmScr->Timelines != NULL) { // if (iVar1 != NULL)
    if (anmScr->TimelineCount < 2) { // if (*(INT *)(iVar1 + -4) < 2)
      HeapFree(DAT_007a0c44, 0, (INT *)(anmScr)); // HeapFree(DAT_007a0c44, 0, (INT *)(iVar1 + -4));
    }
    else {
      anmScr->TimelineCount--; // *(INT *)(iVar1 + -4) = *(INT *)(iVar1 + -4) + -1;
    }
    // anmScr[1] = 0;
    anmScr->Timelines = (ANM_TIMELINE *)NULL;
  }
  // iVar1 = (INT)anmScr->Timelines; // anmScr[1];
  // if (iVar1 != NULL) {
  //   if (*(INT *)(iVar1 + -4) < 2) {
  //     HeapFree(DAT_007a0c44, 0, (INT *)(iVar1 + -4));
  //   }
  //   else {
  //     *(INT *)(iVar1 + -4) = *(INT *)(iVar1 + -4) + -1;
  //   }
  //   // anmScr[1] = 0;
  //   anmScr->Timelines = (ANM_TIMELINE *)NULL;
  // }

  anmScr->TimelineCount = 0; // *anmScr = 0;
  anmScr->Unk2 = 0;          // anmScr[2] = 0;
  anmScr->Unk3 = 0;          // anmScr[3] = 0;
  anmScr->Counter = 0;       // anmScr[4] = 0;
  anmScr->Wait = 0;          // anmScr[6] = 0;
  // iVar1 = 64; // iVar1 = 0x40; // sizeof(ANM_SCRIPT::Variables) / sizeof(INT)
  // pVariables = &anmScr->Variables[0];// puVar2 = anmScr + 8;
  // while (iVar1 != 0) {
  //   iVar1 += -1;
  //   *puVar2 = 0;
  //   puVar2 = puVar2 + 1;
  // }
  for (i = 0; i < 64; i++) {
    anmScr->Variables[i] = 0;
  }
  //iVar1 = anmScr->Unk72; // iVar1 = anmScr[0x48];
  //unk_recursive_dtor_004c9240(*(undefined4 *)(*(INT *)(iVar1 + 0x18) + 4));
  //destructor for stl class
  //unk_recursive_dtor_004c9240(anmScr->Unk72stl->Field72Unk6ptr[1]);
  //Some kind of freeing up an std::linkedlist<>?
  anmScr->Unk72stl->Field72Unk6ptr[1] = (INT *) anmScr->Unk72stl->Field72Unk6ptr;
  anmScr->Unk72stl->Field72Unk7 = 0;
  anmScr->Unk72stl->Field72Unk6ptr[0] = (INT *) anmScr->Unk72stl->Field72Unk6ptr;
  anmScr->Unk72stl->Field72Unk6ptr[2] = (INT *) anmScr->Unk72stl->Field72Unk6ptr;

  // *(INT *)(*(INT *)(iVar1 + 0x18) + 4) = *(INT *)(iVar1 + 0x18);
  // *(undefined4 *)(iVar1 + 0x1c) = 0;
  // *(undefined4 *)*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(iVar1 + 0x18);
  // *(INT *)(*(INT *)(iVar1 + 0x18) + 8) = *(INT *)(iVar1 + 0x18);

  
  // pUnkStruct = anmScr->Unk72stl;
  // ((INT *)pUnkStruct[6])[1] = pUnkStruct[6];
  // pUnkStruct[7] = NULL;
  // ((INT *)pUnkStruct[6])[0] = pUnkStruct[6];
  // ((INT *)pUnkStruct[6])[2] = pUnkStruct[6];
  // ((INT *)pUnkStruct[24])[1] = pUnkStruct[6];
  // ((INT *)pUnkStruct[6])[1] = pUnkStruct[6];
  // ((INT *)pUnkStruct[7])[1] = pUnkStruct[6];
  // // *((INT *) (anmScr->FrameID + 4)) = anmScr->FrameID; // WHAT??
  // // anmScr->Unk79 = 0;
  // // *((INT *) (anmScr->FrameID)) = anmScr->FrameID; // WHAT?????
  // // *((INT *) (anmScr->FrameID + 8)) = anmScr->FrameID; // WHAT????????

  // // WHAT THE FUCK
  // ((INT *) anmScr->FrameID)[1] = anmScr->FrameID;
  // ((INT *) anmScr->FrameID)[0] = anmScr->FrameID;
  // anmScr->Unk79 = 0;
  // ((INT *) anmScr->FrameID)[2] = anmScr->FrameID;

  // *(INT *)(*(INT *)(iVar1 + 0x18) + 4) = *(INT *)(iVar1 + 0x18);
  // *(undefined4 *)(iVar1 + 0x1c) = 0;
  // *(undefined4 *)*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(iVar1 + 0x18);
  // *(INT *)(*(INT *)(iVar1 + 0x18) + 8) = *(INT *)(iVar1 + 0x18);

  anmScr->FrameID = 0; // anmScr[0x4a] = 0;
  anmScr->Blend = 255; // anmScr[0x4b] = 0xff;
  anmScr->Disp = TRUE; // anmScr[0x4c] = 1;
  anmScr->PosX = 0; // anmScr[0x4d] = 0;
  anmScr->PosY = 0; // anmScr[0x4e] = 0;
  
  return TRUE;
}

// Evaluate the value of an argument.
INT kcAnmScript_evalArg(ANM_SCRIPT *anmScr, ANM_ARG arg)

{
  switch (arg.VarType) {
  case TYPE_CONST:
  case TYPE_LABEL:
    return arg.Value;
  
  case TYPE_VARIABLE:
    return anmScr->Variables[arg.Value];

  default:
    return 0; // invalid variable type defaults to 0
  }
}


//INT kcAnmScript_readFile(INT *param_1, UINT param_1_00)
//__thiscall
//description...
BOOL kcAnmScript_readFile(ANM_SCRIPT *anmScr, ANM_FILEINFO *fileData, UINT fileSize)

{
  //INT *param_1; // this
  //UINT param_1_00; // fileSize
  // ANM. .... tcnt ....
  // .... .... .... ....
  // tlns---------------
  // ------------------->
  // fileData[0] : Magic
  // fileData[1] : AnmUnk1
  // fileData[2] : TimelineCount
  // fileData[3] : AnmUnk3
  // fileData[4] : AnmUnk4
  // fileData[5] : AnmUnk5
  // fileData[6] : AnmUnk6
  // fileData[7] : AnmUnk7
  // &fileData[8] : Timelines...
  ANM_FILEINFO *fileData; //INT *in_EAX; // fileData
  ANM_FILEHEADER *fileHeader;
  INT iVar1;
  INT *piVar2;
  INT i;

  fileHeader = &fileData->Header;
  
  //kcAnmScript_dtor_sub();
  kcAnmScript_dtor_sub(anmScr);
  if (fileHeader->Magic != MAGIC_ANM) { // "ANM"
    kclib_logError(LOG_kcAnmScript_different_data_format);
    return FALSE;
  }
  if (fileHeader->TimelineCount == 0) {
    kclib_logError(LOG_kcAnmScript_invalid_timeline_count);
    return FALSE;
  }
  //if (fileSize < fileHeader->TimelineCount * 0x44 + 0x20U)
  if (fileSize < fileHeader->TimelineCount * sizeof(ANM_TIMELINE) + sizeof(ANM_FILEHEADER)) {
    kclib_logError(LOG_kcAnmScript_invalid_data_size);
    return FALSE;
  }
  anmScr->Unk2 = fileHeader->AnmUnk1; // param_1[2] = in_EAX[1]; ANM.[....]tcnt .... ....
  anmScr->TimelineCount = fileHeader->TimelineCount; // in_EAX[2]; ANM. ....[tcnt].... ....
  //iVar1 = (ANM_TIMELINE *)cs2_HeapAlloc_logged_004b02b0(0, fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
  //anmScr->Timelines = iVar1;
  anmScr->Timelines = new ANM_TIMELINE[fileHeader->TimelineCount]; // param_1[1] = iVar1;
  memcpy(&anmScr->Timelines[0], &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE));
  //cs2_strmemcpy_004b1600(iVar1, &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
  kcAnmScript_reset(anmScr);
  // Seems to memzero the anmScr variables
  // iVar1 = 0x40;
  // piVar2 = (INT *) &anmScr->Timelines[0]; // anmScr + 8
  // while (iVar1 != 0) {
  //   iVar1 += -1;
  //   *piVar2 = 0;
  //   piVar2 = piVar2 + 1;
  // }
  for (i = 0; i < 64; i++) {
    anmScr->Variables[i] = 0;
  }
  return TRUE;
}


//__fastcall
// Run the animation script
UINT kcAnmScript_execute(ANM_SCRIPT *anmScr)

{
  ANM_TIMELINE *timeln;
  VAR_TYPE vartype;
  INT argNumber;
  INT argVar;
  INT argLabel;
  INT argMin;
  INT argMax;
  UINT range;
  UINT rnd;
  INT result;
  INT local_20;
  
  if (anmScr->TimelineCount <= anmScr->Instruction) {
    return FALSE;
  }
  local_20 = FUN_00572440();
  if (anmScr->Counter <= local_20) {
    do {
      anmScr->Instruction = anmScr->Instruction + 1;
      if (anmScr->TimelineCount <= anmScr->Instruction) break;
      timeln = &anmScr->Timelines[anmScr->Instruction];
      switch(timeln->CmdType) {

      case CMD_ID: // 0: [ID] [min] (max)
        anmScr->Wait = anmScr->Wait + anmScr->Counter;
        local_20 -= anmScr->Counter;
        anmScr->FrameID = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argMin = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argMax = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        range = argMax - argMin;
        if (range == 0) {
          anmScr->Counter = argMin;
        }
        else {
          rnd = mt_genrand();
          anmScr->Counter = rnd % range + argMin;
        }
        break;

      case CMD_SET: // 1: set [var] [min] (max)
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argMin = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argMax = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        range = argMax - argMin;
        if (range == 0) {
          anmScr->Variables[argVar] = argMin;
        }
        else {
          rnd = mt_genrand();
          anmScr->Variables[argVar] = rnd % range + argMin;
        }
        break;

      case CMD_LOOP: // 2: loop [var] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        if (anmScr->Variables[argVar] < 1) {
          anmScr->Instruction = argLabel - 1;
        }
        else {
          anmScr->Variables[argVar] -= 1;
        }
        break;

      case CMD_JUMP: // 3: jump [label]
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        anmScr->Instruction = argLabel - 1;
        break;

      case CMD_IF: // 4: if [var] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        if (anmScr->Variables[argVar] > 0) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFE: // 5: ife [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] == argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFN: // 6: ifn [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] != argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFG: // 7: ifg [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] > argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFS: // 8: ifs [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] < argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFGE: // 9: ifge [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] >= argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_IFSE: // 10: ifse [var] [value] [label]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        argLabel = kcAnmScript_evalArg(anmScr, timeln->Args[2]);
        if (anmScr->Variables[argVar] <= argNumber) {
          anmScr->Instruction = argLabel - 1;
        }
        break;

      case CMD_MAX: // 11: max [var]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        anmScr->Variables[argVar] = anmScr->MaxFrame;
        break;

      case CMD_BLEND: // 12: blend [param]
        anmScr->Blend = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        break;

      case CMD_DISP: // 13: disp [param]
        anmScr->Disp = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        break;

      case CMD_POS: // 14: pos [x] [y]
        anmScr->PosX = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        anmScr->PosY = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        break;

      case CMD_WAIT: // 15: wait [min] [max]
        anmScr->Wait = anmScr->Wait + anmScr->Counter;
        local_20 -= anmScr->Counter;
        argMin = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argMax = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        range = argMax - argMin;
        if (range == 0) {
          anmScr->Counter = argMin;
        }
        else {
          rnd = mt_genrand();
          anmScr->Counter = rnd % range + argMin;
        }
        break;

      case CMD_ADD: // 16: add [var] [value]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        anmScr->Variables[argVar] += argNumber;
        break;

      case CMD_SUB: // 17: sub [var] [value]
        argVar = kcAnmScript_evalArg(anmScr, timeln->Args[0]);
        argNumber = kcAnmScript_evalArg(anmScr, timeln->Args[1]);
        anmScr->Variables[argVar] -= argNumber;
        break;
    
      default:
        kclib_logError(LOG_kcAnmScript_command_error);
        return anmScr->FrameID;

      }
    } while (anmScr->Counter <= local_20);
  }
  result = FUN_004b1490();
  return (UINT)(result != 0);
}



void FUN_00597780(int param_1)

{
  uint uVar1;
  int iVar2;
  int extraout_ECX;
  int *unaff_ESI;
  int **in_FS_OFFSET;
  int local_10;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_005d4468;
  local_c = *in_FS_OFFSET;
  uVar1 = DAT_00640160 ^ (uint)&stack0xffffffec;
  *(int ***)in_FS_OFFSET = &local_c;
  iVar2 = alloc_func_005179ba(0x20,uVar1);
  local_4 = 0;
  local_10 = iVar2;
  if (iVar2 == 0) {
    iVar2 = 0;
  }
  else {
    FUN_004be8d0(iVar2);
  }
  unaff_ESI[0x4a] = 0x3f800000;
  unaff_ESI[0x4b] = param_1;
  unaff_ESI[1] = 0;
  unaff_ESI[2] = 0;
  *unaff_ESI = iVar2;
  unaff_ESI[3] = 0;
  unaff_ESI[0x49] = 1;
  unaff_ESI[4] = 0;
  unaff_ESI[0x4c] = 0;
  unaff_ESI[5] = 0;
  unaff_ESI[0x4d] = 0;
  unaff_ESI[6] = 0;
  unaff_ESI[7] = 0;
  unaff_ESI[8] = 0;
  unaff_ESI[9] = 0;
  FUN_004cb430();
  unaff_ESI[0x1a] = 0x3f800000;
  unaff_ESI[0x1d] = -1;
  unaff_ESI[0x1b] = 0x3f800000;
  unaff_ESI[0x24] = extraout_ECX;
  unaff_ESI[0x1c] = 0x3f800000;
  unaff_ESI[0x1e] = 0;
  unaff_ESI[0x1f] = 0;
  unaff_ESI[0x20] = 0;
  unaff_ESI[0x21] = 0;
  unaff_ESI[0x22] = 0;
  unaff_ESI[0x23] = 0;
  *in_FS_OFFSET = local_c;
  return;
}



/* WARNING: Could not reconcile some variable overlaps */

void __fastcall
cs2_script_lookup_initfunc_005c0970
          (undefined4 *param_1,char *param_2,undefined4 param_1_00,int param_2_00,undefined4 param_3
          )

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  int **in_FS_OFFSET;
  undefined auStack220 [4];
  char *local_d8;
  int local_d4;
  undefined4 *local_d0;
  undefined4 local_cc;
  undefined local_c8 [4];
  uint local_c4;
  undefined4 local_b4;
  uint local_b0;
  int local_ac;
  undefined local_a8 [4];
  uint local_a4;
  undefined4 local_94;
  uint local_90;
  int local_8c;
  undefined local_88 [4];
  uint local_84;
  undefined4 local_74;
  uint local_70;
  undefined4 local_60 [9];
  int local_3c;
  undefined4 local_38 [8];
  int local_18;
  uint local_14;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_005d4ed4;
  local_c = *in_FS_OFFSET;
  local_14 = DAT_00640160 ^ (uint)auStack220;
  uVar2 = DAT_00640160 ^ (uint)&stack0xffffff14;
  *(int ***)in_FS_OFFSET = &local_c;
  iVar5 = 0;
  local_cc = param_3;
  local_d4 = 0;
  local_d8 = param_2;
  local_d0 = param_1;
  if (0 < param_2_00) {
    do {
      if (false) goto switchD_005c09f0_caseD_1b;
      switch(param_1_00) {
      case 0:
        goto LAB_005c09fc;
      case 1:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 != 0) {
          iVar5 = FUN_005b7000();
          break;
        }
        goto LAB_005c0a17;
      case 2:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c14a0();
        break;
      case 3:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b5730();
        break;
      case 4:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c17f0();
        break;
      case 5:
LAB_005c09fc:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 != 0) {
          iVar5 = FUN_005b5d20(param_3);
          break;
        }
LAB_005c0a17:
        iVar5 = 0;
        goto LAB_005c0e50;
      case 6:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b9b40();
        break;
      case 7:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b97b0();
        break;
      case 8:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c01e0();
        break;
      case 9:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b9f50();
        break;
      case 10:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b38d0();
        break;
      case 0xb:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c44a0(iVar5,param_3);
        break;
      case 0xc:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b3010(iVar5,param_3,local_d0 + 3);
        break;
      case 0xd:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005bae30();
        break;
      case 0xe:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b95d0();
        break;
      case 0xf:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005ba600();
        break;
      case 0x10:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b32f0(iVar5,param_3);
        break;
      case 0x11:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b26c0(iVar5,param_3);
        break;
      case 0x12:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b2340(iVar5,param_3);
        break;
      case 0x13:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b20e0(iVar5,param_3);
        break;
      case 0x14:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b7cd0(iVar5,param_3);
        break;
      case 0x15:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005d2eb0();
        break;
      case 0x16:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005ba8f0();
        break;
      case 0x17:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b4b20();
        break;
      case 0x18:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c0310();
        break;
      case 0x19:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005c1690();
        break;
      case 0x1a:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005ba180();
        break;
      case 0x1c:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b8f60(iVar5,param_3);
        break;
      case 0x1d:
        iVar5 = cs2_unk_read_kcBigFiles_005c4720(uVar2);
        if (iVar5 == 0) goto LAB_005c0a17;
        iVar5 = FUN_005b6060();
      }
switchD_005c09f0_caseD_1b:
      if (iVar5 != 0) {
        pcVar3 = local_d8;
        do {
          cVar1 = *pcVar3;
          pcVar3[(int)((int)local_38 - (int)local_d8)] = cVar1;
          pcVar3 = pcVar3 + 1;
        } while (cVar1 != '\0');
        local_18 = local_d4;
        iVar4 = 9;
        puVar6 = local_38;
        puVar7 = local_60;
        while (iVar4 != 0) {
          iVar4 += -1;
          *puVar7 = *puVar6;
          puVar6 = puVar6 + 1;
          puVar7 = puVar7 + 1;
        }
        local_3c = iVar5;
        FUN_004ae290(*local_d0,local_60);
        param_3 = local_cc;
      }
LAB_005c0e50:
      local_d4 += 1;
    } while (local_d4 < param_2_00);
  }
  local_70 = 0xf;
  local_74 = 0;
  local_84 &= 0xffffff00;
  pcVar3 = local_d8;
  do {
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  FUN_00402c20(local_d8,pcVar3 + -(int)(local_d8 + 1));
  local_4 = 0;
  local_b0 = 0xf;
  local_b4 = 0;
  local_c4 &= 0xffffff00;
  FUN_00402ac0(local_88,0,0xffffffff);
  local_ac = param_2_00;
  local_4._0_1_ = 1;
  local_90 = 0xf;
  local_94 = 0;
  local_a4 &= 0xffffff00;
  FUN_00402ac0(local_c8,0,0xffffffff);
  local_8c = local_ac;
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_004264f0(local_d0[1],local_a8);
  if (0xf < local_90) {
                    /* WARNING: Subroutine does not return */
    std?_free_00517994(local_a4);
  }
  local_90 = 0xf;
  local_94 = 0;
  local_a4 &= 0xffffff00;
  if (0xf < local_b0) {
                    /* WARNING: Subroutine does not return */
    std?_free_00517994(local_c4);
  }
  local_b0 = 0xf;
  local_b4 = 0;
  local_c4 &= 0xffffff00;
  if (local_70 < 0x10) {
    *in_FS_OFFSET = local_c;
    func__end_critical_section();
    return;
  }
                    /* WARNING: Subroutine does not return */
  std?_free_00517994(local_84);
}



uint FUN_00598860(void)

{
  int in_EAX;
  uint uVar1;
  
  if (*(int *)(in_EAX + 0x48) == 0) {
    return *(undefined4 *)(in_EAX + 0x1c);
  }
  uVar1 = (uint)(*(int *)(in_EAX + 0x58) * *(int *)(in_EAX + 0x1c)) / 0xff;
  if (0xff < uVar1) {
    uVar1 = 0xff;
  }
  return uVar1;
}

void __fastcall
kcMotionScript_load_00597870
          (undefined4 param_1,int param_2,int *param_1_00,byte *param_2_00,undefined4 param_3,
          undefined4 param_4,undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  byte bVar1;
  int **ppiVar2;
  byte *pbVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  int iVar7;
  byte *pbVar8;
  int **ppiVar9;
  int **ppiVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  bool bVar13;
  int *local_1a0;
  byte *local_19c;
  int local_198;
  int **local_194;
  int **local_190;
  undefined4 local_18c;
  undefined local_188 [12];
  int local_17c;
  undefined4 local_178 [12];
  undefined4 local_148 [8];
  int local_128;
  undefined4 local_124;
  undefined4 local_120;
  byte local_118 [260];
  undefined4 local_14;
  int local_10;
  uint local_4;
  
  ppiVar9 = DAT_007b0f30;
  local_4 = DAT_00640160 ^ (uint)&local_1a0;
  local_1a0 = param_1_00;
  local_19c = param_2_00;
  local_198 = param_2;
  local_18c = param_1;
  if ((param_2_00 != (byte *)0x0) && (*param_1_00 != 0)) {
    pbVar8 = &DAT_006169ee;
    pbVar3 = param_2_00;
    do {
      bVar1 = *pbVar3;
      bVar13 = bVar1 < *pbVar8;
      if (bVar1 != *pbVar8) {
LAB_005978e0:
        iVar4 = (1 - (uint)bVar13) - (uint)(bVar13 != false);
        goto LAB_005978e5;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar13 = bVar1 < pbVar8[1];
      if (bVar1 != pbVar8[1]) goto LAB_005978e0;
      pbVar3 = pbVar3 + 2;
      pbVar8 = pbVar8 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_005978e5:
    if (iVar4 != 0) {
      ppiVar10 = (int **)*DAT_007b0f44;
      while( true ) {
        ppiVar2 = DAT_007b0f44;
        if ((ppiVar9 == (int **)0x0) || (ppiVar9 != DAT_007b0f30)) {
          call__invalid_parameter();
        }
        if (ppiVar10 == ppiVar2) break;
        if (ppiVar9 == (int **)0x0) {
          call__invalid_parameter();
          piVar5 = (int *)0x0;
        }
        else {
          piVar5 = *ppiVar9;
        }
        if (ppiVar10 == *(int ***)((int)piVar5 + 0x14)) {
          call__invalid_parameter();
        }
        iVar4 = __stricmp((char *)(ppiVar10 + 2),(char *)param_2_00);
        if (iVar4 == 0) break;
        if (ppiVar9 == (int **)0x0) {
          call__invalid_parameter();
          piVar5 = (int *)0x0;
        }
        else {
          piVar5 = *ppiVar9;
        }
        if (ppiVar10 == (int **)piVar5[5]) {
          call__invalid_parameter();
        }
        ppiVar10 = (int **)*ppiVar10;
      }
      ppiVar2 = DAT_007b0f44;
      if ((ppiVar9 == (int **)0x0) || (ppiVar9 != DAT_007b0f30)) {
        call__invalid_parameter();
      }
      if (ppiVar10 == ppiVar2) {
        pbVar3 = param_2_00;
        do {
          bVar1 = *pbVar3;
          pbVar3[(int)(local_118 + -(int)param_2_00)] = bVar1;
          pbVar3 = pbVar3 + 1;
        } while (bVar1 != 0);
        iVar4 = cs2_kcFileBig_open_004b6e40(0);
        if (iVar4 == 0) goto LAB_00597b83;
        uVar6 = cs2_HeapAlloc_logged_004b02b0(0,iVar4);
        local_14 = uVar6;
        local_10 = cs2_kcFileBig_open_004b6e40(uVar6);
        if (local_10 == 0) {
          cs2_HeapFree_004b0300(uVar6);
          goto LAB_00597b83;
        }
        ppiVar9 = DAT_007b0f44 + 1;
        piVar5 = (int *)FUN_004f15e0(DAT_007b0f44,DAT_007b0f44[1],local_118);
        FUN_004f1620();
        *ppiVar9 = piVar5;
        *(int **)piVar5[1] = piVar5;
        local_190 = DAT_007b0f44;
        local_194 = DAT_007b0f30;
        FUN_004d0130();
        cs2_log_sendmessage_004b57b0("motion script loaded > %s",param_2_00);
        ppiVar9 = local_194;
        ppiVar10 = local_190;
      }
      iVar4 = cs2_unk_read_kcBigFiles_005c4720();
      if (iVar4 == 0) {
        iVar4 = 0;
      }
      else {
        if (ppiVar9 == (int **)0x0) {
          call__invalid_parameter();
          piVar5 = (int *)0x0;
        }
        else {
          piVar5 = *ppiVar9;
        }
        if (ppiVar10 == (int **)piVar5[5]) {
          call__invalid_parameter();
        }
        if (ppiVar9 == (int **)0x0) {
          call__invalid_parameter();
        }
        else {
          ppiVar9 = (int **)*ppiVar9;
        }
        if (ppiVar10 == (int **)ppiVar9[5]) {
          call__invalid_parameter();
        }
        iVar4 = FUN_00596590(ppiVar10[0x43],ppiVar10[0x44]);
      }
      *(undefined4 *)(iVar4 + 0x4b0) = param_7;
      *(int *)(iVar4 + 0x4ac) = local_1a0[0x4d];
      iVar7 = FUN_005962e0(iVar4);
      if (iVar7 == 0) {
        FUN_004f0d20();
      }
      else {
        if (local_198 != 0) {
          FUN_00596500(local_198);
        }
        local_17c = local_1a0[0x49];
        local_198 = local_17c;
        FUN_004b0f00();
        piVar5 = local_1a0;
        local_124 = param_6;
        local_120 = param_5;
        iVar7 = 0xc;
        local_128 = iVar4;
        puVar11 = local_148;
        puVar12 = local_178;
        while (iVar7 != 0) {
          iVar7 += -1;
          *puVar12 = *puVar11;
          puVar11 = puVar11 + 1;
          puVar12 = puVar12 + 1;
        }
        FUN_004f0e70(local_188,&local_17c);
        piVar5[0x49] = piVar5[0x49] + 1;
        if (piVar5[0x4c] == 0) {
          piVar5[0x4c] = 1;
        }
      }
    }
  }
LAB_00597b83:
  func__end_critical_section();
  return;
}

