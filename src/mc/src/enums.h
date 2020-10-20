#pragma once

namespace mc {

	enum CharacterFlag
	{
		CF_FirstSjisByte = 0x40000000,
		CF_SecondSjisByte = 0x80000000,
	};

	enum LineType
	{
		LT_Invalid = -1,
		LT_None = 0x0,
		LT_Input = 0x2,
		LT_Page = 0x3,
		LT_Message = 0x20,
		LT_Name = 0x21,
		LT_Command = 0x30,
		LT_ScriptName = 0xF0,
		LT_LineNo = 0xF1,
	};

	enum BlockType
	{
		BT_None = 0,
		BT_Input = 1,
		BT_PageBreak = 2,
	};

	enum ErrorCode
	{
		EC_None = -1,
		EC_DeepNesting = 1,
		EC_EndOfFile = 2,
	};
}
