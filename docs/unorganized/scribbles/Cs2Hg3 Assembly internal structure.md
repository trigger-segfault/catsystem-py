    *(int *)(this + 0x18) = byteDepth;
    iVar11 = byteDepth * width;
    *(uint *)(this + 4) = width;
    *(uint *)(this + 8) = height;
    *(int *)(this + 0x14) = iVar11;
    *(uint *)(this + 0x40) = currentId;
    *(undefined4 *)(this + 0x38) = 0;
    *(undefined4 *)(this + 0x3c) = 0;
    *(uint *)(this + 0x44) = height;
    *(uint *)(this + 0x1c) = totalWidth;
    *(uint *)(this + 0x20) = totalHeight;
    *(undefined4 *)(this + 0x24) = offsetX;
    *(undefined4 *)(this + 0x28) = offsetY;
    if (*(short *)puVar1 < 0x20) {
      *(undefined4 *)(this + 0x2c) = 0;
    }
    else {
      *(uint *)(this + 0x2c) = (uint)(isTransparent != 0) + 1;
    }
    *(undefined4 *)(this + 0x30) = baseX;
    *(undefined4 *)(this + 0x34) = baseY;
    return 1;