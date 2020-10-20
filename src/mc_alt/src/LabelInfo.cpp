#include "LabelInfo.h"

// .text:00411FC0
mc::LabelInfo::LabelInfo()
{
	static_assert(sizeof(LabelInfo) == 0x00000020, "sizeof(LabelInfo) is incorrect");
	this->DataBufferSize = 0;
	this->OffsetBufferSize = 0;
	this->LabelDataGlobal = 0;
	this->LabelOffsetsGlobal = 0;
	this->LabelData = 0;
	this->LabelOffsets = 0;
	this->LabelCount = 0;
	this->LabelDataPtr = 0;
	this->GrowBuffers(4096, 256);
}

// .text:00411FA0
mc::LabelInfo::~LabelInfo()
{
	this->FreeMemory();
}

// .text:00411C30
size_t mc::LabelInfo::GetEntryLength(const char* entry)
{
	size_t length; // eax

	length = strlen(entry);
	return length + 1 + strlen(&entry[length + 1]) + 1;
}

// .text:00411C70
int mc::LabelInfo::FindInsertLocation(const char* labelName)
{
	int cmp; // [esp+Ch] [ebp-10h]
	int low; // [esp+10h] [ebp-Ch]
	int pivot; // [esp+14h] [ebp-8h]
	int high; // [esp+18h] [ebp-4h]

	pivot = 0;
	low = 0;
	high = this->LabelCount - 1;
	if (strcmp(labelName, &this->LabelData[*this->LabelOffsets]) < 0)
		return 0;
	if (high < 0)
		return 1;
	if (strcmp(labelName, &this->LabelData[this->LabelOffsets[high]]) > 0)
		return this->LabelCount;
	while (low != high)
	{
		pivot = (high + low) >> 1;
		cmp = strcmp(labelName, &this->LabelData[this->LabelOffsets[pivot]]);
		if (!cmp)
			return (high + low) >> 1;
		if (high - low <= 1)
			return high;
		if (cmp >= 0)
			low = (high + low) >> 1;
		else
			high = (high + low) >> 1;
	}
	return pivot;
}

// .text:00411D80
void mc::LabelInfo::Unlock()
{
	if (this->LabelDataGlobal && this->LabelData)
	{
		GlobalUnlock(this->LabelDataGlobal);
		this->LabelData = 0;
	}
	if (this->LabelOffsetsGlobal)
	{
		if (this->LabelOffsets)
		{
			GlobalUnlock(this->LabelOffsetsGlobal);
			this->LabelOffsets = 0;
		}
	}
}

// .text:00411DE0
BOOL mc::LabelInfo::Lock()
{
	if (this->LabelDataGlobal && !this->LabelData)
		this->LabelData = (char*)GlobalLock(this->LabelDataGlobal);
	if (this->LabelOffsetsGlobal && !this->LabelOffsets)
		this->LabelOffsets = (int*)GlobalLock(this->LabelOffsetsGlobal);
	return this->LabelOffsets && this->LabelData;
}

// .text:00411E50
void mc::LabelInfo::FreeMemory()
{
	this->Unlock();
	if (this->LabelDataGlobal)
	{
		GlobalFree(this->LabelDataGlobal);
		this->LabelDataGlobal = 0;
		this->DataBufferSize = 0;
	}
	if (this->LabelOffsetsGlobal)
	{
		GlobalFree(this->LabelOffsetsGlobal);
		this->LabelOffsetsGlobal = 0;
		this->OffsetBufferSize = 0;
	}
	this->LabelCount = 0;
	this->LabelDataPtr = 0;
}

// .text:00411ED0
void mc::LabelInfo::GrowBuffers(int dataIncrement, int offsetIncrement)
{
	HGLOBAL labelDataGlobal; // eax
	HGLOBAL labelOffsetsGlobal; // eax

	this->Unlock();
	if (dataIncrement)
	{
		if (this->LabelDataGlobal)
		{
			this->DataBufferSize += dataIncrement;
			labelDataGlobal = GlobalReAlloc(this->LabelDataGlobal, this->DataBufferSize, 0x42u);
		}
		else
		{
			this->DataBufferSize = dataIncrement;
			labelDataGlobal = GlobalAlloc(0x42u, this->DataBufferSize);
		}
		this->LabelDataGlobal = labelDataGlobal;
	}
	if (offsetIncrement)
	{
		if (this->LabelOffsetsGlobal)
		{
			this->OffsetBufferSize += offsetIncrement;
			labelOffsetsGlobal = GlobalReAlloc(this->LabelOffsetsGlobal, 4 * this->OffsetBufferSize, 0x42u);
		}
		else
		{
			this->OffsetBufferSize = offsetIncrement;
			labelOffsetsGlobal = GlobalAlloc(0x42u, 4 * this->OffsetBufferSize);
		}
		this->LabelOffsetsGlobal = labelOffsetsGlobal;
	}
}

// .text:00412030
void mc::LabelInfo::MoveData(int labelIndex, int offset)
{
	char* ptr; // ST1C_4 MAPDST
	int absoluteOffset; // [esp+0h] [ebp-14h]
	signed int existingEntryLength; // [esp+Ch] [ebp-8h]
	int i; // [esp+1Ch] [ebp+8h] MAPDST

	if (offset && labelIndex >= 0 && labelIndex < this->LabelCount)
	{
		existingEntryLength = this->GetEntryLength(&this->LabelData[this->LabelOffsets[labelIndex]]);
		if (offset >= 0)
		{
			// Move in positive direction
			while (offset + this->LabelDataPtr > this->DataBufferSize)
			{
				this->Unlock();
				this->GrowBuffers(4096, 0);
				this->Lock();
			}
			if (labelIndex == this->LabelCount)
			{
				this->LabelDataPtr += offset;
			}
			else
			{
				ptr = &this->LabelData[this->LabelOffsets[labelIndex]];
				memcpy(&ptr[offset], ptr, &this->LabelData[this->LabelDataPtr] - ptr);
				this->LabelDataPtr += offset;
				// Fix up offset table
				for (i = labelIndex + 1; i < this->LabelCount; ++i)
					this->LabelOffsets[i] += offset;
			}
		}
		else
		{
			// Move in negative direction
			if (existingEntryLength >= -offset)
				absoluteOffset = -offset;
			else
				absoluteOffset = existingEntryLength;
			if (labelIndex == this->LabelCount - 1)
			{
				this->LabelDataPtr -= absoluteOffset;
			}
			else
			{
				ptr = &this->LabelData[this->LabelOffsets[labelIndex] + existingEntryLength];
				memcpy(&ptr[-absoluteOffset], ptr, &this->LabelData[this->LabelDataPtr] - ptr);
				this->LabelDataPtr -= absoluteOffset;
				// Fix up offset table
				for (i = labelIndex + 1; i < this->LabelCount; ++i)
					this->LabelOffsets[i] -= absoluteOffset;
			}
		}
	}
}

// .text:00412240
int mc::LabelInfo::FindLabelIndex(const char* labelName)
{
	int index; // [esp+4h] [ebp-4h]

	index = this->FindInsertLocation(labelName);
	if (index < 0 || index >= this->LabelCount)
		return -1;
	if (!strcmp(labelName, &this->LabelData[this->LabelOffsets[index]]))
		return index;
	return -1;
}

// .text:004122A0
char* mc::LabelInfo::GetLabelLocation(char* locationBuffer, const char* labelName)
{
	char* label; // ST10_4
	size_t len; // eax
	char* result; // eax
	int index; // [esp+4h] [ebp-8h]

	index = this->FindLabelIndex(labelName);
	if (index < 0)
	{
		result = locationBuffer;
		*locationBuffer = 0;
	}
	else
	{
		label = &this->LabelData[this->LabelOffsets[index]];
		len = strlen(label);
		result = strcpy(locationBuffer, &label[len + 1]);
	}
	return result;
}

// .text:00412310
int mc::LabelInfo::Insert(const char* labelName, const char* locationString)
{
	size_t nameLength; // ST14_4
	size_t existingEntryLength; // ST10_4
	size_t entryLength; // [esp+8h] [ebp-100Ch]
	char entryBuffer[4096]; // [esp+Ch] [ebp-1008h]
	int insertLocation; // [esp+1010h] [ebp-4h]

	strcpy(entryBuffer, labelName);
	nameLength = strlen(labelName) + 1;
	strcpy(&entryBuffer[nameLength], locationString);
	entryLength = nameLength + strlen(locationString) + 1;
	insertLocation = this->FindInsertLocation(labelName);
	if (!strcmp(labelName, &this->LabelData[this->LabelOffsets[insertLocation]]))
	{
		existingEntryLength = this->GetEntryLength(&this->LabelData[this->LabelOffsets[insertLocation]]);
		this->MoveData(insertLocation, entryLength - existingEntryLength);
		memcpy(&this->LabelData[this->LabelOffsets[insertLocation]], entryBuffer, entryLength);
	}
	else
	{
		if (this->LabelCount >= this->OffsetBufferSize)
		{
			this->Unlock();
			this->GrowBuffers(0, 256);
			this->Lock();
		}
		if (insertLocation >= this->LabelCount)
		{
			while ((signed int)(entryLength + this->LabelDataPtr) > this->DataBufferSize)
			{
				this->Unlock();
				this->GrowBuffers(4096, 0);
				this->Lock();
			}
			this->LabelOffsets[insertLocation] = this->LabelDataPtr;
			this->LabelDataPtr += entryLength;
			++this->LabelCount;
		}
		else
		{
			memcpy(
				&this->LabelOffsets[insertLocation + 1],
				&this->LabelOffsets[insertLocation],
				4 * (this->LabelCount - insertLocation));
			++this->LabelCount;
			this->MoveData(insertLocation, entryLength);
			this->LabelOffsets[insertLocation] = this->LabelOffsets[insertLocation + 1] - entryLength;
		}
		memcpy(&this->LabelData[this->LabelOffsets[insertLocation]], entryBuffer, entryLength);
	}
	return insertLocation;
}
