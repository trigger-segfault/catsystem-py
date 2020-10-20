#include "MacroInfo.h"

// .text:004131F0
mc::MacroInfo::MacroInfo()
{
	static_assert(sizeof(MacroInfo) == 0x000C11B0, "sizeof(MacroInfo) is incorrect");
	this->MacroCount = 0;
	this->Mode = 0;
	this->CurrentMacro = 0;
	this->MacroData = 0;
	this->BufferSize = 0;
	this->UsedSize = 0;
}

// .text:004131C0
mc::MacroInfo::~MacroInfo()
{
	if (this->MacroData)
		GlobalFree(this->MacroData);
}

// .text:00412D40
BOOL mc::MacroInfo::EndMacro()
{
	if (this->Mode == 1)
	{
		// Mode 1: In macro definition
		if (this->CurrentMacro->Offset >= 0)
			++this->MacroCount;
	}
	else if (this->Mode != 2)
	{
		return 0;
	}
	// Mode 2: In macro expansion
	this->CurrentMacro = nullptr;
	this->Mode = 0;
	return 1;
}

// .text:00412DB0
size_t mc::MacroInfo::ExpandMacroLine(char* lineBuffer)
{
	char* v3; // ST10_4
	char* v4; // ST1C_4
	size_t argLength; // ST18_4
	char* dst; // [esp+4h] [ebp-10h]
	int argIndex; // [esp+8h] [ebp-Ch]
	int size; // [esp+Ch] [ebp-8h]
	char* macroLinePtr; // [esp+10h] [ebp-4h]

	if (this->Mode != 2 || !this->CurrentMacro)
		return -1;
	if (this->ExpansionCurrentLine >= this->CurrentMacro->CommandCount)
		return -1;
	dst = lineBuffer;
	macroLinePtr = &this->MacroData[this->CurrentMacro->Offset];
	for (size = this->ExpansionCurrentLine; size; --size)
		macroLinePtr += strlen(macroLinePtr) + 1;
next:
	while (*macroLinePtr)
	{
		if (IsDBCSLeadByteEx(0, *macroLinePtr) == 1 && macroLinePtr[1])
		{
			*dst = *macroLinePtr;
			v3 = dst + 1;
			v4 = macroLinePtr + 1;
			*v3 = *v4;
			dst = v3 + 1;
			macroLinePtr = v4 + 1;
		}
		else if (*macroLinePtr != '%' || macroLinePtr[1] < '1' || macroLinePtr[1] > '9')
		{
			*dst++ = *macroLinePtr++;
		}
		else
		{
			argIndex = macroLinePtr[1] - '0';
			macroLinePtr += 2;
			if (argIndex > 0)
			{
				if (argIndex < this->ExpansionArgumentCount)
				{
					// Insert passed argument value
					if (this->ExpansionArguments[argIndex])
					{
						argLength = strlen(this->ExpansionArguments[argIndex]);
						memcpy(dst, this->ExpansionArguments[argIndex], argLength);
						dst += argLength;
					}
					if (*macroLinePtr == '[')
					{
						++macroLinePtr;
						while (*macroLinePtr)
						{
							if (*macroLinePtr == ']')
							{
								++macroLinePtr;
								goto next;
							}
							++macroLinePtr;
						}
					}
				}
				else if (*macroLinePtr == '[')
				{
					// Insert default argument value
					++macroLinePtr;
					while (*macroLinePtr)
					{
						if (*macroLinePtr == ']')
						{
							++macroLinePtr;
							goto next;
						}
						*dst++ = *macroLinePtr++;
					}
				}
			}
		}
	}
	*dst = 0;
	++this->ExpansionCurrentLine;
	return strlen(lineBuffer);
}

// .text:00413050
int mc::MacroInfo::FindMacroIndex(const char* macroName)
{
	int i; // [esp+4h] [ebp-8h]

	for (i = 0; i < this->MacroCount; ++i)
	{
		if (!strcmp(&this->MacroData[this->MacroDefinitions[i].NameOffset], macroName))
			return i;
	}
	return -1;
}

// .text:004130B0
int mc::MacroInfo::AddString(const char* string)
{
	int offset; // ST18_4
	char* v5; // [esp+4h] [ebp-Ch]
	char* v6; // [esp+4h] [ebp-Ch]
	int length; // [esp+8h] [ebp-8h]

	length = strlen(string) + 1;
	while (length + this->UsedSize > this->BufferSize)
	{
		if (this->MacroData)
		{
			this->BufferSize += 0x1000000;
			v5 = (char*)GlobalReAlloc(this->MacroData, this->BufferSize, 0x40u);
			if (!v5)
			{
				this->BufferSize -= 0x1000000;
				return -1;
			}
			this->MacroData = v5;
		}
		else
		{
			v6 = (char*)GlobalAlloc(0x40u, 0x1000000u);
			if (!v6)
				return -1;
			this->BufferSize = 0x1000000;
			this->MacroData = v6;
			this->UsedSize = 0;
		}
	}
	offset = this->UsedSize;
	memcpy(&this->MacroData[offset], string, length);
	this->UsedSize += length;
	return offset;
}

// .text:00413240
BOOL mc::MacroInfo::PrepareMacroExpansion(const char* macroName)
{
	char* dstPtr; // [esp+4h] [ebp-Ch]
	int argCount; // [esp+8h] [ebp-8h]
	int index; // [esp+8h] [ebp-8h]
	const char* srcPtr; // [esp+Ch] [ebp-4h]

	if (this->Mode)
		return false;
	argCount = 0;
	srcPtr = macroName;
	dstPtr = this->ExpansionArgumentData;
	// Copy macro name and arguments to 0 separated buffer
	while (*srcPtr)
	{
		if (*srcPtr > 0 && *srcPtr <= 32)
			++srcPtr;
		if (!*srcPtr)
			break;
		this->ExpansionArguments[argCount] = dstPtr;
		while ((signed int)(unsigned __int8)*srcPtr > 32)
			*dstPtr++ = *srcPtr++;
		*dstPtr++ = 0;
		++argCount;
	}
	this->ExpansionArgumentCount = argCount;
	while (argCount < 16)
		this->ExpansionArguments[argCount++] = 0;
	if (this->ExpansionArgumentCount <= 0)
		return false;
	index = this->FindMacroIndex(this->ExpansionArguments[0]);
	if (index < 0)
		return false;
	this->CurrentMacro = &this->MacroDefinitions[index];
	this->ExpansionCurrentLine = 0;
	this->Mode = 2;
	return true;
}

// .text:004133A0
void mc::MacroInfo::AppendLine(const char* line)
{
	int offset; // [esp+4h] [ebp-4h]

	if (strlen(line))
	{
		if (this->Mode == 1)
		{
			if (this->CurrentMacro)
			{
				offset = this->AddString(line);
				if (offset >= 0)
				{
					if (this->CurrentMacro->Offset < 0)
						this->CurrentMacro->Offset = offset;
					++this->CurrentMacro->CommandCount;
				}
			}
		}
	}
}

// .text:00413430
BOOL mc::MacroInfo::CreateNewMacro(const char* macroName)
{
	int nameOffset; // [esp+4h] [ebp-4h]

	if (this->Mode)
		return false;
	if (this->MacroCount >= 0x10000)
		return false;
	if (this->FindMacroIndex(macroName) >= 0)
		return false;
	nameOffset = this->AddString(macroName);
	if (nameOffset < 0)
		return false;
	this->CurrentMacro = &this->MacroDefinitions[this->MacroCount];
	this->CurrentMacro->NameOffset = nameOffset;
	this->CurrentMacro->Offset = -1;
	this->CurrentMacro->CommandCount = 0;
	this->Mode = 1;
	return true;
}
