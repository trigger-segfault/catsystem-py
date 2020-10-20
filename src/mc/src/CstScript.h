#pragma once
#include "LabelInfo.h"
#include "MacroInfo.h"
#include "SourceFile.h"
#include "enums.h"

namespace mc {

	struct ScriptBlock
	{
		int Length;
		int Start;
	};

	// sizeof(CstScript) should be 001C0CE0h
	class CstScript sealed
	{
	public:
		char CurrentLabelName[1024];
		int CurrentBlockLength;
		int field_404;
		int LineTablePtr;
		int BlockTablePtr;
		int LineTable[65536];
		ScriptBlock BlockTable[65536];
		char LineData[65536 * 16];
		int LineDataPtr;
		int IncludeLineNumbers;
		BOOL OutputScriptName;
		BOOL OutputLineNo;
		int EnableCompression;
		unsigned int SourceUpdateTime;
		int GeneratedFileCount;
		LabelInfo* LabelInfo;
		char SourcePath[1024];
		SourceFile* IncludeStack[16];
		int CurrentLineStack[16];
		int IncludeStackSize;
		char CurrentLineBuffer[1024];
		char* CurrentLinePtr;
		SourceFile* SourceFile;
		int CurrentInputLine;
		int LastInputLine;
		BlockType NextLineBlockType;
		BlockType CurrentBlockType;
		LineType LastLineType;
		ErrorCode ErrorCode;
		int IsInBlockComment;
		MacroInfo* MacroInfo;
		BOOL IsInMacroDefinition;

		CstScript(const char* dstPath);
		~CstScript();

		void EndBlock();
		void MarkBlockEnd();
		BOOL PopIncludeStack();
		void SetLabelInfo(mc::LabelInfo* info);
		void SetMacroInfo(mc::MacroInfo* info);
		void AppendLine(LineType type, const char* content);
		void WriteToFile();
		void StartMacroDefinition(const char* macroName);
		BOOL ProcessInclude(const char* includeName);
		const char* AppendName(const char* name);
		const char* AppendNameSjis(const char* name);
		void ProcessLabel(const char* labelName);
		int ReadNextLines();
		BOOL CompileInternal();
		BOOL CompileFromSource(const char* srcPath);
	};
}
