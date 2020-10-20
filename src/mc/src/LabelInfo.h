#pragma once
#include <Windows.h>

namespace mc {

	class LabelInfo sealed
	{
	public:
		int DataBufferSize;
		int OffsetBufferSize;
		HGLOBAL LabelDataGlobal;
		HGLOBAL LabelOffsetsGlobal;
		char* LabelData;
		int* LabelOffsets;
		int LabelCount;
		int LabelDataPtr;

		LabelInfo();
		~LabelInfo();

		size_t GetEntryLength(const char* entry);
		int FindInsertLocation(const char* labelName);
		void Unlock();
		BOOL Lock();
		void FreeMemory();
		void GrowBuffers(int dataIncrement, int offsetIncrement);
		void MoveData(int labelIndex, int offset);
		int FindLabelIndex(const char* labelName);
		char* GetLabelLocation(char* locationBuffer, const char* labelName);
		int Insert(const char* labelName, const char* locationString);
	};
}
