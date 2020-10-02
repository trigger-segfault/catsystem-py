
undefined4 * __fastcall hg2_encode_norm_tables(undefined4 *param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  char cVar5;
  int *piVar6;
  
  new_kcImage();
  *param_1 = 0x6138f4;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 1;
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

