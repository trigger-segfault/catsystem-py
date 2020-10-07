#include <assert.h>
#include <stdio.h>

//Temporary defines, used during unobfuscation
#define BYTE unsigned char
#define BOOL int
#define INT int
#define UINT unsigned int
#define LONG long
#define ULONG unsigned long
#define VOID void
#define CHAR char
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
  /*$0,4*/   VAR_TYPE VarType;
  /*$4,4*/   UINT Value;
  /*$8*/
} ANM_ARG;

typedef struct {
  /*$0,4*/   ANM_CMD CmdType;
  /*$4,40*/  ANM_ARG Args[8];
  /*$44*/
} ANM_TIMELINE;

typedef struct {
  // /*$0,18*/  CHAR UnkName[24]; // 0x0:0x16?
  /*$0,18*/  UINT padding[6];
  /*$18,1c*/ UINT **Field72Unk6ptr;
  /*$1c,20*/ UINT Field72Unk7;
} ANM_UNK72;

typedef struct {
  /*$0,4*/   UINT TimelineCount; // Number of timelines
  /*$4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines
  /*$8,4*/   UINT Unk2; // (unused)
  /*c,4*/   UINT Unk3; // (unused)
  /*$10,4*/  UINT Counter; // used in CMD_ID, CMD_WAIT, and during command loop
  /*$14,4*/  UINT Instruction; // instruction ptr index
  /*$18,4*/  UINT Wait; // used in CMD_ID, CMD_WAIT
  /*$1c,4*/  UINT MaxFrame; // used in CMD_MAX
  /*$20,ff*/ INT Variables[64]; // live variables
  /*$120,4*/ ANM_UNK72 *Unk72ptr; // (ptr to unknown structure)
  /*$124,4*/ INT Unk73; // (unused)
  /*$128,4*/ INT FrameID; // current frame
  /*$12c,4*/ INT Blend; // used in CMD_BLEND
  /*$130,4*/ BOOL Disp; // used in CMD_DISP
  /*$134,4*/ INT PosX; // used in CMD_POS
  /*$138,4*/ INT PosY; // used in CMD_POS
  /*$13c,?*/ UINT Unk79;
  /*$140*/
} ANM_SCRIPT;

typedef struct {
  /*$0,4*/   UINT Magic; // "ANM" MAGIC_ANM
  /*$4,4*/   UINT AnmUnk1;
  /*$8,4*/   UINT TimelineCount;
  /*c,4*/   UINT AnmUnk3;
  /*$10,4*/  UINT AnmUnk4;
  /*$14,4*/  UINT AnmUnk5;
  /*$18,4*/  UINT AnmUnk6;
  /*$1c,4*/  UINT AnmUnk7;
  /*$20*/
} ANM_FILEHEADER;

typedef struct {
  /*$0,20*/  ANM_FILEHEADER Header;
  /*$20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
} ANM_FILEINFO;

// static_assert(sizeof(ANM_ARG) == 0x8, "ANM_ARG struct size is not 8 bytes");
// static_assert(sizeof(ANM_TIMELINE) == 0x44, "ANM_TIMELINE struct size is not 68 bytes");
// static_assert(sizeof(ANM_FILEHEADER) == 0x20, "ANM_FILEHEADER struct size is not 32 bytes");

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


typedef struct {
  /*$0,4*/   UINT NodeUnk0;
  /*$4,4*/   UINT NodeUnk1;
  /*$8,4*/   ANM_UNK_NODE *Next;
  /*$8,4*/   ANM_UNK_NODE *Next;
} ANM_UNK_NODE;

void unk_recursive_dtor_004c9240(int *param_1)

{
  if (((char *)param_1)[21] == '\0') {
    unk_recursive_dtor_004c9240(((int **)param_1)[2]);
                    /* WARNING: Subroutine does not return */
    free(param_1);
    //std_free_00517994(param_1);
  }
  return;
}
// void unk_recursive_dtor_004c9240(int param_1)

// {
//   if (*(char *)(param_1 + 0x15) == '\0') {
//     unk_recursive_dtor_004c9240(*(undefined4 *)(param_1 + 8));
//                     /* WARNING: Subroutine does not return */
//     free(param_1);
//     //std_free_00517994(param_1);
//   }
//   return;
// }

// KCANMSCRIPT FUNCTIONS:


//undefined4 kcAnmScript_unk_3_00598540(VOID)
BOOL kcAnmScript_unk_3_00598540(ANM_SCRIPT *anmScr)

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
  unk_recursive_dtor_004c9240(anmScr->Unk72ptr->Field72Unk6ptr[1]);
  //Some kind of freeing up an std::linkedlist<>?
  anmScr->Unk72ptr->Field72Unk6ptr[1] = (INT *) anmScr->Unk72ptr->Field72Unk6ptr;
  anmScr->Unk72ptr->Field72Unk7 = 0;
  anmScr->Unk72ptr->Field72Unk6ptr[0] = (INT *) anmScr->Unk72ptr->Field72Unk6ptr;
  anmScr->Unk72ptr->Field72Unk6ptr[2] = (INT *) anmScr->Unk72ptr->Field72Unk6ptr;

  // *(INT *)(*(INT *)(iVar1 + 0x18) + 4) = *(INT *)(iVar1 + 0x18);
  // *(undefined4 *)(iVar1 + 0x1c) = 0;
  // *(undefined4 *)*(undefined4 *)(iVar1 + 0x18) = *(undefined4 *)(iVar1 + 0x18);
  // *(INT *)(*(INT *)(iVar1 + 0x18) + 8) = *(INT *)(iVar1 + 0x18);

  
  // pUnkStruct = anmScr->Unk72ptr;
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

  fileHeader = &fileData->Header;
  
  //kcAnmScript_unk_3_00598540();
  kcAnmScript_unk_3_00598540(anmScr);
  if (fileHeader->Magic != MAGIC_ANM) { // "ANM"
    // "kcAnmScript : データ形式が違います"
    // ("kcAnmScript: Data format is different")
    kclib_logError(LOG_kcAnmScript_different_data_format);
    return FALSE;
  }
  if (fileHeader->TimelineCount == 0) {
    // "kcAnmScript : タイムライン数が不正です"
    // ("kcAnmScript: Timeline count is invalid")
    kclib_logError(LOG_kcAnmScript_invalid_timeline_count);
    return FALSE;
  }
  //if (fileSize < fileHeader->TimelineCount * 0x44 + 0x20U) {
  if (fileSize < fileHeader->TimelineCount * sizeof(ANM_TIMELINE) + sizeof(ANM_FILEHEADER)) { // * 0x44 + 0x20U
    // "kcAnmScript : データサイズが不正です"
    // ("kcAnmScript: Data size is invalid")
    kclib_logError(LOG_kcAnmScript_invalid_data_size);
    return FALSE;
  }
  anmScr->Unk2 = fileHeader->AnmUnk1; // ANM.[....]tcnt .... ....
  anmScr->TimelineCount = fileHeader->TimelineCount; // ANM. ....[tcnt].... ....
  iVar1 = (ANM_TIMELINE *)cs2_HeapAlloc_logged_004b02b0(0, fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
  anmScr->Timelines = iVar1;
  cs2_strmemcpy_004b1600(iVar1, &fileData->Timelines[0], fileHeader->TimelineCount * sizeof(ANM_TIMELINE)); // * 0x44);
  kcAnmScript_unk_setup_00598500();
  // Seems to memzero the fileData timelines
  iVar1 = 0x40;
  piVar2 = (INT *) &fileData->Timelines[0]; // fileData + 8
  while (iVar1 != 0) {
    iVar1 += -1;
    *piVar2 = 0;
    piVar2 = piVar2 + 1;
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

