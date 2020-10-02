#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    /*8,2*/  short BPP;
    /*A,2*/  short Depth;
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

int main(void) {
    HG2METADATA img;
    int *local_60;
    int iVar7;
    
    memset(&img, '\0', sizeof(HG2METADATA));
    img.Width = 1280;
    img.Height = 1024;
    img.BPP = 32;
    img.Depth = 0xffff;
    img.Slice.SliceIndex = 0;
    img.Slice.SliceLength = 0;
    img.Slice.DataPacked = 0x1900;
    img.Slice.DataUnpacked = 0x10000;
    img.Slice.CtlPacked = 0x96;
    img.Slice.CtlUnpacked = 0x280;
    img.OffsetToData = 48;
    img.ID = 101;
    img.CanvasWidth = 1280;
    img.CanvasHeight = 1024;
    img.OffsetX = 0;
    img.OffsetY = 0;
    img.HasTransparency = 0;
    img.OffsetToNext = 0x4c + img.Slice.DataPacked + img.Slice.CtlPacked;
    img.OriginX = 0;
    img.OriginY = 0;

    local_60 = (int *) &img;
    
    iVar7 = (int)(
        (int)*(short *)(local_60 + 2)
    +
        ((int)*(short *)(local_60 + 2) >> 0x1f & 7U)
    ) >> 3;
    printf("BPP = %ih, Depth = %ih\n", img.BPP, img.Depth);
    printf("iVar7 = %i\n", iVar7);

    return 0;
}