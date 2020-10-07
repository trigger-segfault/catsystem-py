#pragma once

#ifndef KCLIB_KCANMSCRIPT_CLASS_H
#define KCLIB_KCANMSCRIPT_CLASS_H

#include "common.h"


// "ANM\0" (little endian)
#define MAGIC_ANM 0x4d4e41


typedef enum kclib_ANM_CMD
{
    CMD_ID    = 0, // [ID] [min] (max)
    CMD_SET   = 1, // set [var] [min] (max)
    CMD_LOOP  = 2, // loop [var] [label]
    CMD_JUMP  = 3, // jump [label]
    CMD_IF    = 4, // if [var] [label]
    CMD_IFE   = 5, // ife [var] [value] [label]
    CMD_IFN   = 6, // ifn [var] [value] [label]
    CMD_IFG   = 7, // ifg [var] [value] [label]
    CMD_IFS   = 8, // ifs [var] [value] [label]
    CMD_IFGE  = 9, // ifge [var] [value] [label]
    CMD_IFSE  = 10, // ifse [var] [value] [label]
    CMD_MAX   = 11, // max [var]
    CMD_BLEND = 12, // blend [param]
    CMD_DISP  = 13, // disp [param]
    CMD_POS   = 14, // pos [x] [y]
    CMD_WAIT  = 15, // wait [min] (max)
    CMD_ADD   = 16, // add [var] [value]
    CMD_SUB   = 17  // sub [var] [value]
} ANM_CMD;

typedef enum kclib_ANM_VAR_TYPE
{
    TYPE_CONST    = 0, // 16
    TYPE_VARIABLE = 1, // @16
    TYPE_LABEL    = 2  // MyLabel
} VAR_TYPE;


#pragma pack(push, 1)

typedef struct kclib_ANM_ARG
{
    /*$0,4*/   VAR_TYPE VarType;
    /*$4,4*/   unsigned int Value;
    /*$8*/
} ANM_ARG;

typedef struct kclib_ANM_TIMELINE
{
    /*$0,4*/   ANM_CMD CmdType;
    /*$4,40*/  ANM_ARG Args[8];
    /*$44*/
} ANM_TIMELINE;

typedef struct kclib_unk_ANM_UNK72  // likely STL class
{
    // /*$0,18*/  char UnkName[24]; // 0x0:0x16?
    /*$0,18*/  unsigned int padding[6];
    /*$18,1c*/ int **Field72Unk6ptr;
    /*$1c,20*/ int Field72Unk7;
    /*$20*/
} ANM_UNK72;

typedef struct kclib_ANM_SCRIPT
{
    /*$0,4*/   unsigned int TimelineCount; // Number of timelines
    // /*$4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
    /*$4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
    /*$8,4*/   unsigned int Unk2; // (unknown)
    /*$c,4*/   unsigned int Unk3; // (unknown)
    /*$10,4*/  unsigned int Counter; // used in CMD_ID, CMD_WAIT, and during command loop
    /*$14,4*/  int Instruction; // instruction ptr index
    /*$18,4*/  unsigned int Wait; // used in CMD_ID, CMD_WAIT
    /*$1c,4*/  unsigned int MaxFrame; // used in CMD_MAX
    /*$20,ff*/ int Variables[64]; // live variables
    /*$120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
    /*$124,4*/ float Unk73; // (unknown)
    /*$128,4*/ int FrameID; // current frame
    /*$12c,4*/ int Blend; // used in CMD_BLEND
    /*$130,4*/ BOOL Disp; // used in CMD_DISP
    /*$134,4*/ int PosX; // used in CMD_POS
    /*$138,4*/ int PosY; // used in CMD_POS
    /*$140*/
} ANM_SCRIPT;

typedef struct kclib_ANM_FILEHEADER
{
    /*$0,4*/   unsigned int Magic; // "ANM\0" MAGIC_ANM
    /*$4,4*/   unsigned int AnmUnk1;
    /*$8,4*/   unsigned int TimelineCount;
    /*$c,4*/   unsigned int AnmUnk3;
    /*$10,4*/  unsigned int AnmUnk4;
    /*$14,4*/  unsigned int AnmUnk5;
    /*$18,4*/  unsigned int AnmUnk6;
    /*$1c,4*/  unsigned int AnmUnk7;
    /*$20*/
} ANM_FILEHEADER;

typedef struct kclib_ANM_FILEINFO
{
    /*$0,20*/  ANM_FILEHEADER Header;
    /*$20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
} ANM_FILEINFO;

#pragma pack(pop)


#endif /* end include guard */
