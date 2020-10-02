
//DAT_00c35de0  FUN_00636423(auStack528,"%s@@--cs2-debug-key--@@",DAT_00c965d4 + 2);

//uint DAT_00c35de0
0xBB40E64Eh
//uint DAT_00c35de4
0x44BF19B1h


LAB_00980c0b

uint uVarl;
int **in_FS_OFFSET;
uint[] local_280 = new uint[28];
byte[] local_210 = new byte[512];
uint local_10;
int* local_c;
void* puStack8;
uint local_4;

uint lo
uint uStack4;
uint uStack16;

const uint DAT_00c35de0 = 0xBB40E64E;
const uint DAT_00c35de4 = 0x44BF19B1;
const uint DAT_00c965d4 = 

local_4 = 0xffffffff;
puStack8 = &LAB_00980c0b;
local_c = *in_FS_OFFSET;
local_10 = DAT_00c35de0 ^ (uint)local_280;
uVarl = DAT_00c35de0 ^ (uint)&stack0xfffffd7c;
*(int ***)in_FS_OFFSET = &local_c;
FUN_004b5680(uVarl);
local_4 = 0;
FUN_00636423(local_210,"%s@@--cs2-debug-key--@@",DAT_00c965d4 + 2);
uVarl = FUN_4b5f10(local_210);
local_280[0] = uVarl ^ 0x55555555;
FUN_004b5820("cs2_debug_key.dat",uVar1,local_280,0);
local_4 = 0xffffffff;
FUN_004b56d0();
*in_FS_OFFSET = local_c;
FUN_00636414();
return;

uStack4 = 0;


auStack528