
// OOP-only file, functional decompiled versions for ScriptReader
//  are in "TokenParser_functional.*"

#include "common.h"
#ifdef KCLIB_OOP

#include "ScriptReader.h"
#include "kclib_common.h"


///FID:cs2_full_v401/tool/ac.exe: FUN_00411c40
kclib::ScriptReader::ScriptReader()
{
    this->Buffer = nullptr;
    this->Position = 0;
    this->RdrUnk3 = 0; // never seen elsewhere
}

kclib::ScriptReader::~ScriptReader()
{
    this->Close();
}

//undefined4 __thiscall FUN_00411dd0(void *this,char *filename)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411dd0
bool kclib::ScriptReader::OpenFile(IN const char *filename)
{
    if (this->Buffer != nullptr)
    {
        this->Close();
    }

    // int length = 1;
    //DAT_004c3430 is likely kcFileServer,
    // or whatever id used to manage integration files
    ///TMP:IGNORE: int length = kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, 0, 3); // 3 may be some identifier type, like to open as ScriptDecode2 class, etc. Or maybe flags
    FILE *file = std::fopen(filename, "rb");
    if (file == nullptr)
        return false;

    std::fseek(file, 0, SEEK_END);
    unsigned int length = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    if (length != 0U)
    {
        char *buffer = new char[length + 8U]; //char *buffer = (char *)kclib_HeapAlloc(0, length + 8U);
        this->Buffer = buffer; //*(undefined8 **)this = buffer;
        std::memset(buffer, 0, length + 8U); //kclib_MemZero(buffer, length + 8U);

        ///TMP:IGNORE: kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, *(int *)this, 3);
        if (std::fread(this->Buffer, length, 1, file) != 0)
        {
            std::fclose(file);
            this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

            this->ConvertToLF();
            
            this->Length = std::strlen(this->Buffer);
            return true;
        }
        else
        {
            std::fclose(file);
            delete[] this->Buffer;
            this->Buffer = nullptr;
            return false;
        }
    }
    std::fclose(file);
    return false;
    
    // char *buffer = new char[length + 8U]; //char *buffer = (char *)kclib_HeapAlloc(0, length + 8U);
    // this->Buffer = buffer; //*(undefined8 **)this = buffer;
    // std::fread()
    // std::memset(buffer, 0, length + 8U); //kclib_MemZero(buffer, length + 8U);
    
    
    // this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

    // this->ConvertToLF();
    
    // this->Length = std::strlen(this->Buffer);
    // return true;
}
bool kclib::ScriptReader::OpenFile(IN const wchar_t *filename)
{
    if (this->Buffer != nullptr)
    {
        this->Close();
    }

    // int length = 1;
    //DAT_004c3430 is likely kcFileServer,
    // or whatever id used to manage integration files
    ///TMP:IGNORE: int length = kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, 0, 3); // 3 may be some identifier type, like to open as ScriptDecode2 class, etc. Or maybe flags
    FILE *file = _wfopen(filename, L"rb");
    if (file == nullptr)
        return false;

    std::fseek(file, 0, SEEK_END);
    unsigned int length = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    if (length != 0U)
    {
        char *buffer = new char[length + 8U]; //char *buffer = (char *)kclib_HeapAlloc(0, length + 8U);
        this->Buffer = buffer; //*(undefined8 **)this = buffer;
        std::memset(buffer, 0, length + 8U); //kclib_MemZero(buffer, length + 8U);

        ///TMP:IGNORE: kcBigFile_unkLookupOpenMessage(DAT_004c3430, filename, *(int *)this, 3);
        if (std::fread(this->Buffer, length, 1, file) != 0)
        {
            std::fclose(file);
            this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

            this->ConvertToLF();
            
            this->Length = std::strlen(this->Buffer);
            return true;
        }
        else
        {
            std::fclose(file);
            delete[] this->Buffer;
            this->Buffer = nullptr;
            return false;
        }
    }
    std::fclose(file);
    return false;
    
    // char *buffer = new char[length + 8U]; //char *buffer = (char *)kclib_HeapAlloc(0, length + 8U);
    // this->Buffer = buffer; //*(undefined8 **)this = buffer;
    // std::fread()
    // std::memset(buffer, 0, length + 8U); //kclib_MemZero(buffer, length + 8U);
    
    
    // this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

    // this->ConvertToLF();
    
    // this->Length = std::strlen(this->Buffer);
    // return true;
}

//void __fastcall FUN_00411c50(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c50
void kclib::ScriptReader::Close()
{
    if (this->Buffer != nullptr)
    {
        delete[] this->Buffer; //kclib_HeapFree(this->Buffer);
        this->Buffer = nullptr; // null assignment added, not seen in assembly
    }
}

bool kclib::ScriptReader::IsOpen() const
{
    return this->Buffer != nullptr;
}

const char * kclib::ScriptReader::GetBuffer() const
{
    return this->Buffer;
}

//undefined4 __thiscall FUN_004107e0(void *this,undefined8 *param_1)
///FID: FUN_004107e0
bool kclib::ScriptReader::OpenBuffer(IN const char *newBuffer)
{
    if (this->Buffer != nullptr)
    {
        this->Close();
    }

    if (newBuffer == nullptr)
        return false;

    unsigned int length = std::strlen(newBuffer);
    if (length != 0U)
    {
        char *buffer = new char[length + 8U]; //char *buffer = (char *)kclib_HeapAlloc(0, length + 8U);
        this->Buffer = buffer; //*(undefined8 **)this = buffer;
        std::memset(buffer, 0, length + 8U); //kclib_MemZero(buffer, length + 8U);

        std::memcpy(this->Buffer, newBuffer, length);

        this->Position = 0; //*(undefined4 *)((int)this + 8) = 0;

        this->ConvertToLF();
        
        this->Length = std::strlen(this->Buffer);
        return true;
    }
    return false;
}

//uint __fastcall FUN_00411d40(int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d40
bool kclib::ScriptReader::IsEOF() const
{
    return this->Position >= this->Length;
}

//void __thiscall FUN_00411cf0(void *this,int param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cf0
void kclib::ScriptReader::NextLine(OPTIONAL OUT char *lineBuffer)
{
    // lineBuffer is optional, if excluded, decoder still advances to next line

    int linepos = 0;
    while (this->Position < this->Length)
    {
        char c = this->Buffer[this->Position];
        if (c == '\n')
        {
            this->Position++;
            break;
        }
        if (lineBuffer != nullptr)
        {
            lineBuffer[linepos] = c;
        }
        this->Position++;
        linepos++;
    }
    if (lineBuffer != nullptr)
    {
        lineBuffer[linepos] = '\0';
    }
}

// Used to strip CRLF from script?
//void __fastcall FUN_00411d50(char **param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411d50
void kclib::ScriptReader::ConvertToLF()
{
    if (this->Buffer == nullptr)
        return;

    unsigned int str_len = (unsigned int)std::strlen(this->Buffer);

    // temporary heap to hold script decoder buffer,
    // it seems this may be a unnecessary, since the write length
    //   is at least shorter and most equal in length,
    //   and wont override read position.
    //   at most this may be done for the double-null termination??
    char *puHeap = new char[str_len + 1]; //char *puHeap = (char *)kclib_HeapAlloc(0, str_len + 1); // +1 for double-null termination
    std::memcpy(puHeap, this->Buffer, str_len);

    // replace CRLF ("\r\n") with LF ("\n")
    unsigned int j = 0; // output position
    for (unsigned int i = 0; i < str_len; i++, j++)
    {
        if (puHeap[i] == '\r' && puHeap[i + 1] == '\n') // CRLF "\r\n"
        {
            this->Buffer[j] = '\n'; // LF "\n"
            i++;
        }
        else
        {
            this->Buffer[j] = puHeap[i];
        }
    }

    // double-null terminate it seems
    this->Buffer[j] = '\0';
    this->Buffer[j + 1] = '\0';
    delete[] puHeap; //kclib_HeapFree(puHeap);
}

unsigned int kclib::ScriptReader::GetLength() const
{
    if (this->Buffer == nullptr)
        return 0U;

    return this->Length;
}

unsigned int kclib::ScriptReader::GetPosition() const
{
    if (this->Buffer == nullptr)
        return 0U;

    return this->Position;
}

//void __thiscall FUN_00411cd0(void *this,uint param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411cd0
void kclib::ScriptReader::SetPosition(unsigned int newPosition)
{
    if (newPosition >= this->Length)
    {
        newPosition = this->Length;
    }
    this->Position = newPosition;
}

//uint __fastcall FUN_00411c60(int *param_1)
///FID:cs2_full_v401/tool/ac.exe: FUN_00411c60
unsigned int kclib::ScriptReader::FUN_00411c60()
{
    if (this->Position >= this->Length)
        return 0U;

    if (kclib::IsCharDoubleByte(this->Buffer + this->Position))
    {
        unsigned int c1 = (unsigned int)this->Buffer[this->Position] & 0xff;
        unsigned int c2 = (unsigned int)this->Buffer[this->Position + 1] & 0xff;
        this->Position += 2;
        return (this->Position & 0xffff0000) | (c2 << 8 | c1);

        // CONCAT31(x,y) - concatenates two operands together into a larger size object
        // The "3" is the size of x in bytes.
        // The "1" is the size of y in bytes.
        // The result is the 4-byte concatenation of the bits in "x" with the bits in "y". The "x" forms the most signifigant part of the result, "y" the least.
        //return (this->Position & 0xffff0000) | (unsigned int)CONCAT11(c2, c1);
    }
    else
    {
        char c = this->Buffer[this->Position];
        this->Position++;
        return (this->Position & 0xffff0000) | (unsigned int)c;
    }
}

#endif
