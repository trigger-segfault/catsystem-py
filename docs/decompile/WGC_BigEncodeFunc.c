#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned char undefined;
typedef unsigned short ushort;
typedef unsigned short undefined2;
typedef unsigned int uint;
typedef unsigned int undefined4;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef unsigned long long undefined8;
typedef RECT tagRECT;
typedef void code;

#define true TRUE
#define false FALSE
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Could not reconcile some variable overlaps */

void hg_calls_encode_FUN_00412590(HWND *param_1)

{
  char cVar1;
  LONG LVar2;
  float fVar3;
  BOOL BVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  kcImageHg2 *this;
  kcImageHg3 *this_00;
  kcImage *this_01;
  char *pcVar8;
  HWND__ HVar9;
  HWND__ HVar10;
  code **ppcVar11;
  tagRECT *lprc;
  int iVar12;
  int iVar13;
  int *piVar14;
  HWND__ *pHVar15;
  HWND hWnd;
  HWND *ppHVar16;
  undefined4 *_Dst;
  uint uVar17;
  code **ppcVar18;
  int **in_FS_OFFSET;
  bool in_PF;
  bool bVar19;
  ushort in_FPUControlWord;
  char *pcVar20;
  undefined auStack3924 [4];
  HWND *local_f50;
  HWND local_f4c;
  HWND__ local_f48;
  HWND local_f44;
  code **local_f40;
  int local_f3c;
  int local_f38;
  int *local_f34;
  code **local_f30;
  int local_f2c [2];
  HWND__ local_f24;
  int local_f20;
  uint local_f1c;
  int local_f18;
  WPARAM local_f14;
  int local_f10;
  uint local_f0c;
  uint local_f08;
  undefined4 local_f04;
  int local_f00;
  int local_efc;
  int local_ef8;
  HWND__ local_ef0;
  int local_eec;
  int local_ee8;
  int local_ee4;
  int local_ee0;
  int local_edc;
  ulonglong local_ed8;
  int local_ecc;
  int local_ec8;
  RECT local_ec4;
  int local_eb4 [4];
  undefined4 local_ea4;
  undefined4 local_ea0;
  undefined4 local_e9c;
  undefined4 local_e98;
  LONG local_e94 [4];
  undefined4 local_e84;
  undefined4 local_e80;
  undefined4 local_e7c;
  undefined4 local_e78;
  LONG local_e74 [4];
  undefined4 local_e64;
  undefined4 local_e60;
  undefined4 local_e5c;
  undefined4 local_e58;
  HWND__ local_e54 [8];
  int local_e34;
  undefined4 local_e30;
  undefined local_e2c [4];
  int local_e28;
  int local_e24;
  int local_e20;
  undefined4 local_e1c;
  undefined4 local_e18;
  int local_e14;
  undefined4 local_e10;
  undefined4 local_e0c;
  int local_e08;
  int local_e04;
  int local_e00;
  int local_dfc;
  uint local_df8;
  tagRECT local_df4;
  int local_de4 [4];
  undefined4 local_dd4;
  undefined4 local_dd0;
  undefined4 local_dcc;
  int local_dc8 [4];
  undefined4 local_db8;
  undefined4 local_db4;
  undefined4 local_db0;
  undefined4 local_dac;
  undefined4 local_da8;
  tagRECT local_da4;
  undefined local_d94 [4];
  undefined local_d90 [4];
  tagRECT local_d8c;
  HWND__ local_d78;
  undefined local_d74 [4];
  HWND__ local_d70 [3];
  LONG local_d64;
  tagRECT local_d60 [8];
  kcFile local_ce0 [272];
  CHAR local_bd0 [260];
  int *local_acc;
  char local_ac8 [256];
  CHAR local_9c8 [128];
  char local_948 [2];
  undefined local_946 [190];
  char acStack2184 [64];
  undefined4 local_848 [128];
  undefined auStack1608 [264];
  undefined auStack1344 [264];
  CHAR local_438 [256];
  char local_338 [264];
  CHAR local_230 [264];
  undefined local_128 [268];
  uint local_1c;
  int *local_14;
  undefined *puStack16;
  undefined4 local_c;
  
  local_c = 0xffffffff;
  puStack16 = &LAB_00601b86;
  local_14 = *in_FS_OFFSET;
  local_1c = DAT_006883c8 ^ (uint)auStack3924;
  *(int ***)in_FS_OFFSET = &local_14;
  local_f50 = param_1;
  kcFile_ctor(local_ce0);
  local_c = 0;
  lprc = local_d60;
  local_efc = 7;
  do {
    SetRectEmpty((LPRECT)lprc);
    lprc = (tagRECT *)((LPRECT)lprc + 1);
    local_efc += -1;
  } while (-1 < local_efc);
  local_f04 = 1;
  local_f10 = 0;
  local_f0c = 0;
  local_f08 = 0;
  local_f00 = 0;
  local_c = CONCAT31(local_c._1_3_,1);
  SendDlgItemMessageA(*param_1,1000,0x401,0,0xa0000);
  SendDlgItemMessageA(*param_1,1000,0x402,0,0);
  SendDlgItemMessageA(*param_1,0x3e9,0x401,0,(uint)*(ushort *)(param_1 + 7) << 0x10);
  SendDlgItemMessageA(*param_1,0x3e9,0x402,0,0);
  local_f4c = param_1[1];
  local_f48 = (HWND__)((HWND__ *)&param_1[6]->unused)->unused;
  local_efc = 0;
  while( true ) {
    HVar10 = (HWND__)((HWND__ *)(param_1 + 6))->unused;
    if ((local_f4c == (HWND)0x0) || (local_f4c != param_1[1])) {
      FUN_0041d16e();
    }
    if ((local_f48 == HVar10) || (param_1[9] != (HWND)0x0)) goto LAB_00414612;
    local_ee8 = 0;
    local_f3c = 0;
    local_f38 = 0;
    local_ef8 = 0;
    local_f18 = 0;
    _Dst = local_848;
    local_de4[0] = 0;
    local_de4[1] = 0;
    local_de4[2] = 0;
    local_de4[3] = 0;
    local_dd4 = 0;
    local_dd0 = 0;
    local_dcc = 0;
    local_dc8[0] = 0;
    local_e54[0] = (HWND__)0x0;
    local_e54[1] = 0;
    local_e54[2] = 0;
    local_e54[3] = 0;
    local_e54[4] = 0;
    local_e54[5] = 0;
    local_e54[6] = 0;
    local_e54[7] = 0;
    local_eb4[0] = 0;
    local_eb4[1] = 0;
    local_eb4[2] = 0;
    local_eb4[3] = 0;
    local_ea4 = 0;
    local_ea0 = 0;
    local_e9c = 0;
    local_e98 = 0;
    local_dc8[1] = 0;
    local_dc8[2] = 0;
    local_dc8[3] = 0;
    local_db8 = 0;
    local_db4 = 0;
    local_db0 = 0;
    local_dac = 0;
    local_da8 = 0;
    local_e94[0] = 0;
    local_e94[1] = 0;
    local_e94[2] = 0;
    local_e94[3] = 0;
    local_e84 = 0;
    local_e80 = 0;
    local_e7c = 0;
    local_e78 = 0;
    local_e74[0] = 0;
    local_e74[1] = 0;
    local_e74[2] = 0;
    local_e74[3] = 0;
    local_e64 = 0;
    local_e60 = 0;
    local_e5c = 0;
    local_e58 = 0;
    iVar12 = 8;
    do {
      _strcpy_s((char *)_Dst,0x40,"");
      hWnd = local_f4c;
      _Dst = _Dst + 0x10;
      iVar12 += -1;
    } while (iVar12 != 0);
    local_ecc = 0;
    local_edc = 0;
    HVar10 = local_f48;
    if (param_1[9] == (HWND)0x0) {
      if (local_f4c == (HWND)0x0) {
        FUN_0041d16e();
        iVar12 = 0;
      }
      else {
        iVar12 = local_f4c->unused;
      }
      HVar10 = local_f48;
      if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
        FUN_0041d16e();
      }
      SetDlgItemTextA(*param_1,0x3ea,(LPCSTR)((int)HVar10 + 8));
    }
    if (hWnd == (HWND)0x0) {
      FUN_0041d16e();
      iVar12 = 0;
    }
    else {
      iVar12 = hWnd->unused;
    }
    if (HVar10 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
      FUN_0041d16e();
    }
    local_f44 = (HWND)((int)HVar10 + 8);
    BVar4 = kcFile_Open(local_ce0,(LPCSTR)local_f44,KCFILE_READ|KCFILE_OPEN);
    if (BVar4 == 0) break;
LAB_00412921:
    uVar5 = FUN_00403900();
    if ((local_f00 == 0) && (local_f10 != 0)) {
      free_decref(local_f10);
    }
    local_f10 = 0;
    local_f0c = 0;
    local_f08 = 0;
    local_f00 = 0;
    uVar17 = uVar5;
    if ((uVar5 & 7) != 0) {
      uVar17 = (uVar5 - (uVar5 & 7)) + 8;
    }
    local_f10 = malloc_incref(0,uVar17);
    local_f00 = 0;
    local_f0c = uVar17;
    local_f08 = uVar17;
    FUN_00403770(local_f10,uVar5);
    kcFile_Close2(local_ce0);
    local_f1c = 0;
    if (local_f4c == (HWND)0x0) {
      FUN_0041d16e();
      iVar12 = 0;
    }
    else {
      iVar12 = local_f4c->unused;
    }
    if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
      FUN_0041d16e();
    }
    iVar6 = FUN_00411d40(local_f44,&local_f10,&local_f1c,&local_ee4);
    iVar12 = local_ee4;
    if (iVar6 == 0) {
      hWnd = *param_1;
      goto LAB_00414603;
    }
    if (param_1[9] == (HWND)0x0) {
      SendDlgItemMessageA(*param_1,1000,0x401,0,local_ee4 << 0x10);
    }
    local_f14 = 0;
    if (0 < iVar12) {
      do {
        if (local_f50[9] != (HWND)0x0) break;
        ppcVar11 = local_f40;
        hWnd = local_f4c;
        HVar10 = local_f48;
        switch(local_f1c) {
        case 1:
          iVar12 = FUN_00601980(0x80,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_0041b0e0();
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar12 != 0) {
            if (local_f4c == (HWND)0x0) {
              FUN_0041d16e();
              iVar12 = 0;
            }
            else {
              iVar12 = local_f4c->unused;
            }
            HVar10 = local_f48;
            if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
              FUN_0041d16e();
            }
            FUN_00409450((HWND)((int)HVar10 + 8),local_ac8);
            FUN_00409500(local_ac8,0);
            local_f20 = 0;
            break;
          }
          (**(code **)*ppcVar11)(1);
          goto LAB_00414386;
        case 2:
          iVar6 = FUN_00601980(0x5c,
                               "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                               ,0x27);
          if (iVar6 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_0041b460();
          }
          ppcVar11 = local_f40;
          ppHVar16 = local_f50;
          if (local_f50[10][0x293].unused != 0) {
            iVar12 = (**(code **)(*local_f40 + 4))(&local_f10,0);
            hWnd = local_f4c;
            if (iVar12 == 0) {
              (**(code **)*ppcVar11)(1);
              goto LAB_00414386;
            }
            if (local_f4c == (HWND)0x0) {
              FUN_0041d16e();
              iVar12 = 0;
            }
            else {
              iVar12 = local_f4c->unused;
            }
            HVar10 = local_f48;
            if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
              FUN_0041d16e();
            }
            FUN_00409450((HWND)((int)HVar10 + 8),local_ac8);
            FUN_00409500(local_ac8,0);
            local_f20 = 0;
            break;
          }
          iVar12 = FUN_0041c3c0(&local_f10,(iVar12 - local_f14) + -1,local_ac8,&local_f20);
          if (iVar12 != 0) {
            hWnd = local_f4c;
            HVar10 = local_f48;
            if ((local_f20 == 0) && (ppHVar16[10][0x295].unused != 0)) {
              uVar7 = FUN_00412220(local_ac8);
              FUN_00409450(uVar7);
              FUN_00409500(local_ac8,0);
              local_f1c = 3;
              hWnd = local_f4c;
              HVar10 = local_f48;
            }
            break;
          }
          goto LAB_00412ac0;
        case 3:
          this = (kcImageHg2 *)
                 FUN_00601980(0x1174,
                              "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",
                              0x27);
          if (this == (kcImageHg2 *)0x0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)kcImageHg2_ctor(this);
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 8))(&local_f10,local_f14,&local_e30);
          hWnd = local_f4c;
          if (iVar12 == 0) {
            (**(code **)*ppcVar11)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          HVar10 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar10 + 8),auStack1344);
          FUN_00409500(auStack1344,0);
          wsprintfA(local_ac8,"%s(%d)",auStack1344,local_e30);
          local_f20 = 0;
          break;
        case 4:
          this_00 = (kcImageHg3 *)
                    FUN_00601980(0x1178,
                                 "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                 ,0x27);
          if (this_00 == (kcImageHg3 *)0x0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)kcImageHg3_ctor(this_00);
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 8))(&local_f10,local_f14,&local_e18);
          hWnd = local_f4c;
          if (iVar12 == 0) {
            (**(code **)*ppcVar11)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          HVar10 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar10 + 8),auStack1608);
          FUN_00409500(auStack1608,0);
          wsprintfA(local_ac8,"%s(%d)",auStack1608,local_e18);
          local_f44 = (HWND)(local_e54 + local_f18);
          if (local_f44->unused == 0) {
            this_01 = (kcImage *)
                      FUN_00601980(0x58,
                                   "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                   ,0x27);
            if (this_01 == (kcImage *)0x0) {
              this_01 = (kcImage *)0x0;
            }
            else {
              this_01 = kcImage_ctor(this_01);
            }
            *(kcImage **)&local_f44->unused = this_01;
          }
          FUN_00417730(ppcVar11);
          local_f20 = 0;
          break;
        case 5:
          iVar12 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00418060();
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar12 == 0) {
            (**(code **)*ppcVar11)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          HVar10 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar10 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
          break;
        case 6:
          iVar12 = FUN_00601980(0x60,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00417ae0();
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar12 == 0) {
            (**(code **)*ppcVar11)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          HVar10 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar10 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
          break;
        case 7:
          iVar12 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00417930();
          }
          ppcVar11 = local_f40;
          iVar12 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar12 == 0) {
            (**(code **)*ppcVar11)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          HVar10 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar10 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
        }
        iVar12 = FUN_00411ed0(local_ac8,local_438);
        if (iVar12 != 0) {
          if (hWnd == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = hWnd->unused;
          }
          if (HVar10 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_004093d0((HWND)((int)HVar10 + 8),local_230);
          SetCurrentDirectoryA(local_230);
          BVar4 = kcFile_Open(local_ce0,local_438,KCFILE_READ|KCFILE_OPEN);
          if (BVar4 == 0) {
            wsprintfA(local_9c8,(LPCSTR)&param_2_00613528,local_438);
            ppHVar16 = local_f50;
            iVar12 = MessageBoxA(*local_f50,local_9c8,(LPCSTR)&lpCaption_0065d1f4,1);
            if (iVar12 != 2) goto LAB_004130c3;
            hWnd = *ppHVar16;
          }
          else {
LAB_004130c3:
            uVar5 = FUN_00403900();
            if ((local_f00 == 0) && (local_f10 != 0)) {
              free_decref(local_f10);
            }
            local_f10 = 0;
            local_f0c = 0;
            local_f08 = 0;
            local_f00 = 0;
            uVar17 = uVar5;
            if ((uVar5 & 7) != 0) {
              uVar17 = (uVar5 - (uVar5 & 7)) + 8;
            }
            local_f10 = malloc_incref(0,uVar17);
            local_f00 = 0;
            local_f0c = uVar17;
            local_f08 = uVar17;
            FUN_00403770(local_f10,uVar5);
            kcFile_Close2(local_ce0);
            ppHVar16 = local_f50;
            iVar12 = FUN_00411d40(local_128,&local_f10,&local_f1c,&local_ee4);
            if (iVar12 != 0) {
              if (ppHVar16[9] == (HWND)0x0) {
                SendDlgItemMessageA(*ppHVar16,1000,0x401,0,local_ee4 << 0x10);
              }
              FUN_004161d0(&local_edc,&local_ecc);
              local_f14 = 0xffffffff;
LAB_00412ac0:
              if (ppcVar11 != (code **)0x0) {
                (**(code **)*ppcVar11)(1);
              }
              goto LAB_00414386;
            }
            hWnd = *ppHVar16;
          }
          ppHVar16[9] = (HWND)0x1;
          PostMessageA(hWnd,0x404,0,0);
          break;
        }
        FUN_00409fa0(local_948,local_ac8);
        _strcpy_s(local_948,0x100,local_ac8);
        piVar14 = (int *)(local_ef8 * 0x40);
        hWnd = (HWND)(local_848 + local_ef8 * 0x10);
        local_f44 = hWnd;
        local_f34 = piVar14;
        FUN_00409e90(local_ac8,local_948,&DAT_0061351c,hWnd);
        if ((local_ac8[0] == '@') && (local_ac8[1] == '(')) {
          _strcpy_s(local_948,0x100,local_ac8);
          wsprintfA(local_ac8,"@%s(%s",hWnd,local_946);
        }
        ppHVar16 = local_f50;
        if (local_f50[9] == (HWND)0x0) {
          SetDlgItemTextA(*local_f50,0x3eb,local_ac8);
        }
        if (local_f20 == 1) {
          if (local_f3c < 7) {
            LVar2 = local_e94[local_f3c];
            iVar12 = local_f3c + 1;
            local_e74[iVar12] = local_e74[local_f3c];
            local_e94[iVar12] = LVar2;
            local_dc8[local_f3c + 2] = local_dc8[iVar12];
            local_f38 += 1;
            local_ef8 += 1;
            iVar13 = local_ef8 * 0x40;
            iVar6 = local_ef8 * 0x10;
            local_eb4[local_f38] = (&local_ec4.bottom)[local_f38];
            local_f3c = iVar12;
            _strcpy_s((char *)(local_848 + iVar6),0x40,acStack2184 + iVar13);
            iVar12 = local_e54[local_f18].unused;
            local_f18 += 1;
            if (iVar12 != 0) {
              this_01 = (kcImage *)
                        FUN_00601980(0x58,
                                     "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                     ,0x27);
              if (this_01 == (kcImage *)0x0) {
                this_01 = (kcImage *)0x0;
              }
              else {
                this_01 = kcImage_ctor(this_01);
              }
              *(kcImage **)&local_e54[local_f18].unused = this_01;
              FUN_00417730(iVar12);
            }
            local_de4[local_f3c] = (&local_df4.bottom)[local_f3c];
            bVar19 = local_ac8[0] == '[';
            *(int *)(local_d60 + local_f3c) = local_d70[local_f3c * 4].unused;
            *(int *)((int)(local_d60 + local_f3c) + 4) = local_d70[local_f3c * 4 + 1].unused;
            *(int *)((int)(local_d60 + local_f3c) + 8) = local_d70[local_f3c * 4 + 2].unused;
            *(LONG *)((int)(local_d60 + local_f3c) + 0xc) = (&local_d64)[local_f3c * 4];
            if (bVar19) {
              iVar12 = 0;
              if (local_ac8[1] != ']') {
                pcVar8 = local_ac8 + 1;
                _Dst = (undefined4 *)((int)(local_848 + iVar6) - (int)pcVar8);
                do {
                  if (*pcVar8 == '\0') break;
                  *(char *)((int)_Dst + (int)pcVar8) = *pcVar8;
                  pcVar8 = pcVar8 + 1;
                  iVar12 += 1;
                } while (*pcVar8 != ']');
              }
              iVar13 += iVar12;
              *(undefined *)((int)local_848 + iVar13) = 0;
              *(undefined *)((int)local_848 + iVar13 + 1) = 0;
            }
          }
          iVar12 = 0;
          cVar1 = local_ac8[0];
          while ((cVar1 != '(' && (local_ac8[iVar12] != '\0'))) {
            cVar1 = local_ac8[iVar12 + 1];
            iVar12 = iVar12 + 1;
          }
          if ((local_ac8[iVar12] == '(') &&
             (iVar12 = FUN_00409af0(local_ac8 + iVar12 + 1,local_d94,&local_e34), iVar12 != 0)) {
            local_eb4[local_f38] = local_eb4[local_f38] + local_e34;
          }
          if (local_f40 != (code **)0x0) {
            (**(code **)*local_f40)(1);
          }
          goto LAB_00414386;
        }
        if (local_f20 == 2) {
          if (0 < local_f3c) {
            local_f3c += -1;
          }
          if (0 < local_f38) {
            local_f38 += -1;
          }
          if (0 < local_ef8) {
            local_ef8 += -1;
          }
          if (ppcVar11 != (code **)0x0) {
            (**(code **)*ppcVar11)(1);
          }
          iVar12 = local_f18;
          if ((code **)local_e54[local_f18].unused != (code **)0x0) {
            (***(code ***)(code **)local_e54[local_f18].unused)(1);
            local_e54[iVar12].unused = 0;
          }
          if (0 < iVar12) {
            local_f18 = iVar12 + -1;
          }
          goto LAB_00414386;
        }
        local_f2c[0] = 0;
        iVar12 = 0;
        cVar1 = local_ac8[0];
        while (((cVar1 != '(' && (local_ac8[iVar12] != ' ')) && (local_ac8[iVar12] != '\0'))) {
          cVar1 = local_ac8[iVar12 + 1];
          iVar12 = iVar12 + 1;
        }
        if ((local_ac8[iVar12] == '(') &&
           (iVar12 = FUN_00409af0(local_ac8 + iVar12 + 1,local_e2c,local_f2c), iVar12 == 0)) {
          kclib_logError2(&DAT_006134f4);
        }
        iVar12 = lstrcmpiA(local_ac8,"_base_");
        if (iVar12 == 0) {
          SetRectEmpty((LPRECT)&local_da4);
          FUN_004161d0(local_d70,&local_e24);
          FUN_00416920(&local_da4);
          iVar12 = local_da4.top;
          if (ppHVar16[10][0x28f].unused != 0) {
            iVar12 = local_e24 - local_da4.bottom;
          }
          local_e74[local_f3c] = local_da4.left;
          local_e94[local_f3c] = iVar12;
          local_dc8[local_f3c + 1] = 1;
        }
        iVar12 = local_f3c;
        iVar6 = lstrcmpiA(local_ac8,"_clip_");
        if (iVar6 == 0) {
          SetRectEmpty((LPRECT)&local_ec4);
          iVar6 = FUN_00416920(&local_ec4);
          if (iVar6 != 0) {
            FUN_004161d0(local_d90,&local_ec8);
            if (local_f50[10][0x28f].unused != 0) {
              local_ec4.bottom = local_ec8 - local_ec4.bottom;
              local_ec8 -= local_ec4.top;
              local_ec4.top = local_ec4.bottom;
              local_ec4.bottom = local_ec8;
            }
            BVar4 = IsRectEmpty(&local_ec4);
            if (BVar4 == 0) {
              *(LONG *)(local_d60 + iVar12) = local_ec4.left;
              *(int *)((int)(local_d60 + iVar12) + 4) = local_ec4.top;
              *(LONG *)((int)(local_d60 + iVar12) + 8) = local_ec4.right;
              *(int *)((int)(local_d60 + iVar12) + 0xc) = local_ec4.bottom;
              local_de4[iVar12] = 1;
            }
          }
        }
        iVar12 = lstrcmpiA(local_ac8,"_no_clip_");
        if (iVar12 == 0) {
          local_ee8 = 1;
          goto LAB_00414354;
        }
        if (local_ac8[0] == '\\') {
          SetRectEmpty((LPRECT)&local_df4);
          iVar12 = FUN_00409af0(local_ac8 + 1,local_d74,&local_e1c);
          if (iVar12 != 0) {
            FUN_004161d0(&local_d64,&local_e20);
            FUN_00416920(&local_df4);
            local_e04 = local_df4.top;
            if (local_f50[10][0x28f].unused != 0) {
              local_e04 = local_e20 - local_df4.bottom;
            }
            local_e00 = local_df4.right - local_df4.left;
            local_e08 = local_df4.left;
            local_dfc = local_df4.bottom - local_df4.top;
            local_df8 = FUN_00416660(local_df4.left,local_df4.top);
            iVar12 = FUN_00416210();
            if (iVar12 == 3) {
              local_df8 = local_df8 | 0xff000000;
            }
            pHVar15 = local_e54 + local_f18;
            if (local_e54[local_f18].unused == 0) {
              this_01 = (kcImage *)
                        FUN_00601980(0x58,
                                     "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                     ,0x27);
              if (this_01 == (kcImage *)0x0) {
                this_01 = (kcImage *)0x0;
              }
              else {
                this_01 = kcImage_ctor(this_01);
              }
              *(kcImage **)&pHVar15->unused = this_01;
            }
            FUN_004175c0(local_e1c,0,&local_e08);
          }
          goto LAB_00414354;
        }
        if (local_ac8[0] == '[') {
          iVar12 = 0;
          if (local_ac8[1] != ']') {
            pcVar8 = local_ac8 + 1;
            hWnd = (HWND)((int)local_f44 - (int)pcVar8);
            do {
              piVar14 = local_f34;
              if (*pcVar8 == '\0') break;
              pcVar8[(int)hWnd] = *pcVar8;
              pcVar8 = pcVar8 + 1;
              iVar12 += 1;
            } while (*pcVar8 != ']');
          }
          *(undefined *)((int)local_848 + iVar12 + (int)piVar14) = 0;
          *(undefined *)((int)local_848 + iVar12 + (int)piVar14 + 1) = 0;
          goto LAB_00414354;
        }
        if (local_f1c == 2) {
          if ((local_f50[10][0x28c].unused != 0) && (local_ac8[0] != '@')) goto LAB_00414354;
          if (((local_edc != 0) && (local_ecc != 0)) &&
             ((FUN_004161d0(&local_e28,&local_e14), local_e28 != local_edc ||
              (local_e14 != local_ecc)))) {
            FUN_00414800(ppcVar11,local_edc,local_ecc);
          }
        }
        ppHVar16 = local_f50;
        if (local_f50[10][0x28d].unused != 0) {
          FUN_00416300();
        }
        if (ppHVar16[10][0x28e].unused != 0) {
          FUN_004163d0(0xffffff);
        }
        if (ppHVar16[10][0x28f].unused != 0) {
          ppcVar11 = (code **)FUN_00411c00(ppcVar11);
          local_f40 = ppcVar11;
        }
        if (ppHVar16[10][0x290].unused != 0) {
          FUN_00416700();
        }
        if (ppHVar16[10][0x291].unused != 0) {
          FUN_004167c0();
        }
        FUN_00416ff0(2);
        iVar12 = local_f3c;
        if (local_de4[local_f3c] != 0) {
          FUN_004168b0(local_d60 + local_f3c);
        }
        if (in_PF) {
          FUN_00416220(&local_ee0,&local_eec);
          fVar3 = (float)local_ee0;
          if (local_ee0 < 0) {
            fVar3 = fVar3 + 4294967296.00000000;
          }
          local_f34 = (int *)((uint)local_f34 & 0xffff0000 | (uint)in_FPUControlWord);
          local_ed8._0_4_ =
               (undefined *)(int)(longlong)ROUND(fVar3 * (float)ppHVar16[10][0x2db].unused);
          iVar6 = (int)(undefined *)local_ed8;
          local_ee0 = (int)(undefined *)local_ed8;
          fVar3 = (float)local_eec;
          if (local_eec < 0) {
            fVar3 = fVar3 + 4294967296.00000000;
          }
          local_ed8 = (ulonglong)ROUND(fVar3 * (float)ppHVar16[10][0x2db].unused);
          local_eec = (int)(undefined *)local_ed8;
          FUN_00417240(iVar6,(undefined *)local_ed8);
        }
        if (local_dc8[iVar12 + 1] == 0) {
          SetRectEmpty((LPRECT)&local_d8c);
          FUN_00416920(&local_d8c);
          LVar2 = local_d8c.top;
          local_e74[iVar12] = local_d8c.left;
          local_e94[iVar12] = LVar2;
        }
        piVar14 = (int *)FUN_00601980(0x14,
                                      "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                      ,0x27);
        if (piVar14 == (int *)0x0) {
          piVar14 = (int *)0x0;
        }
        else {
          piVar14[3] = 1;
          *piVar14 = 0;
          piVar14[1] = 0;
          piVar14[2] = 0;
          piVar14[4] = 0;
        }
        ppcVar18 = local_f30;
        local_f34 = piVar14;
        if (false) goto LAB_00413e69;
        switch(local_f50[10][0x28b].unused) {
        case 1:
          iVar12 = FUN_00601980(0x80,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
LAB_00413afb:
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_0041b0e0();
          }
          goto LAB_00413b01;
        case 3:
          this = (kcImageHg2 *)
                 FUN_00601980(0x1174,
                              "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h",
                              0x27);
          if (this == (kcImageHg2 *)0x0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)kcImageHg2_ctor(this);
          }
          ppcVar18 = local_f30;
          FUN_00416d50(ppcVar11);
          ppHVar16 = local_f50;
          if (local_ee8 == 0) {
            iVar12 = local_f50[10][0x294].unused;
          }
          else {
            iVar12 = 1;
          }
          FUN_0041a0d0((uint)(iVar12 == 0));
          if ((2 < local_f1c) && (local_f1c < 5)) {
            FUN_00416270(&local_e10,&local_e0c);
            (**(code **)(*ppcVar18 + 0xc))
                      (piVar14,local_eb4[local_f38] + local_f2c[0],local_e10,local_e0c,
                       ppHVar16[10][0x292].unused);
            break;
          }
          iVar12 = ppHVar16[10][0x292].unused;
          goto LAB_00413b17;
        case 4:
          this_00 = (kcImageHg3 *)
                    FUN_00601980(0x1178,
                                 "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                 ,0x27);
          if (this_00 == (kcImageHg3 *)0x0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)kcImageHg3_ctor(this_00);
          }
          ppcVar18 = local_f30;
          FUN_00416d50(ppcVar11);
          FUN_00417730(ppcVar11);
          uVar5 = (uint)(local_f50[10][0x2d9].unused != 0);
          if (local_e54[local_f18].unused != 0) {
            FUN_00417730(local_e54[local_f18].unused);
          }
          FUN_004189f0(local_f50[10][0x2da].unused);
          if ((local_f1c < 3) || (4 < local_f1c)) {
            FUN_00416ad0(local_e74[local_f3c],local_e94[local_f3c]);
            if (local_ee8 == 0) {
              iVar12 = local_f50[10][0x294].unused;
            }
            else {
              iVar12 = 1;
            }
            FUN_0041a0d0((uint)(iVar12 == 0));
            hg3_endecode_func_FUN_00419ae0(piVar14,local_eb4[local_f38] + local_f2c[0],0,1,uVar5);
          }
          else {
            FUN_0041a0d0(0);
            hg3_endecode_func_FUN_00419ae0(piVar14,local_eb4[local_f38] + local_f2c[0],0,1,uVar5);
          }
          break;
        case 5:
          iVar12 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_00418060();
          }
          ppcVar18 = local_f30;
          FUN_00416d50(ppcVar11);
          iVar12 = FUN_00416210();
          if (iVar12 == 4) {
            FUN_004163d0(0xff00);
          }
          ppHVar16 = local_f50;
          FUN_00418090(local_f50[10][0x2da].unused);
          (**(code **)(*ppcVar18 + 0xc))
                    (piVar14,local_eb4[local_f38] + local_f2c[0],local_e74[local_f3c],
                     local_e94[local_f3c],ppHVar16[10][0x292].unused);
          break;
        case 6:
          iVar12 = FUN_00601980(0x60,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_00417ae0();
          }
          ppcVar18 = local_f30;
          FUN_00416d50(ppcVar11);
          if (local_ee8 == 0) {
            FUN_00417b00((uint)(local_f50[10][0x294].unused == 0));
          }
          else {
            FUN_00417b00(0);
          }
          goto LAB_00413b09;
        case 7:
          iVar12 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar12 == 0) goto LAB_00413afb;
          local_f30 = (code **)FUN_00417930();
LAB_00413b01:
          ppcVar18 = local_f30;
          FUN_00416d50(ppcVar11);
LAB_00413b09:
          iVar12 = local_f50[10][0x292].unused;
LAB_00413b17:
          (**(code **)(*ppcVar18 + 0xc))
                    (piVar14,local_eb4[local_f38] + local_f2c[0],local_e74[local_f3c],
                     local_e94[local_f3c],iVar12);
        }
LAB_00413e69:
        iVar12 = 0;
        cVar1 = local_ac8[0];
        if ((local_f50[10][0x28c].unused == 0) || (local_f44 = (HWND)0x1, local_f1c != 2)) {
          local_f44 = (HWND)0x0;
        }
        while (((cVar1 != '(' && (local_ac8[iVar12] != ' ')) && (local_ac8[iVar12] != '\0'))) {
          iVar6 = iVar12 + 1;
          iVar12 = iVar12 + 1;
          cVar1 = local_ac8[iVar6];
        }
        if (local_ac8[iVar12] == '(') {
          local_ed8 = local_ed8 & 0xffffffff00000000 | ZEXT48(local_ac8 + iVar12 + 1);
          iVar6 = FUN_00409af0(local_ac8 + iVar12 + 1,local_e2c,local_f2c);
          ppHVar16 = local_f50;
          if (iVar6 == 0) {
            kclib_logError2(&DAT_006134f4);
          }
          else {
            hWnd = local_f50[10];
            *(undefined *)local_ed8 = 0;
            local_ac8[iVar12] = '\0';
            if (true) {
              switch(hWnd[0x28b].unused) {
              case 1:
              case 5:
              case 6:
              case 7:
                pcVar8 = local_ac8 + (int)local_f44;
                _sprintf(local_338,"%s_%04d",pcVar8,local_eb4[local_f38] + local_f2c[0]);
                if (local_f4c == (HWND)0x0) {
                  FUN_0041d16e();
                  iVar12 = 0;
                }
                else {
                  iVar12 = local_f4c->unused;
                }
                if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
                  FUN_0041d16e();
                }
                iVar12 = piVar14[1];
                iVar6 = *piVar14;
                pcVar20 = local_338;
                goto LAB_004142f8;
              case 3:
              case 4:
                local_f24 = (HWND__)((HWND__ *)(local_f50 + 0x10))->unused;
                HVar10 = (HWND__)((HWND__ *)(local_f50 + 0xb))->unused;
                pHVar15 = (HWND__ *)(local_f50 + 0xb);
                while( true ) {
                  local_f24 = *(HWND__ *)local_f24;
                  local_d70[2] = (HWND__)((HWND__ *)(ppHVar16 + 0x10))->unused;
                  if ((HVar10 == (HWND__)0x0) || (HVar10 != (HWND__)pHVar15->unused)) {
                    FUN_0041d16e();
                  }
                  if (local_f24 == local_d70[2]) break;
                  if (HVar10 == (HWND__)0x0) {
                    FUN_0041d16e();
                    HVar9 = (HWND__)0x0;
                  }
                  else {
                    HVar9 = *(HWND__ *)HVar10;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar9 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                  iVar12 = lstrcmpiA(local_ac8 + (int)local_f44,(LPCSTR)((int)local_f24 + 8));
                  if (iVar12 == 0) break;
                  if (HVar10 == (HWND__)0x0) {
                    FUN_0041d16e();
                    HVar9 = (HWND__)0x0;
                  }
                  else {
                    HVar9 = *(HWND__ *)HVar10;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar9 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                }
                local_d78 = (HWND__)((HWND__ *)(ppHVar16 + 0x10))->unused;
                if ((HVar10 == (HWND__)0x0) || (HVar10 != (HWND__)pHVar15->unused)) {
                  FUN_0041d16e();
                }
                if (local_f24 == local_d78) {
                  lstrcpyA(local_bd0,local_ac8 + (int)local_f44);
                  hWnd = ppHVar16[0x10];
                  local_acc = local_f34;
                  iVar12 = FUN_00412270(hWnd,hWnd[1].unused,local_bd0);
                  FUN_00412380(1);
                  hWnd[1].unused = iVar12;
                  **(int **)(iVar12 + 4) = iVar12;
                  ppcVar18 = local_f30;
                }
                else {
                  hWnd = (HWND)FUN_00601980(0x14,
                                            "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                            ,0x27);
                  if (hWnd == (HWND)0x0) {
                    hWnd = (HWND)0x0;
                  }
                  else {
                    hWnd[3].unused = 1;
                    hWnd->unused = 0;
                    hWnd[1].unused = 0;
                    hWnd[2].unused = 0;
                    hWnd[4].unused = 0;
                  }
                  local_f44 = hWnd;
                  if (local_f50[10][0x28b].unused == 3) {
                    if (HVar10 == (HWND__)0x0) {
                      FUN_0041d16e();
                      HVar9 = (HWND__)0x0;
                    }
                    else {
                      HVar9 = *(HWND__ *)HVar10;
                    }
                    if (local_f24 == (HWND__)((HWND__ *)((int)HVar9 + 0x14))->unused) {
                      FUN_0041d16e();
                    }
                    iVar12 = FUN_0041a390(*(undefined4 *)((int)local_f24 + 0x10c),local_f34,hWnd);
                    if (iVar12 != 0) goto LAB_00414206;
                    kclib_logError2(&DAT_006134b4);
joined_r0x004141d2:
                    if (hWnd != (HWND)0x0) {
                      if ((hWnd[4].unused == 0) && (hWnd->unused != 0)) {
                        free_decref(hWnd->unused);
                      }
                      hWnd->unused = 0;
                      hWnd[1].unused = 0;
                      hWnd[2].unused = 0;
                      FUN_006018c0(hWnd);
                    }
                  }
                  else {
                    local_f44 = (HWND)FUN_00601980(0x14,
                                                  "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                                  ,0x27);
                    if (local_f44 == (HWND)0x0) {
                      local_f44 = (HWND)0x0;
                    }
                    else {
                      local_f44[3].unused = 1;
                      local_f44->unused = 0;
                      local_f44[1].unused = 0;
                      local_f44[2].unused = 0;
                      local_f44[4].unused = 0;
                    }
                    hWnd = local_f44;
                    if (HVar10 == (HWND__)0x0) {
                      FUN_0041d16e();
                      HVar9 = (HWND__)0x0;
                    }
                    else {
                      HVar9 = *(HWND__ *)HVar10;
                    }
                    if (local_f24 == (HWND__)((HWND__ *)((int)HVar9 + 0x14))->unused) {
                      FUN_0041d16e();
                    }
                    iVar12 = FUN_00418c30(*(undefined4 *)((int)local_f24 + 0x10c),local_f34,hWnd);
                    if (iVar12 == 0) {
                      kclib_logError2(&DAT_006134b4);
                      goto joined_r0x004141d2;
                    }
LAB_00414206:
                    if (HVar10 == (HWND__)0x0) {
                      FUN_0041d16e();
                      HVar9 = (HWND__)0x0;
                    }
                    else {
                      HVar9 = *(HWND__ *)HVar10;
                    }
                    if (local_f24 == (HWND__)((HWND__ *)((int)HVar9 + 0x14))->unused) {
                      FUN_0041d16e();
                    }
                    piVar14 = *(int **)((int)local_f24 + 0x10c);
                    if (piVar14 != (int *)0x0) {
                      if ((piVar14[4] == 0) && (*piVar14 != 0)) {
                        free_decref(*piVar14);
                      }
                      *piVar14 = 0;
                      piVar14[1] = 0;
                      piVar14[2] = 0;
                      FUN_006018c0(piVar14);
                    }
                    if (HVar10 == (HWND__)0x0) {
                      FUN_0041d16e();
                    }
                    else {
                      HVar10 = *(HWND__ *)HVar10;
                    }
                    if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                      FUN_0041d16e();
                    }
                    *(HWND *)((int)local_f24 + 0x10c) = local_f44;
                  }
                  piVar14 = local_f34;
                  if (local_f34 != (int *)0x0) goto LAB_0041431b;
                }
              }
            }
          }
        }
        else {
          if (*piVar14 == 0) goto switchD_00413f32_caseD_2;
          iVar12 = 0;
          cVar1 = local_ac8[0];
          while (cVar1 != '\0') {
            if (local_ac8[iVar12] == ' ') {
              local_ac8[iVar12] = '\0';
              break;
            }
            cVar1 = local_ac8[iVar12 + 1];
            iVar12 = iVar12 + 1;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar12 = 0;
          }
          else {
            iVar12 = local_f4c->unused;
          }
          if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
            FUN_0041d16e();
          }
          iVar12 = piVar14[1];
          iVar6 = *piVar14;
          pcVar8 = local_ac8 + (int)local_f44;
          pcVar20 = pcVar8;
LAB_004142f8:
          iVar12 = FUN_004119b0((HWND)((int)local_f48 + 8),pcVar20,iVar6,iVar12);
          if (iVar12 == 0) {
            kclib_logError2(&DAT_0061349c,pcVar8);
          }
LAB_0041431b:
          if ((piVar14[4] == 0) && (*piVar14 != 0)) {
            free_decref(*piVar14);
          }
          *piVar14 = 0;
          piVar14[1] = 0;
          piVar14[2] = 0;
          FUN_006018c0(piVar14);
        }
switchD_00413f32_caseD_2:
        ppcVar11 = local_f40;
        if (ppcVar18 != (code **)0x0) {
          (**(code **)*ppcVar18)(1);
          ppcVar11 = local_f40;
        }
LAB_00414354:
        if (ppcVar11 != (code **)0x0) {
          (**(code **)*ppcVar11)(1);
        }
        if (local_f50[9] == (HWND)0x0) {
          SendDlgItemMessageA(*local_f50,1000,0x402,local_f14,0);
        }
LAB_00414386:
        local_f14 += 1;
        iVar12 = local_ee4;
      } while ((int)local_f14 < local_ee4);
    }
    param_1 = local_f50;
    local_ef0 = (HWND__)((HWND__ *)(local_f50 + 0x10))->unused;
    hWnd = local_f50[0xb];
    while( true ) {
      local_ef0 = *(HWND__ *)local_ef0;
      HVar10 = (HWND__)((HWND__ *)(param_1 + 0x10))->unused;
      if ((hWnd == (HWND)0x0) || (hWnd != param_1[0xb])) {
        FUN_0041d16e();
      }
      if (local_ef0 == HVar10) break;
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar12 = 0;
      }
      else {
        iVar12 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
        FUN_0041d16e();
      }
      HVar10 = local_ef0;
      iVar12 = (int)local_ef0 + 8;
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar6 = 0;
      }
      else {
        iVar6 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar6 + 0x14))->unused) {
        FUN_0041d16e();
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar6 = 0;
      }
      else {
        iVar6 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar6 + 0x14))->unused) {
        FUN_0041d16e();
      }
      if (local_f4c == (HWND)0x0) {
        FUN_0041d16e();
        iVar6 = 0;
      }
      else {
        iVar6 = local_f4c->unused;
      }
      if (local_f48 == (HWND__)((HWND__ *)(iVar6 + 0x14))->unused) {
        FUN_0041d16e();
      }
      iVar6 = FUN_004119b0((HWND)((int)local_f48 + 8),iVar12,**(undefined4 **)((int)HVar10 + 0x10c),
                           (*(undefined4 **)((int)HVar10 + 0x10c))[1]);
      if (iVar6 == 0) {
        if (hWnd == (HWND)0x0) {
          FUN_0041d16e();
          iVar6 = 0;
        }
        else {
          iVar6 = hWnd->unused;
        }
        if (local_ef0 == (HWND__)((HWND__ *)(iVar6 + 0x14))->unused) {
          FUN_0041d16e();
        }
        kclib_logError2(&DAT_0061349c,iVar12);
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar12 = 0;
      }
      else {
        iVar12 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
        FUN_0041d16e();
      }
      piVar14 = *(int **)((int)local_ef0 + 0x10c);
      if (piVar14 != (int *)0x0) {
        if ((piVar14[4] == 0) && (*piVar14 != 0)) {
          free_decref(*piVar14);
        }
        *piVar14 = 0;
        piVar14[1] = 0;
        piVar14[2] = 0;
        FUN_006018c0(piVar14);
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar12 = 0;
      }
      else {
        iVar12 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
        FUN_0041d16e();
      }
    }
    HVar10 = (HWND__)((HWND__ *)(param_1 + 0x10))->unused;
    HVar9 = *(HWND__ *)HVar10;
    *(HWND__ *)HVar10 = HVar10;
    ((HWND)(((HWND__ *)(param_1 + 0x10))->unused + 4))->unused =
         ((HWND__ *)(param_1 + 0x10))->unused;
    param_1[0x11] = (HWND)0x0;
    if (HVar9 != (HWND__)((HWND__ *)(param_1 + 0x10))->unused) {
                    /* WARNING: Subroutine does not return */
      FUN_0041cc92(HVar9);
    }
    if (param_1[9] == (HWND)0x0) {
      SendDlgItemMessageA(*param_1,0x3e9,0x402,local_efc + 1,0);
    }
    if (local_f4c == (HWND)0x0) {
      FUN_0041d16e();
      iVar12 = 0;
    }
    else {
      iVar12 = local_f4c->unused;
    }
    HVar10 = local_f48;
    if (local_f48 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
      FUN_0041d16e();
    }
    local_efc += 1;
    local_f48 = *(HWND__ *)HVar10;
  }
  if (hWnd == (HWND)0x0) {
    FUN_0041d16e();
    iVar12 = 0;
  }
  else {
    iVar12 = hWnd->unused;
  }
  if (HVar10 == (HWND__)((HWND__ *)(iVar12 + 0x14))->unused) {
    FUN_0041d16e();
  }
  wsprintfA(local_9c8,(LPCSTR)&param_2_00613528,local_f44);
  iVar12 = MessageBoxA(*param_1,local_9c8,(LPCSTR)&lpCaption_0065d1f4,1);
  if (iVar12 != 2) goto LAB_00412921;
  hWnd = *param_1;
LAB_00414603:
  param_1[9] = (HWND)0x1;
  PostMessageA(hWnd,0x404,0,0);
LAB_00414612:
  iVar12 = 0;
  do {
    if ((code **)local_e54[iVar12].unused != (code **)0x0) {
      (***(code ***)(code **)local_e54[iVar12].unused)(1);
      local_e54[iVar12].unused = 0;
    }
    iVar12 += 1;
  } while (iVar12 < 8);
  if (param_1[9] == (HWND)0x0) {
    PostMessageA(*param_1,0x403,0,0);
  }
  if ((local_f00 == 0) && (local_f10 != 0)) {
    free_decref(local_f10);
  }
  local_f10 = 0;
  local_f0c = 0;
  local_f08 = 0;
  local_c = 0xffffffff;
  FUN_004035c0();
  *in_FS_OFFSET = local_14;
  g_stack_security_FUN_0041cc83();
  return;
}

