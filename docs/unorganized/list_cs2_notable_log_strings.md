# CatSystem Log Errors


## HG-2

> *Used as file signature/magic*<br/>
> "HG-2"

// "HG-2" - file signature
#define SIGNATURE_HG2 "HG-2"

// "HG2(%d) : No corresponding ID found"
#define LOG_HG2_NO_CORRESPONDING_ID_FOUND "HG2(%d) : 対応するIDが見つかりません"
// "HG2 : Unsupported version"
#define LOG_HG2_UNSUPPORTED_VERSION "HG2 : 未対応のバージョンです"
// "HG2 : Header is different"
#define LOG_HG2_HEADER_IS_DIFFERENT "HG2 : ヘッダが違います"

// "kcImageHg2 : Size of byte area is abnormal"
#define LOG_KCIMAGEHG2_UNEXPECTED_BYTEAREA_SIZE "kcImageHg2 : バイト領域のサイズが異常です"
// "kcImageHg2 : Bit area size is abnormal"
#define LOG_KCIMAGEHG2_UNEXPECTED_BITAREA_SIZE "kcImageHg2 : ビット領域のサイズが異常です"

// "Not compatible with this number of colors"
#define LOG_INCOMPATIBLE_NUMBER_OF_COLORS "この色数には対応していません"
// "Image type is different"
#define LOG_IMAGE_TYPE_IS_DIFFERENT "イメージのタイプが違います"

> *Used when HG-2 image ID search finds no results*<br/>
> "HG2(%d) : 対応するIDが見つかりません"
> > "HG2(%d) : No corresponding ID found"

> *Used when Header.Version != 0x10, 0x20, or 0x25*<br/>
> "HG2 : 未対応のバージョンです"
> > "HG2 : Unsupported version"

> *Used when file signature does not match*<br/>
> "HG2 : ヘッダが違います"
> > "HG2 : Header is different"

> *Used when ZRLE data bytes buffer unpacked size is invalid*<br/>
> "kcImageHg2 : バイト領域のサイズが異常です"
> > "kcImageHg2 : Size of byte area is abnormal"

> *Used when ZRLE cmd bits buffer unpacked size is invalid*<br/>
> "kcImageHg2 : ビット領域のサイズが異常です"
> > "kcImageHg2 : Bit area size is abnormal"

> *Used when ...*<br/>
> "この色数には対応していません"
> > "Not compatible with this number of colors"

> *Used when ...*<br/>
> "イメージのタイプが違います"<br/>
> > "Image type is different"


```c

typedef int bool32;
typedef unsigned int undefined4;

#define MAGIC_HG2 0x322D4748U
#define MAGIC_HG3 0x332d4748U

// Header of both HG-2 and HG-3 images.
typedef struct {
    /*0,4*/  unsigned int Magic;
    /*4,4*/  unsigned int HeaderSize;
    /*8,2*/  short Version; // compared as short in HG-2
    /*A,2*/  short x_padding;
    /*C*/
} HGHEADER;



typedef struct {
    /*0,4*/  int SliceIndex;
    /*4,4*/  int SliceLength;
    /*8,4*/  int DataPacked;
    /*C,4*/  int DataUnpacked;
    /*10,4*/ int CtlPacked;
    /*14,4*/ int CtlUnpacked;
    /*18*/
} HGSLICE;

typedef struct {
    // Version 0x10:
    /*0,4*/  int Width;
    /*4,4*/  int Height;
    /*8,4*/  int BPP;
    /*C,18*/ HGSLICE Slice;
    // Version 0x20:
    /*24,4*/ int OffsetToData;
    /*28,4*/ int ID;
    /*2C,4*/ int CanvasWidth;
    /*30,4*/ int CanvasHeight;
    /*34,4*/ int OffsetX;
    /*38,4*/ int OffsetY;
    /*3C,4*/ int HasTransparency; //bool32
    /*40,4*/ int OffsetToNext;
    // Version 0x25:
    /*44,4*/ int OriginX;
    /*48,4*/ int OriginY;
    /*4C*/
} HG2METADATA;

typedef struct {
    /*0,4*/  unsigned int Magic;
    /*4,4*/  unsigned int HeaderSize;
    /*8,2*/  short Version; // compared as short in HG-2
    /*A,2*/  short x_padding;
    /*C,4C*/ HG2METADATA ImageAddr[1];
} HG2FILEINFO;

typedef struct {
    HG2FILEINFO *FileInfo;

} kcImageHg2;

//hg2_unkfunc_2_004e7710
//kcImageHg2_ReadImageID

unsigned int kcImageHg2_ReadImageAt(kcImageHg2 *this, unsigned int index, unsigned int *param_3)

{
    //undefined4 uVar1;
    //int iVar2;
    //unsigned int uVar3;
    //unsigned int result; //undefined4 uVar1;
    HG2METADATA *img; //int iVar2;
    unsigned int i; //unsigned int uVar3;
    

    if (this->FileInfo->Version < 0x25)
    {
        if (index == 0)
        {
            *param_3 = 0;
            return kcImageHg2_ReadImageID(this, 0);
        }
    }
    else
    {
        // Find ID of image at index
        img = this->FileInfo->ImageAddr;
        for (i = 0; i < index && img->OffsetToNext; i++)
        {
            img = (HG2METADATA *)((int)img + img->OffsetToNext);
        }
        if (i == index)
        {
            *param_3 = img->ID;
            return kcImageHg2_ReadImageID(this, img->ID);
        }
    }
    return 0;
}



undefined4 __thiscall hg2_unkfunc_2_004e7710(int param_1,int param_1_00)

{
  int *piVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  byte bVar5;
  int in_EAX;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  byte *pbVar13;
  char *pcVar14;
  uint uVar15;
  byte bVar16;
  int iVar17;
  uint *puVar18;
  char *pcVar19;
  int *piVar20;
  uint *puVar21;
  int *piVar22;
  char *pcVar23;
  uint uVar24;
  int *local_60;
  byte *local_5c;
  int local_54;
  byte *local_4c;
  int local_40;
  int local_30 [4];
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  undefined4 local_10;
  undefined4 local_c;
  
  iVar6 = unk_strcmp();
  if (iVar6 != 0) {
    cs2_log_sendmessage_004b57b0(s_HG2_:_00628b64);
    return 0;
  }
  sVar3 = *(short *)(in_EAX + 8);
  if (sVar3 == 0x10) {
    local_30[2] = *(undefined4 *)(in_EAX + 0xc);
    local_30[3] = *(undefined4 *)(in_EAX + 0x10);
    local_60 = (int *)(in_EAX + 0xc);
    local_20 = 0;
    local_1c = 0;
    local_18 = 0;
    local_30[0] = in_EAX + 0x30;
  }
  else {
    if (sVar3 != 0x20) {
      if (sVar3 != 0x25) {
        cs2_log_sendmessage_004b57b0(s_HG2_:_00628b44);
        return 0;
      }
      local_60 = (int *)(in_EAX + 0xc);
      piVar1 = (int *)(in_EAX + 0x30);
      iVar6 = *(int *)(in_EAX + 0x34);
      while (iVar6 != param_1_00) {
        if (piVar1[7] == 0) goto LAB_004e7809;
        local_60 = (int *)((int)local_60 + piVar1[7]);
        iVar6 = local_60[10];
        piVar1 = local_60 + 9;
      }
      iVar6 = 10;
      piVar20 = piVar1;
      piVar22 = local_30;
      while (iVar6 != 0) {
        iVar6 += -1;
        *piVar22 = *piVar20;
        piVar20 = piVar20 + 1;
        piVar22 = piVar22 + 1;
      }
      local_30[0] += (int)piVar1;
      goto LAB_004e7854;
    }
    local_60 = (int *)(in_EAX + 0xc);
    piVar1 = (int *)(in_EAX + 0x30);
    iVar6 = *(int *)(in_EAX + 0x34);
    while (iVar6 != param_1_00) {
      if (piVar1[7] == 0) {
LAB_004e7809:
        cs2_log_sendmessage_004b57b0(s_HG2(%d)_:_ID_00628b1c,param_1_00);
        return 0;
      }
      local_60 = (int *)((int)local_60 + piVar1[7]);
      iVar6 = local_60[10];
      piVar1 = local_60 + 9;
    }
    iVar6 = 8;
    piVar20 = piVar1;
    piVar22 = local_30;
    while (iVar6 != 0) {
      iVar6 += -1;
      *piVar22 = *piVar20;
      piVar20 = piVar20 + 1;
      piVar22 = piVar22 + 1;
    }
    local_30[0] += (int)piVar1;
  }
  local_c = 0;
  local_10 = 0;
LAB_004e7854:
  iVar6 = *local_60;
  iVar4 = local_60[1];
  iVar7 = (int)((int)*(short *)(local_60 + 2) + ((int)*(short *)(local_60 + 2) >> 0x1f & 7U)) >> 3;
  iVar8 = iVar7 * iVar4 * iVar6;
  iVar17 = local_60[6] + 0x10;
  iVar9 = cs2_HeapAlloc_logged_004b02b0(0,local_60[6] + 0x2010 + iVar8 * 2);
  puVar21 = (uint *)(iVar9 + iVar17);
  local_4c = (byte *)(iVar9 + 0x1000 + iVar8 + iVar17);
  iVar10 = FUN_005c3260(iVar9,local_60[6] + 0x10,local_30[0],local_60[5]);
  iVar17 = local_60[5];
  if (iVar10 != local_60[6]) {
    cs2_log_sendmessage_004b57b0(s_kcImageHg2_:_00628b7c);
    if (iVar9 != 0) {
      if (1 < *(int *)(iVar9 + -4)) {
        *(int *)(iVar9 + -4) = *(int *)(iVar9 + -4) + -1;
        return 0;
      }
      HeapFree(DAT_007a0c44,0,(int *)(iVar9 + -4));
    }
    return 0;
  }
  FUN_004e94c0(param_1);
  iVar17 = FUN_005c3260(*(undefined4 *)(param_1 + 0x58),local_60[8] + 0x10,local_30[0] + iVar17,
                        local_60[7]);
  if (iVar17 != local_60[8]) {
    cs2_HeapFree_004b0300(iVar9);
    FUN_004e82b0();
    cs2_log_sendmessage_004b57b0(s_kcImageHg2_:_00628ba8);
    return 0;
  }
  iVar11 = FUN_004e8520();
  iVar10 = FUN_004e83b0();
  std_vectorfunc_unk_004b17a0(local_4c,iVar10);
  local_5c = local_4c;
  iVar17 = iVar9;
  if (iVar11 != 0) {
    iVar11 = FUN_004e83b0();
    iVar10 -= iVar11;
    cs2_strmemcpy_004b1600(local_4c,iVar9,iVar11);
    iVar17 = iVar11 + iVar9;
    local_5c = local_4c + iVar11;
  }
  while (0 < iVar10) {
    iVar11 = FUN_004e83b0();
    local_5c = local_5c + iVar11;
    if (iVar10 - iVar11 < 1) break;
    iVar12 = FUN_004e83b0();
    iVar10 = (iVar10 - iVar11) - iVar12;
    cs2_strmemcpy_004b1600(local_5c,iVar17,iVar12);
    local_5c = local_5c + iVar12;
    iVar17 = iVar17 + iVar12;
  }
  if (((uint)(local_5c + -(int)local_4c) & 3) == 0) {
    iVar17 = (int)(local_5c + -(int)local_4c) >> 2;
    if (0 < iVar17) {
      pbVar13 = local_4c + iVar17 * 2;
      puVar18 = puVar21;
      local_54 = iVar17;
      do {
        *puVar18 = *(uint *)(param_1 + 0xd74 + (uint)pbVar13[iVar17] * 4) |
                   *(uint *)(param_1 + 0x574 + (uint)pbVar13[-iVar17] * 4) |
                   *(uint *)(param_1 + 0x174 + (uint)*local_4c * 4) |
                   *(uint *)(param_1 + 0x974 + (uint)*pbVar13 * 4);
        local_4c = local_4c + 1;
        pbVar13 = pbVar13 + 1;
        local_54 += -1;
        puVar18 = puVar18 + 1;
      } while (local_54 != 0);
    }
    FUN_004db480(iVar8);
    *(int *)(param_1 + 0x14) = iVar7 * iVar6;
    *(int *)(param_1 + 4) = iVar6;
    *(int *)(param_1 + 8) = iVar4;
    *(int *)(param_1 + 0x18) = iVar7;
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(int *)(param_1 + 0x40) = iVar6;
    *(int *)(param_1 + 0x44) = iVar4;
    iVar17 = *(int *)(param_1 + 0x14);
    pcVar14 = (char *)((*(int *)(param_1 + 8) + -1) * iVar17 + *(int *)(param_1 + 0xc));
    iVar8 = iVar7;
    pcVar23 = pcVar14;
    while (iVar8 != 0) {
      iVar8 += -1;
      *pcVar23 = *(char *)((uint)*(byte *)puVar21 + 0x74 + param_1);
      pcVar23 = pcVar23 + 1;
      puVar21 = (uint *)((int)puVar21 + 1);
    }
    local_5c = (byte *)((iVar6 + -1) * iVar7);
    if (local_5c != (byte *)0x0) {
      pcVar19 = pcVar23 + -iVar7;
      do {
        local_5c = (byte *)((int)local_5c + -1);
        *pcVar23 = *(char *)((uint)*(byte *)puVar21 + 0x74 + param_1) + *pcVar19;
        pcVar23 = pcVar23 + 1;
        pcVar19 = pcVar19 + 1;
        puVar21 = (uint *)((int)puVar21 + 1);
      } while (local_5c != (byte *)0x0);
    }
    pcVar14 = pcVar14 + -iVar17;
    if (1 < iVar4) {
      local_40 = iVar4 + -1;
      do {
        pcVar23 = pcVar14 + iVar17;
        iVar8 = iVar7 * iVar6;
        pcVar19 = pcVar14;
        while (iVar8 != 0) {
          cVar2 = *pcVar23;
          pcVar23 = pcVar23 + 1;
          iVar8 += -1;
          *pcVar19 = *(char *)((uint)*(byte *)puVar21 + 0x74 + param_1) + cVar2;
          pcVar19 = pcVar19 + 1;
          puVar21 = (uint *)((int)puVar21 + 1);
        }
        pcVar14 = pcVar14 + -iVar17;
        local_40 += -1;
      } while (local_40 != 0);
    }
    iVar6 = *(int *)(param_1 + 0x58);
    if (iVar6 != 0) {
      if (*(int *)(iVar6 + -4) < 2) {
        HeapFree(DAT_007a0c44,0,(int *)(iVar6 + -4));
      }
      else {
        *(int *)(iVar6 + -4) = *(int *)(iVar6 + -4) + -1;
      }
    }
    *(undefined4 *)(param_1 + 0x58) = 0;
    if (iVar9 != 0) {
      if (*(int *)(iVar9 + -4) < 2) {
        HeapFree(DAT_007a0c44,0,(int *)(iVar9 + -4));
      }
      else {
        *(int *)(iVar9 + -4) = *(int *)(iVar9 + -4) + -1;
      }
    }
    if (*(short *)((int)local_60 + 10) != 0) {
      puVar21 = *(uint **)(param_1 + 0xc);
      cVar2 = (char)*(undefined2 *)((int)local_60 + 10);
      bVar5 = 8 - cVar2;
      bVar16 = cVar2 - bVar5;
      uVar15 = (0xff << (bVar5 & 0x1f) & 0xffU) * 0x1010101;
      local_4c = (byte *)iVar4;
      if (0 < iVar4) {
        do {
          uVar24 = *(uint *)(param_1 + 0x14) >> 2;
          puVar18 = puVar21;
          while (uVar24 != 0) {
            uVar24 -= 1;
            *puVar18 = *puVar18 >> (bVar16 & 0x1f) & ~uVar15 | *puVar18 << (bVar5 & 0x1f) & uVar15;
            puVar18 = puVar18 + 1;
          }
          uVar24 = *(uint *)(param_1 + 0x14) & 3;
          while (uVar24 != 0) {
            uVar24 -= 1;
            *(byte *)puVar18 =
                 *(byte *)puVar18 >> (bVar16 & 0x1f) | *(byte *)puVar18 << (bVar5 & 0x1f);
            puVar18 = (uint *)((int)puVar18 + 1);
          }
          puVar21 = (uint *)((int)puVar21 + *(int *)(param_1 + 0x14));
          local_4c = (byte *)((int)local_4c + -1);
        } while (local_4c != (byte *)0x0);
      }
    }
    *(undefined4 *)(param_1 + 0x1c) = local_30[2];
    *(undefined4 *)(param_1 + 0x20) = local_30[3];
    *(undefined4 *)(param_1 + 0x24) = local_20;
    *(undefined4 *)(param_1 + 0x28) = local_1c;
    if (*(short *)(in_EAX + 8) < 0x20) {
      *(undefined4 *)(param_1 + 0x2c) = 0;
    }
    else {
      *(int *)(param_1 + 0x2c) = (uint)(local_18 != 0) + 1;
    }
    *(undefined4 *)(param_1 + 0x30) = local_10;
    *(undefined4 *)(param_1 + 0x34) = local_c;
    return 1;
  }
  cs2_HeapFree_004b0300(iVar9);
  FUN_004e82b0();
  return 0;
}



//undefined4 kcImageHg2_virtfunc_2(int *param_1, unsigned int param_2, undefined4 *param_3)
//   for (i = 0; i <= index; i++) {
//       *param_3 = img->ID;
//       if (img->OffsetToNext == 0) {
//         if (i != index)
//           return 0;
//         break;
//       }
//       img = (HG2METADATA *)((int)img + img->OffsetToNext);
//   }
//   return hg2_unkfunc_2_004e7710(*param_3);
//   i = 0;
//   do {
//     //*param_3 = *(undefined4 *)(iVar2 + 0x28);
//     //if (*(int *)(iVar2 + 0x40) == 0) {
//     *param_3 = img->ID;
//     if (img->OffsetToNext == 0) {
//       // this is the last image
//       //if (uVar3 != param_2) {
//       if (i != index) {
//         return 0;
//       }
//       break;
//     }
//     //uVar3 += 1;
//     i++;
//     //iVar2 += *(int *)(iVar2 + 0x40);
//     img = (HG2METADATA *)((int)img + img->OffsetToNext);
//   } while (i <= index);
//   //} while (uVar3 <= param_2);
//   result = hg2_unkfunc_2_004e7710(*param_3);
//   return result;
// }

```