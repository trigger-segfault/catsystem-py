
// functional definition for ac_parseLines
// 
// file used to decompile the ac.exe parseLines function bit by bit
// Most of the code was replaced with cleaner alternatives that should hopefully
// produce the excact same result, most of this revolving around STL containers.
// ...Assumptions were made, and the crowd said "std::vector"!

#include "common.h"
#ifndef KCLIB_OOP

#include "TokenParser_functional.h"
#include "kcAnmScript.h"
#include "ghidra_types_min.h"
#include <windows.h>
#include <vector>


/// GET STL SIZE
// #include <iostream>
// #include <list>
// #include <vector>
// #include <queue>
// #include <stack>
// #include <deque>
// #include <string>
// #include <map>
// #include <stdio.h>
// using namespace std;
  
// struct MYSTRUCT {
//   char Name[256];
//   unsigned int Field1;
//   unsigned int Field2;
// };
  
// #define printsize(Stl) printf("Size of " #Stl " = 0x%x\n", (int)sizeof(std::Stl<MYSTRUCT>))
// #define printsize2(Stl) printf("Size of " #Stl " = 0x%x\n", (int)sizeof(std::Stl<std::string, MYSTRUCT>))
  
// int main() {
//   bool mybool;
//   if (((mybool = true), mybool)) printf("yes\n");
//   else  printf("no\n");
//   if ((mybool = true), mybool) printf("yes2\n");
//   else  printf("no2\n");
//   if (mybool = true) printf("yes3\n");
//   else  printf("no3\n");
//   //std::list<MYSTRUCT> mylist();
//   printsize(list);
//   printsize(vector);
//   printsize(queue);
//   printsize(stack);
//   printsize(deque);
//   printsize2(map);
//   //printf("Size of list = %i\n", (int) sizeof(std::list<MYSTRUCT>));
//   // your code goes here
//   return 0;
// }

// Size of list = 0x18
// Size of vector = 0x18
// Size of queue = 0x50
// Size of stack = 0x50
// Size of deque = 0x50
// Size of map = 0x30


// STL std::vector / map / etc for LABEL name/lineNumber
///FID: DAT_004a29a0
// #define VECTOR_LABEL_NAMES ((void *)0)
#define VECTOR_LABEL_NAMES g_ac_LabelNames
// STL std::vector / map / etc (same as above) for parsed ANM lines
///FID: DAT_004a29bc
// #define VECTOR_ANM_LINES ((void *)0)
#define VECTOR_ANM_LINES g_ac_AnmLines


std::vector<ANM_LABEL> g_ac_LabelNames; // sizeof() == 0x1c
std::vector<ANM_TIMELINE> g_ac_AnmLines; // sizeof() == 0x1c



///FID: FUN_00413a36
void __cdecl FUN_00413a36(size_t allocSIze)

{
  // int iVar1;
  // void *pvVar2;
  // bad_exception local_10 [12];
  
  // do {
  //   pvVar2 = _malloc(param_1);
  //   if (pvVar2 != (void *)0x0) {
  //     return;
  //   }
  //   iVar1 = std::_callnewh(param_1);
  // } while (iVar1 != 0);
  // if ((_DAT_004c3448 & 1) == 0) {
  //   _DAT_004c3448 |= 1;
  //   std::bad_alloc((bad_alloc *)&DAT_004c343c);
  //   atexit(&LAB_00450d87);
  // }
  // std::bad_exception(local_10,(bad_exception *)&DAT_004c343c);
  //                   /* WARNING: Subroutine does not return */
  // _CxxThrowException@8(local_10,&DAT_00472540);
}



///FID: FUN_004119b0
undefined4 * __fastcall FUN_004119b0(undefined4 *param_1)

{
  // std::list
  undefined4 *puVar1;
  undefined4 *puVar2;
  int **in_FS_OFFSET;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  // local_4 = 0xffffffff;
  // puStack8 = &LAB_0045030b;
  // local_c = *in_FS_OFFSET;
  // *(int ***)in_FS_OFFSET = &local_c;
  puVar2 = (undefined4 *)FUN_00413a36(0x20);
  local_4 = 0;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar1 = (undefined4 *)FUN_00413a36(4);
    if (puVar1 == (undefined4 *)0x0) {
      *puVar2 = 0;
      puVar2[4] = 0;
      puVar2[5] = 0;
      puVar2[6] = 0;
      puVar2[7] = 0;
    }
    else {
      // Looks like classic linked list, nodes pointing to each other
      *(undefined4 **)puVar1 = puVar2;
      *(undefined4 **)puVar2 = puVar1;
      puVar2[4] = 0;
      puVar2[5] = 0;
      puVar2[6] = 0;
      puVar2[7] = 0;
    }
  }
  *(undefined4 **)param_1 = puVar2;
  param_1[1] = 0;
  // *in_FS_OFFSET = local_c;
  return param_1;
}


// //undefined4 * __fastcall FUN_00410780(undefined4 *param_1)
// ///FID: FUN_00410780
// SCRIPT_DECODER * __fastcall ScriptDecoder_ctor(SCRIPT_DECODER *this)
// ///ARGS: local_600
// {
//   // passed local_600 (sizeof == 0x128)
//   undefined4 *puVar1;
  
//   this->Buffer = NULL;
//   // *param_1 = 0; //0x0 (Buffer?)
//   this->LastTokenType = assert_enum(0, TOKEN_NONE);
//   // param_1[0x46] = 0; // 0x118 (LastTokenType?)
//   puVar1 = (undefined4 *)
//            FUN_00407810(8,"e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",0x27
//                        );
//   if (puVar1 == (undefined4 *)0x0) {
//     puVar1 = (undefined4 *)0x0;
//   }
//   else {
//     puVar1 = FUN_004119b0(puVar1);
//   }
//   // *(undefined4 **)(param_1 + 0x47) = puVar1; // 0x11c (Identifiers?)
//   this->Identifiers = (void *)puVar1;
//   this->Position = 0;
//   this->Length = 0;
//   this->EnableKeywords = 1;
//   return this;
//   // param_1[2] = 0; // 0x8 (Position?)
//   // param_1[1] = 0; // 0x4 (Length?)
//   // param_1[0x48] = 1; // 0x120 (EnableKeywords?)
//   // return param_1;
// }

///FID: FUN_00403cf0
undefined4 * __fastcall FUN_00403cf0(undefined4 *param_1)
///ARGS: &local_4b0
///ARGS: &local_4a8
{
  String_const_iterator<>(param_1);
  return param_1;
}

///FID: FUN_00410870
void __thiscall FUN_00410870(void *this,undefined4 param_1)
///ARGS: local_600, 0
{
  *(undefined4 *)((int)this + 0x120) = param_1; // (EnableKeywords?)
  return;
}
///FID: FUN_00410870
// unsigned int __thiscall FUN_00410870(void *this, unsigned int param_1)
// ///ARGS: local_600, 0
// {
//   *(undefined4 *)((int)this + 0x120) = param_1; // (EnableKeywords?)
//   return param_1;
// }


// Definitely STL, I think they're using vectors actually... not maps, or lists(??)
// clear method, maybe
///FID: FUN_00403be0
void __fastcall FUN_00403be0(int param_1)
///ARGS: &VECTOR_ANM_LINES
///ARGS: &VECTOR_LABEL_NAMES
{
  _Container_proxy *p_Var1;
  _Container_proxy **pp_Var2;
  undefined4 *puVar3;
  _Container_proxy *local_8;
  
  pp_Var2 = (_Container_proxy **)FUN_00403ef0(*(undefined4 *)(param_1 + 0x14));
  local_8 = *pp_Var2;
  puVar3 = (undefined4 *)FUN_00403ef0(*(undefined4 *)(param_1 + 0x14));
  *puVar3 = *(undefined4 *)(param_1 + 0x14);
  puVar3 = (undefined4 *)FUN_00403e40(*(int *)(param_1 + 0x14));
  *puVar3 = *(undefined4 *)(param_1 + 0x14);
  *(undefined4 *)(param_1 + 0x18) = 0;
  while (local_8 != *(_Container_proxy **)(param_1 + 0x14)) {
    pp_Var2 = (_Container_proxy **)FUN_00403ef0(local_8);
    p_Var1 = *pp_Var2;
    destroy((allocator<struct_std::_Container_proxy> *)(param_1 + 8),local_8);
    FUN_00404990(local_8);
    local_8 = p_Var1;
  }
  return;
}

///FID: FUN_00404c70
void __fastcall FUN_00404c70(int *param_1)

{
  uint uVar1;
  int iVar2;
  
  if (true) {
    uVar1 = FUN_00403df0(param_1);
    if ((uVar1 & 0xff) == 0) {
      __invalid_parameter_none();
    }
    do {
    } while (false);
  }
  if (true) {
    iVar2 = FUN_00403fa0(param_1);
    if (param_1[1] == *(int *)(iVar2 + 0x14)) {
      __invalid_parameter_none();
    }
    do {
    } while (false);
  }
  FUN_004040a0(param_1[1]);
  return;
}


///FID: FUN_00403fe0
void __fastcall FUN_00403fe0(int *param_1)

{
  FUN_00404c70(param_1);
  return;
}

///FID: FUN_00403d10
void __fastcall FUN_00403d10(int *param_1)

{
  FUN_00403fe0(param_1);
  return;
}

//void __thiscall FUN_00403cb0(void *this,undefined4 *param_1)
///FID: FUN_00403cb0
void __thiscall ac_AddLabel(std::vector<ANM_LABEL> *this, ANM_LABEL *label)

{
  this->push_back(*label);
  // undefined4 *puVar1;
  // undefined local_14 [8];
  // undefined4 local_c;
  // undefined4 local_8;
  
  // puVar1 = (undefined4 *)FUN_00403b50(this,local_14);
  // local_c = *puVar1;
  // local_8 = puVar1[1];
  // FUN_00403f00(this,local_c,local_8,label);
  // return;
}

//void __thiscall FUN_00403ba0(void *this,ANM_TIMELINE *param_1)
///FID: FUN_00403ba0
void __thiscall ac_AddTimeline(std::vector<ANM_TIMELINE> *this, ANM_TIMELINE *anmline)

{
  this->push_back(*anmline);
  // undefined4 *puVar1;
  // undefined local_14 [8];
  // undefined4 local_c;
  // undefined4 local_8;
  
  // puVar1 = (undefined4 *)FUN_00403b50(this,local_14);
  // local_c = *puVar1;
  // local_8 = puVar1[1];
  // FUN_00403e50(this,local_c,local_8,(undefined4 *)anmline);
  // return;
}


///FID: FUN_00403860
int * __cdecl FUN_00403860(int *param_1, char *param_2)
///ARGS: local_824, local_438
{
  int *piVar1;
  uint uVar2;
  char *_Str1;
  int iVar3;
  char *_Str2;
  undefined local_24 [8];
  int local_1c [2];
  undefined local_14 [8];
  int local_c;
  int local_8;
  
  FUN_00403cf0(&local_c); //String_const_iterator<>(&local_c);
  piVar1 = (int *)FUN_00403c80(&VECTOR_LABEL_NAMES, local_14);
  local_c = *piVar1;
  local_8 = piVar1[1];
  while( true ) {
    piVar1 = (int *)FUN_00403b50(&VECTOR_LABEL_NAMES, local_24);
    uVar2 = operator!=(&local_c, piVar1);
    if ((uVar2 & 0xff) == 0) {
      FUN_00403b50(&VECTOR_LABEL_NAMES,param_1);
      return param_1;
    }
    _Str2 = param_2;
    _Str1 = (char *)FUN_00403d10(&local_c);
    iVar3 = _stricmp(_Str1, _Str2);
    if (iVar3 == 0) break;
    FUN_00403d30(&local_c, local_1c);
  }
  *param_1 = local_c;
  param_1[1] = local_8;
  return param_1;
}

///FID: FUN_00403b50
void * __thiscall FUN_00403b50(void *this,void *param_1)

{
  Vector_iterator<>(param_1,*(undefined4 *)((int)this + 0x14),(undefined4 *)this);
  return param_1;
}



//void __cdecl FUN_004036f0(void *param_1,undefined4 *param_2,undefined4 param_3)
///FID: FUN_004036f0
BOOL __cdecl ac_ParseAnmArg(void *this, ANM_ARG *arg, unsigned int lineNumber)
///ARGS: local_600
{
  int iVar1;
  int *piVar2;
  uint uVar3;
  undefined local_444 [8];
  int local_43c [2];
  int local_434;
  int local_430;
  TOKEN_RESULT token; //undefined8 local_42c;
  // undefined8 local_42c;
  // undefined4 local_424;
  // char local_40c [1028];
  
  FUN_00403cf0(&local_434); //String_const_iterator<>(&local_434);
  if (token_NextSkipWS((SCRIPT_DECODER *)this, &token)) {
    if (token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
      arg->VarType = assert_enum(0, ANM_TYPE_CONST);
      arg->Value = token.TokenValue.UInt32; //local_424;
    }
    else if (token.TokenType == assert_enum(0x27, TOKEN_ATSIGN_KEYWORD)) {
      if (token_NextSkipWS((SCRIPT_DECODER *)this, &token) &&
        token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
        arg->VarType = assert_enum(1, ANM_TYPE_VARIABLE);
        arg->Value = token.TokenValue.UInt32; //local_424;
      }
      else {
        ///JP: printf("error (%d) : 変数番号が不正です。\n", lineNumber);
        printf("error (%d) : Invalid variable number.\n", lineNumber);
        return FALSE;
      }
      // if (!token_NextSkipWS((SCRIPT_DECODER *)this, &token)) {
      //   ///JP: printf("error (%d) : 変数番号が不正です。\n", lineNumber);
      //   printf("error (%d) : Invalid variable number.\n", lineNumber);
      // }
      // else if (token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
      //   arg->VarType = assert_enum(1, ANM_TYPE_VARIABLE);
      //   arg->Value = token.TokenValue.UInt32; //local_424;
      // }
      // else {
      //   ///JP: printf("error (%d) : 変数番号が不正です。\n", lineNumber);
      //   printf("error (%d) : Invalid variable number.\n", lineNumber);
      // }
    }
    else {
      // Looks like an STL std::map<std::string, LABEL_STRUCT>
      //                or std::map<const char *, LABEL_STRUCT>
      piVar2 = FUN_00403860(local_43c, &token.TokenText[0]);
      local_434 = *piVar2;
      local_430 = piVar2[1];
      piVar2 = (int *)FUN_00403b50(&VECTOR_LABEL_NAMES, local_444);
      uVar3 = FUN_00403d70(&local_434, piVar2);
      if ((uVar3 & 0xff) == 0) {
        arg->VarType = assert_enum(2, ANM_TYPE_LABEL);
        iVar1 = FUN_00403d10(&local_434);
        arg->Value = *(unsigned int *)(iVar1 + 0x24);
      }
      else {
        ///JP: printf("error (%d) : 未定義のラベルです。\n", lineNumber);
        printf("error (%d) : Undefined label.\n", lineNumber);
        return FALSE;
      }
    }
  }
  return TRUE;
}




// This is a hand-made function simplify the parsing process.
//  this function may have been compiled away, or maybe for loops were used,
//  or maybe it looked just like we see it now, could have even used a macro
inline BOOL ac_ParseNArgs(SCRIPT_DECODER *this, ANM_TIMELINE *anmline, int lineNumber)
{

}

// #define ac_ParseArgN(n)

// #define ac_ParseTimeline(n, lastopt) \
//         for (int i_ = 0; i < n; i++)
//         if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
//           ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
//           printf("error (%d) : Invalid parameter.\n", lineNumber);
//           errorCount++;
//         }
//         else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
//           ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
//           printf("error (%d) : Invalid parameter.\n", lineNumber);
//           errorCount++;
//         }
//         else {
//           ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
//           local_30 += 1;
//         }


/* WARNING: Type propagation algorithm not settling */
/* WARNING: Could not reconcile some variable overlaps */

unsigned int __cdecl ac_parseLines(char *filename)

{
  //BOOL BVar1;
  // int iVar2;
  // int *piVar3;
  // uint uVar4;
  // int iVar5;
  //int **in_FS_OFFSET;
  // undefined local_854 [8];
  // int local_84c [2];
  // undefined local_844 [8];
  // int local_83c [2];
  // undefined local_834 [8];
  // undefined local_82c [8];
  // int local_824 [2];
  // undefined local_81c [8];
  // int local_814 [2];
  // int local_80c;
  char lineBuffer [0x200]; //undefined8 local_808 [64]; // 512 (0x200)
  unsigned int errorCount; //int local_604;
  SCRIPT_DECODER decoder; //int local_600 [74]; //sizeof() == 0x128 - SCRIPT_DECODER?
  ANM_LABEL label; //undefined4 local_4d8 [8]; // 0x20
  // undefined4 local_4d8 [8]; // 0x20
  // undefined4 local_4b8; // here -> token (token_458) (0x60)
  // int local_4b4;
  // int local_4b0;
  // int local_4ac;
  // int local_4a8;
  // int local_4a4;
  ANM_TIMELINE anmline; // undefined4 local_4a0;
  // undefined4 local_4a0; // ANM_ARG
  // undefined4 local_49c;
  // int local_498;
  // undefined4 local_494;
  // int local_490;
  // undefined4 local_48c;
  // int local_488;
  TOKEN_RESULT token; //token_458; //undefined8 local_458;
  //undefined8 local_458; // TOKEN_RESULT? (sizeof is 0x420)
                        // TOKEN_RESULT->TokenGroup
                        // TOKEN_RESULT->TokenType
  //int local_450; // TOKENRESULT->TokenValue
  //TOKEN_VALUE tokvalue_450; //int local450; // TOKEN_RESULT->TokenValue
  ///UNUSED:unsigned int local_44c; // TOKEN_RESULT->TokUnk3
  ///UNUSED:int local_448;          // TOKEN_RESULT->TokUnk4
  ///UNUSED:unsigned int local_444; // TOKEN_RESULT->TokUnk5
  ///UNUSED:unsigned int local_440; // TOKEN_RESULT->TokUnk6
  ///UNUSED:TOKEN_TYPE local_43c; // TOKEN_RESULT->LiteralType
  //char local_438 [1028]; // TOKEN_RESULT->TokenText (+4, previous field?) (0x404)
  // uint local_34;
  // int local_30;
  // int local_2c;
  // undefined4 local_28;
  //int local_24 [4];
  SCRIPT_READER reader; //int local_24 [4];
                    // Can't be SCRIPT_DECODER, too small...
                    // maybe SCRIPT_DECODER with that full 0x124+ size is an extended version?
                    // possibly file text parser
  int lineNumber; //int local_14;
  // int *local_10;
  // undefined *puStack12;
  undefined4 local_8;
  
  // local_8 = 0xffffffff;
  // puStack12 = &LAB_0044fd03;
  // local_10 = *in_FS_OFFSET;
  // local_34 = DAT_004817cc ^ (uint)&stack0xfffffffc;
  // *(int ***)in_FS_OFFSET = &local_10;
  ScriptDecoder_ctor(&decoder);
  // local_8 = 0;
  // local_28 = 0;
  // FUN_00403cf0(&local_4b0); //String_const_iterator<>(&local_4b0);
  // FUN_00403cf0(&local_4a8); //String_const_iterator<>(&local_4a8);
  ScriptReader_ctor(&reader);
  // local_8._0_1_ = 1;
  errorCount = 0;
  
  ScriptDecoder_SetScrUnk72(&decoder, 0); //FUN_00410870(&decoder, 0);
  VECTOR_ANM_LINES.clear(); //FUN_00403be0((int)&VECTOR_ANM_LINES);
  VECTOR_LABEL_NAMES.clear(); //FUN_00403be0((int)&VECTOR_LABEL_NAMES);
  if (!ScriptReader_Open(&reader, filename)) {
    errorCount++;
    ///JP: printf("ファイルのロードに失敗しました : %s", filename);
    printf("Failed to load file : %s", filename);
    //local_80c = errorCount; // EAX <- errorCount
    // local_8 = (uint)local_8._1_3_ << 8;
    ScriptReader_Close(&reader);
    // local_8 = 0xffffffff;
    ScriptDecoder_dtor(&decoder);
    return errorCount;
  }

  ///SECTION: Parse labels
  // local_30 = 0;
  int labelIndex = 0;
  lineNumber = 1;
  while (!ScriptReader_IsEOF(&reader)) {
    ScriptReader_NextLine(&reader, lineBuffer);
    ScriptDecoder_SetBuffer(&decoder, lineBuffer);
    if (token_NextSkipWS(&decoder, &token) &&
        (token.TokenType == assert_enum(1, TOKEN_POUND)))
    {
      if (!token_NextSkipWS(&decoder, &token)) {
        ///JP: printf("error (%d) : 不正なラベル名です。\n", lineNumber);
        printf("error (%d) : Illegal label name.\n", lineNumber);
        errorCount++;
        break;
      }
      // That's right, as long as the token parses, you can have ANYTHING as a label name

      bool labelExists = false;
      for (int k = 0; k < (int)VECTOR_LABEL_NAMES.size(); k++)
      {
        if (strncmp(VECTOR_LABEL_NAMES[k].LabelName, &token.TokenText[0], 0x20) == 0)
        {
          labelExists = true;
          break;
        }
      }
      if (labelExists)
      {
        ///JP: printf("error (%d) : %s このラベル名は既に使用されています。\n", lineNumber, &token.TokenText[0]);
        printf("error (%d) : %s Label name already in use.\n", lineNumber, &token.TokenText[0]);
        errorCount++;
        break;
      }
      
      label.LabelAddress = 0xffffffff; // invalid address
      label.LabelIndex = labelIndex;
      strcpy_s(label.LabelName, 0x20, &token.TokenText[0]);
      ac_AddLabel(&VECTOR_LABEL_NAMES, &label);
      labelIndex++;


      // piVar3 = FUN_00403860(local_814, &token.TokenText[0]);
      // local_4a8 = *piVar3;
      // local_4a4 = piVar3[1];
      // piVar3 = (int *)FUN_00403b50(&VECTOR_LABEL_NAMES, local_81c);
      // uVar4 = operator!=(&local_4a8, piVar3);
      // if ((uVar4 & 0xff) != 0) {
      //   ///JP: printf("error (%d) : %s このラベル名は既に使用されています。\n", lineNumber, &token.TokenText[0]);
      //   printf("error (%d) : %s Label name already in use.\n", lineNumber, &token.TokenText[0]);
      //   errorCount++;
      //   break;
      // }
      // label.LabelAddress = 0xffffffff;
      // label.LabelIndex = labelIndex;
      // strcpy_s(label.LabelName, 0x20, &token.TokenText[0]);
      // ac_AddLabel(&VECTOR_LABEL_NAMES, &label);
      // //VECTOR_LABEL_NAMES.push_back(label);
      // labelIndex++;
    }
    lineNumber++;
  }
  
  ///SECTION: Parse commands
  ScriptReader_SetPosition(&reader, 0);
  // local_30 = 0;
  int cmdIndex = 0;
  lineNumber = 1;
  while (!ScriptReader_IsEOF(&reader)) {
    ScriptReader_NextLine(&reader, lineBuffer);
    // zero-out the ANM_TIMELINE structure
    memset(&anmline, 0, 0x44); //kclib_MemZero(&local_4a0,0x44);
    ScriptDecoder_SetBuffer(&decoder, lineBuffer);
    if (token_NextSkipWS(&decoder, &token)) {
      if (token.TokenType == assert_enum(1, TOKEN_POUND)) { // #label definition
        if (token_NextSkipWS(&decoder, &token)) {
          for (int k = 0; k < (int)VECTOR_LABEL_NAMES.size(); k++)
          {
            if (strncmp(VECTOR_LABEL_NAMES[k].LabelName, &token.TokenText[0], 0x20) == 0)
            {
              VECTOR_LABEL_NAMES[k].LabelAddress = cmdIndex;
              break;
            }
          }
          // piVar3 = FUN_00403860(local_824, &token.TokenText[0]);
          // local_4a8 = *piVar3;
          // local_4a4 = piVar3[1];
          // piVar3 = (int *)FUN_00403b50(&VECTOR_LABEL_NAMES, local_82c);
          // uVar4 = operator!=(&local_4a8, piVar3);
          // if ((uVar4 & 0xff) != 0) {
          //   iVar2 = FUN_00403d10(&local_4a8);
          //   *(int *)(iVar2 + 0x20) = cmdIndex;
          // }
        }
      }
      // [ID] command
      else if (token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
        anmline.CmdType = assert_enum(0, ANM_CMD_ID);
        anmline.Args[0].VarType = assert_enum(0, ANM_TYPE_CONST);
        anmline.Args[0].Value = token.TokenValue.UInt32;
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
            anmline.Args[2].VarType = anmline.Args[1].VarType;
            anmline.Args[2].Value   = anmline.Args[1].Value;
          }
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      // [@ID] command
      else if (token.TokenType == assert_enum(0x27, TOKEN_ATSIGN_KEYWORD)) {
        anmline.CmdType = assert_enum(0, ANM_CMD_ID);
        anmline.Args[0].VarType = assert_enum(1, ANM_TYPE_VARIABLE);
        // read next token for [@ID] variable number
        if (!token_NextSkipWS(&decoder, &token)) {
          ///JP: printf("error (%d) : 変数番号が不正です。\n", lineNumber);
          printf("error (%d) : Invalid variable number.\n", lineNumber);
          errorCount++;
        }
        // Check ANM local variable limit (0-63 (0x3f))
        else if (token.TokenValue.Int32 < 0 || token.TokenValue.Int32 >= 0x40) {
          ///JP: printf("error (%d) : 使用できる変数の範囲(0〜63)を超えています。\n", lineNumber);
          printf("error (%d) : Exceeded range of available variables.\n", lineNumber);
          errorCount++;
        }
        else {
          anmline.Args[0].Value = token.TokenValue.UInt32;
          if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
            ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
            printf("error (%d) : Invalid parameter.\n", lineNumber);
            errorCount++;
          }
          else { // optional max range parameter
            if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
              anmline.Args[2].VarType = anmline.Args[1].VarType;
              anmline.Args[2].Value   = anmline.Args[1].Value;
            }
            ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
            cmdIndex += 1;
          }
        }
      }
      else if (_stricmp(&token.TokenText[0], "jump") == 0) {
        anmline.CmdType = assert_enum(3, ANM_CMD_JUMP);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "loop") == 0) {
        anmline.CmdType = assert_enum(2, ANM_CMD_LOOP);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "set") == 0) {
        anmline.CmdType = assert_enum(1, ANM_CMD_SET);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
              anmline.Args[2].VarType = anmline.Args[1].VarType;
              anmline.Args[2].Value   = anmline.Args[1].Value;
          }
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "if") == 0) {
        anmline.CmdType = assert_enum(4, ANM_CMD_IF);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ife") == 0) {
        anmline.CmdType = assert_enum(5, ANM_CMD_IFE);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifn") == 0) {
        anmline.CmdType = assert_enum(6, ANM_CMD_IFN);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifg") == 0) {
        anmline.CmdType = assert_enum(7, ANM_CMD_IFG);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifs") == 0) {
        anmline.CmdType = assert_enum(8, ANM_CMD_IFS);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifge") == 0) {
        anmline.CmdType = assert_enum(9, ANM_CMD_IFGE);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifse") == 0) {
        anmline.CmdType = assert_enum(10, ANM_CMD_IFSE);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "max") == 0) {
        anmline.CmdType = assert_enum(0xb, ANM_CMD_MAX);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "blend") == 0) {
        anmline.CmdType = assert_enum(0xc, ANM_CMD_BLEND);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "disp") == 0) {
        anmline.CmdType = assert_enum(0xd, ANM_CMD_DISP);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "pos") == 0) {
        anmline.CmdType = assert_enum(0xe, ANM_CMD_POS);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "wait") == 0) {
        anmline.CmdType = assert_enum(0xf, ANM_CMD_WAIT);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
            anmline.Args[1].VarType = anmline.Args[0].VarType;
            anmline.Args[1].Value   = anmline.Args[0].Value;
          }
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "add") == 0) {
        anmline.CmdType = assert_enum(0x10, ANM_CMD_ADD);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "sub") == 0) {
        anmline.CmdType = assert_enum(0x11, ANM_CMD_SUB);
        if (!ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: printf("error (%d) : パラメータが不正です。\n", lineNumber);
          printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          ac_AddTimeline(&VECTOR_ANM_LINES, &anmline);
          cmdIndex += 1;
        }
      }
    }
    lineNumber += 1;
  }
  
  
  ///SECTION: Resolve labels
  for (int i = 0; i < (int)VECTOR_ANM_LINES.size(); i++)
  {
    ANM_TIMELINE *curline = &VECTOR_ANM_LINES[i];
    for (int j = 0; j < 8; j++)
    {
      ANM_ARG *curarg = &curline->Args[j];
      if (curarg->VarType != ANM_TYPE_LABEL)
        continue;

      //int labelIndex = (int)curarg->Value;
      //if (labelIndex < 0 || labelIndex >= (int)VECTOR_LABEL_NAMES.size())
      unsigned int labelIndex = (unsigned int)curarg->Value;
      if (labelIndex >= (unsigned int)VECTOR_LABEL_NAMES.size())
      {
          ///JP: printf("error : ラベル変換中に異常が発生しました。\n");
          printf("error : An error occurred during label conversion.\n");
          break;
      }

      ///TODO: Magic things...

      ///PLACEHOLDER:
      unsigned int *curlabel = ((unsigned int *)0);


      curarg->VarType = assert_enum(0, ANM_TYPE_CONST);
      curarg->Value = VECTOR_LABEL_NAMES[labelIndex].LabelAddress;
    }
  }
  // STL iterator
  // piVar3 = (int *)FUN_00403c80(&VECTOR_ANM_LINES,local_834);
  // local_4b0 = *piVar3;
  // local_4ac = piVar3[1];
  // while( true ) {
  //   piVar3 = (int *)FUN_00403b50(&VECTOR_ANM_LINES,local_844);
  //   uVar4 = operator!=(&local_4b0,piVar3);
  //   if ((uVar4 & 0xff) == 0) break;
  //   local_2c = 0;
  //   // for each parameter in the ANM_TIMELINE
  //   //  always 8, regardless of number used by cmd,
  //   //  I'm glad ANM 8-parameters could finally be confirmed
  //   while (local_2c < 8) {
  //     iVar2 = FUN_00403d10(&local_4b0);
  //     if (*(int *)(iVar2 + 4 + local_2c * 8) == 2) {
  //       iVar2 = FUN_00403d10(&local_4b0);
  //       piVar3 = FUN_00403900(local_84c,*(int *)(iVar2 + 8 + local_2c * 8));
  //       local_4a8 = *piVar3;
  //       local_4a4 = piVar3[1];
  //       piVar3 = (int *)FUN_00403b50(&VECTOR_LABEL_NAMES,local_854);
  //       uVar4 = operator!=(&local_4a8,piVar3);
  //       if ((uVar4 & 0xff) == 0) {
  //         ///JP: printf("error : ラベル変換中に異常が発生しました。\n");
  //         printf("error : An error occurred during label conversion.\n");
  //         break;
  //       }
  //       ///REWRITE: Less STL madness

  //       iVar2 = FUN_00403d10(&local_4b0);
  //       *(undefined4 *)(iVar2 + 4 + local_2c * 8) = 0;
  //       iVar2 = FUN_00403d10(&local_4a8);
  //       iVar5 = FUN_00403d10(&local_4b0);
  //       *(undefined4 *)(iVar5 + 8 + local_2c * 8) = *(undefined4 *)(iVar2 + 0x20); // label line number
  //     }
  //     local_2c += 1;
  //   }
  //   FUN_00403d30(&local_4b0,local_83c);
  // }
  // local_8 = (uint)local_8._1_3_ << 8;
  ScriptReader_Close(&reader);
  // local_8 = 0xffffffff;
  ScriptDecoder_dtor(&decoder);
  // *in_FS_OFFSET = local_10;
  // __end_security_critical();
  // return;
  ///WHAT:??? I still don't understand what the return is basing things off if a compile with errors still writes to th file
  // So errorcount only returns on a failure to open the file,
  // so that's what's happening
  return 0; //errorCount;
}

#endif
