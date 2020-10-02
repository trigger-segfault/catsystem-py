
undefined4 * __fastcall hg3_encode_norm_tables(undefined4 *param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  char cVar5;
  int *piVar6;
  
  new_kcImage();
  *param_1 = 0x6137f4;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 1;
  param_1[0x45d] = 0x5a;
  uVar3 = 0;
  piVar6 = param_1 + 0x15d;
  do {
    uVar1 = (((uVar3 & 0xc0) << 6 | uVar3 & 0x30) << 6 | uVar3 & 0xc) << 6 | uVar3 & 3;
    piVar6[-0x100] = uVar1 << 6;
    *piVar6 = uVar1 << 4;
    piVar6[0x100] = uVar1 * 4;
    piVar6[0x200] = uVar1;
    uVar3 = uVar3 + 1;
    piVar6 = piVar6 + 1;
  } while ((int)uVar3 < 0x100);
  iVar4 = 0;
  cVar5 = -1;
  puVar2 = param_1 + 0x1d;
  do {
    *(char *)puVar2 = (char)iVar4;
    *(char *)((int)puVar2 + 1) = cVar5;
    iVar4 = iVar4 + 1;
    puVar2 = (undefined4 *)((int)puVar2 + 2);
    cVar5 = cVar5 + -1;
  } while (iVar4 < 0x80);
  return param_1;
}



undefined4 hg3_vftable_func1(int *param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = FUN_00419200(*param_1,param_1[1],param_2);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = *param_1;
  if (iVar1 != 0) {
    piVar2 = (int *)(iVar1 + 0xc);
    iVar1 = *(int *)(iVar1 + 0x10);
    while( true ) {
      if (iVar1 == param_2) {
        FUN_00418ac0(piVar2 + 2);
        return 1;
      }
      if (*piVar2 == 0) break;
      piVar2 = (int *)((int)piVar2 + *piVar2);
      iVar1 = piVar2[1];
    }
  }
  FUN_00418ac0(0);
  return 1;
}




undefined4 hg3_vftable_func2(int *param_1,uint param_2,int *param_3)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  
  iVar2 = *param_1;
  if ((iVar2 != 0) && (uVar1 = FUN_00409940(iVar2,&DAT_00613804,4), uVar1 == 0)) {
    piVar4 = (int *)(iVar2 + 0xc);
    if (param_2 != 0) {
      do {
        if (*piVar4 == 0) {
          return 0;
        }
        uVar1 = uVar1 + 1;
        piVar4 = (int *)((int)piVar4 + *piVar4);
      } while (uVar1 < param_2);
    }
    iVar2 = FUN_00419200(*param_1,param_1[1],piVar4[1]);
    if (iVar2 != 0) {
      iVar2 = *param_1;
      if (iVar2 == 0) {
LAB_00419ab8:
        piVar3 = (int *)0x0;
      }
      else {
        piVar3 = (int *)(iVar2 + 0xc);
        iVar2 = *(int *)(iVar2 + 0x10);
        while (iVar2 != piVar4[1]) {
          if (*piVar3 == 0) goto LAB_00419ab8;
          piVar3 = (int *)((int)piVar3 + *piVar3);
          iVar2 = piVar3[1];
        }
        piVar3 = piVar3 + 2;
      }
      FUN_00418ac0(piVar3);
      *param_3 = piVar4[1];
      return 1;
    }
  }
  return 0;
}







byte * hg3_subfunc12_FUN_00418a20(byte *param_1,byte *param_2,undefined4 *param_3)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  bool bVar5;
  
  if (param_1 == (byte *)0x0) {
    return param_1;
  }
  pbVar3 = param_1;
  pbVar4 = param_2;
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = 0;
  }
  do {
    bVar1 = *pbVar3;
    bVar5 = bVar1 < *pbVar4;
    if (bVar1 != *pbVar4) {
LAB_00418a63:
      iVar2 = (1 - (uint)bVar5) - (uint)(bVar5 != false);
      goto joined_r0x00418a6a;
    }
    if (bVar1 == 0) break;
    bVar1 = pbVar3[1];
    bVar5 = bVar1 < pbVar4[1];
    if (bVar1 != pbVar4[1]) goto LAB_00418a63;
    pbVar3 = pbVar3 + 2;
    pbVar4 = pbVar4 + 2;
  } while (bVar1 != 0);
  iVar2 = 0;
joined_r0x00418a6a:
  do {
    if (iVar2 == 0) {
      if (param_3 != (undefined4 *)0x0) {
        *param_3 = *(undefined4 *)(param_1 + 0xc);
      }
      return param_1 + 0x10;
    }
    if (*(int *)(param_1 + 8) == 0) {
      return (byte *)0;
    }
    param_1 = param_1 + *(int *)(param_1 + 8);
    pbVar3 = param_1;
    pbVar4 = param_2;
    do {
      bVar1 = *pbVar3;
      bVar5 = bVar1 < *pbVar4;
      if (bVar1 != *pbVar4) {
LAB_00418aa0:
        iVar2 = (1 - (uint)bVar5) - (uint)(bVar5 != false);
        goto joined_r0x00418a6a;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar5 = bVar1 < pbVar4[1];
      if (bVar1 != pbVar4[1]) goto LAB_00418aa0;
      pbVar3 = pbVar3 + 2;
      pbVar4 = pbVar4 + 2;
    } while (bVar1 != 0);
    iVar2 = 0;
  } while( true );
}





void __thiscall hg3_func12_FUN_00419200(int param_1,int param_1_00,undefined4 param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  undefined *puVar3;
  byte *pbVar4;
  undefined4 uVar5;
  char *pcVar6;
  undefined *puVar7;
  int iVar8;
  char *pcVar9;
  uint *puVar10;
  undefined4 *puVar11;
  byte *pbVar12;
  char *pcVar13;
  int *piVar14;
  int *piVar15;
  uint uVar16;
  int iVar17;
  uint *puVar18;
  int **in_FS_OFFSET;
  undefined4 *local_c4;
  uint *local_c0;
  int local_bc;
  int local_b8;
  uint local_b4;
  int *local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  undefined4 *local_9c;
  undefined4 *local_98;
  undefined4 local_94;
  int local_90;
  byte *local_8c;
  int *local_88;
  int local_84;
  uint *local_80;
  char local_20 [16];
  uint local_10;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_00601d23;
  local_c = *in_FS_OFFSET;
  local_10 = DAT_006883c8 ^ (uint)&local_c4;
  uVar1 = DAT_006883c8 ^ (uint)&stack0xffffff2c;
  *(int ***)in_FS_OFFSET = &local_c;
  iVar2 = FUN_00409940(param_1_00,"HG-3",4,uVar1);
  if (iVar2 == 0) {
    if (*(int *)(param_1_00 + 4) == 0xc) {
      piVar15 = (int *)(param_1_00 + 0xc);
      uVar1 = 0;
      iVar2 = *(int *)(param_1_00 + 0x10);
      while (iVar2 != param_3) {
        if (*piVar15 == 0) {
          piVar15 = (int *)0x0;
          goto LAB_004192a7;
        }
        piVar15 = (int *)((int)piVar15 + *piVar15);
        iVar2 = piVar15[1];
      }
      piVar15 = piVar15 + 2;
LAB_004192a7:
      local_b0 = piVar15;
      piVar14 = (int *)hg3_tag_subfunc12_FUN_00418a20(piVar15,"stdinfo",&local_c4);
      local_88 = piVar14;
      if (piVar14 != (int *)0x0) {
        iVar2 = hg3_tag_subfunc12_FUN_00418a20(piVar15,"img_jpg",&local_c4);
        if (iVar2 == 0) {
          local_c0 = (uint *)piVar14[1];
          local_a4 = *piVar14;
          local_84 = (int)((int)*(short *)(piVar14 + 2) +
                          ((int)*(short *)(piVar14 + 2) >> 0x1f & 7U)) >> 3;
          iVar17 = local_84 * (int)local_c0 * local_a4;
          local_ac = local_84;
          hg3_subfunc12_FUN_00416b10(iVar17);
          *(int *)(param_1 + 0x18) = local_84;
          local_84 = local_84 * local_a4;
          *(int *)(param_1 + 0x14) = local_84;
          *(uint **)(param_1 + 8) = local_c0;
          *(int *)(param_1 + 4) = local_a4;
          *(uint **)(param_1 + 0x44) = local_c0;
          *(undefined4 *)(param_1 + 0x38) = 0;
          *(undefined4 *)(param_1 + 0x3c) = 0;
          *(int *)(param_1 + 0x40) = local_a4;
          local_c4 = (undefined4 *)0x0;
          _sprintf(local_20,"img%04d",0);
          iVar2 = hg3_tag_subfunc12_FUN_00418a20(local_b0,local_20,&local_c4);
          if (iVar2 != 0) {
            iVar8 = 0;
            do {
              if (local_c4 < *(undefined4 **)(iVar2 + 0xc)) {
                local_c4 = *(undefined4 **)(iVar2 + 0xc);
              }
              iVar8 = iVar8 + 1;
              _sprintf(local_20,"img%04d",iVar8);
              iVar2 = hg3_tag_subfunc12_FUN_00418a20(local_b0,local_20,&local_c4);
            } while (iVar2 != 0);
          }
          puVar11 = local_c4 + 4;
          local_b8 = FUN_0040a4e0(0,(int)local_c4 + iVar17 * 2 + 0x2010);
          local_c0 = (uint *)(local_b8 + (int)puVar11);
          pbVar4 = (byte *)(local_b8 + 0x1000 + iVar17 + (int)puVar11);
          local_a8 = 0;
          local_8c = pbVar4;
          local_80 = local_c0;
          _sprintf(local_20,"img%04d",0);
          puVar11 = (undefined4 *)hg3_tag_subfunc12_FUN_00418a20(local_b0,local_20,&local_c4);
          while (local_c4 = puVar11, puVar11 != (undefined4 *)0x0) {
            iVar2 = hg3_subfunc12_FUN_006015c0(local_b8,puVar11[3] + 0x10,puVar11 + 6,puVar11[2]);
            if (iVar2 != puVar11[3]) {
              FUN_00403450(s_kcImageHg3_:_0061386c);
              FUN_0040a530(local_b8);
              goto LAB_004199a4;
            }
            iVar2 = puVar11[2];
            uVar5 = FUN_0040a4e0(0,puVar11[5] + 0x10);
            *(undefined4 *)(param_1 + 0x58) = uVar5;
            *(undefined4 *)(param_1 + 0x5c) = uVar5;
            *(undefined4 *)(param_1 + 0x60) = 0;
            *(undefined *)(param_1 + 100) = 0;
            iVar2 = hg3_subfunc12_FUN_006015c0
                              (uVar5,puVar11[5] + 0x10,(int)(puVar11 + 6) + iVar2,puVar11[4]);
            if (iVar2 != puVar11[5]) {
              FUN_0040a530(local_b8);
              FUN_0040a530(*(undefined4 *)(param_1 + 0x58));
              *(undefined4 *)(param_1 + 0x58) = 0;
              FUN_00403450(s_kcImageHg3_:_00613840);
              goto LAB_004199a4;
            }
            if (*(int *)(param_1 + 0x60) == 0) {
              *(undefined *)(param_1 + 100) = **(undefined **)(param_1 + 0x5c);
              *(undefined **)(param_1 + 0x5c) = *(undefined **)(param_1 + 0x5c) + 1;
            }
            local_b4 = (uint)(*(byte *)(param_1 + 100) >> (*(byte *)(param_1 + 0x60) & 0x1f)) & 1;
            *(uint *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1U & 7;
            iVar2 = FUN_00419180();
            local_bc = local_b8;
            FUN_00409a50(pbVar4,iVar2);
            pbVar12 = pbVar4;
            if (local_b4 != 0) {
              iVar17 = FUN_00419180();
              iVar2 = iVar2 - iVar17;
              FUN_00409980(pbVar4,local_b8,iVar17);
              local_bc = iVar17 + local_b8;
              pbVar12 = pbVar4 + iVar17;
            }
            while (0 < iVar2) {
              iVar17 = FUN_00419180();
              pbVar12 = pbVar12 + iVar17;
              if (iVar2 - iVar17 < 1) break;
              local_b4 = FUN_00419180();
              iVar2 = (iVar2 - iVar17) - local_b4;
              FUN_00409980(pbVar12,local_bc,local_b4);
              local_bc = local_bc + local_b4;
              pbVar12 = pbVar12 + local_b4;
            }
            puVar18 = local_c0;
            if (((uint)(pbVar12 + -(int)pbVar4) & 3) != 0) {
              FUN_0040a530(local_b8);
              FUN_0040a530(*(undefined4 *)(param_1 + 0x58));
              *(undefined4 *)(param_1 + 0x58) = 0;
              goto LAB_004199a4;
            }
            // Likely getting section lengths of the 4 buffers
            iVar2 = (int)(pbVar12 + -(int)pbVar4) >> 2; // sectlen
            local_b4 = iVar2 * 2; // sectlen * 2
            if (0 < iVar2) {
              // weighted table normalization
              // pbVar12 / pbVar4 (2D orthogonal transform??)
              // pbVar4; // sect1
              pbVar12 = pbVar4 + local_b4; // sect3
              local_b4 = -iVar2;
              puVar10 = local_c0;
              local_bc = iVar2;
              do {
                //puVar10; // outbuf
                //*puVar10 = table4[sect4[i]] | table2[sect2[i]] | table3[sect3[i]] | table1[sect1[i]]
                *puVar10 = *(uint *)(param_1 + 0xd74 + (uint)pbVar12[iVar2] * 4) | // table4[sect4[i]]
                           *(uint *)(param_1 + 0x574 + (uint)pbVar12[local_b4] * 4) | // table2[sect2[i]]
                           *(uint *)(param_1 + 0x174 + (uint)*pbVar4 * 4) | // table1[sect1[i]]
                           *(uint *)(param_1 + 0x974 + (uint)*pbVar12 * 4); // table3[sect3[i]]
                pbVar4 = pbVar4 + 1;
                pbVar12 = pbVar12 + 1;
                local_bc = local_bc + -1;
                puVar10 = puVar10 + 1;
              } while (local_bc != 0);
            }
            pcVar6 = (char *)FUN_004162e0(0,*local_c4);
            pbVar4 = local_8c;
            iVar2 = *(int *)(param_1 + 0x14);
            pcVar9 = pcVar6;
            iVar17 = local_ac;
            while (iVar17 != 0) {
              iVar17 = iVar17 + -1;
              *pcVar9 = *(char *)((uint)*(byte *)puVar18 + 0x74 + param_1);
              pcVar9 = pcVar9 + 1;
              puVar18 = (uint *)((int)puVar18 + 1);
            }
            iVar17 = (local_a4 + -1) * local_ac;
            if (iVar17 != 0) {
              pcVar13 = pcVar9 + -local_ac;
              do {
                iVar17 = iVar17 + -1;
                *pcVar9 = *(char *)((uint)*(byte *)puVar18 + 0x74 + param_1) + *pcVar13;
                pcVar9 = pcVar9 + 1;
                pcVar13 = pcVar13 + 1;
                puVar18 = (uint *)((int)puVar18 + 1);
              } while (iVar17 != 0);
            }
            pcVar6 = pcVar6 + -iVar2;
            local_bc = 1;
            if (1 < (int)local_c4[1]) {
              do {
                pcVar9 = pcVar6 + iVar2;
                pcVar13 = pcVar6;
                iVar17 = local_84;
                while (iVar17 != 0) {
                  iVar17 = iVar17 + -1;
                  *pcVar13 = *(char *)((uint)*(byte *)puVar18 + 0x74 + param_1) + *pcVar9;
                  pcVar13 = pcVar13 + 1;
                  pcVar9 = pcVar9 + 1;
                  puVar18 = (uint *)((int)puVar18 + 1);
                }
                local_bc = local_bc + 1;
                pcVar6 = pcVar6 + -iVar2;
              } while (local_bc < (int)local_c4[1]);
            }
            local_a8 = local_a8 + 1;
            local_c0 = local_80;
            _sprintf(local_20,"img%04d",local_a8);
            puVar11 = (undefined4 *)hg3_tag_subfunc12_FUN_00418a20(local_b0,local_20,&local_c4);
            piVar14 = local_88;
          }
          FUN_0040a530(*(undefined4 *)(param_1 + 0x58));
          *(undefined4 *)(param_1 + 0x58) = 0;
          FUN_0040a530(local_b8);
        }
        else {
          FUN_00418060();
          local_94 = 1;
          local_4 = 1;
          local_90 = 1;
          local_9c = local_c4;
          local_98 = local_c4;
          local_a0 = iVar2;
          FUN_004182f0(&local_a0,0);
          *(int *)(param_1 + 4) = *piVar14;
          *(int *)(param_1 + 8) = piVar14[1];
          iVar2 = (int)((int)*(short *)(piVar14 + 2) + ((int)*(short *)(piVar14 + 2) >> 0x1f & 7U))
                  >> 3;
          *(int *)(param_1 + 0x18) = iVar2;
          *(int *)(param_1 + 0x14) = *(int *)(param_1 + 4) * iVar2;
          *(int *)(param_1 + 0x40) = *(int *)(param_1 + 4);
          *(undefined4 *)(param_1 + 0x38) = 0;
          *(undefined4 *)(param_1 + 0x3c) = 0;
          *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(param_1 + 8);
          hg3_subfunc12_FUN_00416b10
                    (*(int *)(param_1 + 8) * *(int *)(param_1 + 0x18) * *(int *)(param_1 + 4));
          if (*(int *)(param_1 + 0x18) == 4) {
            if (*(int *)(param_1 + 8) != 0) {
              do {
                puVar3 = (undefined *)FUN_004162b0(0,uVar1);
                puVar7 = (undefined *)(*(int *)(param_1 + 0x14) * uVar1 + *(int *)(param_1 + 0xc));
                uVar16 = 0;
                if (*(int *)(param_1 + 4) != 0) {
                  do {
                    *puVar7 = *puVar3;
                    puVar7[1] = puVar3[1];
                    puVar7[2] = puVar3[2];
                    puVar7[3] = 0xff;
                    uVar16 = uVar16 + 1;
                    puVar3 = puVar3 + 3;
                    puVar7 = puVar7 + 4;
                  } while (uVar16 < *(uint *)(param_1 + 4));
                }
                uVar1 = uVar1 + 1;
              } while (uVar1 < *(uint *)(param_1 + 8));
            }
          }
          else {
            if ((*(int *)(param_1 + 0x18) == 3) && (uVar1 = 0, *(int *)(param_1 + 8) != 0)) {
              do {
                puVar3 = (undefined *)FUN_004162b0(0,uVar1);
                puVar7 = (undefined *)(*(int *)(param_1 + 0x14) * uVar1 + *(int *)(param_1 + 0xc));
                uVar16 = 0;
                if (*(int *)(param_1 + 4) != 0) {
                  do {
                    *puVar7 = *puVar3;
                    puVar7[1] = puVar3[1];
                    puVar7[2] = puVar3[2];
                    uVar16 = uVar16 + 1;
                    puVar7 = puVar7 + 3;
                    puVar3 = puVar3 + 3;
                  } while (uVar16 < *(uint *)(param_1 + 4));
                }
                uVar1 = uVar1 + 1;
              } while (uVar1 < *(uint *)(param_1 + 8));
            }
          }
          if ((*(int *)(param_1 + 0x18) == 4) &&
             (puVar11 = (undefined4 *)hg3_tag_subfunc12_FUN_00418a20(local_b0,"img_al",&local_c4),
             puVar11 != (undefined4 *)0x0)) {
            uVar1 = puVar11[1];
            if (uVar1 < 0x400) {
              uVar1 = 0x400;
            }
            local_c0 = (uint *)FUN_0040a4e0(0,uVar1);
            iVar2 = hg3_subfunc12_FUN_006015c0(local_c0,uVar1,puVar11 + 2,*puVar11);
            if ((iVar2 == puVar11[1]) && (iVar2 = 0, 0 < piVar14[1])) {
              iVar17 = *piVar14;
              puVar18 = local_c0;
              do {
                pbVar4 = (byte *)(*(int *)(param_1 + 0x14) * iVar2 + 3 + *(int *)(param_1 + 0xc));
                iVar8 = 0;
                if (0 < iVar17) {
                  do {
                    *pbVar4 = *(byte *)puVar18;
                    iVar17 = *piVar14;
                    pbVar4 = pbVar4 + *(int *)(param_1 + 0x18);
                    iVar8 = iVar8 + 1;
                    puVar18 = (uint *)((int)puVar18 + 1);
                  } while (iVar8 < iVar17);
                }
                iVar2 = iVar2 + 1;
              } while (iVar2 < piVar14[1]);
            }
            if (local_c0 != (uint *)0x0) {
              FUN_0040a530(local_c0);
            }
          }
          if ((local_90 == 0) && (local_a0 != 0)) {
            FUN_0040a530(local_a0);
          }
          local_a0 = 0;
          local_9c = (undefined4 *)0x0;
          local_98 = (undefined4 *)0x0;
          local_4 = 0xffffffff;
          FUN_00418080();
        }
        *(int *)(param_1 + 0x1c) = piVar14[5];
        *(int *)(param_1 + 0x20) = piVar14[6];
        *(int *)(param_1 + 0x24) = piVar14[3];
        *(int *)(param_1 + 0x28) = piVar14[4];
        *(int *)(param_1 + 0x2c) = (uint)(piVar14[7] != 0) + 1;
        *(int *)(param_1 + 0x30) = piVar14[8];
        *(int *)(param_1 + 0x34) = piVar14[9];
      }
    }
    else {
      FUN_00403450(s_HG3_:_006138b0);
    }
  }
  else {
    FUN_00403450(s_HG3_:_006138d0);
  }
LAB_004199a4:
  *in_FS_OFFSET = local_c;
  FUN_0041cc83();
  return;
}



