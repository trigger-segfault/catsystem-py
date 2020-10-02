
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Could not reconcile some variable overlaps */

void hg_calls_encode_FUN_00412590(HWND *param_1)

{
  char cVar1;
  LONG LVar2;
  float fVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  char *pcVar8;
  BOOL BVar9;
  HWND__ HVar10;
  HWND__ HVar11;
  code **ppcVar12;
  tagRECT *lprc;
  int iVar13;
  int iVar14;
  int *piVar15;
  HWND__ *pHVar16;
  HWND hWnd;
  HWND *ppHVar17;
  undefined4 *_Dst;
  uint uVar18;
  code **ppcVar19;
  int **in_FS_OFFSET;
  bool in_PF;
  bool bVar20;
  ushort in_FPUControlWord;
  char *pcVar21;
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
  tagRECT local_d60 [25];
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
  undefined local_438 [256];
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
  uVar4 = DAT_006883c8 ^ (uint)&stack0xfffff0a0;
  *(int ***)in_FS_OFFSET = &local_14;
  local_f50 = param_1;
  FUN_00403570(uVar4);
  local_c = 0;
  lprc = local_d60;
  local_efc = 7;
  do {
    SetRectEmpty((LPRECT)lprc);
    lprc = (tagRECT *)((LPRECT)lprc + 1);
    local_efc = local_efc + -1;
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
    HVar11 = (HWND__)((HWND__ *)(param_1 + 6))->unused;
    if ((local_f4c == (HWND)0x0) || (local_f4c != param_1[1])) {
      FUN_0041d16e();
    }
    if ((local_f48 == HVar11) || (param_1[9] != (HWND)0x0)) goto LAB_00414612;
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
    iVar13 = 8;
    do {
      _strcpy_s((char *)_Dst,0x40,"");
      hWnd = local_f4c;
      _Dst = _Dst + 0x10;
      iVar13 = iVar13 + -1;
    } while (iVar13 != 0);
    local_ecc = 0;
    local_edc = 0;
    HVar11 = local_f48;
    if (param_1[9] == (HWND)0x0) {
      if (local_f4c == (HWND)0x0) {
        FUN_0041d16e();
        iVar13 = 0;
      }
      else {
        iVar13 = local_f4c->unused;
      }
      HVar11 = local_f48;
      if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
        FUN_0041d16e();
      }
      SetDlgItemTextA(*param_1,0x3ea,(LPCSTR)((int)HVar11 + 8));
    }
    if (hWnd == (HWND)0x0) {
      FUN_0041d16e();
      iVar13 = 0;
    }
    else {
      iVar13 = hWnd->unused;
    }
    if (HVar11 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
      FUN_0041d16e();
    }
    local_f44 = (HWND)((int)HVar11 + 8);
    iVar13 = FUN_004035e0(local_f44,0x101);
    if (iVar13 == 0) break;
LAB_00412921:
    uVar4 = FUN_00403900();
    if ((local_f00 == 0) && (local_f10 != 0)) {
      FUN_0040a530(local_f10);
    }
    local_f10 = 0;
    local_f0c = 0;
    local_f08 = 0;
    local_f00 = 0;
    uVar18 = uVar4;
    if ((uVar4 & 7) != 0) {
      uVar18 = (uVar4 - (uVar4 & 7)) + 8;
    }
    local_f10 = FUN_0040a4e0(0,uVar18);
    local_f00 = 0;
    local_f0c = uVar18;
    local_f08 = uVar18;
    FUN_00403770(local_f10,uVar4);
    FUN_00403730();
    local_f1c = 0;
    if (local_f4c == (HWND)0x0) {
      FUN_0041d16e();
      iVar13 = 0;
    }
    else {
      iVar13 = local_f4c->unused;
    }
    if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
      FUN_0041d16e();
    }
    iVar5 = FUN_00411d40(local_f44,&local_f10,&local_f1c,&local_ee4);
    iVar13 = local_ee4;
    if (iVar5 == 0) {
      hWnd = *param_1;
      goto LAB_00414603;
    }
    if (param_1[9] == (HWND)0x0) {
      SendDlgItemMessageA(*param_1,1000,0x401,0,local_ee4 << 0x10);
    }
    local_f14 = 0;
    if (0 < iVar13) {
      do {
        if (local_f50[9] != (HWND)0x0) break;
        ppcVar12 = local_f40;
        hWnd = local_f4c;
        HVar11 = local_f48;
        switch(local_f1c) {
        case 1:
          iVar13 = FUN_00601980(0x80,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_0041b0e0();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar13 != 0) {
            if (local_f4c == (HWND)0x0) {
              FUN_0041d16e();
              iVar13 = 0;
            }
            else {
              iVar13 = local_f4c->unused;
            }
            HVar11 = local_f48;
            if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
              FUN_0041d16e();
            }
            FUN_00409450((HWND)((int)HVar11 + 8),local_ac8);
            FUN_00409500(local_ac8,0);
            local_f20 = 0;
            break;
          }
          (**(code **)*ppcVar12)(1);
          goto LAB_00414386;
        case 2:
          iVar5 = FUN_00601980(0x5c,
                               "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                               ,0x27);
          if (iVar5 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_0041b460();
          }
          ppcVar12 = local_f40;
          ppHVar17 = local_f50;
          if (local_f50[10][0x293].unused != 0) {
            iVar13 = (**(code **)(*local_f40 + 4))(&local_f10,0);
            hWnd = local_f4c;
            if (iVar13 == 0) {
              (**(code **)*ppcVar12)(1);
              goto LAB_00414386;
            }
            if (local_f4c == (HWND)0x0) {
              FUN_0041d16e();
              iVar13 = 0;
            }
            else {
              iVar13 = local_f4c->unused;
            }
            HVar11 = local_f48;
            if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
              FUN_0041d16e();
            }
            FUN_00409450((HWND)((int)HVar11 + 8),local_ac8);
            FUN_00409500(local_ac8,0);
            local_f20 = 0;
            break;
          }
          iVar13 = FUN_0041c3c0(&local_f10,(iVar13 - local_f14) + -1,local_ac8,&local_f20);
          if (iVar13 != 0) {
            hWnd = local_f4c;
            HVar11 = local_f48;
            if ((local_f20 == 0) && (ppHVar17[10][0x295].unused != 0)) {
              uVar6 = FUN_00412220(local_ac8);
              FUN_00409450(uVar6);
              FUN_00409500(local_ac8,0);
              local_f1c = 3;
              hWnd = local_f4c;
              HVar11 = local_f48;
            }
            break;
          }
          goto LAB_00412ac0;
        case 3: // DSTTYPE=3 "HG-2"
          iVar13 = FUN_00601980(0x1174,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)hg_encode_norm_tables_b();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 8))(&local_f10,local_f14,&local_e30);
          hWnd = local_f4c;
          if (iVar13 == 0) {
            (**(code **)*ppcVar12)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          HVar11 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar11 + 8),auStack1344);
          FUN_00409500(auStack1344,0);
          wsprintfA(local_ac8,"%s(%d)",auStack1344,local_e30);
          local_f20 = 0;
          break;
        case 4: // DSTTYPE=4 "HG-3"
          iVar13 = FUN_00601980(0x1178,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)hg_encode_norm_tables_a();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 8))(&local_f10,local_f14,&local_e18);
          hWnd = local_f4c;
          if (iVar13 == 0) {
            (**(code **)*ppcVar12)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          HVar11 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar11 + 8),auStack1608);
          FUN_00409500(auStack1608,0);
          wsprintfA(local_ac8,"%s(%d)",auStack1608,local_e18);
          local_f44 = (HWND)(local_e54 + local_f18);
          if (local_f44->unused == 0) {
            iVar13 = FUN_00601980(0x58,
                                  "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                  ,0x27);
            if (iVar13 == 0) {
              iVar13 = 0;
            }
            else {
              iVar13 = FUN_00416140();
            }
            local_f44->unused = iVar13;
          }
          FUN_00417730(ppcVar12);
          local_f20 = 0;
          break;
        case 5:
          iVar13 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00418060();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar13 == 0) {
            (**(code **)*ppcVar12)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          HVar11 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar11 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
          break;
        case 6:
          iVar13 = FUN_00601980(0x60,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00417ae0();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar13 == 0) {
            (**(code **)*ppcVar12)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          HVar11 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar11 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
          break;
        case 7:
          iVar13 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f40 = (code **)0x0;
          }
          else {
            local_f40 = (code **)FUN_00417930();
          }
          ppcVar12 = local_f40;
          iVar13 = (**(code **)(*local_f40 + 4))(&local_f10,0);
          hWnd = local_f4c;
          if (iVar13 == 0) {
            (**(code **)*ppcVar12)(1);
            goto LAB_00414386;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          HVar11 = local_f48;
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_00409450((HWND)((int)HVar11 + 8),local_ac8);
          FUN_00409500(local_ac8,0);
          local_f20 = 0;
        }
        iVar13 = FUN_00411ed0(local_ac8,local_438);
        if (iVar13 != 0) {
          if (hWnd == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = hWnd->unused;
          }
          if (HVar11 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          FUN_004093d0((HWND)((int)HVar11 + 8),local_230);
          SetCurrentDirectoryA(local_230);
          iVar13 = FUN_004035e0(local_438,0x101);
          if (iVar13 == 0) {
            wsprintfA(local_9c8,(LPCSTR)&param_2_00613528,local_438);
            ppHVar17 = local_f50;
            iVar13 = MessageBoxA(*local_f50,local_9c8,(LPCSTR)&lpCaption_0065d1f4,1);
            if (iVar13 != 2) goto LAB_004130c3;
            hWnd = *ppHVar17;
          }
          else {
LAB_004130c3:
            uVar4 = FUN_00403900();
            if ((local_f00 == 0) && (local_f10 != 0)) {
              FUN_0040a530(local_f10);
            }
            local_f10 = 0;
            local_f0c = 0;
            local_f08 = 0;
            local_f00 = 0;
            uVar18 = uVar4;
            if ((uVar4 & 7) != 0) {
              uVar18 = (uVar4 - (uVar4 & 7)) + 8;
            }
            local_f10 = FUN_0040a4e0(0,uVar18);
            local_f00 = 0;
            local_f0c = uVar18;
            local_f08 = uVar18;
            FUN_00403770(local_f10,uVar4);
            FUN_00403730();
            ppHVar17 = local_f50;
            iVar13 = FUN_00411d40(local_128,&local_f10,&local_f1c,&local_ee4);
            if (iVar13 != 0) {
              if (ppHVar17[9] == (HWND)0x0) {
                SendDlgItemMessageA(*ppHVar17,1000,0x401,0,local_ee4 << 0x10);
              }
              FUN_004161d0(&local_edc,&local_ecc);
              local_f14 = 0xffffffff;
LAB_00412ac0:
              if (ppcVar12 != (code **)0x0) {
                (**(code **)*ppcVar12)(1);
              }
              goto LAB_00414386;
            }
            hWnd = *ppHVar17;
          }
          ppHVar17[9] = (HWND)0x1;
          PostMessageA(hWnd,0x404,0,0);
          break;
        }
        FUN_00409fa0(local_948,local_ac8);
        _strcpy_s(local_948,0x100,local_ac8);
        piVar15 = (int *)(local_ef8 * 0x40);
        hWnd = (HWND)(local_848 + local_ef8 * 0x10);
        local_f44 = hWnd;
        local_f34 = piVar15;
        FUN_00409e90(local_ac8,local_948,&DAT_0061351c,hWnd);
        if ((local_ac8[0] == '@') && (local_ac8[1] == '(')) {
          _strcpy_s(local_948,0x100,local_ac8);
          wsprintfA(local_ac8,"@%s(%s",hWnd,local_946);
        }
        ppHVar17 = local_f50;
        if (local_f50[9] == (HWND)0x0) {
          SetDlgItemTextA(*local_f50,0x3eb,local_ac8);
        }
        if (local_f20 == 1) {
          if (local_f3c < 7) {
            LVar2 = local_e94[local_f3c];
            iVar13 = local_f3c + 1;
            local_e74[iVar13] = local_e74[local_f3c];
            local_e94[iVar13] = LVar2;
            local_dc8[local_f3c + 2] = local_dc8[iVar13];
            local_f38 = local_f38 + 1;
            local_ef8 = local_ef8 + 1;
            iVar14 = local_ef8 * 0x40;
            iVar5 = local_ef8 * 0x10;
            local_eb4[local_f38] = (&local_ec4.bottom)[local_f38];
            local_f3c = iVar13;
            _strcpy_s((char *)(local_848 + iVar5),0x40,acStack2184 + iVar14);
            iVar13 = local_e54[local_f18].unused;
            local_f18 = local_f18 + 1;
            if (iVar13 != 0) {
              iVar7 = FUN_00601980(0x58,
                                   "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                   ,0x27);
              if (iVar7 == 0) {
                iVar7 = 0;
              }
              else {
                iVar7 = FUN_00416140();
              }
              local_e54[local_f18].unused = iVar7;
              FUN_00417730(iVar13);
            }
            local_de4[local_f3c] = (&local_df4.bottom)[local_f3c];
            bVar20 = local_ac8[0] == '[';
            *(int *)(local_d60 + local_f3c) = local_d70[local_f3c * 4].unused;
            *(int *)((int)(local_d60 + local_f3c) + 4) = local_d70[local_f3c * 4 + 1].unused;
            *(int *)((int)(local_d60 + local_f3c) + 8) = local_d70[local_f3c * 4 + 2].unused;
            *(LONG *)((int)(local_d60 + local_f3c) + 0xc) = (&local_d64)[local_f3c * 4];
            if (bVar20) {
              iVar13 = 0;
              if (local_ac8[1] != ']') {
                pcVar8 = local_ac8 + 1;
                _Dst = (undefined4 *)((int)(local_848 + iVar5) - (int)pcVar8);
                do {
                  if (*pcVar8 == '\0') break;
                  *(char *)((int)_Dst + (int)pcVar8) = *pcVar8;
                  pcVar8 = pcVar8 + 1;
                  iVar13 = iVar13 + 1;
                } while (*pcVar8 != ']');
              }
              iVar14 = iVar14 + iVar13;
              *(undefined *)((int)local_848 + iVar14) = 0;
              *(undefined *)((int)local_848 + iVar14 + 1) = 0;
            }
          }
          iVar13 = 0;
          cVar1 = local_ac8[0];
          while ((cVar1 != '(' && (local_ac8[iVar13] != '\0'))) {
            cVar1 = local_ac8[iVar13 + 1];
            iVar13 = iVar13 + 1;
          }
          if ((local_ac8[iVar13] == '(') &&
             (iVar13 = FUN_00409af0(local_ac8 + iVar13 + 1,local_d94,&local_e34), iVar13 != 0)) {
            local_eb4[local_f38] = local_eb4[local_f38] + local_e34;
          }
          if (local_f40 != (code **)0x0) {
            (**(code **)*local_f40)(1);
          }
          goto LAB_00414386;
        }
        if (local_f20 == 2) {
          if (0 < local_f3c) {
            local_f3c = local_f3c + -1;
          }
          if (0 < local_f38) {
            local_f38 = local_f38 + -1;
          }
          if (0 < local_ef8) {
            local_ef8 = local_ef8 + -1;
          }
          if (ppcVar12 != (code **)0x0) {
            (**(code **)*ppcVar12)(1);
          }
          iVar13 = local_f18;
          if ((code **)local_e54[local_f18].unused != (code **)0x0) {
            (***(code ***)(code **)local_e54[local_f18].unused)(1);
            local_e54[iVar13].unused = 0;
          }
          if (0 < iVar13) {
            local_f18 = iVar13 + -1;
          }
          goto LAB_00414386;
        }
        local_f2c[0] = 0;
        iVar13 = 0;
        cVar1 = local_ac8[0];
        while (((cVar1 != '(' && (local_ac8[iVar13] != ' ')) && (local_ac8[iVar13] != '\0'))) {
          cVar1 = local_ac8[iVar13 + 1];
          iVar13 = iVar13 + 1;
        }
        if ((local_ac8[iVar13] == '(') &&
           (iVar13 = FUN_00409af0(local_ac8 + iVar13 + 1,local_e2c,local_f2c), iVar13 == 0)) {
          FUN_00403450(&DAT_006134f4);
        }
        iVar13 = lstrcmpiA(local_ac8,"_base_");
        if (iVar13 == 0) {
          SetRectEmpty((LPRECT)&local_da4);
          FUN_004161d0(local_d70,&local_e24);
          FUN_00416920(&local_da4);
          iVar13 = local_da4.top;
          if (ppHVar17[10][0x28f].unused != 0) {
            iVar13 = local_e24 - local_da4.bottom;
          }
          local_e74[local_f3c] = local_da4.left;
          local_e94[local_f3c] = iVar13;
          local_dc8[local_f3c + 1] = 1;
        }
        iVar13 = local_f3c;
        iVar5 = lstrcmpiA(local_ac8,"_clip_");
        if (iVar5 == 0) {
          SetRectEmpty((LPRECT)&local_ec4);
          iVar5 = FUN_00416920(&local_ec4);
          if (iVar5 != 0) {
            FUN_004161d0(local_d90,&local_ec8);
            if (local_f50[10][0x28f].unused != 0) {
              local_ec4.bottom = local_ec8 - local_ec4.bottom;
              local_ec8 = local_ec8 - local_ec4.top;
              local_ec4.top = local_ec4.bottom;
              local_ec4.bottom = local_ec8;
            }
            BVar9 = IsRectEmpty(&local_ec4);
            if (BVar9 == 0) {
              *(LONG *)(local_d60 + iVar13) = local_ec4.left;
              *(int *)((int)(local_d60 + iVar13) + 4) = local_ec4.top;
              *(LONG *)((int)(local_d60 + iVar13) + 8) = local_ec4.right;
              *(int *)((int)(local_d60 + iVar13) + 0xc) = local_ec4.bottom;
              local_de4[iVar13] = 1;
            }
          }
        }
        iVar13 = lstrcmpiA(local_ac8,"_no_clip_");
        if (iVar13 == 0) {
          local_ee8 = 1;
          goto LAB_00414354;
        }
        if (local_ac8[0] == '\\') {
          SetRectEmpty((LPRECT)&local_df4);
          iVar13 = FUN_00409af0(local_ac8 + 1,local_d74,&local_e1c);
          if (iVar13 != 0) {
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
            iVar13 = FUN_00416210();
            if (iVar13 == 3) {
              local_df8 = local_df8 | 0xff000000;
            }
            pHVar16 = local_e54 + local_f18;
            if (local_e54[local_f18].unused == 0) {
              iVar13 = FUN_00601980(0x58,
                                    "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                    ,0x27);
              if (iVar13 == 0) {
                iVar13 = 0;
              }
              else {
                iVar13 = FUN_00416140();
              }
              pHVar16->unused = iVar13;
            }
            FUN_004175c0(local_e1c,0,&local_e08);
          }
          goto LAB_00414354;
        }
        if (local_ac8[0] == '[') {
          iVar13 = 0;
          if (local_ac8[1] != ']') {
            pcVar8 = local_ac8 + 1;
            hWnd = (HWND)((int)local_f44 - (int)pcVar8);
            do {
              piVar15 = local_f34;
              if (*pcVar8 == '\0') break;
              pcVar8[(int)hWnd] = *pcVar8;
              pcVar8 = pcVar8 + 1;
              iVar13 = iVar13 + 1;
            } while (*pcVar8 != ']');
          }
          *(undefined *)((int)local_848 + iVar13 + (int)piVar15) = 0;
          *(undefined *)((int)local_848 + iVar13 + (int)piVar15 + 1) = 0;
          goto LAB_00414354;
        }
        if (local_f1c == 2) {
          if ((local_f50[10][0x28c].unused != 0) && (local_ac8[0] != '@')) goto LAB_00414354;
          if (((local_edc != 0) && (local_ecc != 0)) &&
             ((FUN_004161d0(&local_e28,&local_e14), local_e28 != local_edc ||
              (local_e14 != local_ecc)))) {
            FUN_00414800(ppcVar12,local_edc,local_ecc);
          }
        }
        ppHVar17 = local_f50;
        if (local_f50[10][0x28d].unused != 0) {
          FUN_00416300();
        }
        if (ppHVar17[10][0x28e].unused != 0) {
          FUN_004163d0(0xffffff);
        }
        if (ppHVar17[10][0x28f].unused != 0) {
          ppcVar12 = (code **)FUN_00411c00(ppcVar12);
          local_f40 = ppcVar12;
        }
        if (ppHVar17[10][0x290].unused != 0) {
          FUN_00416700();
        }
        if (ppHVar17[10][0x291].unused != 0) {
          FUN_004167c0();
        }
        FUN_00416ff0(2);
        iVar13 = local_f3c;
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
               (undefined *)(int)(longlong)ROUND(fVar3 * (float)ppHVar17[10][0x2db].unused);
          iVar5 = (int)(undefined *)local_ed8;
          local_ee0 = (int)(undefined *)local_ed8;
          fVar3 = (float)local_eec;
          if (local_eec < 0) {
            fVar3 = fVar3 + 4294967296.00000000;
          }
          local_ed8 = (ulonglong)ROUND(fVar3 * (float)ppHVar17[10][0x2db].unused);
          local_eec = (int)(undefined *)local_ed8;
          FUN_00417240(iVar5,(undefined *)local_ed8);
        }
        if (local_dc8[iVar13 + 1] == 0) {
          SetRectEmpty((LPRECT)&local_d8c);
          FUN_00416920(&local_d8c);
          LVar2 = local_d8c.top;
          local_e74[iVar13] = local_d8c.left;
          local_e94[iVar13] = LVar2;
        }
        piVar15 = (int *)FUN_00601980(0x14,
                                      "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                      ,0x27);
        if (piVar15 == (int *)0x0) {
          piVar15 = (int *)0x0;
        }
        else {
          piVar15[3] = 1;
          *piVar15 = 0;
          piVar15[1] = 0;
          piVar15[2] = 0;
          piVar15[4] = 0;
        }
        ppcVar19 = local_f30;
        local_f34 = piVar15;
        switch(local_f50[10][0x28b].unused) {
        case 1:
          iVar13 = FUN_00601980(0x80,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
LAB_00413afb:
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_0041b0e0();
          }
          goto LAB_00413b01;
        case 3:
          iVar13 = FUN_00601980(0x1174,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)hg_encode_norm_tables_b();
          }
          ppcVar19 = local_f30;
          FUN_00416d50(ppcVar12);
          ppHVar17 = local_f50;
          if (local_ee8 == 0) {
            iVar13 = local_f50[10][0x294].unused;
          }
          else {
            iVar13 = 1;
          }
          FUN_0041a0d0((uint)(iVar13 == 0));
          if ((2 < local_f1c) && (local_f1c < 5)) {
            FUN_00416270(&local_e10,&local_e0c);
            (**(code **)(*ppcVar19 + 0xc))
                      (piVar15,local_eb4[local_f38] + local_f2c[0],local_e10,local_e0c,
                       ppHVar17[10][0x292].unused);
            break;
          }
          iVar13 = ppHVar17[10][0x292].unused;
          goto LAB_00413b17;
        case 4:
          iVar13 = FUN_00601980(0x1178,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)hg_encode_norm_tables_a();
          }
          ppcVar19 = local_f30;
          FUN_00416d50(ppcVar12);
          FUN_00417730(ppcVar12);
          uVar4 = (uint)(local_f50[10][0x2d9].unused != 0);
          if (local_e54[local_f18].unused != 0) {
            FUN_00417730(local_e54[local_f18].unused);
          }
          FUN_004189f0(local_f50[10][0x2da].unused);
          if ((local_f1c < 3) || (4 < local_f1c)) {
            FUN_00416ad0(local_e74[local_f3c],local_e94[local_f3c]);
            if (local_ee8 == 0) {
              iVar13 = local_f50[10][0x294].unused;
            }
            else {
              iVar13 = 1;
            }
            FUN_0041a0d0((uint)(iVar13 == 0));
            FUN_00419ae0(piVar15,local_eb4[local_f38] + local_f2c[0],0,1,uVar4);
          }
          else {
            FUN_0041a0d0(0);
            FUN_00419ae0(piVar15,local_eb4[local_f38] + local_f2c[0],0,1,uVar4);
          }
          break;
        case 5:
          iVar13 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_00418060();
          }
          ppcVar19 = local_f30;
          FUN_00416d50(ppcVar12);
          iVar13 = FUN_00416210();
          if (iVar13 == 4) {
            FUN_004163d0(0xff00);
          }
          ppHVar17 = local_f50;
          FUN_00418090(local_f50[10][0x2da].unused);
          (**(code **)(*ppcVar19 + 0xc))
                    (piVar15,local_eb4[local_f38] + local_f2c[0],local_e74[local_f3c],
                     local_e94[local_f3c],ppHVar17[10][0x292].unused);
          break;
        case 6:
          iVar13 = FUN_00601980(0x60,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) {
            local_f30 = (code **)0x0;
          }
          else {
            local_f30 = (code **)FUN_00417ae0();
          }
          ppcVar19 = local_f30;
          FUN_00416d50(ppcVar12);
          if (local_ee8 == 0) {
            FUN_00417b00((uint)(local_f50[10][0x294].unused == 0));
          }
          else {
            FUN_00417b00(0);
          }
          goto LAB_00413b09;
        case 7:
          iVar13 = FUN_00601980(0x5c,
                                "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
                                ,0x27);
          if (iVar13 == 0) goto LAB_00413afb;
          local_f30 = (code **)FUN_00417930();
LAB_00413b01:
          ppcVar19 = local_f30;
          FUN_00416d50(ppcVar12);
LAB_00413b09:
          iVar13 = local_f50[10][0x292].unused;
LAB_00413b17:
          (**(code **)(*ppcVar19 + 0xc))
                    (piVar15,local_eb4[local_f38] + local_f2c[0],local_e74[local_f3c],
                     local_e94[local_f3c],iVar13);
        }
        iVar13 = 0;
        cVar1 = local_ac8[0];
        if ((local_f50[10][0x28c].unused == 0) || (local_f44 = (HWND)0x1, local_f1c != 2)) {
          local_f44 = (HWND)0x0;
        }
        while (((cVar1 != '(' && (local_ac8[iVar13] != ' ')) && (local_ac8[iVar13] != '\0'))) {
          iVar5 = iVar13 + 1;
          iVar13 = iVar13 + 1;
          cVar1 = local_ac8[iVar5];
        }
        if (local_ac8[iVar13] == '(') {
          local_ed8 = local_ed8 & 0xffffffff00000000 | ZEXT48(local_ac8 + iVar13 + 1);
          iVar5 = FUN_00409af0(local_ac8 + iVar13 + 1,local_e2c,local_f2c);
          ppHVar17 = local_f50;
          if (iVar5 == 0) {
            FUN_00403450(&DAT_006134f4);
          }
          else {
            hWnd = local_f50[10];
            *(undefined *)local_ed8 = 0;
            local_ac8[iVar13] = '\0';
            switch(hWnd[0x28b].unused) {
            case 1:
            case 5:
            case 6:
            case 7:
              pcVar8 = local_ac8 + (int)local_f44;
              _sprintf(local_338,"%s_%04d",pcVar8,local_eb4[local_f38] + local_f2c[0]);
              if (local_f4c == (HWND)0x0) {
                FUN_0041d16e();
                iVar13 = 0;
              }
              else {
                iVar13 = local_f4c->unused;
              }
              if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
                FUN_0041d16e();
              }
              iVar13 = piVar15[1];
              iVar5 = *piVar15;
              pcVar21 = local_338;
              goto LAB_004142f8;
            case 3:
            case 4:
              local_f24 = (HWND__)((HWND__ *)(local_f50 + 0x10))->unused;
              HVar11 = (HWND__)((HWND__ *)(local_f50 + 0xb))->unused;
              pHVar16 = (HWND__ *)(local_f50 + 0xb);
              while( true ) {
                local_f24 = *(HWND__ *)local_f24;
                local_d70[2] = (HWND__)((HWND__ *)(ppHVar17 + 0x10))->unused;
                if ((HVar11 == (HWND__)0x0) || (HVar11 != (HWND__)pHVar16->unused)) {
                  FUN_0041d16e();
                }
                if (local_f24 == local_d70[2]) break;
                if (HVar11 == (HWND__)0x0) {
                  FUN_0041d16e();
                  HVar10 = (HWND__)0x0;
                }
                else {
                  HVar10 = *(HWND__ *)HVar11;
                }
                if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                  FUN_0041d16e();
                }
                iVar13 = lstrcmpiA(local_ac8 + (int)local_f44,(LPCSTR)((int)local_f24 + 8));
                if (iVar13 == 0) break;
                if (HVar11 == (HWND__)0x0) {
                  FUN_0041d16e();
                  HVar10 = (HWND__)0x0;
                }
                else {
                  HVar10 = *(HWND__ *)HVar11;
                }
                if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                  FUN_0041d16e();
                }
              }
              local_d78 = (HWND__)((HWND__ *)(ppHVar17 + 0x10))->unused;
              if ((HVar11 == (HWND__)0x0) || (HVar11 != (HWND__)pHVar16->unused)) {
                FUN_0041d16e();
              }
              if (local_f24 == local_d78) {
                lstrcpyA(local_bd0,local_ac8 + (int)local_f44);
                hWnd = ppHVar17[0x10];
                local_acc = local_f34;
                iVar13 = FUN_00412270(hWnd,hWnd[1].unused,local_bd0);
                FUN_00412380(1);
                hWnd[1].unused = iVar13;
                **(int **)(iVar13 + 4) = iVar13;
                ppcVar19 = local_f30;
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
                  if (HVar11 == (HWND__)0x0) {
                    FUN_0041d16e();
                    HVar10 = (HWND__)0x0;
                  }
                  else {
                    HVar10 = *(HWND__ *)HVar11;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                  iVar13 = FUN_0041a390(*(undefined4 *)((int)local_f24 + 0x10c),local_f34,hWnd);
                  if (iVar13 != 0) goto LAB_00414206;
                  FUN_00403450(&DAT_006134b4);
joined_r0x004141d2:
                  if (hWnd != (HWND)0x0) {
                    if ((hWnd[4].unused == 0) && (hWnd->unused != 0)) {
                      FUN_0040a530(hWnd->unused);
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
                  if (HVar11 == (HWND__)0x0) {
                    FUN_0041d16e();
                    HVar10 = (HWND__)0x0;
                  }
                  else {
                    HVar10 = *(HWND__ *)HVar11;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                  iVar13 = FUN_00418c30(*(undefined4 *)((int)local_f24 + 0x10c),local_f34,hWnd);
                  if (iVar13 == 0) {
                    FUN_00403450(&DAT_006134b4);
                    goto joined_r0x004141d2;
                  }
LAB_00414206:
                  if (HVar11 == (HWND__)0x0) {
                    FUN_0041d16e();
                    HVar10 = (HWND__)0x0;
                  }
                  else {
                    HVar10 = *(HWND__ *)HVar11;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar10 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                  piVar15 = *(int **)((int)local_f24 + 0x10c);
                  if (piVar15 != (int *)0x0) {
                    if ((piVar15[4] == 0) && (*piVar15 != 0)) {
                      FUN_0040a530(*piVar15);
                    }
                    *piVar15 = 0;
                    piVar15[1] = 0;
                    piVar15[2] = 0;
                    FUN_006018c0(piVar15);
                  }
                  if (HVar11 == (HWND__)0x0) {
                    FUN_0041d16e();
                  }
                  else {
                    HVar11 = *(HWND__ *)HVar11;
                  }
                  if (local_f24 == (HWND__)((HWND__ *)((int)HVar11 + 0x14))->unused) {
                    FUN_0041d16e();
                  }
                  *(HWND *)((int)local_f24 + 0x10c) = local_f44;
                }
                piVar15 = local_f34;
                if (local_f34 != (int *)0x0) goto LAB_0041431b;
              }
            }
          }
        }
        else {
          if (*piVar15 == 0) goto switchD_00413f32_caseD_2;
          iVar13 = 0;
          cVar1 = local_ac8[0];
          while (cVar1 != '\0') {
            if (local_ac8[iVar13] == ' ') {
              local_ac8[iVar13] = '\0';
              break;
            }
            cVar1 = local_ac8[iVar13 + 1];
            iVar13 = iVar13 + 1;
          }
          if (local_f4c == (HWND)0x0) {
            FUN_0041d16e();
            iVar13 = 0;
          }
          else {
            iVar13 = local_f4c->unused;
          }
          if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
            FUN_0041d16e();
          }
          iVar13 = piVar15[1];
          iVar5 = *piVar15;
          pcVar8 = local_ac8 + (int)local_f44;
          pcVar21 = pcVar8;
LAB_004142f8:
          iVar13 = FUN_004119b0((HWND)((int)local_f48 + 8),pcVar21,iVar5,iVar13);
          if (iVar13 == 0) {
            FUN_00403450(&DAT_0061349c,pcVar8);
          }
LAB_0041431b:
          if ((piVar15[4] == 0) && (*piVar15 != 0)) {
            FUN_0040a530(*piVar15);
          }
          *piVar15 = 0;
          piVar15[1] = 0;
          piVar15[2] = 0;
          FUN_006018c0(piVar15);
        }
switchD_00413f32_caseD_2:
        ppcVar12 = local_f40;
        if (ppcVar19 != (code **)0x0) {
          (**(code **)*ppcVar19)(1);
          ppcVar12 = local_f40;
        }
LAB_00414354:
        if (ppcVar12 != (code **)0x0) {
          (**(code **)*ppcVar12)(1);
        }
        if (local_f50[9] == (HWND)0x0) {
          SendDlgItemMessageA(*local_f50,1000,0x402,local_f14,0);
        }
LAB_00414386:
        local_f14 = local_f14 + 1;
        iVar13 = local_ee4;
      } while ((int)local_f14 < local_ee4);
    }
    param_1 = local_f50;
    local_ef0 = (HWND__)((HWND__ *)(local_f50 + 0x10))->unused;
    hWnd = local_f50[0xb];
    while( true ) {
      local_ef0 = *(HWND__ *)local_ef0;
      HVar11 = (HWND__)((HWND__ *)(param_1 + 0x10))->unused;
      if ((hWnd == (HWND)0x0) || (hWnd != param_1[0xb])) {
        FUN_0041d16e();
      }
      if (local_ef0 == HVar11) break;
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar13 = 0;
      }
      else {
        iVar13 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
        FUN_0041d16e();
      }
      HVar11 = local_ef0;
      iVar13 = (int)local_ef0 + 8;
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar5 = 0;
      }
      else {
        iVar5 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar5 + 0x14))->unused) {
        FUN_0041d16e();
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar5 = 0;
      }
      else {
        iVar5 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar5 + 0x14))->unused) {
        FUN_0041d16e();
      }
      if (local_f4c == (HWND)0x0) {
        FUN_0041d16e();
        iVar5 = 0;
      }
      else {
        iVar5 = local_f4c->unused;
      }
      if (local_f48 == (HWND__)((HWND__ *)(iVar5 + 0x14))->unused) {
        FUN_0041d16e();
      }
      iVar5 = FUN_004119b0((HWND)((int)local_f48 + 8),iVar13,**(undefined4 **)((int)HVar11 + 0x10c),
                           (*(undefined4 **)((int)HVar11 + 0x10c))[1]);
      if (iVar5 == 0) {
        if (hWnd == (HWND)0x0) {
          FUN_0041d16e();
          iVar5 = 0;
        }
        else {
          iVar5 = hWnd->unused;
        }
        if (local_ef0 == (HWND__)((HWND__ *)(iVar5 + 0x14))->unused) {
          FUN_0041d16e();
        }
        FUN_00403450(&DAT_0061349c,iVar13);
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar13 = 0;
      }
      else {
        iVar13 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
        FUN_0041d16e();
      }
      piVar15 = *(int **)((int)local_ef0 + 0x10c);
      if (piVar15 != (int *)0x0) {
        if ((piVar15[4] == 0) && (*piVar15 != 0)) {
          FUN_0040a530(*piVar15);
        }
        *piVar15 = 0;
        piVar15[1] = 0;
        piVar15[2] = 0;
        FUN_006018c0(piVar15);
      }
      if (hWnd == (HWND)0x0) {
        FUN_0041d16e();
        iVar13 = 0;
      }
      else {
        iVar13 = hWnd->unused;
      }
      if (local_ef0 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
        FUN_0041d16e();
      }
    }
    HVar11 = (HWND__)((HWND__ *)(param_1 + 0x10))->unused;
    HVar10 = *(HWND__ *)HVar11;
    *(HWND__ *)HVar11 = HVar11;
    ((HWND)(((HWND__ *)(param_1 + 0x10))->unused + 4))->unused =
         ((HWND__ *)(param_1 + 0x10))->unused;
    param_1[0x11] = (HWND)0x0;
    if (HVar10 != (HWND__)((HWND__ *)(param_1 + 0x10))->unused) {
                    /* WARNING: Subroutine does not return */
      FUN_0041cc92(HVar10);
    }
    if (param_1[9] == (HWND)0x0) {
      SendDlgItemMessageA(*param_1,0x3e9,0x402,local_efc + 1,0);
    }
    if (local_f4c == (HWND)0x0) {
      FUN_0041d16e();
      iVar13 = 0;
    }
    else {
      iVar13 = local_f4c->unused;
    }
    HVar11 = local_f48;
    if (local_f48 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
      FUN_0041d16e();
    }
    local_efc = local_efc + 1;
    local_f48 = *(HWND__ *)HVar11;
  }
  if (hWnd == (HWND)0x0) {
    FUN_0041d16e();
    iVar13 = 0;
  }
  else {
    iVar13 = hWnd->unused;
  }
  if (HVar11 == (HWND__)((HWND__ *)(iVar13 + 0x14))->unused) {
    FUN_0041d16e();
  }
  wsprintfA(local_9c8,(LPCSTR)&param_2_00613528,local_f44);
  iVar13 = MessageBoxA(*param_1,local_9c8,(LPCSTR)&lpCaption_0065d1f4,1);
  if (iVar13 != 2) goto LAB_00412921;
  hWnd = *param_1;
LAB_00414603:
  param_1[9] = (HWND)0x1;
  PostMessageA(hWnd,0x404,0,0);
LAB_00414612:
  iVar13 = 0;
  do {
    if ((code **)local_e54[iVar13].unused != (code **)0x0) {
      (***(code ***)(code **)local_e54[iVar13].unused)(1);
      local_e54[iVar13].unused = 0;
    }
    iVar13 = iVar13 + 1;
  } while (iVar13 < 8);
  if (param_1[9] == (HWND)0x0) {
    PostMessageA(*param_1,0x403,0,0);
  }
  if ((local_f00 == 0) && (local_f10 != 0)) {
    FUN_0040a530(local_f10);
  }
  local_f10 = 0;
  local_f0c = 0;
  local_f08 = 0;
  local_c = 0xffffffff;
  FUN_004035c0();
  *in_FS_OFFSET = local_14;
  FUN_0041cc83();
  return;
}

