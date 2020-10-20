#include "CstScript.h"

#include <Windows.h>
#include <cstdio>
#include <zlib/zlib.h>

#include "File.h"
#include "mc.h"

// .text:00413F10
mc::CstScript::CstScript(const char* dstPath)
{
	static_assert(sizeof(CstScript) == 0x001C0CE0, "sizeof(CstScript) is incorrect");
	this->LabelInfo = nullptr;
	this->IncludeStackSize = 0;
	this->ErrorCode = EC_None;
	this->Compress = 1;
	this->IsInBlockComment = 0;
	this->CurrentLinePtr = nullptr;
	this->SourceFile = nullptr;
	this->CurrentInputLine = 0;
	this->MacroInfo = nullptr;
	this->IsInMacroDefinition = 0;
	this->OutputScriptName = 0;
	this->OutputLineNo = 0;
	this->FlagL = 0;
	this->SourceUpdateTime = 0;
	this->GeneratedFileCount = 0;
}

// .text:00413F00
mc::CstScript::~CstScript() = default;

// .text:00413C30
void mc::CstScript::EndBlock()
{
	BlockType type; // [esp+0h] [ebp-Ch]
	LineType lineType; // [esp+8h] [ebp-4h]

	if (this->NextLineBlockType)
	{
		lineType = LT_Invalid;
		type = this->NextLineBlockType;
		if (type == BT_Input)
		{
			lineType = LT_Input;
		}
		else if (type == BT_PageBreak)
		{
			lineType = LT_Page;
		}
		// If lineType = LT_Invalid, no block end line is emitted.
		// This is the case if this->NextLineBlockType == BT_None.
		if (lineType > 0)
		{
			this->LineTable[this->LineTablePtr++] = this->LineDataPtr;
			++this->CurrentBlockLength;
			this->LineData[this->LineDataPtr++] = 1;
			this->LineData[this->LineDataPtr++] = static_cast<char>(lineType);
			this->LineData[this->LineDataPtr++] = 0;
			this->LastLineType = lineType;
		}
	}
	this->NextLineBlockType = BT_None;
	this->CurrentBlockType = BT_None;
	if (this->LineTablePtr > 0)
	{
		this->BlockTable[this->BlockTablePtr++].Length = this->CurrentBlockLength;
		this->CurrentBlockLength = 0;
	}
}

// .text:00413DC0
void mc::CstScript::MarkBlockEnd()
{
	this->NextLineBlockType = this->CurrentBlockType;
}

// .text:00413DE0
BOOL mc::CstScript::PopIncludeStack()
{
	if (this->MacroInfo)
	{
		this->MacroInfo->EndMacro();
		this->IsInMacroDefinition = 0;
	}
	if (this->IncludeStackSize > 0)
	{
		this->SourceFile = this->IncludeStack[--this->IncludeStackSize];
		this->CurrentInputLine = this->CurrentLineStack[this->IncludeStackSize];
		this->SourceFile->Lock();
		this->LastInputLine = this->CurrentInputLine;
		return true;
	}
	else
	{
		this->CurrentLinePtr = nullptr;
		this->SourceFile = nullptr;
		this->CurrentInputLine = 0;
		return false;
	}
}

// .text:00413EC0
void mc::CstScript::SetLabelInfo(mc::LabelInfo * info)
{
	this->LabelInfo = info;
}

// .text:00413EE0
void mc::CstScript::SetMacroInfo(mc::MacroInfo * info)
{
	this->MacroInfo = info;
}

// .text:00413FF0
void mc::CstScript::AppendLine(LineType type, const char* content)
{
	char buffer[260]; // [esp+4h] [ebp-110h]
	bool isMessageContinuation; // [esp+10Fh] [ebp-5h]
	size_t size; // [esp+110h] [ebp-4h]

	if (this->CurrentLabelName[0])
	{
		if (this->NextLineBlockType != BT_None)
			this->EndBlock();
		isMessageContinuation = false;
		if (type != LT_LineNo && type != LT_ScriptName)
		{
			if (type == LT_Message && this->LastLineType == LT_Message)
				isMessageContinuation = true;
			this->LastLineType = type;
		}
		if (this->OutputLineNo && this->FlagL)
		{
			this->OutputLineNo = false;
			sprintf(buffer, "%d", this->LastInputLine + 1);
			this->AppendLine(LT_LineNo, buffer);
		}
		if (this->OutputScriptName && this->FlagL)
		{
			this->OutputScriptName = false;
			this->AppendLine(LT_ScriptName, this->SourceFile->Path);
		}
		if (this->CurrentBlockLength == 0)
			this->BlockTable[this->BlockTablePtr].Start = this->LineTablePtr;
		this->LineTable[this->LineTablePtr++] = this->LineDataPtr;
		++this->CurrentBlockLength;
		this->LineData[this->LineDataPtr++] = 1;
		this->LineData[this->LineDataPtr++] = static_cast<char>(type);
		size = strlen(content) + 1;
		if (isMessageContinuation)
		{
			// Prepend a line feed to the line
			const char* newLine = "\\n";
			memcpy(&this->LineData[this->LineDataPtr], newLine, strlen(newLine));
			this->LineDataPtr += strlen(newLine);
		}
		memcpy(&this->LineData[this->LineDataPtr], content, size);
		this->LineDataPtr += size;
		if (type == LT_Message)
			this->CurrentBlockType = BT_Input;
	}
}

// .text:00414360
void mc::CstScript::WriteToFile()
{
	File* cstFile; // [esp+14h] [ebp-46Ch]
	int* scriptData; // [esp+50h] [ebp-430h]
	int blockTableSize; // [esp+54h] [ebp-42Ch]
	size_t scriptBodySize; // [esp+58h] [ebp-428h]
	unsigned long compressedSize; // [esp+5Ch] [ebp-424h] MAPDST
	int lineDataSize; // [esp+64h] [ebp-41Ch]
	HGLOBAL hMem; // [esp+70h] [ebp-410h]
	int lineTableSize; // [esp+74h] [ebp-40Ch]
	char buffer[1024]; // [esp+78h] [ebp-408h]
	char* scriptBody; // [esp+47Ch] [ebp-4h]

	if (this->MacroInfo)
	{
		this->MacroInfo->EndMacro();
		this->IsInMacroDefinition = 0;
	}
	if (this->CurrentLabelName[0])
	{
		this->EndBlock();
		sprintf(buffer, "%s.cst", this->CurrentLabelName);
		cstFile = new File(buffer);
		if (this->SourceUpdateTime && cstFile->GetUpdateTime() >= this->SourceUpdateTime)
		{
			delete cstFile;
		}
		else if (this->LineTablePtr)
		{
			blockTableSize = 8 * this->BlockTablePtr;
			lineTableSize = 4 * this->LineTablePtr;
			lineDataSize = this->LineDataPtr;
			scriptBodySize = lineDataSize + lineTableSize + blockTableSize;
			scriptData = (int*)GlobalAlloc(0x40u, lineDataSize + lineTableSize + blockTableSize + 16);
			hMem = GlobalAlloc(0x40u, scriptBodySize + 16);
			if (scriptData && hMem)
			{
				*scriptData = scriptBodySize;
				scriptData[1] = this->BlockTablePtr;
				scriptData[2] = blockTableSize;
				scriptData[3] = lineTableSize + blockTableSize;
				scriptBody = (char*)(scriptData + 4);
				// Write block table
				memcpy(scriptData + 4, this->BlockTable, blockTableSize);
				scriptBody += blockTableSize;
				// Write line table
				memcpy(scriptBody, this->LineTable, lineTableSize);
				scriptBody += lineTableSize;
				// Write line data
				memcpy(scriptBody, this->LineData, lineDataSize);
				compressedSize = 0;
				if (this->Compress)
				{
					// This has been adapted from the original decompiled source.
					// The original used an object-oriented zlib implementation.
					compressedSize = scriptBodySize + 16;
					compress((Bytef*)hMem, &compressedSize, (Bytef*)scriptData, scriptBodySize + 16);
					if (compressedSize >= scriptBodySize + 16)
						compressedSize = 0;
				}
				cstFile->PrepareWrite2();
				cstFile->Open();
				sprintf(buffer, "CatScene");
				*(DWORD*)&buffer[8] = compressedSize;
				*(DWORD*)&buffer[12] = scriptBodySize + 16;
				cstFile->Write(buffer, 0x10u);
				if (compressedSize)
					cstFile->Write((const char*)hMem, compressedSize);
				else
					cstFile->Write((const char*)scriptData, scriptBodySize + 16);
				cstFile->CloseHandles();
				delete cstFile;
				GlobalFree(hMem);
				GlobalFree(scriptData);
				this->CurrentLabelName[0] = 0;
				++this->GeneratedFileCount;
			}
			else
			{
				if (scriptData)
					GlobalFree(scriptData);
				if (hMem)
					GlobalFree(hMem);
			}
		}
		else
		{
			cstFile->Delete();
			delete cstFile;
		}
	}
}

// .text:00414940
void mc::CstScript::StartMacroDefinition(const char* macroName)
{
	this->WriteToFile();
	if (this->MacroInfo)
	{
		this->MacroInfo->CreateNewMacro(macroName);
		this->IsInMacroDefinition = true;
	}
}

// .text:00414990
BOOL mc::CstScript::ProcessInclude(const char* includeName)
{
	mc::SourceFile* sourceFile; // [esp+4h] [ebp-20h]

	if (this->IncludeStackSize < 16)
	{
		sourceFile = new mc::SourceFile(includeName);
		if (sourceFile && sourceFile->LineCount)
		{
			this->SourceFile->Unlock();
			this->IncludeStack[this->IncludeStackSize] = this->SourceFile;
			this->CurrentLineStack[this->IncludeStackSize++] = this->CurrentInputLine;
			this->SourceFile = sourceFile;
			this->CurrentInputLine = 0;
			this->SourceFile->Lock();
			this->LastInputLine = this->CurrentInputLine;
			this->OutputScriptName = 1;
			return true;
		}
		else
		{
			// Failed to include file
			printf("インクルード出来ません：%s\n", includeName);
			this->ErrorCode = EC_EndOfFile;
			return false;
		}
	}
	else
	{
		// Includes are too deeply nested
		printf("インクルードのネストが深すぎます（多重インクルードし過ぎです）\n");
		this->ErrorCode = EC_DeepNesting;
		return false;
	}
}

// .text:00414B20
const char* mc::CstScript::AppendName(const char* name)
{
	char buffer[1024]; // [esp+4h] [ebp-410h]
	char* ptr; // [esp+40Ch] [ebp-8h]

	ptr = buffer;
	while ((signed int)(unsigned __int8)*name > ' ')
		*ptr++ = *name++;
	// Overwrite following whitespace with zeroes
	for (*ptr = 0; ptr > buffer; *ptr = 0)
	{
		if (*--ptr != ' ' && *ptr != '\t')
			break;
	}
	if (buffer[0])
		this->AppendLine(LT_Name, buffer);
	return name;
}

// .text:00414BE0
const char* mc::CstScript::AppendNameSjis(const char* name)
{
	char buffer[1024]; // [esp+4h] [ebp-410h]
	char* ptr; // [esp+40Ch] [ebp-8h]

	ptr = buffer;
	// The bytes 129 and 117 form the quote character 「.
	// So a name can be immediately followed by 「 and still be interpreted correctly.
	while ((signed int)(unsigned __int8)*name > 32 && ((unsigned __int8)*name != 129 || name[1] != 117))
		*ptr++ = *name++;
	*ptr = 0;
	if (buffer[0])
		this->AppendLine(LT_Name, buffer);
	return name;
}

// .text:00414C90
void mc::CstScript::ProcessLabel(const char* labelName)
{
	char* dstPtr; // [esp+4h] [ebp-40Ch]
	char location[1024]; // [esp+8h] [ebp-408h]
	const char* srcPtr; // [esp+40Ch] [ebp-4h]

	this->WriteToFile();
	if (this->LabelInfo)
	{
		this->LabelInfo->Lock();
		if (this->LabelInfo->FindLabelIndex(labelName) >= 0)
		{
			printf("#%s が以下の箇所で重複定義されています\n", labelName);
			this->LabelInfo->GetLabelLocation(location, (char*)labelName);
			printf("%s\n", location);
			sprintf(location, "%s(%d)", this->SourcePath, this->LastInputLine + 1);
			printf("%s\n", location);
		}
		else
		{
			sprintf(location, "%s(%d)", this->SourcePath, this->LastInputLine + 1);
			this->LabelInfo->Insert(labelName, location);
		}
		this->LabelInfo->Unlock();
	}
	// Copy label name to this->CurrentLabelName
	srcPtr = labelName;
	dstPtr = this->CurrentLabelName;
	while (*srcPtr > 32 && *srcPtr < 127)
		*dstPtr++ = *srcPtr++;
	*dstPtr = 0;
	this->CurrentBlockLength = 0;
	this->LineTablePtr = 0;
	this->BlockTablePtr = 0;
	this->field_404 = 0;
	this->LineDataPtr = 0;
	this->NextLineBlockType = BT_None;
	this->CurrentBlockType = BT_None;
}

// .text:00414EB0
int mc::CstScript::ReadNextLines()
{
	this->CurrentLinePtr = nullptr;
	while (this->SourceFile)
	{
		this->LastInputLine = this->CurrentInputLine;
		if (this->SourceFile->HasLine(this->CurrentInputLine))
		{
			this->SourceFile->ReadNextLines(this->CurrentLineBuffer, 1024, &this->CurrentInputLine, 1);
			this->SourceFile->Normalize(this->CurrentLineBuffer, this->CurrentLineBuffer, 1);
			if (this->CurrentLineBuffer[0] && *(WORD*)this->CurrentLineBuffer != ' ')
			{
				this->CurrentLinePtr = this->CurrentLineBuffer;
				break;
			}
			// Line was empty -> end the current block
			this->MarkBlockEnd();
		}
		else
		{
			if (this->IsInBlockComment)
			{
				if (this->MacroInfo)
					// Unterminated comment in macro
					printf("マクロ内でコメントアウトが閉じられていません\n");
				else
					// Unterminated comment
					printf("コメントアウトが閉じられていません：%s\n", this->SourceFile->Path);
				this->IsInBlockComment = 0;
			}
			if (!this->PopIncludeStack())
				break;
		}
	}
	if (this->CurrentLinePtr)
		return strlen(this->CurrentLinePtr);
	else
		return -1;
}

// .text:00415020
BOOL mc::CstScript::CompileInternal()
{
	char chr; // [esp+5h] [ebp-7h]
	bool isMessage1; // [esp+6h] [ebp-6h]
	bool isMessage2; // [esp+7h] [ebp-5h]
	char* ptr; // [esp+8h] [ebp-4h]
	char* ptra; // [esp+8h] [ebp-4h]
	char* ptrb; // [esp+8h] [ebp-4h]
	char* ptrc; // [esp+8h] [ebp-4h]

	this->LastLineType = LT_None;
	this->NextLineBlockType = BT_None;
	this->CurrentBlockType = BT_None;
	this->CurrentLabelName[0] = 0;
	while (this->SourceFile)
	{
		if (this->ReadNextLines() > 0)
		{
			isMessage1 = false;
			ptr = this->CurrentLinePtr;
			if (*ptr != '/' || ptr[1] != '*')
			{
				if (*ptr != '*' || ptr[1] != '/')
				{
					if (!this->IsInBlockComment)
					{
						if (*ptr == '#')
						{
							if (!memcmp(ptr + 1, "include", 7u))
							{
								ptra = ptr + 8;
								if (*ptra == ' ' || *ptra == '\t')
									this->ProcessInclude(ptra + 1);
							}
							else
							{
								this->ProcessLabel(ptr + 1);
							}
						}
						else if (*ptr == '%')
						{
							if (!memcmp(ptr + 1, "echo", 4u))
							{
								ptrb = ptr + 5;
								if (*ptrb == ' ' || *ptrb == '\t')
									printf("%echo : %s\n", ptrb + 1);
							}
							else
							{
								this->StartMacroDefinition(ptr + 1);
							}
						}
						else if (this->IsInMacroDefinition)
						{
							this->MacroInfo->AppendLine(ptr);
						}
						else
						{
							this->OutputLineNo = true;
							if (*ptr != ' ')
							{
								if (*ptr == '$' || CharacterFlags[(unsigned __int8)*ptr] & CF_FirstSjisByte)
								{
									ptr = (char*)this->AppendNameSjis(ptr);
								}
								else if (*ptr == '\\')
								{
									if (*++ptr == ' ' || *ptr == '\t')
										isMessage1 = 1;
								}
								else
								{
									ptr = (char*)this->AppendName(ptr);
									isMessage1 = 1;
								}
							}
							// Skip leading whitespace
							while (*ptr > 0 && *ptr <= ' ')
								++ptr;
							if (*ptr)
							{
								isMessage2 = false;
								if (*ptr == '\\')
								{
									chr = ptr[1];
									isMessage2 = (chr < '0' || chr > '9') && chr != '(';
								}
								// Everything starting with one of these characters is
								// considered a message, everything else a command
								if (*ptr == '$'
									|| *ptr == '['
									|| *ptr == '<'
									|| *ptr == '"'
									// Shift-JIS single byte half-width katakana
									|| (signed int)(unsigned __int8)*ptr >= 160 && (signed int)(unsigned __int8)*ptr <= 223
									// Shift-JIS first byte of a double-byte character
									|| CharacterFlags[(unsigned __int8)*ptr] & CF_FirstSjisByte
									|| isMessage1 == 1
									|| isMessage2 == 1)
								{
									// Ignore \@ at the start of a message
									if (*ptr == '\\' && ptr[1] == '@' && ptr[2])
										ptr += 2;
									if (!strcmp(ptr, "\\p")
										|| !strcmp(ptr, "\\P")
										|| !strcmp(ptr, "￥ｐ")
										|| !strcmp(ptr, "￥Ｐ")
										|| !strcmp(ptr, "￥せ"))
									{
										// If the block type is BT_None, a previous line ended with \@.
										// In that case, the \p is ignored.
										if (this->CurrentBlockType)
										{
											this->CurrentBlockType = BT_PageBreak;
											this->MarkBlockEnd();
										}
									}
									else if (!strcmp(ptr, "\\n")
										|| !strcmp(ptr, "\\N")
										|| !strcmp(ptr, "￥ｎ")
										|| !strcmp(ptr, "￥Ｎ")
										|| !strcmp(ptr, "￥み"))
									{
										this->AppendLine(LT_Message, "");
										this->CurrentBlockType = BT_Input;
									}
									else
									{
										this->AppendLine(LT_Message, ptr);
										while (*ptr)
										{
											if (CharacterFlags[(unsigned __int8)*ptr] & CF_FirstSjisByte
												&& CharacterFlags[(unsigned __int8)ptr[1]] & 0x80000000)
											{
												this->CurrentBlockType = BT_Input;
												ptr += 2;
											}
											else if (*ptr == '\\' && ptr[1])
											{
												// \\ or \@ sets the block type to BT_None
												this->CurrentBlockType = (ptr[1] != '\\' && ptr[1] != '@') ? BT_Input : BT_None;
												ptr += 2;
											}
											else
											{
												if ((signed int)(unsigned __int8)*ptr > ' ')
													this->CurrentBlockType = BT_Input;
												++ptr;
											}
										}
									}
								}
								else if (*ptr == '%')
								{
									if (this->MacroInfo)
									{
										if (this->MacroInfo->PrepareMacroExpansion(ptr + 1))
										{
											while ((this->MacroInfo->ExpandMacroLine(this->CurrentLinePtr) & 0x80000000) == 0)
											{
												for (ptrc = this->CurrentLinePtr; *ptrc > 0 && *ptrc <= ' '; ++ptrc)
													;
												if (*ptrc)
													this->AppendLine(LT_Command, ptrc);
											}
											this->MacroInfo->EndMacro();
										}
										else
										{
											// Macro expansion error
											printf("マクロ展開エラー：%s\n", ptr + 1);
										}
									}
								}
								else
								{
									this->AppendLine(LT_Command, ptr);
								}
							}
						}
					}
				}
				else if (this->IsInBlockComment)
				{
					this->IsInBlockComment = 1;
				}
				else
				{
					// Found comment end without being in a comment
					printf("コメントアウトの閉じが余分です：%s\n", this->SourceFile->Path);
				}
			}
			else if (this->IsInBlockComment)
			{
				// Comment in comment
				printf("２重にコメントアウトされています：%s\n", this->SourceFile->Path);
			}
			else
			{
				this->IsInBlockComment = 1;
			}
		}
	}
	this->MarkBlockEnd();
	this->WriteToFile();
	return true;
}

// .text:004156E0
BOOL mc::CstScript::CompileFromSource(const char* srcPath)
{
	mc::SourceFile* sourceFile; // [esp+4h] [ebp-1Ch]

	strcpy(this->SourcePath, srcPath);
	this->OutputScriptName = true;
	sourceFile = new mc::SourceFile(srcPath);
	this->SourceFile = sourceFile;
	if (this->SourceFile && this->SourceFile->LineCount)
	{
		this->CurrentInputLine = 0;
		this->SourceFile->Lock();
		this->CompileInternal();
		if (this->SourceFile)
			this->SourceFile->Unlock();
		return true;
	}
	else
	{
		this->ErrorCode = EC_EndOfFile;
		return false;
	}
}
