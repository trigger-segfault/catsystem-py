
undefined4 __thiscall
hg3_endecode_func_FUN_00419ae0
          (int param_1,int param_1_00,undefined4 param_2,int param_3,uint param_4,byte param_5)

{
  short sVar1;
  int **ppiVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  BOOL BVar6;
  undefined *puVar7;
  undefined4 uVar8;
  int iVar9;
  int *piVar10;
  undefined4 *_Dest;
  char *_Dest_00;
  int **in_FS_OFFSET;
  int local_a4;
  int *local_a0;
  int *local_9c;
  int local_98;
  uint local_94;
  int local_90;
  int local_8c;
  uint local_88;
  undefined4 local_84;
  undefined4 local_80;
  int local_7c;
  RECT local_78 [6];
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_00601d73;
  local_c = *in_FS_OFFSET;
  uVar4 = DAT_006883c8 ^ (uint)&stack0xffffff4c;
  *(int ***)in_FS_OFFSET = &local_c;
  SetRectEmpty((LPRECT)local_78);
  iVar5 = FUN_00416920(local_78);
  if (iVar5 != 0) {
    BVar6 = IsRectEmpty(local_78);
    if (BVar6 != 0) {
      local_78[0].left = 0;
      local_78[0].right = 2;
      local_78[0].top = 0;
      local_78[0].bottom = 2;
    }
    if ((*(int *)(param_1 + 0x70) != 0) && (iVar5 = FUN_00416be0(local_78), iVar5 != 0)) {
      *(LONG *)(param_1 + 0x24) = local_78[0].left;
      *(LONG *)(param_1 + 0x28) = local_78[0].top;
    }
  }
  iVar5 = 0;
  if (param_3 != 0) {
    FUN_00416b70(uVar4);
  }
  local_94 = *(int *)(param_1 + 4) * *(int *)(param_1 + 8) * *(int *)(param_1 + 0x18) * 2;
  if (local_94 < 0x19000) {
    local_94 = 0x19000;
  }
  puVar7 = (undefined *)FUN_00412170(local_94);
  *puVar7 = 0x48;
  puVar7[1] = 0x47;
  puVar7[2] = 0x2d;
  puVar7[3] = 0x33;
  *(undefined4 *)(puVar7 + 4) = 0xc;
  _Dest = (undefined4 *)(puVar7 + 0x14);
  *(undefined2 *)(puVar7 + 10) = 0;
  *(undefined2 *)(puVar7 + 8) = 0x300;
  *(undefined4 *)(puVar7 + 0x10) = param_2;
  *(undefined4 *)(puVar7 + 0xc) = 0;
  *_Dest = 0x69647473;
  *(undefined4 *)(puVar7 + 0x18) = 0x6f666e;
  *(undefined4 *)(puVar7 + 0x1c) = 0x38;
  *(undefined4 *)(puVar7 + 0x20) = 0x28;
  *(undefined4 *)(puVar7 + 0x24) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(puVar7 + 0x28) = *(undefined4 *)(param_1 + 8);
  sVar1 = *(short *)(param_1 + 0x18);
  *(undefined2 *)(puVar7 + 0x2e) = 0;
  *(short *)(puVar7 + 0x2c) = sVar1 * 8;
  *(undefined4 *)(puVar7 + 0x30) = *(undefined4 *)(param_1 + 0x24);
  *(undefined4 *)(puVar7 + 0x34) = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(puVar7 + 0x38) = *(undefined4 *)(param_1 + 0x1c);
  *(undefined4 *)(puVar7 + 0x3c) = *(undefined4 *)(param_1 + 0x20);
  uVar8 = FUN_004165d0();
  *(undefined4 *)(puVar7 + 0x40) = uVar8;
  *(undefined4 *)(puVar7 + 0x44) = *(undefined4 *)(param_1 + 0x30);
  *(undefined4 *)(puVar7 + 0x48) = *(undefined4 *)(param_1 + 0x34);
  local_a4 = 0x4c;
  if ((param_5 & 1) == 0) {
    local_a0 = (int *)((*(int *)(param_1 + 8) + -1 + param_4) / param_4);
    local_98 = 0;
    if (0 < *(int *)(param_1 + 8)) {
      do {
        _Dest = (undefined4 *)((int)_Dest + _Dest[2]);
        piVar10 = (int *)(*(int *)(param_1 + 8) - iVar5);
        if ((int)local_a0 < (int)piVar10) {
          piVar10 = local_a0;
        }
        _sprintf((char *)_Dest,"img%04d",local_98);
        iVar9 = FUN_00418d60(_Dest + 4,local_94 - local_a4,iVar5,piVar10);
        _Dest[3] = iVar9;
        if (iVar9 == -1) {
          uVar8 = 0;
          goto LAB_00419fef;
        }
        local_98 = local_98 + 1;
        _Dest[2] = iVar9 + 0x10;
        iVar5 = iVar5 + (int)piVar10;
        local_a4 = local_a4 + 0x10 + iVar9;
      } while (iVar5 < *(int *)(param_1 + 8));
    }
  }
  else {
    FUN_00418060();
    local_80 = 1;
    local_8c = 0;
    local_88 = 0;
    local_84 = 0;
    local_7c = 0;
    local_4 = 1;
    FUN_00418090(*(undefined4 *)(param_1 + 0x1174));
    FUN_00416d50(param_1);
    FUN_00416510();
    iVar5 = FUN_00418540(&local_8c,0,0,0,1);
    if (iVar5 != 0) {
      uVar4 = 0;
      iVar9 = 0;
      local_a0 = (int *)0x0;
      local_98 = 0;
      iVar5 = FUN_00418b70(&local_a0,&local_98);
      if (iVar5 != 0) {
        iVar5 = local_98 * 2;
        iVar9 = FUN_0040a4e0(0,iVar5);
        uVar4 = FUN_00601460(iVar9,iVar5,local_a0,local_98);
        FUN_0040a530(local_a0);
      }
      if (local_88 < local_94 - 0x4c) {
        _Dest = (undefined4 *)((int)_Dest + *(int *)(puVar7 + 0x1c));
        _sprintf((char *)_Dest,"img_jpg");
        FUN_00409980(_Dest + 4,local_8c,local_88);
        _Dest[3] = local_88;
        _Dest[2] = local_88 + 0x10;
        local_a4 = local_88 + 0x5c;
      }
      if (iVar9 != 0) {
        _Dest = (undefined4 *)((int)_Dest + _Dest[2]);
        local_a0 = _Dest + 4;
        local_90 = local_98;
        local_94 = uVar4;
        _sprintf((char *)_Dest,"img_al");
        FUN_00409980(local_a0,&local_94,8);
        FUN_00409980(local_a0 + 2,iVar9,uVar4);
        _Dest[3] = uVar4 + 8;
        local_a4 = local_a4 + 0x18 + uVar4;
        _Dest[2] = uVar4 + 0x18;
        FUN_0040a530(iVar9);
      }
    }
    if ((local_7c == 0) && (local_8c != 0)) {
      FUN_0040a530(local_8c);
    }
    local_8c = 0;
    local_88 = 0;
    local_84 = 0;
    local_4 = 0xffffffff;
    FUN_00418080();
  }
  ppiVar2 = *(int ***)(param_1 + 0x48);
  if (ppiVar2 != (int **)0x0) {
    local_9c = (int *)*ppiVar2[6];
    local_a0 = *ppiVar2;
    while( true ) {
      piVar3 = local_a0;
      piVar10 = (*(int ***)(param_1 + 0x48))[6];
      if ((local_a0 == (int *)0x0) || (local_a0 != **(int ***)(param_1 + 0x48))) {
        FUN_0041d16e();
      }
      if (local_9c == piVar10) break;
      if (piVar3 == (int *)0x0) {
        FUN_0041d16e();
        iVar5 = 0;
      }
      else {
        iVar5 = *piVar3;
      }
      if (local_9c == *(int **)(iVar5 + 0x18)) {
        FUN_0041d16e();
      }
      if ((local_9c[3] & 0xffff0000U) == 0) {
        if (piVar3 == (int *)0x0) {
          FUN_0041d16e();
          iVar5 = 0;
        }
        else {
          iVar5 = *piVar3;
        }
        if (local_9c == *(int **)(iVar5 + 0x18)) {
          FUN_0041d16e();
        }
        iVar5 = local_9c[4];
        if (iVar5 != 0) {
          _Dest = (undefined4 *)((int)_Dest + _Dest[2]);
          if (piVar3 == (int *)0x0) {
            FUN_0041d16e();
            iVar9 = 0;
          }
          else {
            iVar9 = *piVar3;
          }
          piVar10 = local_9c;
          if (local_9c == *(int **)(iVar9 + 0x18)) {
            FUN_0041d16e();
          }
          _sprintf((char *)_Dest,"ats%04d",(uint)*(ushort *)(piVar10 + 3));
          FUN_00409980(_Dest + 4,iVar5,0x14);
          local_a4 = local_a4 + 0x24;
          _Dest[3] = 0x14;
          _Dest[2] = 0x24;
        }
      }
      FUN_00409280();
    }
  }
  _Dest_00 = (char *)((int)_Dest + _Dest[2]);
  _sprintf(_Dest_00,"cptype");
  *(undefined4 *)(_Dest_00 + 0x10) = *(undefined4 *)(param_1 + 0x54);
  *(undefined4 *)(_Dest_00 + 0xc) = 4;
  *(undefined4 *)(_Dest_00 + 8) = 0;
  *(int *)(param_1_00 + 4) = local_a4 + 0x14;
  uVar8 = 1;
LAB_00419fef:
  *in_FS_OFFSET = local_c;
  return uVar8;
}









void __thiscall
hg3_img_encode_FUN_00418d60(int param_1,int *param_1_00,int param_2,int param_3,int param_4)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  char *pcVar5;
  byte *pbVar6;
  int iVar7;
  int iVar8;
  char *pcVar9;
  int **in_FS_OFFSET;
  char *local_18c;
  uint byteDepth_local_188;
  undefined4 local_184;
  char *local_180;
  int *local_17c;
  int local_178;
  int local_174;
  int *local_170;
  int stride_local_16c;
  char local_110 [255];
  char local_11;
  uint local_10;
  int *local_c;
  undefined *puStack8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack8 = &LAB_00601cdb;
  local_c = *in_FS_OFFSET;
  local_10 = DAT_006883c8 ^ (uint)&local_18c;
  uVar2 = DAT_006883c8 ^ (uint)&stack0xfffffe64;
  *(int ***)in_FS_OFFSET = &local_c;
  local_170 = param_1_00;
  new_kcImage(uVar2);
  local_4 = 0;
  if ((-1 < param_3) && (iVar8 = *(int *)(param_1 + 8), param_3 < iVar8)) {
    if (iVar8 < param_3 + param_4) {
      param_4 = iVar8 - param_3;
    }
    local_17c = param_1_00 + 6;
    hg3_unk_encode_FUN_004162b0(0,param_3);
    byteDepth_local_188 = *(uint *)(param_1 + 0x18); // bytedepth
    stride_local_16c = *(int *)(param_1 + 0x14); // height/stride?
    iVar8 = *(int *)(param_1 + 4);
    // Handle bytedepth??
    if ((byteDepth_local_188 == 3) || (byteDepth_local_188 == 4)) {
      hg3_subencode_func1_FUN_00416e10(param_1,0,param_3,iVar8,param_4);
      local_178 = byteDepth_local_188 * iVar8;
      local_18c = (char *)(local_178 * param_4);
      if ((int)local_18c < 0x400) {
        local_18c = (char *)0x400;
      }
      iVar3 = (int)local_18c * 2;
      local_174 = iVar3;
      local_180 = (char *)hg3_HeapAlloc_FUN_0040a4e0(0,(int)local_18c * 3);
      local_184 = hg3_HeapAlloc_FUN_0040a4e0(0,iVar3);
      iVar3 = iVar8 * param_4;
      if (iVar3 < 0x400) {
        iVar3 = 0x400;
      }
      uVar4 = hg3_HeapAlloc_FUN_0040a4e0(0,iVar3);
      iVar3 = 1;
      *(undefined4 *)(param_1 + 0x58) = uVar4;
      *(undefined4 *)(param_1 + 0x5c) = uVar4;
      *(undefined4 *)(param_1 + 0x60) = 0;
      *(undefined *)(param_1 + 100) = 0;

      /////IMAGE DELTA
      // Delta by stride (rows)
      if (param_4 > 1) {
        do {
          pcVar5 = (char *)hg3_unkfunc_FUN_004162e0(0,param_4 - iVar3);
          iVar7 = local_178;
          while (iVar7 != 0) {
            *pcVar5 = *pcVar5 - pcVar5[stride_local_16c];
            iVar7 = iVar7 + -1;
            pcVar5 = pcVar5 + 1;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < param_4);
      }
      iVar3 = hg3_unkfunc_FUN_004162e0(iVar8,0);
      iVar8 = (iVar8 + -1) * byteDepth_local_188;
      pcVar5 = (char *)(iVar3 + -1);
      // Delta by byteDepth (first row)
      if (iVar8 != 0) {
        pcVar9 = pcVar5 + -byteDepth_local_188;
        do {
          *pcVar5 = *pcVar5 - *pcVar9;
          pcVar5 = pcVar5 + -1;
          pcVar9 = pcVar9 + -1;
          iVar8 = iVar8 + -1;
        } while (iVar8 != 0);
      }

      // String copy?? [255]
      cVar1 = '\0';
      pcVar5 = &local_11;
      pcVar9 = local_110;
      iVar8 = 0x80;
      do {
        *pcVar9 = cVar1;
        *pcVar5 = cVar1 + '\x01';
        pcVar9 = pcVar9 + 1;
        pcVar5 = pcVar5 + -1;
        cVar1 = cVar1 + '\x02';
        iVar8 = iVar8 + -1;
      } while (iVar8 != 0);
      iVar8 = 0;
      pcVar5 = local_180;
      if (0 < param_4) {
        do {
          pbVar6 = (byte *)hg3_unkfunc_FUN_004162e0(0,iVar8);
          iVar3 = local_178;
          while (iVar3 != 0) {
            iVar3 = iVar3 + -1;
            *pcVar5 = local_110[*pbVar6];
            pcVar5 = pcVar5 + 1;
            pbVar6 = pbVar6 + 1;
          }
          iVar8 = iVar8 + 1;
        } while (iVar8 < param_4);
      }
      local_18c = (char *)hg3_unk_encode_tablefunc_FUN_0041a290(local_180,local_184,local_18c);
      pcVar5 = local_180;
      uVar2 = 0;
      byteDepth_local_188 = 0;
      *(byte *)(param_1 + 100) |= (*local_180 != '\0') << ((byte)*(int*)(param_1 + 0x60) & 0x1f);
      //*(byte *)(param_1 + 100) |= (*local_180 != '\0') << ((byte)*(int*)(param_1 + 0x60) & 0x1f);
      //*(byte *)(param_1 + 100) =
      //     *(byte *)(param_1 + 100) |
      //     (*local_180 != '\0') << ((byte)*(int *)(param_1 + 0x60) & 0x1f);
      iVar8 = *(int *)(param_1 + 0x60) + 1;
      *(int *)(param_1 + 0x60) = iVar8;
      if (7 < iVar8) {
        **(undefined **)(param_1 + 0x5c) = *(undefined *)(param_1 + 100);
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        *(undefined *)(param_1 + 100) = 0;
        *(undefined4 *)(param_1 + 0x60) = 0;
      }
      hg3_unk_encodefunc_FUN_0041a180(local_18c);
      pcVar9 = pcVar5;
      while (local_18c != (char *)0x0) {
        local_18c = local_18c + -1;
        if (*pcVar5 == '\0') {
          if (uVar2 != 0) {
            hg3_unk_encodefunc_FUN_0041a180(uVar2);
            uVar2 = 0;
          }
          byteDepth_local_188 = byteDepth_local_188 + 1;
        }
        else {
          if (byteDepth_local_188 != 0) {
            hg3_unk_encodefunc_FUN_0041a180(byteDepth_local_188);
            byteDepth_local_188 = 0;
          }
          *pcVar9 = *pcVar5;
          pcVar9 = pcVar9 + 1;
          uVar2 = uVar2 + 1;
        }
        pcVar5 = pcVar5 + 1;
      }
      if ((byteDepth_local_188 | uVar2) != 0) {
        hg3_unk_encodefunc_FUN_0041a180(byteDepth_local_188 | uVar2);
      }
      local_18c = pcVar9 + -(int)local_180;
      iVar8 = zlib_encode_FUN_00601460(local_184,local_174,local_180,local_18c);
      if (param_2 + -0x18 < iVar8) {
        iVar3 = 0;
      }
      else {
        hg3_unk_copy_memstr_FUN_00409980(local_17c,local_184,iVar8);
        local_17c = (int *)(iVar8 + (int)local_17c);
        iVar3 = (param_2 + -0x18) - iVar8;
      }
      local_170[2] = iVar8;
      *(char **)(local_170 + 3) = local_18c;
      local_18c = (char *)FUN_0041a240();
      iVar8 = zlib_encode_FUN_00601460
                        (local_184,local_174,*(undefined4 *)(param_1 + 0x58),local_18c);
      if (iVar8 <= iVar3) {
        hg3_unk_copy_memstr_FUN_00409980(local_17c,local_184,iVar8);
      }
      *(char **)(local_170 + 5) = local_18c;
      local_170[4] = iVar8;
      *local_170 = param_3;
      local_170[1] = param_4;
      hg3_HeapFree_FUN_0040a530(*(undefined4 *)(param_1 + 0x58));
      *(undefined4 *)(param_1 + 0x58) = 0;
      hg3_HeapFree_FUN_0040a530(local_180);
      hg3_HeapFree_FUN_0040a530(local_184);
      local_4 = 0xffffffff;
      dtor_kcImage();
      goto LAB_00418e37;
    }
    cs2log_FUN_00403450(s__00613820);
  }
  local_4 = 0xffffffff;
  dtor_kcImage();
LAB_00418e37:
  *in_FS_OFFSET = local_c;
  g_stack_security_FUN_0041cc83();
  return;
}





int __thiscall hg3_unk_encode_FUN_004162b0(int param_1,uint param_1_00,uint param_2)

{
  if ((param_1_00 < *(uint *)(param_1 + 4)) && (param_2 < *(uint *)(param_1 + 8))) {
    return *(int *)(param_1 + 0x18) * param_1_00 + *(int *)(param_1 + 0x14) * param_2 +
           *(int *)(param_1 + 0xc);
  }
  return 0;
}






undefined4 __thiscall
hg3_subencode_func1_FUN_00416e10
          (int param_1,int param_1_00,uint param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar1 = *(int *)(param_1_00 + 4);
  iVar3 = *(int *)(param_1_00 + 8);
  if (*(int *)(param_1_00 + 0x10) == 0) {
    return 0;
  }
  if (((int)param_2 <= iVar1) && (param_3 <= iVar3)) {
    if (iVar1 < (int)(param_4 + param_2)) {
      param_4 = iVar1 - param_2;
    }
    if (iVar3 < param_3 + param_5) {
      param_5 = iVar3 - param_3;
    }
    iVar1 = hg3_subfunc12_FUN_00416b10(*(int *)(param_1_00 + 0x18) * param_4 * param_5);
    if (iVar1 == 0) {
      return 0;
    }
    *(int *)(param_1 + 4) = param_4;
    *(int *)(param_1 + 8) = param_5;
    iVar1 = *(int *)(param_1_00 + 0x18);
    *(int *)(param_1 + 0x18) = iVar1;
    uVar2 = iVar1 * param_4;
    *(uint *)(param_1 + 0x14) = uVar2;
    if ((uVar2 & 3) != 0) {
      *(int *)(param_1 + 0x14) = (uVar2 - (uVar2 & 3)) + 4;
    }
    *(int *)(param_1 + 0x1c) = param_4;
    *(int *)(param_1 + 0x20) = param_5;
    *(undefined4 *)(param_1 + 0x24) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
    *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1_00 + 0x2c);
    *(int *)(param_1 + 0x40) = param_4;
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(int *)(param_1 + 0x44) = param_5;
    uVar2 = 0;
    *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(param_1_00 + 0x54);
    if (0 < param_5) {
      do {
        if ((param_2 < *(uint *)(param_1_00 + 4)) && (uVar2 + param_3 < *(uint *)(param_1_00 + 8)))
        {
          iVar1 = *(int *)(param_1_00 + 0x14) * (uVar2 + param_3) +
                  *(int *)(param_1_00 + 0x18) * param_2 + *(int *)(param_1_00 + 0xc);
        }
        else {
          iVar1 = 0;
        }
        if ((*(int *)(param_1 + 4) == 0) || (*(uint *)(param_1 + 8) <= uVar2)) {
          iVar3 = 0;
        }
        else {
          iVar3 = *(int *)(param_1 + 0x14) * uVar2 + *(int *)(param_1 + 0xc);
        }
        hg3_unk_copy_memstr_FUN_00409980
                  (iVar3,iVar1,*(int *)(param_1 + 0x18) * *(int *)(param_1 + 4));
        uVar2 = uVar2 + 1;
      } while ((int)uVar2 < param_5);
    }
    return 1;
  }
  return 0;
}




void hg3_unk_copy_memstr_FUN_00409980(undefined8 *param_1,undefined8 *param_2,uint param_3)

{
  undefined4 uVar1;
  uint _Size;
  undefined8 uVar2;
  
  if (((param_3 != 0) && (param_1 != (undefined8 *)0x0)) && (param_2 != (undefined8 *)0x0)) {
    if (DAT_00769b84 == 0) {
      _memcpy(param_1,param_2,param_3);
      return;
    }
    uVar2 = FUN_00401c60();
    if (*(int *)((int)uVar2 + 0x28) == 0) {
      if ((_Size & 3) == 0) {
        param_3 = param_3 >> 2;
        do {
          uVar1 = *(undefined4 *)param_2;
          param_2 = (undefined8 *)((int)param_2 + 4);
          *(undefined4 *)param_1 = uVar1;
          param_1 = (undefined8 *)((int)param_1 + 4);
          param_3 = param_3 - 1;
        } while (param_3 != 0);
        return;
      }
    }
    else {
      if ((_Size & 7) == 0) {
        param_3 = param_3 >> 3;
        do {
          uVar2 = *param_2;
          param_2 = param_2 + 1;
          *param_1 = uVar2;
          param_1 = param_1 + 1;
          param_3 = param_3 - 1;
        } while (param_3 != 0);
        return;
      }
      if ((_Size & 3) == 0) {
        param_3 = param_3 >> 2;
        do {
          uVar1 = *(undefined4 *)param_2;
          param_2 = (undefined8 *)((int)param_2 + 4);
          *(undefined4 *)param_1 = uVar1;
          param_1 = (undefined8 *)((int)param_1 + 4);
          param_3 = param_3 - 1;
        } while (param_3 != 0);
        return;
      }
    }
    _memcpy(param_1,(void *)((ulonglong)uVar2 >> 0x20),_Size);
  }
  return;
}



int __thiscall hg3_unkfunc_FUN_004162e0(int param_1,int param_1_00,int param_2)

{
  return ((*(int *)(param_1 + 8) - param_2) + -1) * *(int *)(param_1 + 0x14) +
         *(int *)(param_1 + 0x18) * param_1_00 + *(int *)(param_1 + 0xc);
}


byte * hg3_unk_encode_tablefunc_FUN_0041a290(byte *param_1,int param_2,int param_3)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  byte *pbVar8;
  
  std_vector_func_FUN_00409a50(param_1 + param_3,8);
  iVar5 = (int)((param_3 + 3U & 0xfffffffc) + ((int)(param_3 + 3U) >> 0x1f & 3U)) >> 2;
  param_3 = 6;
  iVar2 = iVar5;
  pbVar3 = (byte *)param_2;
  pbVar8 = param_1;
  do {
    while (iVar2 != 0) {
// for 
// bVar4 = 0
// for j in range(0, 8, 2):
//    bVar1 = pbVar8[i+j+0]
//    bVar4 |= ((bVar1 >> (param_3 & 0x1f)) & 0x3) << (j & 0x1f)  & 0xff  # mask byte
// pbVar3[i] = bVar4
   
      bVar4 = 0;
      iVar6 = 0;
      do {
        bVar1 = *pbVar8;
        iVar7 = iVar6 + 2;
        pbVar8 = pbVar8 + 1;
        bVar4 = bVar4 | (bVar1 >> ((byte)param_3 & 0x1f) & 3) << ((byte)iVar6 & 0x1f);
        iVar6 = iVar7;
      } while (iVar7 < 8);
      *pbVar3 = bVar4;
      iVar2 = iVar2 + -1;
      pbVar3 = pbVar3 + 1;
    }
    param_3 = param_3 + -2;
    iVar2 = iVar5;
    pbVar8 = param_1;
  } while (-1 < param_3);
  hg3_unk_copy_memstr_FUN_00409980(param_1,param_2,pbVar3 + -param_2);
  return pbVar3 + -param_2;
}





void __thiscall hg3_unk_encodefunc_FUN_0041a180(int param_1,int param_1_00)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = 0;
  iVar1 = 2;
  iVar2 = iVar1;
  if (1 < param_1_00) {
    do {
      iVar1 = *(int *)(param_1 + 0x60);
      iVar3 = iVar1 + 1;
      *(int *)(param_1 + 0x60) = iVar3;
      *(byte *)(param_1 + 100) = *(byte *)(param_1 + 100) | '\0' << ((byte)iVar1 & 0x1f);
      if (7 < iVar3) {
        **(undefined **)(param_1 + 0x5c) = *(undefined *)(param_1 + 100);
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        *(undefined4 *)(param_1 + 0x60) = 0;
        *(undefined *)(param_1 + 100) = 0;
      }
      iVar1 = iVar2 * 2;
      iVar4 = iVar4 + 1;
      iVar3 = iVar2 * -2;
      iVar2 = iVar1;
    } while (SBORROW4(param_1_00,iVar1) == param_1_00 + iVar3 < 0);
  }
  iVar2 = *(int *)(param_1 + 0x60);
  iVar3 = iVar2 + 1;
  *(int *)(param_1 + 0x60) = iVar3;
  *(byte *)(param_1 + 100) = *(byte *)(param_1 + 100) | '\x01' << ((byte)iVar2 & 0x1f);
  if (7 < iVar3) {
    **(undefined **)(param_1 + 0x5c) = *(undefined *)(param_1 + 100);
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    *(undefined4 *)(param_1 + 0x60) = 0;
    *(undefined *)(param_1 + 100) = 0;
  }
  if (iVar4 != 0) {
    do {
      iVar2 = *(int *)(param_1 + 0x60);
      iVar4 = iVar4 + -1;
      iVar3 = iVar2 + 1;
      *(int *)(param_1 + 0x60) = iVar3;
      *(byte *)(param_1 + 100) =
           *(byte *)(param_1 + 100) |
           ((byte)(param_1_00 - iVar1 / 2 >> ((byte)iVar4 & 0x1f)) & 1) << ((byte)iVar2 & 0x1f);
      if (7 < iVar3) {
        **(undefined **)(param_1 + 0x5c) = *(undefined *)(param_1 + 100);
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        *(undefined4 *)(param_1 + 0x60) = 0;
        *(undefined *)(param_1 + 100) = 0;
      }
    } while (iVar4 != 0);
  }
  return;
}



