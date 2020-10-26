
// OOP kcAnmScript class function definition for kcAnmScript::ParseScript
// 
// NOTE: kcAnmScript likely has no part in the actual ac.exe program,
//       but it's easier to group them all into the same place, even if a bit awkward.
//       some parts still need cleaning up, especially around field reuse.

#include "common.h"
#ifdef KCLIB_OOP

#include "kcAnmScript.h"
#include "kclib_common.h"
#include "ghidra_types_min.h"
#include <windows.h>
#include <vector>


//void __cdecl FUN_004036f0(void *param_1,undefined4 *param_2,undefined4 param_3)
///FID: FUN_004036f0
bool kclib::kcAnmScript::ac_ParseAnmArg(TokenParser *decoder, OUT ANM_ARG *arg, unsigned int lineNumber)
///ARGS: local_600
{
  // int iVar1;
  // int *piVar2;
  // uint uVar3;
  // undefined local_444 [8];
  // int local_43c [2];
  // int local_434;
  // int local_430;
  TOKEN_RESULT token; //undefined8 local_42c;
  // undefined8 local_42c;
  // undefined4 local_424;
  // char local_40c [1028];
  
  // FUN_00403cf0(&local_434); //String_const_iterator<>(&local_434);
  if (decoder->NextTokenSkipWS(&token)) {
    if (token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
      arg->VarType = assert_enum(0, ANM_TYPE_CONST);
      arg->Value = token.TokenValue.UInt32; //local_424;
      return true;
    }
    else if (token.TokenType == assert_enum(0x27, TOKEN_ATSIGN_KEYWORD)) {
      if (decoder->NextTokenSkipWS(&token) &&
        token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
        arg->VarType = assert_enum(1, ANM_TYPE_VARIABLE);
        arg->Value = token.TokenValue.UInt32; //local_424;
        return true;
      }
      else {
        ///JP: std::printf("error (%d) : 変数番号が不正です。\n", lineNumber);
        std::printf("error (%d) : Invalid variable number.\n", lineNumber);
        return false;
      }
      // if (!decoder->NextTokenSkipWS(&token)) {
      //   ///JP: std::printf("error (%d) : 変数番号が不正です。\n", lineNumber);
      //   std::printf("error (%d) : Invalid variable number.\n", lineNumber);
      // }
      // else if (token.TokenType == assert_enum(0xd, TOKEN_INTEGER_LITERAL)) {
      //   arg->VarType = assert_enum(1, ANM_TYPE_VARIABLE);
      //   arg->Value = token.TokenValue.UInt32; //local_424;
      // }
      // else {
      //   ///JP: std::printf("error (%d) : 変数番号が不正です。\n", lineNumber);
      //   std::printf("error (%d) : Invalid variable number.\n", lineNumber);
      // }
    }
    else {
      // Looks like an STL std::map<std::string, LABEL_STRUCT>
      //                or std::map<const char *, LABEL_STRUCT>
      for (int k = 0; k < (int)this->ac_LabelNames.size(); k++)
      {
        if (std::strncmp(this->ac_LabelNames[k].LabelName, &token.TokenText[0], 0x20) == 0)
        {
          arg->VarType = assert_enum(2, ANM_TYPE_LABEL);
          arg->Value = this->ac_LabelNames[k].LabelIndex;
          return true;
        }
      }
      ///JP: std::printf("error (%d) : 未定義のラベルです。\n", lineNumber);
      std::printf("error (%d) : Undefined label.\n", lineNumber);
      return false;

      // piVar2 = FUN_00403860(local_43c, &token.TokenText[0]);
      // local_434 = *piVar2;
      // local_430 = piVar2[1];
      // piVar2 = (int *)FUN_00403b50(&this->ac_LabelNames, local_444);
      // uVar3 = FUN_00403d70(&local_434, piVar2);
      // if ((uVar3 & 0xff) == 0) {
      //   arg->VarType = assert_enum(2, ANM_TYPE_LABEL);
      //   iVar1 = FUN_00403d10(&local_434);
      //   arg->Value = *(unsigned int *)(iVar1 + 0x24);
      // }
      // else {
      //   ///JP: std::printf("error (%d) : 未定義のラベルです。\n", lineNumber);
      //   std::printf("error (%d) : Undefined label.\n", lineNumber);
      //   return false;
      // }
    }
  }
  // return true;
  return false;
}


bool kclib::kcAnmScript::ac_ParseScript(ScriptReader *reader, OPTIONAL OUT unsigned int *outErrorCount)

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
  // unsigned int errorCount; //int local_604;
  TokenParser decoder; //int local_600 [74]; //sizeof() == 0x128 - SCRIPT_DECODER?
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
  //SCRIPT_READER reader; //int local_24 [4];
                    // Can't be SCRIPT_DECODER, too small...
                    // maybe SCRIPT_DECODER with that full 0x124+ size is an extended version?
                    // possibly file text parser
  // int lineNumber; //int local_14;
  // int *local_10;
  // undefined *puStack12;
  //undefined4 local_8;
  
  // local_8 = 0xffffffff;
  // puStack12 = &LAB_0044fd03;
  // local_10 = *in_FS_OFFSET;
  // local_34 = DAT_004817cc ^ (uint)&stack0xfffffffc;
  // *(int ***)in_FS_OFFSET = &local_10;
  // ScriptDecoder_ctor(&decoder);
  // local_8 = 0;
  // local_28 = 0;
  // FUN_00403cf0(&local_4b0); //String_const_iterator<>(&local_4b0);
  // FUN_00403cf0(&local_4a8); //String_const_iterator<>(&local_4a8);
  //ScriptReader_ctor(&reader);
  // local_8._0_1_ = 1;
  unsigned int errorCount = 0;
  
  decoder.SetEnableKeywords(0); //ScriptDecoder_SetScrUnk72(&decoder, 0); //FUN_00410870(&decoder, 0);
  
  
  this->ac_AnmLines.clear(); //VECTOR_ANM_LINES.clear(); //FUN_00403be0((int)&VECTOR_ANM_LINES);
  this->ac_LabelNames.clear(); //VECTOR_LABEL_NAMES.clear(); //FUN_00403be0((int)&VECTOR_LABEL_NAMES);
  // if (!ScriptReader_Open(&reader, filename)) {
  //   errorCount++;
  //   ///JP: std::printf("ファイルのロードに失敗しました : %s", filename);
  //   std::printf("Failed to load file : %s", filename);
  //   //local_80c = errorCount; // EAX <- errorCount
  //   // local_8 = (uint)local_8._1_3_ << 8;
  //   ScriptReader_Close(&reader);
  //   // local_8 = 0xffffffff;
  //   ScriptDecoder_dtor(&decoder);
  //   return errorCount;
  // }

  ///SECTION: Parse labels
  // local_30 = 0;
  int labelIndex = 0;
  int lineNumber = 1;
  while (!reader->IsEOF()) {
    reader->NextLine(lineBuffer);
    decoder.SetBuffer(lineBuffer);
    if (decoder.NextTokenSkipWS(&token) &&
        (token.TokenType == assert_enum(1, TOKEN_POUND)))
    {
      if (!decoder.NextTokenSkipWS(&token)) {
        ///JP: std::printf("error (%d) : 不正なラベル名です。\n", lineNumber);
        std::printf("error (%d) : Illegal label name.\n", lineNumber);
        errorCount++;
        break;
      }
      // That's right, as long as the token parses, you can have ANYTHING as a label name

      bool labelExists = false;
      for (int k = 0; k < (int)this->ac_LabelNames.size(); k++)
      {
        if (std::strncmp(this->ac_LabelNames[k].LabelName, &token.TokenText[0], 0x20) == 0)
        {
          labelExists = true;
          break;
        }
      }
      if (labelExists)
      {
        ///JP: std::printf("error (%d) : %s このラベル名は既に使用されています。\n", lineNumber, &token.TokenText[0]);
        std::printf("error (%d) : %s Label name already in use.\n", lineNumber, &token.TokenText[0]);
        errorCount++;
        break;
      }
      
      label.LabelAddress = 0xffffffff; // invalid address
      label.LabelIndex = labelIndex;
      //strcpy_s(label.LabelName, 0x20, &token.TokenText[0]);
      std::strncpy(label.LabelName, &token.TokenText[0], 0x20);
      // ac_AddLabel(&this->ac_LabelNames, &label);
      this->ac_LabelNames.push_back(label);
      labelIndex++;


      // piVar3 = FUN_00403860(local_814, &token.TokenText[0]);
      // local_4a8 = *piVar3;
      // local_4a4 = piVar3[1];
      // piVar3 = (int *)FUN_00403b50(&this->ac_LabelNames, local_81c);
      // uVar4 = operator!=(&local_4a8, piVar3);
      // if ((uVar4 & 0xff) != 0) {
      //   ///JP: std::printf("error (%d) : %s このラベル名は既に使用されています。\n", lineNumber, &token.TokenText[0]);
      //   std::printf("error (%d) : %s Label name already in use.\n", lineNumber, &token.TokenText[0]);
      //   errorCount++;
      //   break;
      // }
      // label.LabelAddress = 0xffffffff;
      // label.LabelIndex = labelIndex;
      // strcpy_s(label.LabelName, 0x20, &token.TokenText[0]);
      // ac_AddLabel(&this->ac_LabelNames, &label);
      // //this->ac_LabelNames.push_back(label);
      // labelIndex++;
    }
    lineNumber++;
  }
  
  ///SECTION: Parse commands
  reader->SetPosition(0);
  // local_30 = 0;
  int cmdIndex = 0;
  lineNumber = 1;
  while (!reader->IsEOF()) {
    reader->NextLine(lineBuffer);
    // zero-out the ANM_TIMELINE structure
    std::memset(&anmline, 0, 0x44); //kclib_MemZero(&local_4a0,0x44);
    decoder.SetBuffer(lineBuffer);
    if (decoder.NextTokenSkipWS(&token)) {
      if (token.TokenType == assert_enum(1, TOKEN_POUND)) { // #label definition
        if (decoder.NextTokenSkipWS(&token)) {
          for (int k = 0; k < (int)this->ac_LabelNames.size(); k++)
          {
            if (std::strncmp(this->ac_LabelNames[k].LabelName, &token.TokenText[0], 0x20) == 0)
            {
              this->ac_LabelNames[k].LabelAddress = cmdIndex;
              break;
            }
          }
          // piVar3 = FUN_00403860(local_824, &token.TokenText[0]);
          // local_4a8 = *piVar3;
          // local_4a4 = piVar3[1];
          // piVar3 = (int *)FUN_00403b50(&this->ac_LabelNames, local_82c);
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
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
            anmline.Args[2].VarType = anmline.Args[1].VarType;
            anmline.Args[2].Value   = anmline.Args[1].Value;
          }
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      // [@ID] command
      else if (token.TokenType == assert_enum(0x27, TOKEN_ATSIGN_KEYWORD)) {
        anmline.CmdType = assert_enum(0, ANM_CMD_ID);
        anmline.Args[0].VarType = assert_enum(1, ANM_TYPE_VARIABLE);
        // read next token for [@ID] variable number
        if (!decoder.NextTokenSkipWS(&token)) {
          ///JP: std::printf("error (%d) : 変数番号が不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid variable number.\n", lineNumber);
          errorCount++;
        }
        // Check ANM local variable limit (0-63 (0x3f))
        else if (token.TokenValue.Int32 < 0 || token.TokenValue.Int32 >= 0x40) {
          ///JP: std::printf("error (%d) : 使用できる変数の範囲(0〜63)を超えています。\n", lineNumber);
          std::printf("error (%d) : Exceeded range of available variables.\n", lineNumber);
          errorCount++;
        }
        else {
          anmline.Args[0].Value = token.TokenValue.UInt32;
          if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
            ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
            std::printf("error (%d) : Invalid parameter.\n", lineNumber);
            errorCount++;
          }
          else { // optional max range parameter
            if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
              anmline.Args[2].VarType = anmline.Args[1].VarType;
              anmline.Args[2].Value   = anmline.Args[1].Value;
            }
            this->ac_AnmLines.push_back(anmline);
            cmdIndex += 1;
          }
        }
      }
      else if (_stricmp(&token.TokenText[0], "jump") == 0) {
        anmline.CmdType = assert_enum(3, ANM_CMD_JUMP);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "loop") == 0) {
        anmline.CmdType = assert_enum(2, ANM_CMD_LOOP);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "set") == 0) {
        anmline.CmdType = assert_enum(1, ANM_CMD_SET);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
              anmline.Args[2].VarType = anmline.Args[1].VarType;
              anmline.Args[2].Value   = anmline.Args[1].Value;
          }
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "if") == 0) {
        anmline.CmdType = assert_enum(4, ANM_CMD_IF);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ife") == 0) {
        anmline.CmdType = assert_enum(5, ANM_CMD_IFE);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifn") == 0) {
        anmline.CmdType = assert_enum(6, ANM_CMD_IFN);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifg") == 0) {
        anmline.CmdType = assert_enum(7, ANM_CMD_IFG);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifs") == 0) {
        anmline.CmdType = assert_enum(8, ANM_CMD_IFS);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifge") == 0) {
        anmline.CmdType = assert_enum(9, ANM_CMD_IFGE);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "ifse") == 0) {
        anmline.CmdType = assert_enum(10, ANM_CMD_IFSE);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[2], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "max") == 0) {
        anmline.CmdType = assert_enum(0xb, ANM_CMD_MAX);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "blend") == 0) {
        anmline.CmdType = assert_enum(0xc, ANM_CMD_BLEND);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "disp") == 0) {
        anmline.CmdType = assert_enum(0xd, ANM_CMD_DISP);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "pos") == 0) {
        anmline.CmdType = assert_enum(0xe, ANM_CMD_POS);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "wait") == 0) {
        anmline.CmdType = assert_enum(0xf, ANM_CMD_WAIT);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else { // optional max range parameter
          if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
            anmline.Args[1].VarType = anmline.Args[0].VarType;
            anmline.Args[1].Value   = anmline.Args[0].Value;
          }
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "add") == 0) {
        anmline.CmdType = assert_enum(0x10, ANM_CMD_ADD);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
      else if (_stricmp(&token.TokenText[0], "sub") == 0) {
        anmline.CmdType = assert_enum(0x11, ANM_CMD_SUB);
        if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[0], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else if (!this->ac_ParseAnmArg(&decoder, &anmline.Args[1], lineNumber)) {
          ///JP: std::printf("error (%d) : パラメータが不正です。\n", lineNumber);
          std::printf("error (%d) : Invalid parameter.\n", lineNumber);
          errorCount++;
        }
        else {
          this->ac_AnmLines.push_back(anmline);
          cmdIndex += 1;
        }
      }
    }
    lineNumber += 1;
  }
  
  
  ///SECTION: Resolve labels
  ///NOTE: Labels that appear after the last instruction ARE legal!
  for (int i = 0; i < (int)this->ac_AnmLines.size(); i++)
  {
    ANM_TIMELINE *curline = &this->ac_AnmLines[i];
    for (int j = 0; j < 8; j++)
    {
      ANM_ARG *curarg = &curline->Args[j];
      if (curarg->VarType != ANM_TYPE_LABEL)
        continue;

      //int labelIndex = (int)curarg->Value;
      //if (labelIndex < 0 || labelIndex >= (int)this->ac_LabelNames.size())
      unsigned int labelIndex = (unsigned int)curarg->Value;
      if (labelIndex >= (unsigned int)this->ac_LabelNames.size())
      {
          ///JP: std::printf("error : ラベル変換中に異常が発生しました。\n");
          std::printf("error : An error occurred during label conversion.\n");
          break;
      }

      curarg->VarType = assert_enum(0, ANM_TYPE_CONST);
      curarg->Value = this->ac_LabelNames[labelIndex].LabelAddress;
    }
  }
  // STL iterator
  // piVar3 = (int *)FUN_00403c80(&this->ac_AnmLines,local_834);
  // local_4b0 = *piVar3;
  // local_4ac = piVar3[1];
  // while( true ) {
  //   piVar3 = (int *)FUN_00403b50(&this->ac_AnmLines,local_844);
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
  //       piVar3 = (int *)FUN_00403b50(&this->ac_LabelNames,local_854);
  //       uVar4 = operator!=(&local_4a8,piVar3);
  //       if ((uVar4 & 0xff) == 0) {
  //         ///JP: std::printf("error : ラベル変換中に異常が発生しました。\n");
  //         std::printf("error : An error occurred during label conversion.\n");
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
  // *in_FS_OFFSET = local_10;
  // __end_security_critical();
  // return;
  ///WHAT:??? I still don't understand what the return is basing things off if a compile with errors still writes to th file
  // So errorcount only returns on a failure to open the file,
  // so that's what's happening
  *outErrorCount = errorCount;

  // success if at least one command is parsed (not sure if done this way by Cs2)
  return !this->ac_AnmLines.empty(); //errorCount;
}

#endif
