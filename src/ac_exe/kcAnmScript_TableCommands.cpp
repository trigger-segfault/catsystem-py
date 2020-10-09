
// Standalone file to define extern TABLE_ANM_COMMANDS (no code)
// This is a custom table, not part of ac.exe or Cs2, its used visualize
//  and decompile binary ANM files.

#include "kcAnmScript.h"

#ifndef KCLIB_OOP
// extern defined in "TokenParser.h"
ANM_COMMAND_ENTRY TABLE_ANM_COMMANDS[19] = {
#else
using namespace kclib;

// extern defined in "TokenParser.h"
kclib::ANM_COMMAND_ENTRY kclib::TABLE_ANM_COMMANDS[19] = {
#endif
//kclib::ANM_COMMAND_ENTRY kclib::TABLE_ANM_COMMANDS[19] = {
    { assert_enum(0, ANM_CMD_ID), "", 3, ANM_FLAG_RANGE, "[ID] [min] (max)" }, //+[@ID] [min] (max)
    
    { assert_enum(1, ANM_CMD_SET), "set", 3, ANM_FLAG_RANGE, "set [var] [min] (max)" },
    { assert_enum(2, ANM_CMD_LOOP), "loop", 2, ANM_FLAG_LABEL, "loop [var] [label]" },
    { assert_enum(3, ANM_CMD_JUMP), "jump", 1, ANM_FLAG_LABEL, "jump [label]" },
    { assert_enum(4, ANM_CMD_IF), "if", 2, ANM_FLAG_LABEL, "if [var] [label]" },
    { assert_enum(5, ANM_CMD_IFE), "ife", 3, ANM_FLAG_LABEL, "ife [var] [value] [label]" },
    { assert_enum(6, ANM_CMD_IFN), "ifn", 3, ANM_FLAG_LABEL, "ifn [var] [value] [label]" },
    { assert_enum(7, ANM_CMD_IFG), "ifg", 3, ANM_FLAG_LABEL, "ifg [var] [value] [label]" },
    { assert_enum(8, ANM_CMD_IFS), "ifs", 3, ANM_FLAG_LABEL, "ifs [var] [value] [label]" },
    { assert_enum(9, ANM_CMD_IFGE), "ifge", 3, ANM_FLAG_LABEL, "ifge [var] [value] [label]" },
    { assert_enum(10, ANM_CMD_IFSE), "ifse", 3, ANM_FLAG_LABEL, "ifse [var] [value] [label]" },
    { assert_enum(11, ANM_CMD_MAX), "max", 1, ANM_FLAG_NONE, "max [var]" },
    { assert_enum(12, ANM_CMD_BLEND), "blend", 1, ANM_FLAG_NONE, "blend [value]" },
    { assert_enum(13, ANM_CMD_DISP), "disp", 1, ANM_FLAG_NONE, "disp [bool]" },
    { assert_enum(14, ANM_CMD_POS), "pos", 2, ANM_FLAG_NONE, "pos [x] [y]" },
    { assert_enum(15, ANM_CMD_WAIT), "wait", 2, ANM_FLAG_RANGE, "wait [min] (max)" },
    { assert_enum(16, ANM_CMD_ADD), "add", 2, ANM_FLAG_NONE, "add [var] [value]" },
    { assert_enum(17, ANM_CMD_SUB), "sub", 2, ANM_FLAG_NONE, "sub [var] [value]" },

    { assert_enum(18, ANM_CMD_END), "", 0, ANM_FLAG_NONE, "" }
};


// #ifndef KCLIB_OOP
// // extern defined in "TokenParser.h"
// const char * TABLE_ANM_COMMAND_NAMES[19] = {
// #else
// using namespace kclib;

// // extern defined in "TokenParser.h"
// const char * kclib::TABLE_ANM_COMMAND_NAMES[19] = {
// #endif
// //const char * kclib::TABLE_ANM_COMMAND_NAMES[19] = {
//     "[ID]",      // [ID] [min] (max)
//              // [@ID] [min] (max)
    
//     "set",   // set [var] [min] (max)
//     "loop",  // loop [var] [label]
//     "jump",  // jump [label]
//     "if",    // if [var] [label]
//     "ife",   // ife [var] [value] [label]
//     "ifn",   // ifn [var] [value] [label]
//     "ifg",   // ifg [var] [value] [label]
//     "ifs",   // ifs [var] [value] [label]
//     "ifge",  // ifge [var] [value] [label]
//     "ifse",  // ifse [var] [value] [label]
//     "max",   // max [var]
//     "blend", // blend [value]
//     "disp",  // disp [bool]
//     "pos",   // pos [x] [y]
//     "wait",  // wait [min] (max)
//     "add",   // add [var] [value]
//     "sub",   // sub [var] [value]

//     ""
// };

static_assert(TOKEN_END == 0xbd);
