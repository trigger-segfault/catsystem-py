#pragma once
#include <Windows.h>

namespace mc {

	struct MacroDefinition
	{
		int NameOffset;
		int Offset;
		int CommandCount;
	};

	class MacroInfo sealed
	{
	public:
		int Mode;
		char* MacroData;
		int BufferSize;
		int UsedSize;
		int MacroCount;
		MacroDefinition MacroDefinitions[65536];
		MacroDefinition* CurrentMacro;
		int ExpansionCurrentLine;
		int ExpansionArgumentCount;
		char* ExpansionArguments[100];
		char ExpansionArgumentData[4096];

		MacroInfo();
		~MacroInfo();

		BOOL EndMacro();
		size_t ExpandMacroLine(char* lineBuffer);
		int FindMacroIndex(const char* macroName);
		int AddString(const char* string);
		BOOL PrepareMacroExpansion(const char* macroName);
		void AppendLine(const char* line);
		BOOL CreateNewMacro(const char* macroName);
	};
}
