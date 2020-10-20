#include "SourceFile.h"

#include <cstdio>

#include "File.h"
#include "enums.h"
#include "mc.h"

// .text:004119A0
mc::SourceFile::SourceFile(const char* srcPath)
{
	static_assert(sizeof(SourceFile) == 0x00000420, "sizeof(SourceFile) is incorrect");
	this->FileContent = 0;
	this->FileContentGlobal = 0;
	this->LineOffsets = 0;
	this->LineOffsetsGlobal = 0;
	this->FileSize = 0;
	this->LineCount = 0;
	this->field_0 = 0;
	this->MultiLine_ = 0;
	strcpy(this->Path, srcPath);
	this->Load(srcPath);
}

// .text:004111F0
size_t mc::SourceFile::NormalizeQuote(unsigned char** dst, const unsigned char** src, BOOL stripEscapes)
{
	size_t length; // ST08_4
	unsigned __int8* dstPtr; // [esp+4h] [ebp-10h] MAPDST
	char quoteChar; // [esp+Fh] [ebp-5h]
	const unsigned __int8* srcPtr; // [esp+10h] [ebp-4h] MAPDST

	srcPtr = *src;
	dstPtr = *dst;
	quoteChar = **src;
	// Return if the string does not start with a quote
	if (quoteChar != '"' && quoteChar != '\'')
		return 0;
	// Copy quote character
	*dstPtr++ = *srcPtr++;
	while (*srcPtr)
	{
		if (*srcPtr == (unsigned __int8)quoteChar)
		{
			*dstPtr++ = *srcPtr++;
			break;
		}
		if (*srcPtr == '\\')
		{
			// Handle escape sequence
			if (!stripEscapes)
				*dstPtr++ = *srcPtr;
			if (!*++srcPtr)
				break;
			*dstPtr++ = *srcPtr++;
		}
		else if (CharacterFlags[*srcPtr] & CF_FirstSjisByte)
		{
			// Copy shift-jis character
			*dstPtr++ = *srcPtr;
			if (!(CharacterFlags[*++srcPtr] & 0x80000000))
				--dstPtr;
			else
				*dstPtr++ = *srcPtr++;
		}
		else
		{
			*dstPtr++ = *srcPtr++;
		}
	}
	length = dstPtr - *dst;
	*src = srcPtr;
	*dst = dstPtr;
	return length;
}

// .text:004113B0
void mc::SourceFile::Unlock()
{
	if (this->FileContent)
	{
		GlobalUnlock(this->FileContentGlobal);
		this->FileContent = nullptr;
	}
	if (this->LineOffsets)
	{
		GlobalUnlock(this->LineOffsetsGlobal);
		this->LineOffsets = nullptr;
	}
}

// .text:00411410
BOOL mc::SourceFile::Lock()
{
	if (!this->FileContent || !this->LineOffsets)
	{
		if (this->FileContentGlobal)
			this->FileContent = (char*)GlobalLock(this->FileContentGlobal);
		if (this->LineOffsetsGlobal)
			this->LineOffsets = (int*)GlobalLock(this->LineOffsetsGlobal);
	}
	if (this->FileContent && this->LineOffsets)
		return true;
	this->Unlock();
	return false;
}

// .text:004114A0
BOOL mc::SourceFile::IsLocked()
{
	return this->FileContent && this->LineOffsets;
}

// .text:004114D0
BOOL mc::SourceFile::Load(const char* path)
{
	BOOL result; // eax
	File* file; // [esp+4h] [ebp-44h]
	HGLOBAL lineOffsetsGlobal; // [esp+1Ch] [ebp-2Ch]
	int* lineOffsets; // [esp+20h] [ebp-28h]
	unsigned __int8 chr; // [esp+27h] [ebp-21h]
	char* buffer; // [esp+28h] [ebp-20h]
	const char* fileContent; // [esp+28h] [ebp-20h]
	signed int fileSize; // [esp+2Ch] [ebp-1Ch]
	signed int i; // [esp+30h] [ebp-18h]
	FILE* handle; // [esp+38h] [ebp-10h]
	int readBytes; // [esp+3Ch] [ebp-Ch]
	int offset; // [esp+3Ch] [ebp-Ch]
	HGLOBAL fileContentGlobal; // [esp+40h] [ebp-8h]
	int lineCount; // [esp+44h] [ebp-4h]

	fileContentGlobal = nullptr;
	file = new File(path);
	file->PrepareRead();
	fileSize = file->GetSize();
	delete file;
	lineCount = 0;
	if (fileSize > 0)
	{
		fileSize += 16;
		fileContentGlobal = GlobalAlloc(0x42u, fileSize);
		if (fileContentGlobal)
		{
			buffer = (char*)GlobalLock(fileContentGlobal);
			if (buffer)
			{
				handle = fopen(path, "rt");
				if (handle)
				{
					readBytes = 0;
					while (!feof(handle) && fgets(&buffer[readBytes], fileSize - readBytes, handle))
					{
						// Trim control characters at the end of the line?
						for (i = strlen(&buffer[readBytes]); i > 0; --i)
						{
							chr = buffer[readBytes - 1 + i];
							if ((signed int)chr >= ' ' || chr == '\t')
								break;
						}
						buffer[readBytes + i] = 0;
						readBytes += i + 1;
						++lineCount;
					}
					fclose(handle);
				}
				GlobalUnlock(fileContentGlobal);
			}
		}
	}
	if (lineCount > 0)
	{
		this->LineCount = lineCount;
		this->FileSize = fileSize;
		this->FileContentGlobal = fileContentGlobal;
		lineOffsetsGlobal = GlobalAlloc(0x42u, 4 * lineCount);
		if (lineOffsetsGlobal)
		{
			fileContent = (const char*)GlobalLock(fileContentGlobal);
			if (fileContent)
			{
				lineOffsets = (int*)GlobalLock(lineOffsetsGlobal);
				if (lineOffsets)
				{
					offset = 0;
					for (i = 0; i < this->LineCount; ++i)
					{
						lineOffsets[i] = offset;
						offset += strlen(&fileContent[offset]) + 1;
					}
					GlobalUnlock(lineOffsetsGlobal);
				}
				GlobalUnlock(fileContentGlobal);
			}
		}
		this->LineOffsetsGlobal = lineOffsetsGlobal;
		result = 1;
	}
	else
	{
		if (fileContentGlobal)
			GlobalFree(fileContentGlobal);
		result = 0;
	}
	return result;
}

// .text:00411770
size_t mc::SourceFile::NormalizeString(unsigned char** dst, const unsigned char** src, BOOL stripComments)
{
	unsigned __int8* dstPtr; // [esp+4h] [ebp-Ch]
	size_t length; // [esp+8h] [ebp-8h]
	const unsigned __int8* srcPtr; // [esp+Ch] [ebp-4h]

	srcPtr = *src;
	dstPtr = *dst;
	while (*srcPtr)
	{
		if ((signed int)*srcPtr <= 32)
		{
			// Normalize leading whitespace to a single space
			++srcPtr;
			*dstPtr++ = ' ';
			while (*srcPtr && (signed int)*srcPtr <= 32)
				++srcPtr;
		}
		else if (*srcPtr != '"' && *srcPtr != '\'')
		{
			if (CharacterFlags[*srcPtr] & CF_FirstSjisByte)
			{
				// Copy shift-jis character
				*dstPtr++ = *srcPtr;
				if ((CharacterFlags[*++srcPtr] & CF_SecondSjisByte) == 0)
					--dstPtr;
				else
					*dstPtr++ = *srcPtr++;
			}
			else
			{
				// Stop at line comment
				if (stripComments && *srcPtr == '/' && srcPtr[1] == '/')
					break;
				*dstPtr++ = *srcPtr++;
			}
		}
		else
		{
			this->NormalizeQuote(&dstPtr, &srcPtr, 0);
		}
	}
	*dstPtr = 0;
	length = dstPtr - *dst;
	*src = srcPtr;
	*dst = dstPtr;
	return length;
}

// .text:004118F0
size_t mc::SourceFile::Normalize(char* dst, const char* src, BOOL stripComments)
{
	return this->NormalizeString((unsigned __int8**)&dst, (const unsigned __int8**)&src, stripComments);
}

// .text:00411920
const char* mc::SourceFile::GetLine(int lineNo)
{
	if (!this->IsLocked())
		return nullptr;
	if (lineNo >= 0 && lineNo < this->LineCount)
		return &this->FileContent[this->LineOffsets[lineNo]];
	return nullptr;
}

// .text:00411970
BOOL mc::SourceFile::HasLine(int lineNo)
{
	return this->GetLine(lineNo) != nullptr;
}

// .text:00411A30
size_t mc::SourceFile::ReadNextLines(char* dstPtr, int bufferSize, int* pLineNo, BOOL allowMultipleLines)
{
	char v5; // ST1F_1
	int lineLength; // [esp+4h] [ebp-28h] MAPDST
	char* dst; // [esp+8h] [ebp-24h]
	signed int notFirst; // [esp+Ch] [ebp-20h]
	int lineNo; // [esp+14h] [ebp-18h]
	unsigned __int8* v13; // [esp+1Ch] [ebp-10h]
	signed int chrWidth; // [esp+20h] [ebp-Ch]
	const char* linePtr; // [esp+24h] [ebp-8h]
	unsigned __int16 chr; // [esp+28h] [ebp-4h]
	__int16 chra; // [esp+28h] [ebp-4h]

	lineNo = *pLineNo;
	dst = dstPtr;
	notFirst = 0;
	while (1)
	{
		linePtr = this->GetLine(lineNo);
		if (!linePtr)
			break;
		if (this->MultiLine_)
			this->MultiLine_ = 0;
		if (notFirst)
		{
			// Trim control characters from the start
			while (*linePtr && (signed int)(unsigned __int8)*linePtr <= ' ')
				++linePtr;
		}
		lineLength = strlen(linePtr);
		++lineNo;
		if (bufferSize <= lineLength)
		{
			memcpy(dst, linePtr, bufferSize - 1);
			dst += bufferSize - 1;
			break;
		}
		memcpy(dst, linePtr, lineLength);
		v13 = (unsigned __int8*)dst;
		dst += lineLength;
		bufferSize -= lineLength;
		if (!allowMultipleLines || lineLength <= 0 || *(dst - 1) != '\\')
			break;
		for (chr = *(WORD*)v13; lineLength >= 2; chr = *(WORD*)v13)
		{
			// Skip one or two bytes, depending on whether the next byte is part of a multi-byte character
			chrWidth = (signed int)(unsigned __int8)chr < 128 ? 1 : 2;
			lineLength -= chrWidth;
			v13 += chrWidth;
			if (lineLength < 2)
				break;
		}
		chra = chr >> 8;
		if (!lineLength && chrWidth == 2 && chra == '\\')
		{
			// Overwrite terminating \\ with \0
			v5 = *dst;
			*dst = 0;
			this->MultiLine_ = 1;
			*dst = v5;
			break;
		}
		notFirst = 1;
		// Step back over the terminating \0
		--dst;
	}
	*pLineNo = lineNo;
	*dst = 0;
	// Return length of the written string
	return dst - dstPtr;
}
