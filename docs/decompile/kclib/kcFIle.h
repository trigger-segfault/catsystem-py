
#define IN
#define OUT
#define OPTIONAL
typedef int BOOL;


// kcFile open mode flags (must specify READ or WRITE, and one creation disposition)
enum KCFILE_MODE
{
    // file access (specify one, names have been confirmed)
    KCFILE_READ  = 0x1, //GENERIC_READ, FILE_SHARE_READ
    KCFILE_WRITE = 0x2, //GENERIC_WRITE, 0
    // _KCFILE_READWRITE = 0x3, //(helper enum, not used)

    // creation disposition (specify one, names are unconfirmed)
    KCFILE_OPEN     = 0x100, //OPEN_EXISTING
    KCFILE_NEW      = 0x200, //CREATE_NEW
    KCFILE_CREATE   = 0x400, //CREATE_NEW then TRUNCATE_EXISTING, requires KCFILE_WRITE
    KCFILE_ALWAYS   = 0x800, //OPEN_ALWAYS

    // This enum should include special mode flags for kcBigFile, kcMemFile doesn't use any... I think
};


class kcFileBaseA
{
    // /*$0,4*/    void* _vftable;
    // /*$4*/

public:
    kcFileBaseA();
    ~kcFileBaseA(); // + scalar dtor

    /// VIRTUAL ///

    virtual bool Open(IN const char *filename, KCFILE_MODE mode); // 0 on failure
    virtual bool Close(); // 0 on failure
    virtual unsigned int Read(OUT unsigned char *outBuffer, unsigned int numBytes); // 0xffffffff (-1) on failure
    virtual unsigned int Write(IN const unsigned char *inBuffer, unsigned int numBytes); // 0xffffffff (-1) on failure
    virtual unsigned int GetPosition() const; // 0xffffffff (-1)
    virtual unsigned int SetPosition(unsigned int newPosition); // 0xffffffff (-1) on failure
    virtual const char * GetFilename(OPTIONAL OUT char * optoutFilename) const; // NULL on failure

    virtual unsigned int GetFileSize(IN const char *filename) const; // 0 on failure // static

    virtual unsigned int GetSize() const; // 0xffffffff on failure
    virtual void * GetHandle() const; // 0xffffffff (-1) on failure
    virtual bool FileExists(IN const char *filename) const; // 0 on failure // static

    virtual unsigned int GetFileAttributes(IN const unsigned char *filename) const; // 0xffffffff (-1) on failure // static
    virtual void SetAttributes(unsigned int Attributes); // Literally WINAPI SetFileAttributes (for kcFile anyway)
    virtual bool DeleteFile(IN const char *filename) const; // instance, 0 on failure
};

class kcFileBaseW
{
    // /*$0,4*/    void* _vftable;
    // /*$4*/

public:
    kcFileBaseW();
    ~kcFileBaseW(); // + scalar dtor

    /// VIRTUAL ///

    // virtual functions not-yet mapped for UTF-8 unicode support Cs2, but they do vary a bit from the older ac.exe
};

class kcFileA : kcFileBaseA
{
    // /*$0,4*/    void* _vftable;
    /*$4,4*/    KCFILE_MODE Mode; // open mode
    /*$8,4*/    void* Handle; // file handle (HANDLE)
    /*$c,100*/  char Filename[256]; // name of current open file
    /*$10c,4*/  unsigned int Position; // file position
    /*$110*/

public:
    kcFileA();
    ~kcFileA();

    /// OVERLOADS ///
};

class kcFileW : kcFileBaseW
{
    // /*$0,4*/    void* _vftable;
    /*$4,4*/    KCFILE_MODE Mode; // open mode
    /*$8,4*/    void* Handle; // file handle (HANDLE)
    /*$c,200*/  wchar_t Filename[256]; // name of current open file
    /*$20c,4*/  unsigned int Position; // file position
    /*$210*/

public:
    kcFileW();
    ~kcFileW();

    /// OVERLOADS ///
};

class kcMemFileA : kcFileBaseA
{
    // /*$0,4*/    void *_vftable;
    /*$4,4*/    KCFILE_MODE MemUnk1; // open mode? (used by kcFile)
    /*$8,4*/    unsigned int MemUnk2; // HANDLE? (used by kcFile)
    /*$c,100*/  char Filename[256]; // Copied from vftable[7]->GetFileName()
    /*$10c,4*/  unsigned int Position;
    /*$110,4*/  void *MemoryBuffer; //lpMem *(int *)((char *)lpMem - 4) is ref count
    /*$114,4*/  unsigned int Size;
    /*$118,4*/  unsigned int MemUnk70;
    /*$11c,4*/  int MemUnk71; // reference count?
    /*$120*/

public:
    kcMemFileA();
    ~kcMemFileA();

    /// OVERLOADS ///
};

class kcMemFileW : kcFileBaseW
{
    // /*$0,4*/    void *_vftable;
    /*$4,4*/    unsigned int MemUnk1; // open mode? (used by kcFile) - set to 0xffffffff on ctor
    /*$8,4*/    unsigned int MemUnk2; // HANDLE? (used by kcFile) - set to 0xffffffff on ctor
    /*$c,200*/  wchar_t Filename[256]; // Copied from vftable[7]->GetFileName() - copied to from empty string ptr on ctor
    /*$20c,4*/  unsigned int Position; // - set to 0 on ctor
    /*$210,4*/  void *MemoryBuffer; //lpMem *(int *)((char *)lpMem - 4) is ref count
    /*$214,4*/  unsigned int Size; // - set to 0 on ctor
    /*$218,4*/  unsigned int MemUnk70;
    /*$21c,4*/  int MemUnk71; // reference count? - set to 0 on ctor
    /*$220*/

public:
    kcMemFileW();
    ~kcMemFileW();

    /// OVERLOADS ///
};

class kcBigFileA : kcFileBaseA
{
    // /*$0,4*/    void *_vftable;
    /*$4,4*/    unsigned int BigUnk1; // assigned to ctor param_1 (ptr to kcFileBase??)
    /*$8,4*/    unsigned int BigUnk2; // 0 (ptr to kcFileBase?? NULL?)
    /*$c*/
    
public:
    kcBigFileA();
    ~kcBigFileA();

    /// OVERLOADS ///
};

// No difference in class, except internal functions
class kcBigFileW : kcFileBaseW
{
    // /*$0,4*/    void *_vftable;
    /*$4,4*/    unsigned int BigUnk1; // assigned to ctor param_1 (ptr to kcFileBase??)
    /*$8,4*/    unsigned int BigUnk2; // 0 (ptr to kcFileBase?? NULL?)
    /*$c*/
    
public:
    kcBigFileW();
    ~kcBigFileW();

    /// OVERLOADS ///
};