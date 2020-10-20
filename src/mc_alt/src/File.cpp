#include "File.h"

#include "mc.h"

// .text:004137F0
mc::File::File(const char* path)
{
	static_assert(sizeof(File) == 0x00000598, "sizeof(File) is incorrect");
	size_t pathLength; // [esp+0h] [ebp-Ch]

	// This historic landmark is probably the only
	// bounds check in the entire program!
	if (strlen(path) >= 1023)
		pathLength = 1023;
	else
		pathLength = strlen(path);
	memcpy(this->Path, path, pathLength);
	this->Path[pathLength] = 0;
	this->FileHandle = nullptr;
	this->FindFileHandle = nullptr;
	this->DesiredAccess = GENERIC_WRITE | GENERIC_READ;
	this->ShareMode = 3;
	this->CreationDisposition = 3;
	this->FlagsAndAttributes = 128;
	this->TemplateFile = 0;
	this->field_424 = 0;
	++GlobalOpenFileCount;
}

// .text:004137D0
mc::File::~File()
{
	this->CloseHandles();
	--GlobalOpenFileCount;
}

// .text:004134F0
void mc::File::CloseFindHandle()
{
	if (this->FindFileHandle)
	{
		FindClose(this->FindFileHandle);
		this->FindFileHandle = nullptr;
	}
}

// .text:00413520
BOOL mc::File::LoadFileTimes()
{
	BOOL success; // [esp+4h] [ebp-20h]
	HANDLE hFile; // [esp+8h] [ebp-1Ch]
	struct _FILETIME CreationTime; // [esp+Ch] [ebp-18h]
	struct _FILETIME LastWriteTime; // [esp+14h] [ebp-10h]
	struct _FILETIME LastAccessTime; // [esp+1Ch] [ebp-8h]

	if (this->FileHandle)
	{
		hFile = this->FileHandle;
	}
	else
	{
		hFile = CreateFileA(this->Path, GENERIC_READ, this->ShareMode & 1, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;
	}
	success = GetFileTime(hFile, &CreationTime, &LastAccessTime, &LastWriteTime);
	if (!this->FileHandle)
		CloseHandle(hFile);
	if (success == TRUE)
	{
		FileTimeToLocalFileTime(&LastWriteTime, &this->FileTime);
		FileTimeToSystemTime(&this->FileTime, &this->SystemTime);
	}
	return success;
}

// .text:004135E0
size_t mc::File::GetSize()
{
	DWORD FileSizeHigh; // [esp+4h] [ebp-8h]
	HANDLE hFile; // [esp+8h] [ebp-4h]

	if (this->FileHandle)
	{
		hFile = this->FileHandle;
	}
	else
	{
		hFile = CreateFileA(this->Path, GENERIC_READ, this->ShareMode & 1, 0, 3u, 0x80u, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return -1;
	}
	FileSizeHigh = GetFileSize(hFile, &FileSizeHigh);
	if (!this->FileHandle)
		CloseHandle(hFile);
	return FileSizeHigh;
}

// .text:00413660
size_t mc::File::Write(const char* buffer, size_t size)
{
	if (!this->FileHandle)
		return 0;
	if (WriteFile(this->FileHandle, buffer, size, &this->BytesWritten, nullptr))
		return this->BytesWritten;
	return 0;
}

// .text:004136B0
void mc::File::CloseHandles()
{
	if (this->FindFileHandle)
	{
		FindClose(this->FindFileHandle);
		this->FindFileHandle = nullptr;
	}
	if (this->FileHandle)
	{
		CloseHandle(this->FileHandle);
		this->FileHandle = nullptr;
	}
}

// .text:00413710
BOOL mc::File::Open()
{
	this->FileHandle = CreateFileA(
		this->Path,
		this->DesiredAccess,
		this->ShareMode,
		nullptr,
		this->CreationDisposition,
		this->FlagsAndAttributes,
		this->TemplateFile);
	if (this->FileHandle != INVALID_HANDLE_VALUE)
		return true;
	this->FileHandle = nullptr;
	return false;
}

// .text:00413770
void mc::File::PrepareWrite()
{
	this->DesiredAccess = GENERIC_WRITE;
	this->ShareMode &= FILE_SHARE_WRITE;
	this->CreationDisposition = CREATE_ALWAYS;
}

// .text:004137A0
void mc::File::PrepareRead()
{
	this->DesiredAccess = GENERIC_READ;
	this->ShareMode &= FILE_SHARE_READ;
	this->CreationDisposition = OPEN_EXISTING;
}

// .text:004138C0
signed int mc::File::FindFileAttributes()
{
	if (this->FindFileHandle)
	{
		if (!FindNextFileA(this->FindFileHandle, &this->FindData))
		{
			this->CloseFindHandle();
			return -1;
		}
	}
	else
	{
		this->FindFileHandle = FindFirstFileA(this->Path, &this->FindData);
		if (this->FindFileHandle == INVALID_HANDLE_VALUE)
		{
			this->CloseFindHandle();
			return -1;
		}
	}
	strcpy(this->Path, this->FindData.cFileName);
	return this->FindData.dwFileAttributes;
}

// .text:00413960
char* mc::File::GetFileName()
{
	char* slash; // esi
	char* backslash; // [esp+4h] [ebp-10h] MAPDST
	char* space; // [esp+Ch] [ebp-8h]

	space = strchr(this->Path, ' ');
	if (space)
		*space = '\0';
	if (!this->Path[0])
		return this->Path;
	slash = strrchr(this->Path, '/');
	if (slash <= strrchr(this->Path, '\\'))
		backslash = strrchr(this->Path, '\\');
	else
		backslash = strrchr(this->Path, '/');
	if (backslash || (backslash = strchr(this->Path, ':')) != 0)
	{
		if (space)
			*space = ' ';
		return backslash + 1;
	}
	else
	{
		if (space)
			*space = ' ';
		return this->Path;
	}
}

// .text:00413A50
unsigned mc::File::GetUpdateTime()
{
	if (this->LoadFileTimes())
		return this->SystemTime.wSecond & 63 | ((this->SystemTime.wMinute & 63 | ((this->SystemTime.wHour & 31 | 32 * (this->SystemTime.wDay & 31 | 32 * (this->SystemTime.wMonth & 0xF | 16 * (((unsigned __int8)this->SystemTime.wYear + 0x44) & 0x3F)))) << 6)) << 6);
	else
		return 0;
}

// .text:00413B00
BOOL mc::File::Delete()
{
	this->CloseHandles();
	return DeleteFileA(this->Path);
}

// .text:00413B20
void mc::File::PrepareWrite2()
{
	// What is the purpose of this method?
	this->PrepareWrite();
}

// .text:00413B40
signed int mc::File::GetNextFileAttributes()
{
	signed int attrib; // [esp+4h] [ebp-4h]

	do
		attrib = this->FindFileAttributes();
	while (attrib >= 0 && attrib & 0x110);
	return attrib;
}

// .text:00413B70
char* mc::File::GetExtension()
{
	char* name; // eax
	char* result; // eax
	char* period; // [esp+4h] [ebp-4h]

	name = (char*)this->GetFileName();
	period = strrchr(name, '.');
	if (period)
		result = period + 1;
	else
		result = 0;
	return result;
}

// .text:00413BB0
void mc::File::ChangeExtension(const char* newExt)
{
	size_t pathLength; // eax
	char* oldExt; // [esp+4h] [ebp-4h]

	oldExt = this->GetExtension();
	if (!oldExt)
	{
		pathLength = strlen(this->Path);
		this->Path[pathLength] = '.';
		oldExt = &this->Path[pathLength + 1];
	}
	if (newExt && *newExt)
		strcpy(oldExt, newExt);
	else
		*(oldExt - 1) = 0;
}
