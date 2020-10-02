
void __thiscall
cs2_kcKCScript_lookuptable_005b1020(int param_1,int param_1_00,undefined4 param_2,int param_3)

{
  char cVar1;
  float fVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int *piVar7;
  float *pfVar8;
  char *pcVar9;
  undefined4 *puVar10;
  int **in_FS_OFFSET;
  float10 fVar11;
  float local_36c;
  undefined auStack852 [4];
  int local_350;
  int local_34c;
  float local_348;
  float local_344;
  float local_340;
  float local_33c;
  float local_338;
  int *local_334;
  int local_330;
  int local_32c;
  undefined4 local_328;
  int local_324;
  int local_320;
  float local_31c [33];
  undefined local_298 [4];
  uint local_294;
  undefined4 local_284;
  uint local_280;
  undefined local_27c [488];
  char local_94 [64];
  char local_54 [64];
  uint local_14;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_005d4f85;
  local_c = *in_FS_OFFSET;
  local_14 = DAT_00640160 ^ (uint)auStack852;
  uVar3 = DAT_00640160 ^ (uint)&stack0xfffffc9c;
  *(int ***)in_FS_OFFSET = &local_c;
  local_324 = param_1_00;
  local_328 = param_2;
  local_34c = param_1;
  if ((param_1_00 != 0) && (iVar4 = FUN_005ad5a0(local_54), iVar4 != 0)) {
    iVar4 = 0;
    if (true) {
      iVar5 = 0;
      do {
        iVar5 = FUN_004b1430("setid" + iVar5,local_54,uVar3);
        if (iVar5 == 0) break;
        iVar4 += 1;
        iVar5 = iVar4 * 0x14;
      } while ((&DAT_SCRIPT_KCS_CMDTABLE_FUNC)[iVar4 * 5] != (undefined *)0x0);
    }
    if ((code *)(&DAT_SCRIPT_KCS_CMDTABLE_FUNC)[iVar4 * 5] == (code *)0x0) {
      pcVar6 = local_54;
      local_280 = 0xf;
      local_284 = 0;
      local_294 &= 0xffffff00;
      do {
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      FUN_00402c20(local_54,pcVar6 + -(int)(local_54 + 1));
      local_4 = 0;
      piVar7 = (int *)FUN_004a3e10(local_27c,local_298);
      puVar10 = (undefined4 *)*piVar7;
      iVar4 = piVar7[1];
      local_4 = 0xffffffff;
      if (0xf < local_280) {
                    /* WARNING: Subroutine does not return */
        std?_free_00517994(local_294);
      }
      local_280 = 0xf;
      iVar5 = (*(int **)(param_1 + 0x5c))[6];
      local_284 = 0;
      local_294 &= 0xffffff00;
      if ((puVar10 == (undefined4 *)0x0) || (puVar10 != (undefined4 *)**(int **)(param_1 + 0x5c))) {
        call__invalid_parameter();
      }
      if (iVar4 == iVar5) {
        cs2_log_sendmessage_004b57b0(s__:_%s_0061aff0,local_324);
      }
      else {
        if (puVar10 == (undefined4 *)0x0) {
          call__invalid_parameter();
        }
        else {
          puVar10 = (undefined4 *)*puVar10;
        }
        if (iVar4 == puVar10[6]) {
          call__invalid_parameter();
        }
        local_330 = iVar4 + 0x28;
        if (param_3 == -1) {
          param_3 = 0;
          local_32c = 0;
          local_320 = FUN_005c0810();
        }
        else {
          local_32c = param_3;
          local_320 = param_3 + 1;
        }
        if (param_3 < local_320) {
          do {
            piVar7 = (int *)FUN_005c06b0(local_328,param_3);
            if (piVar7 != (int *)0x0) {
              std_vectorfunc_unk_004b17a0(local_31c,0x80);
              iVar4 = 0;
              if (0 < *(int *)(local_330 + 0x168)) {
                local_334 = (int *)(local_330 + 0x1ac);
                local_350 = local_330 + 0x68;
                do {
                  if (false) goto switchD_005b12a7_caseD_1;
                  switch(local_334[-0x10]) {
                  case 0:
                    iVar5 = FUN_005ad5a0(local_94);
                    if (iVar5 == 0) {
                      pcVar6 = (char *)(*local_334 * 0x10 + 0x68 + local_330);
                      pcVar9 = local_94;
                      do {
                        cVar1 = *pcVar6;
                        *pcVar9 = cVar1;
                        pcVar6 = pcVar6 + 1;
                        pcVar9 = pcVar9 + 1;
                      } while (cVar1 != '\0');
                    }
                    break;
                  default:
                    goto switchD_005b12a7_caseD_1;
                  case 0x25:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 0x18);
                    fVar11 = (float10)(int)local_338;
                    if (false) {
                      fVar11 = fVar11 + (float10)4294967296.00000000;
                    }
                    goto LAB_005b1b12;
                  case 0x29:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 & 0xff);
                    fVar11 = (float10)(int)local_338;
                    goto LAB_005b1b12;
                  case 0x2a:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 8 & 0xff);
                    fVar11 = (float10)(int)local_338;
                    goto LAB_005b1b12;
                  case 0x2b:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 0x10 & 0xff);
                    fVar11 = (float10)(int)local_338;
                    goto LAB_005b1b12;
                  case 0x2c:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 0x18);
                    fVar11 = (float10)(int)local_338;
                    if (false) {
                      fVar11 = fVar11 + (float10)4294967296.00000000;
                    }
                    goto LAB_005b1b12;
                  case 0x2d:
                    fVar11 = (float10)*local_334;
                    goto LAB_005b1b12;
                  }
                  switch(*(undefined4 *)(local_330 + 0x28 + *local_334 * 4)) {
                  case 0:
                    fVar2 = 0.00000000;
                    local_36c = fVar2;
                    goto LAB_005b1313;
                  case 1:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b133e;
                  case 2:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b136d;
                  case 3:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b13c2;
                  case 4:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b13f4;
                  case 5:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b1449;
                  case 6:
                    pfVar8 = (float *)FUN_005b0cc0();
                    goto LAB_005b147b;
                  case 7:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b133e;
                  case 8:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b136d;
                  case 9:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b13c2;
                  case 10:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b13f4;
                  case 0xb:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b1449;
                  case 0xc:
                    pfVar8 = (float *)FUN_005b0d40();
                    goto LAB_005b147b;
                  case 0xd:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b133e:
                    fVar2 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    local_36c = fVar2;
                    local_348 = fVar2;
                    goto LAB_005b1313;
                  case 0xe:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b136d:
                    fVar2 = *pfVar8;
                    local_348 = fVar2;
LAB_005b137e:
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0eb0(local_350,fVar2,local_348);
                    fVar11 = fVar11 - (float10)local_348;
                    break;
                  case 0xf:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b13c2:
                    fVar2 = pfVar8[1];
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    local_36c = fVar2;
                    local_348 = *pfVar8;
                    local_344 = fVar2;
                    goto LAB_005b1313;
                  case 0x10:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b13f4:
                    fVar2 = pfVar8[1];
                    local_344 = fVar2;
LAB_005b1406:
                    local_348 = *pfVar8;
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0eb0(local_350,fVar2,local_344);
                    fVar11 = fVar11 - (float10)local_344;
                    break;
                  case 0x11:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b1449:
                    fVar2 = pfVar8[2];
                    local_33c = pfVar8[3];
                    local_36c = fVar2;
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = fVar2;
                    goto LAB_005b1313;
                  case 0x12:
                    pfVar8 = (float *)FUN_005b0c80();
LAB_005b147b:
                    local_348 = *pfVar8;
                    fVar2 = pfVar8[2];
                    local_340 = fVar2;
LAB_005b1493:
                    local_344 = pfVar8[1];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0eb0(local_350,fVar2,local_340);
                    fVar11 = fVar11 - (float10)local_340;
                    break;
                  case 0x13:
                    pfVar8 = (float *)FUN_005b0c10();
                    fVar2 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_348 = fVar2;
                    goto LAB_005b15c5;
                  case 0x14:
                    pfVar8 = (float *)FUN_005b0c10();
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0dc0(local_350,local_348,local_348);
                    fVar11 = fVar11 - (float10)local_348;
                    break;
                  case 0x15:
                    pfVar8 = (float *)FUN_005b0c10();
                    fVar2 = pfVar8[1];
                    local_348 = *pfVar8;
                    local_344 = fVar2;
LAB_005b15c5:
                    local_340 = pfVar8[2];
LAB_005b15cc:
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0dc0(local_350,fVar2,fVar2);
                    break;
                  case 0x16:
                    pfVar8 = (float *)FUN_005b0c10();
                    local_344 = pfVar8[1];
                    local_348 = *pfVar8;
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0dc0(local_350,local_344,local_344);
                    fVar11 = fVar11 - (float10)local_344;
                    break;
                  case 0x17:
                    pfVar8 = (float *)FUN_005b0c10();
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    fVar2 = pfVar8[2];
                    local_340 = fVar2;
                    goto LAB_005b15cc;
                  case 0x18:
                    pfVar8 = (float *)FUN_005b0c10();
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    local_33c = pfVar8[3];
                    fVar11 = (float10)FUN_005b0dc0(local_350,local_340,local_340);
                    fVar11 = fVar11 - (float10)local_340;
                    break;
                  case 0x19:
                    pfVar8 = (float *)FUN_005b0b90();
                    local_33c = pfVar8[3];
                    fVar2 = 1.00000000;
                    local_36c = *pfVar8;
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    goto LAB_005b1313;
                  case 0x1a:
                    pfVar8 = (float *)FUN_005b0b90();
                    fVar2 = 1.00000000;
                    local_348 = *pfVar8;
                    goto LAB_005b137e;
                  case 0x1b:
                    pfVar8 = (float *)FUN_005b0b90();
                    local_33c = pfVar8[3];
                    fVar2 = 1.00000000;
                    local_36c = pfVar8[1];
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    goto LAB_005b1313;
                  case 0x1c:
                    pfVar8 = (float *)FUN_005b0b90();
                    fVar2 = 1.00000000;
                    local_344 = pfVar8[1];
                    goto LAB_005b1406;
                  case 0x1d:
                    pfVar8 = (float *)FUN_005b0b90();
                    local_33c = pfVar8[3];
                    fVar2 = 1.00000000;
                    local_36c = pfVar8[2];
                    local_348 = *pfVar8;
                    local_344 = pfVar8[1];
                    local_340 = pfVar8[2];
                    goto LAB_005b1313;
                  case 0x1e:
                    pfVar8 = (float *)FUN_005b0b90();
                    local_348 = *pfVar8;
                    fVar2 = 1.00000000;
                    local_340 = pfVar8[2];
                    goto LAB_005b1493;
                  case 0x1f:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 0x10 & 0xff);
                    if (false) {
                      local_338 = local_338 + 4294967296.00000000;
                    }
LAB_005b1891:
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    break;
                  case 0x20:
                    uVar3 = FUN_005b0b30();
                    uVar3 = uVar3 >> 0x10 & 0xff;
                    goto LAB_005b18d4;
                  case 0x21:
                    uVar3 = FUN_005b0b30();
                    fVar2 = (float)(uVar3 >> 8 & 0xff);
                    local_36c = fVar2;
                    local_338 = fVar2;
                    if (false) {
                      fVar2 = fVar2 + 4294967296.00000000;
                      local_36c = fVar2;
                      local_338 = fVar2;
                    }
LAB_005b1313:
                    fVar11 = (float10)FUN_005b0eb0(local_350,fVar2,local_36c);
                    break;
                  case 0x22:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 8 & 0xff);
                    if (false) {
                      local_338 = local_338 + 4294967296.00000000;
                    }
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    fVar11 = fVar11 - (float10)local_338;
                    break;
                  case 0x23:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 & 0xff);
                    if (false) {
                      local_338 = local_338 + 4294967296.00000000;
                    }
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    break;
                  case 0x24:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 & 0xff);
                    if (false) {
                      local_338 = local_338 + 4294967296.00000000;
                    }
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    fVar11 = fVar11 - (float10)local_338;
                    break;
                  case 0x25:
                    uVar3 = FUN_005b0b30();
                    local_338 = (float)(uVar3 >> 0x18);
                    if (-1 < (int)(uVar3 >> 0x18)) goto LAB_005b1891;
                    local_338 = local_338 + 4294967296.00000000;
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    break;
                  case 0x26:
                    uVar3 = FUN_005b0b30();
                    uVar3 >>= 0x18;
LAB_005b18d4:
                    local_338 = (float)uVar3;
                    if (false) {
                      local_338 = local_338 + 4294967296.00000000;
                    }
                    fVar11 = (float10)FUN_005b0eb0(local_350,local_338,local_338);
                    fVar11 = fVar11 - (float10)local_338;
                    break;
                  default:
                    goto switchD_005b12a7_caseD_1;
                  }
LAB_005b1b12:
                  local_31c[iVar4] = (float)fVar11;
switchD_005b12a7_caseD_1:
                  local_350 += 0x10;
                  local_334 = local_334 + 1;
                  iVar4 += 1;
                } while (iVar4 < *(int *)(local_330 + 0x168));
              }
              if (piVar7[6] != 0) {
                iVar4 = FUN_005b5af0();
                if (iVar4 == 0) {
                  (**(code **)(*piVar7 + 4))();
                }
                kcMotionScript_load_00597870
                          (piVar7[6],local_330,*(undefined4 *)(local_330 + 0x168),0,
                           *(undefined4 *)(local_330 + 0x20),0,piVar7[10]);
              }
            }
            param_3 = local_32c + 1;
            local_32c = param_3;
          } while (param_3 < local_320);
        }
      }
    }
    else {
      (*(code *)(&DAT_SCRIPT_KCS_CMDTABLE_FUNC)[iVar4 * 5])(param_1,param_1_00,local_328,param_3);
    }
  }
  *in_FS_OFFSET = local_c;
  func__end_critical_section();
  return;
}

