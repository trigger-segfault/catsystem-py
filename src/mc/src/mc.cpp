#include <iostream>

#pragma comment(lib, "lib/zlib/zlib.lib")

#include "mc.h"
#include "enums.h"
#include "CstScript.h"
#include "SourceFile.h"
#include "LabelInfo.h"
#include "MacroInfo.h"
#include "File.h"

namespace mc {

	// global data

	// .data:0041FF94
	LabelInfo* GlobalLabelStorage;
	// .data:0041FF98
	MacroInfo* GlobalMacroStorage;
	// .data:0041FF9C
	int GlobalOpenFileCount;
	// .data:0041FFA0
	bool OnlyUpdateIfNewer = false;
	// .data:0041FFA4
	bool IncludeLineNumbers = false;
	// .data:0041FFA8
	bool FlagD = false;
	// .data:0041F088
	bool EnableCompression = true;
	// .data:0041F0A0
	const unsigned int CharacterFlags[256] = {
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00080000, 0x10000000, 0x00000000, 0x00000000, 0x00080000, 0x00080000, 0x10000000,
		0x10000000, 0x10000000, 0x00080000, 0x00080000, 0x00000000, 0x00080000, 0x00000000, 0x00080000,
		0x00050000, 0x00050001, 0x00050002, 0x00050003, 0x00050004, 0x00050005, 0x00050006, 0x00050007,
		0x00050008, 0x00050009, 0x00000000, 0x00000000, 0x10080000, 0x00080000, 0x10080000, 0x00000000,
		0x80000000, 0x80060000, 0x80060001, 0x80060002, 0x80060003, 0x80060004, 0x80060005, 0x80040006,
		0x80040007, 0x80040008, 0x80040009, 0x8004000A, 0x8004000B, 0x8004000C, 0x8004000D, 0x8004000E,
		0x8004000F, 0x80040010, 0x80040011, 0x80040012, 0x80040013, 0x80040014, 0x80040015, 0x80040016,
		0x80040017, 0x80040018, 0x80040019, 0x90000000, 0x80000000, 0x90000000, 0x80080000, 0x8004001A,
		0x80000000, 0x80060000, 0x80060001, 0x80060002, 0x80060003, 0x80060004, 0x80060005, 0x80040006,
		0x80040007, 0x80040008, 0x80040009, 0x8004000A, 0x8004000B, 0x8004000C, 0x8004000D, 0x8004000E,
		0x8004000F, 0x80040010, 0x80040011, 0x80040012, 0x80040013, 0x80040014, 0x80040015, 0x80040016,
		0x80040017, 0x80040018, 0x80040019, 0x90000000, 0x80080000, 0x90000000, 0x80080000, 0x00000000,
		0x80000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000,
		0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000,
		0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000,
		0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000,
		0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
		0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000,
	};

	// .text:004125F0
	const char* __cdecl GetFlagFromArgument(const char* arg)
	{
		if (*arg != '-' && *arg != '/')
			return nullptr;
		else
			return arg + 1;
	}

	// .text:00412620
	void __cdecl CompileScript(const char* dstPath, const char* srcPath, const char* pathBuffer, unsigned int srcUpdateTime, bool verbose)
	{
		CstScript* script; // [esp+4h] [ebp-18h]

		printf("compile : %s\n", srcPath);
		script = new CstScript(dstPath);
		script->SetMacroInfo(GlobalMacroStorage);
		script->SetLabelInfo(GlobalLabelStorage);
		script->IncludeLineNumbers = IncludeLineNumbers;
		script->EnableCompression = EnableCompression;
		script->SourceUpdateTime = srcUpdateTime;
		if (script->CompileFromSource(srcPath))
		{
			if (verbose && script->GeneratedFileCount)
				printf("  [%d] scenes update!\n", script->GeneratedFileCount);
		}
		else
		{
			printf("Warning!!!  Compile error!!!\n");
		}
		delete script;
	}

	// .text:00412730
	void __cdecl CompileFile(const char* inputPath, char* pathBuffer)
	{
		File* file; // [esp+4h] [ebp-228h] MAPDST
		char dstPath[256]; // [esp+1Ch] [ebp-210h]
		unsigned int updateTime; // [esp+11Ch] [ebp-110h]
		char srcPath[260]; // [esp+124h] [ebp-108h]

		file = new File(inputPath);
		file->ChangeExtension("txt");
		while (file->GetNextFileAttributes() >= 0)
		{
			if (file->Path[0] != '%' && file->Path[0] != '&')
			{
				updateTime = file->GetUpdateTime();
				strcpy(srcPath, file->Path);
				file->ChangeExtension("cst");
				file->GetUpdateTime();
				strcpy(dstPath, file->Path);
				if (OnlyUpdateIfNewer)
					CompileScript(dstPath, srcPath, pathBuffer, updateTime, 1);
				else
					CompileScript(dstPath, srcPath, pathBuffer, 0, 1);
			}
		}
		delete file;
	}
}

// .text:004128E0
int main(int argc, char* argv[])
{
	mc::File* macroFile; // [esp+1Ch] [ebp-160h] MAPDST
	bool foundMacroFile; // [esp+67h] [ebp-115h]
	char pathBuffer[260]; // [esp+6Ch] [ebp-110h]
	int i; // [esp+174h] [ebp-8h]
	const char* flag; // [esp+178h] [ebp-4h]

	mc::GlobalMacroStorage = new mc::MacroInfo();
	foundMacroFile = false;
	macroFile = new mc::File("&macro.txt");
	while (macroFile->GetNextFileAttributes() >= 0)
	{
		mc::CompileScript("&macro.sts", macroFile->Path, "", 0, 0);
		foundMacroFile = true;
	}
	delete macroFile;
	if (!foundMacroFile)
	{
		// Decompiler messed up here
		for (macroFile = new mc::File("%macro.txt");
			macroFile->GetNextFileAttributes() >= 0;
			mc::CompileScript("%macro.sts", macroFile->Path, "", 0, 0))
		{
			;
		}
		delete macroFile;
	}
	mc::GlobalLabelStorage = nullptr;
	if (argc == 1)
	{
		printf("Message Compiler for CatSystem2\t\tRelease 2010.2\n");
		printf("usage  : mc [/u] [file_name]\n");
		printf("option : /u\tアップデート\n");
		// GlobalLabelStorage and GlobalMacroStorage are not deleted
	}
	else
	{
		pathBuffer[0] = 0;
		mc::GlobalLabelStorage = new mc::LabelInfo();
		for (i = 1; i < argc; ++i)
		{
			flag = mc::GetFlagFromArgument(argv[i]);
			if (flag)
			{
				switch (*flag)
				{
				case 'D':
				case 'd':
					mc::FlagD = true;
					break;
				case 'L':
				case 'l':
					mc::IncludeLineNumbers = true;
					break;
				case 'U':
				case 'u':
					mc::OnlyUpdateIfNewer = true;
					break;
				case 'X':
				case 'x':
					mc::EnableCompression = false;
					break;
				default:
					continue;
				}
			}
			else
			{
				mc::CompileFile(argv[i], pathBuffer);
			}
		}
		delete mc::GlobalLabelStorage;
		delete mc::GlobalMacroStorage;
	}
	return 0;
}