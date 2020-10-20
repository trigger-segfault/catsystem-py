#pragma once
#include <windows.h>

namespace mc {

	class SourceFile sealed
	{
	public:
		int field_0;
		int LineCount;
		char* FileContent;
		int* LineOffsets;
		int MultiLine_;
		char Path[1024];
		HGLOBAL FileContentGlobal;
		HGLOBAL LineOffsetsGlobal;
		int FileSize;

		SourceFile(const char* srcPath);
		// no destructor -> HGLOBALs might not be free'd
		~SourceFile() = default;

		size_t NormalizeQuote(unsigned char** dst, const unsigned char** src, BOOL stripEscapes);
		void Unlock();
		BOOL Lock();
		BOOL IsLocked();
		BOOL Load(const char* path);
		size_t NormalizeString(unsigned char** dst, const unsigned char** src, BOOL stripComments);
		size_t Normalize(char* dst, const char* src, BOOL stripComments);
		const char* GetLine(int lineNo);
		BOOL HasLine(int lineNo);
		size_t ReadNextLines(char* dstPtr, int bufferSize, int* pLineNo, BOOL allowMultipleLines);
	};
}
