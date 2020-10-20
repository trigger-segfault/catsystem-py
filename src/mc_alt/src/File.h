#pragma once
#include <Windows.h>

namespace mc {

	class File sealed
	{
	public:
		HANDLE FileHandle;
		HANDLE FindFileHandle;
		int DesiredAccess;
		int ShareMode;
		int CreationDisposition;
		int FlagsAndAttributes;
		HANDLE TemplateFile;
		int field_1C;
		unsigned long BytesWritten;
		char Path[1024];
		int field_424;
		WIN32_FIND_DATAA FindData;
		SYSTEMTIME SystemTime;
		FILETIME FileTime;
		int field_580;
		int field_584;
		int field_588;
		int field_58C;
		int field_590;
		int field_594;

		File(const char* path);
		~File();

		void CloseFindHandle();
		BOOL LoadFileTimes();
		size_t GetSize();
		size_t Write(const char* buffer, size_t size);
		void CloseHandles();
		BOOL Open();
		void PrepareWrite();
		void PrepareRead();
		signed int FindFileAttributes();
		char* GetFileName();
		unsigned int GetUpdateTime();
		BOOL Delete();
		void PrepareWrite2();
		signed int GetNextFileAttributes();
		char* GetExtension();
		void ChangeExtension(const char* newExt);
	};
}
