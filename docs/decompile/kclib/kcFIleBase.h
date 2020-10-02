

#ifndef KCLIB_KCFILEBASE_H
#define KCLIB_KCFILEBASE_H

#include "kcBaseClass.h"

typedef unsigned long BOOL;
typedef unsigned long DWORD;
typedef void* HANDLE;
#define TRUE ((BOOL) 1)
#define FALSE ((BOOL) 0)

//#define BOOL unsigned long

// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
// https://www.pinvoke.net/default.aspx/kernel32.CreateFile

enum KCFILE {
  KCFILE_READ  = 0x0001,  // dwDesiredAccess = 0x80000000
  KCFILE_WRITE = 0x0002,  // dwDesiredAccess = 0x40000000
  KCFILE_READWRITE = 0x0003, // MASK ONLY
  KCFILE_OPEN_EXISTING = 0x0100,  // dwCreationDisposition = 3
  KCFILE_CREATE_NEW = 0x0200,  // dwCreationDisposition |= 1
  KCFILE_DIRECTORY = 0x0400,  // dwFlagsAndAttributes = 0x80 (if KCFILE_WRITE)
  KCFILE_OPEN_ALWAYS = 0x0800,  // dwCreationDisposition |= 4
  KCFILE_CREATE,  
  KCFILE_DIRECTORY,
  // dwFlagsAndAttributes = 0x80
};

enum FILE_CREATION
{
  FILE_CREATION_NONE = 0,
  CREATE_NEW = 1,
  CREATE_ALWAYS = 2,
  OPEN_EXISTING = 3,
  OPEN_ALWAYS = 4,
  TRUNCATE_EXISTING = 5,
};

enum FILE_ACCESS
{
  FILE_ACCESS_NONE = 0,
  GENERIC_READ = 0x80000000,
  GENERIC_WRITE = 0x40000000,
  GENERIC_EXECUTE = 0x20000000,
  GENERIC_ALL = 0x10000000,
};

enum FILE_ATTRIBUTE
{
  FILE_ATTRIBUTE_NONE = 0,
  FILE_ATTRIBUTE_READONLY = 0x1,
  FILE_ATTRIBUTE_HIDDEN = 0x2,
  FILE_ATTRIBUTE_SYSTEM = 0x4,
  FILE_ATTRIBUTE_DIRECTORY = 0x10,
  FILE_ATTRIBUTE_ARCHIVE = 0x20,
  FILE_ATTRIBUTE_DEVICE = 0x40,
  FILE_ATTRIBUTE_NORMAL = 0x80,
  FILE_FLAG_DELETE_ON_CLOSE = 0x04000000,
};

enum FILE_SHARE
{
  FILE_SHARE_NONE = 0,
  FILE_SHARE_READ = 0x00000001,
  FILE_SHARE_WRITE = 0x00000002,
  FILE_SHARE_DELETE = 0x00000004,
};

class kcFileBase : kcClassBase
{
protected:
  unsigned long flags; // 0x4
  void* handle; // 0x8
  wchar_t filename[256]; // 0xc - 0x20c

  unsigned long position; // 0x20c

public:
  // vftable[1]
  virtual BOOL Open(wchar_t* filename, KCFILE flags)
  {
    return FALSE; // abstract
  }
  // vftable[2]
  virtual BOOL Close()
  {
    return FALSE; // abstract
  }
};

// Filesystem reader/writer (with static methods)
class kcFile : kcFileBase
{
  BOOL Open(wchar_t* filename, KCFILE flags)
  {
    DWORD dwCreationDisposition;
    HANDLE hFile;
    DWORD dwDesiredAccess;
    DWORD dwFlagsAndAttributes;
    if ((unsigned long) this->handle != -1)
    {
      // ERROR, already open
    }
    if ((flags & KCFILE_READWRITE) == 0)
    {
      // ERROR, must specify KCFILE_READ or KCFILE_WRITE
    }
    BOOL readMode = flags & KCFILE_READ;
    if (readMode == FALSE)
    {
      dwDesiredAccess = GENERIC_WRITE;
    }
    else
    {
      dwDesiredAccess = GENERIC_READ;
    }
    dwCreationDisposition = 0;
    if ((flags & KCFILE_OPEN_EXISTING) != 0)
    {
      dwCreationDisposition |= OPEN_EXISTING;
    }
    if ((flags & KCFILE_CREATE_NEW) != 0)
    {
      dwCreationDisposition |= CREATE_NEW;
    }
    if ((flags & KCFILE_OPEN_ALWAYS) != 0)
    {
      dwCreationDisposition |= OPEN_ALWAYS;
    }
    if ((flags & KCFILE_DIRECTORY) == 0)
    {
      if (readMode == FALSE)
      {
        dwFlagsAndAttributes = FILE_ATTRIBUTE_DIRECTORY;
      }
      else
      {
        dwFlagsAndAttributes = 0;
      }
    }
    else
    {
      // Create Directory
      if (dwDesiredAccess != GENERIC_WRITE)
      {
        // ERROR, not accessible with KCFILE_READ
      }
      hFile = CreateFileW(filename, GENERIC_WRITE, NULL, (LPSECURITY_ATTRIBUTES) NULL, CREATE_NEW, FILE_ATTRIBUTE_DIRECTORY, (HANDLE) NULL);
      this->handle = hFile;
      if (hFile != (HANDLE) (0xffffffff))
      {
        // GOTO ()
      }
      dwFlagsAndAttributes = FILE_ATTRIBUTE_DIRECTORY;
      dwCreationDisposition = TRUNCATE_EXISTING;
      dwDesiredAccess = GENERIC_WRITE;
    }
    hFile = CreateFileW(filename, dwDesiredAccess, (readMode ? FILE_SHARE_READ : FILE_SHARE_NONE), (LPSECURITY_ATTRIBUTES) NULL, dwCreationDisposition, dwFlagsAndAttributes, (HANDLE) NULL);
    this->handle = hFile;
    if (hFile == (HANDLE) (0xffffffff))
    {
      return FALSE;
    }
    lstrcpyW(&this->filename[0], filepath);
    this->flags = flags;
    this->field0x20c = 0; // Position?
    return TRUE;
  }
  // vftable[2]
  virtual BOOL Close()
  {
    BOOL result;

    if (this->handle == (HANDLE) (0xffffffff))
    {
      return FALSE;
    }
    result = CloseHandle(this->handle);
    if (result != FALSE)
    {
      this->flags = 0xffffffff;
      this->handle = (HANDLE) 0xffffffff;
      //this->filename clear
      //lstrcpyW(this->filename, (LPCWSTR) &PTR_00898318);
      lstrcpyW(this->filename, (LPCWSTR) NULL); // ???
      this->position = 0;
    }
    return result;
  }
  // vftable[3]
  virtual DWORD Read(unsigned char* buf, unsigned long numBytes)
  {
    BOOL result;
    DWORD bytesRead;
    if (this->handle == (HANDLE) 0xffffffff)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    if ((this->flags & KCFILE_READ) == 0)
    {
      // ERROR, not readable
    }
    result = ReadFile(this->handle, buf, numBytes, &bytesRead, (LPOVERLAPPED) NULL);
    if (result == FALSE)
    {
      // WARNING? Read failed (log?)
      return 0xffffffff;
    }
    this->position += bytesRead;
    return bytesRead;
  }
  // vftable[4]
  virtual DWORD Write(unsigned char* buf, unsigned long numBytes)
  {
    BOOL result;
    DWORD bytesWritten;
    if (this->handle == (HANDLE) 0xffffffff)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    if ((this->flags & KCFILE_WRITE) == 0)
    {
      // ERROR, not writeable
    }
    result = WriteFile(this->handle, buf, numBytes, &bytesWritten, (LPOVERLAPPED) NULL);
    if (result == FALSE)
    {
      // WARNING? Write failed (log?)
      return 0xffffffff;
    }
    this->position += bytesWritten;
    return bytesWritten;
  }
  // vftable[5]
  virtual DWORD GetPosition()
  {
    if (this->handle == (HANDLE) 0xffffffff)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    return this->position;
  }
  // vftable[6]
  virtual DWORD SetPosition(unsigned long position)
  {
    DWORD fileSize, filePosition;
    if (this->handle == (HANDLE) 0xffffffff)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    fileSize = GetFileSize(this->handle, (LPDWORD) NULL);
    if (fileSize <= position)
    {
      position = filesize - 1;
    }
    filePosition = SetFilePointer(this->handle, position, (PLONG) NULL, 0);
    this->position = filePosition;
    return filePosition;
  }
  // vftable[7]
  virtual DWORD Unknown_vf7(void)
  {
    return 0;
  }
  // vftable[9]
  virtual DWORD GetSize(unsigned long position)
  {
    DWORD fileSize;
    if (this->handle == (HANDLE) 0xffffffff)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    fileSize = GetFileSize(this->handle, (LPDWORD) NULL);
    if (fileSize == 0xffffffff)
    {
      fileSize = 0xffffffff
    }
    return fileSize;
  }
  // vftable[10]
  virtual HANDLE GetHandle()
  {
    return this->handle;
  }
};

class kcMemFile : kcFileBase
{
  // unsigned long position; // 0x20c
  void* memBuffer; // 0x210
  unsigned long memSize; // 0x214
  unsigned long field0x21c; // 0x21c (== 0)
public:
  // vftable[5]
  virtual DWORD GetPosition()
  {
    if (this->memBuffer == NULL) // -1?? (BUG)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    return this->position;
  }
  // vftable[6]
  virtual DWORD SetPosition(unsigned long position)
  {
    DWORD fileSize, filePosition;
    if (this->memBuffer == NULL)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    if (this->memSize <= position)
    {
      position = this->memSize - 1;
    }
    this->position = position;
    return position;
  }
  // vftable[9]
  virtual DWORD GetSize(unsigned long position)
  {
    DWORD fileSize;
    if (this->memBuffer == NULL)
    {
      // ERROR, not open
      return 0xffffffff;
    }
    return this->memSize;
  }
};

// KIF archive reader/writer
class kcBigFile : kcFileBase
{

};

#endif  /* KCLIB_KCFILEBASE_H */
