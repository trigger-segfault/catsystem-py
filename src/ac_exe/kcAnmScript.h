#pragma once

#ifndef KCLIB_KCANMSCRIPT_CLASS_H
#define KCLIB_KCANMSCRIPT_CLASS_H

#include "common.h"

#ifdef KCLIB_OOP
#include "ScriptReader.h"
#include "TokenParser.h"
#endif

#ifdef KCLIB_OOP
namespace kclib
{
#endif
    // "ANM\0" (little endian)
    #define MAGIC_ANM 0x4d4e41

    enum ANM_CMD
    {
        ANM_CMD_ID    = 0, // [ID] [min] (max)
        ANM_CMD_SET   = 1, // set [var] [min] (max)
        ANM_CMD_LOOP  = 2, // loop [var] [label]
        ANM_CMD_JUMP  = 3, // jump [label]
        ANM_CMD_IF    = 4, // if [var] [label]
        ANM_CMD_IFE   = 5, // ife [var] [value] [label]
        ANM_CMD_IFN   = 6, // ifn [var] [value] [label]
        ANM_CMD_IFG   = 7, // ifg [var] [value] [label]
        ANM_CMD_IFS   = 8, // ifs [var] [value] [label]
        ANM_CMD_IFGE  = 9, // ifge [var] [value] [label]
        ANM_CMD_IFSE  = 10, // ifse [var] [value] [label]
        ANM_CMD_MAX   = 11, // max [var]
        ANM_CMD_BLEND = 12, // blend [param]
        ANM_CMD_DISP  = 13, // disp [param]
        ANM_CMD_POS   = 14, // pos [x] [y]
        ANM_CMD_WAIT  = 15, // wait [min] (max)
        ANM_CMD_ADD   = 16, // add [var] [value]
        ANM_CMD_SUB   = 17,  // sub [var] [value]

        ANM_CMD_END  // identifies end of TABLE_ANM_COMMANDS (not part of ac.exe or Cs2)
    };

    enum ANM_VAR_TYPE
    {
        ANM_TYPE_CONST    = 0, // 16
        ANM_TYPE_VARIABLE = 1, // @16
        ANM_TYPE_LABEL    = 2  // MyLabel
    };


    #pragma pack(push, 1)


    struct ANM_LABEL
    {
        /*$0,20*/  char LabelName[0x20]; // identifier name
        /*$20,4*/  unsigned int LabelAddress; // line number
        /*$24,4*/  unsigned int LabelIndex; // index of label in the label table (heh)
        /*$24*/
    };

    struct ANM_ARG
    {
        /*$0,4*/   ANM_VAR_TYPE VarType;
        /*$4,4*/   unsigned int Value;
        /*$8*/
    };

    static_assert(sizeof(ANM_ARG) == 0x8);

    struct ANM_TIMELINE
    {
        /*$0,4*/   ANM_CMD CmdType;
        /*$4,40*/  ANM_ARG Args[8];
        /*$44*/
    };

    static_assert(sizeof(ANM_TIMELINE) == 0x44);

    struct ANM_UNK72  // likely STL class
    {
        // /*$0,18*/  char UnkName[24]; // 0x0:0x16?
        /*$0,18*/  unsigned int padding[6];
        /*$18,1c*/ int **Field72Unk6ptr;
        /*$1c,20*/ int Field72Unk7;
        /*$20*/
    };

    struct ANM_SCRIPT
    {
        /*$0,4*/   unsigned int TimelineCount; // Number of timelines
        // /*$4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
        /*$4,4*/   ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
        /*$8,4*/   unsigned int Unk2; // (unknown)
        /*$c,4*/   unsigned int Unk3; // (unknown)
        /*$10,4*/  unsigned int Counter; // used in ANM_CMD_ID, ANM_CMD_WAIT, and during command loop
        /*$14,4*/  int Instruction; // instruction ptr index
        /*$18,4*/  unsigned int Wait; // used in ANM_CMD_ID, ANM_CMD_WAIT
        /*$1c,4*/  unsigned int MaxFrame; // used in ANM_CMD_MAX
        /*$20,ff*/ int Variables[64]; // live variables
        /*$120,4*/ ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
        /*$124,4*/ float Unk73; // (unknown)
        /*$128,4*/ int FrameID; // current frame
        /*$12c,4*/ int Blend; // used in ANM_CMD_BLEND
        /*$130,4*/ BOOL Disp; // used in ANM_CMD_DISP
        /*$134,4*/ int PosX; // used in ANM_CMD_POS
        /*$138,4*/ int PosY; // used in ANM_CMD_POS
        /*$140*/
    };

    struct ANM_FILEHEADER
    {
        /*$0,4*/   unsigned int Magic; // "ANM\0" MAGIC_ANM (little endian)
        /*$4,4*/   unsigned int AnmUnk1;
        /*$8,4*/   unsigned int TimelineCount;
        /*$c,4*/   unsigned int AnmUnk3;
        /*$10,4*/  unsigned int AnmUnk4;
        /*$14,4*/  unsigned int AnmUnk5;
        /*$18,4*/  unsigned int AnmUnk6;
        /*$1c,4*/  unsigned int AnmUnk7;
        /*$20*/
    };

    struct ANM_FILEINFO
    {
        /*$0,20*/  ANM_FILEHEADER Header;
        /*$20,*/   ANM_TIMELINE Timelines[1]; // Treat as start address, variable-length
    };


    #ifdef KCLIB_OOP
    
    class kcAnmScript
    {
    private:
        /*$0,4*/    unsigned int TimelineCount; // Number of timelines
        // /*$4,4*/   std::shared_ptr<ANM_TIMELINE> Timelines; // Pointer timelines
        /*$4,4*/    ANM_TIMELINE *Timelines; // Pointer timelines, this field is created with a reference counter at address -4, with the function HeapAlloc
        /*$8,4*/    unsigned int Unk2; // (unknown)
        /*c,4*/     unsigned int Unk3; // (unknown)
        /*$10,4*/   unsigned int Counter; // used in CMD_ID, CMD_WAIT, and during command loop
        /*$14,4*/   int Instruction; // instruction ptr index
        /*$18,4*/   unsigned int Wait; // used in CMD_ID, CMD_WAIT
        /*$1c,4*/   unsigned int MaxFrame; // used in CMD_MAX
        /*$20,100*/ int Variables[64]; // live variables
        /*$120,4*/  ANM_UNK72 *Unk72stl; // (ptr to unknown stl structure, probably)
        /*$124,4*/  float Unk73; // (unknown)
        /*$128,4*/  int FrameID; // current frame
        /*$12c,4*/  int Blend; // used in CMD_BLEND
        /*$130,4*/  BOOL Disp; // used in CMD_DISP
        /*$134,4*/  int PosX; // used in CMD_POS
        /*$138,4*/  int PosY; // used in CMD_POS

        

    public:
        //kcAnmScript_new [005986f0]
        kcAnmScript();
        //kcAnmScript_dtor_sub [00598540]
        ~kcAnmScript();

        bool OpenFile(IN const char *filename);
        bool OpenFile(IN const wchar_t *filename);

        // naming conventions are pretty bad here...
        //  ReadFile and WriteFile are not equivalents

        //kcAnmScript_readFile [005985e0]
        bool ReadFile(IN const unsigned char *fileData, unsigned int fileSize);

        bool OpenScript(IN const char *filename, OPTIONAL OUT unsigned int *outErrorCount = nullptr);
        bool OpenScript(IN const wchar_t *filename, OPTIONAL OUT unsigned int *outErrorCount = nullptr);

        bool ReadScript(IN const char *str, OPTIONAL OUT unsigned int *outErrorCount = nullptr);
        
        bool ReadScript(ScriptReader *reader, OPTIONAL OUT unsigned int *outErrorCount = nullptr);

        bool WriteFile(IN const char *filename);
        bool WriteFile(IN const wchar_t *filename);
        bool WriteScript(IN const char *filename);
        bool WriteScript(IN const wchar_t *filename);

        void Close();

        bool IsOpen() const;

        //kcAnmScript_reset [00598500]
        bool ResetScript();

        void PrintTimeline(IN const ANM_TIMELINE *anmline) const;
        void PrintTimeline(FILE *file, IN const ANM_TIMELINE *anmline) const;

        // custom method to print the currently read script to the console
        // including all commands
        void PrintScript() const;
        // custom method to print the currently read script to the console
        // including all commands
        void PrintScript(FILE *file) const;

        // Evaluate the value of an argument.
        int EvaluateArg(ANM_ARG arg);

        //kcAnmScript_execute [00597cf0]
        unsigned int RunScript();
        
private:
        std::vector<kclib::ANM_LABEL> ac_LabelNames; // sizeof() == 0x1c
        std::vector<kclib::ANM_TIMELINE> ac_AnmLines; // sizeof() == 0x1c


        bool ac_ParseAnmArg(TokenParser *decoder, OUT ANM_ARG *arg, unsigned int lineNumber);

        bool ac_ParseScript(ScriptReader *reader, OPTIONAL OUT unsigned int *outErrorCount = nullptr);
    };

    #endif

    // custom enum for ANM_COMMAND_ENTRY
    //  not part of ac.exe or Cs2
    enum ANM_CMD_FLAGS
    {
        ANM_FLAG_NONE = 0, // normal command, fixed argument count

        ANM_FLAG_RANGE = 0x1, // last argument is optional range [min] (max)
        ANM_FLAG_LABEL = 0x2, // last argument is label [label]
    };

    // custom table entry in custom TABLE_ANM_COMMANDS
    //  not part of ac.exe or Cs2
    struct ANM_COMMAND_ENTRY
    {
        /*$0,4*/    ANM_CMD ID;
        /*$4,10*/   char Name[0x10];
        /*$14,4*/   unsigned int ArgCount;
        /*$18,4*/   ANM_CMD_FLAGS Flags; // flags about the command
        /*$20,80*/  char Usage[0x80];
        /*$a0*/
    };

    #pragma pack(pop)

    
    // custom table of anm command entries,
    //  used for visual representation of binary timelines
    //  not part of ac.exe or Cs2
    extern ANM_COMMAND_ENTRY TABLE_ANM_COMMANDS[19];

    // custom table for visual representation of binary commands.
    //  not part of ac.exe or Cs2
    // extern const char *TABLE_ANM_COMMAND_NAMES[19];
    

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
