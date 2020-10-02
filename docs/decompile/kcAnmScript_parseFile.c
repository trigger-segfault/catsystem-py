#include <assert.h>
#include <stdio.h>

//Temporary defines, used during unobfuscation
#define BOOL int
#define INT int
#define UINT unsigned int
#define LONG long
#define ULONG unsigned long
#define VOID void
#define LPCSTR const char*
#define FALSE 0
#define TRUE 1
// undetermined signedness
#define undefined4 UINT

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


//__cdecl
void __cdecl
__invalid_parameter(
    wchar_t const* const expression,
    wchar_t const* const function_name,
    wchar_t const* const file_name,
    unsigned int   const line_number,
    uintptr_t      const reserved);


VOID kcAnmScript_sub_ParseFile_unk_004c0c90(VOID)

{
  // dummy func
}
INT cs2_unk_read_kcBigFiles_005c4720(VOID)

{
  return 0U; // dummy func
}
INT kcAnmScript_readFile(UINT param_2)

{
  return 0; // dummy func
}
UINT kcAnmScript_Create_unk_005986f0(VOID)

{
  return 0U; // dummy func
}
VOID call__invalid_parameter(VOID)

{
  __invalid_parameter(0,0,0,0,0);
}
VOID FUN_004c0d10(VOID)

{
  // dummy func
}
VOID cs2_strmemcpy_004b1600(INT *src, INT *dst, UINT size)

{
  // dummy func
}

//undefined4 kcAnmScript_ParseFile_005992a0(undefined4 param_1, UINT param_2)
//  **************************************************************
//  *                          FUNCTION                          *
//  **************************************************************
// undefined kcAnmScript_ParseFile_005992a0(undefined4 para
// undefined         AL:1           <RETURN>
// undefined4        Stack[0x4]:4   param_1
// undefined4        Stack[0x8]:4   param_2
BOOL kcAnmScript_parseFile(undefined4 param_1, UINT param_2)

{
  INT **ppiVar1;
  INT *piVar2;
  INT *piVar3;
  INT iVar4;
  undefined4 uVar5;
  INT unaff_EBX;
  INT **ppiVar6;
  
  if (*(INT *)(unaff_EBX + 0x48) != 0) {
    kcAnmScript_sub_ParseFile_unk_004c0c90();
    *(undefined4 *)(unaff_EBX + 0x48) = 0;
  }
  iVar4 = cs2_unk_read_kcBigFiles_005c4720();
  if (iVar4 == 0) {
    uVar5 = 0;
  }
  else {
    uVar5 = kcAnmScript_Create_unk_005986f0();
  }
  *(undefined4 *)(unaff_EBX + 0x48) = uVar5;
  iVar4 = kcAnmScript_readFile(param_2);
  if (iVar4 == 0) {
    if (*(INT *)(unaff_EBX + 0x48) != 0) {
      kcAnmScript_sub_ParseFile_unk_004c0c90();
      *(undefined4 *)(unaff_EBX + 0x48) = 0;
    }
    return FALSE; // 0;
  }
  ppiVar1 = *(INT ***)(unaff_EBX + 0x3c);
  if (ppiVar1 != (INT **)0x0) {
    piVar2 = (INT *)*ppiVar1[6];
    ppiVar1 = (INT **)*ppiVar1;
    param_2 = 0;
    while (TRUE) {
      piVar3 = (*(INT ***)(unaff_EBX + 0x3c))[6];
      if ((ppiVar1 == (INT **)0x0) || (ppiVar1 != (INT **)**(INT ***)(unaff_EBX + 0x3c))) {
        __invalid_parameter(0,0,0,0,0); //call__invalid_parameter();
      }
      if (piVar2 == piVar3) break;
      if (ppiVar1 == (INT **)0x0) {
        __invalid_parameter(0,0,0,0,0); //call__invalid_parameter();
        iVar4 = 0;
      }
      else {
        iVar4 = (INT)*ppiVar1;
      }
      if (piVar2 == *(INT **)(iVar4 + 0x18)) {
        __invalid_parameter(0,0,0,0,0); //call__invalid_parameter();
      }
      if (param_2 < (UINT)piVar2[3]) {
        if (ppiVar1 == (INT **)0x0) {
          __invalid_parameter(0,0,0,0,0); //call__invalid_parameter();
          ppiVar6 = ppiVar1;
        }
        else {
          ppiVar6 = (INT **)*ppiVar1;
        }
        if (piVar2 == (INT *)((INT *)ppiVar6)[6]) {
          __invalid_parameter(0,0,0,0,0); //call__invalid_parameter();
        }
        param_2 = piVar2[3];
      }
      FUN_004c0d10();
    }
    *(UINT *)(*(INT *)(unaff_EBX + 0x48) + 0x1c) = param_2;
  }
  if (unaff_EBX + 0x54 != 0) {
    cs2_strmemcpy_004b1600(unaff_EBX + 0x54,*(INT *)(unaff_EBX + 0x48) + 0x128,0x14);
  }
  return TRUE; // 1;
}

