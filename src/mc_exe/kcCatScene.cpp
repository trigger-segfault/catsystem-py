
#include "common_mc.h"
///TEMP: Also run OOP class of kcFileInfo until we're done with the functional stuff
#ifndef KCLIB_OOP
#undef this
// #ifdef KCLIB_OOP

#include "kcCatScene.h"
#include "kcFileInfo.h"


//unsigned int * kcMacroReader_initRun(void *this, char *param_1)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004119a0
kclib::kcFileLineBuffer::kcFileLineBuffer(IN const char *filename)
{
    this->LineBuffer    = nullptr;
    this->MemoryLines   = nullptr;
    this->LineOffsets   = nullptr;
    this->MemoryOffsets = nullptr;
    this->BufferSize    = 0;
    this->LineCount     = 0;
    this->MacUnk0 = 0; // ???
    this->LastLineMultibyteContinue = FALSE;

    std::strcpy(&this->Filename[0], filename);
    this->Read(filename);
}

///CUSTOM: destructor not observed, but added anyway
kclib::kcFileLineBuffer::~kcFileLineBuffer()
{
    // dummy func (no cleanup, for now)
    ///FIXME: decide if we need to add cleanup here, for mc_tool
}

// undefined4 kclib::kcFileLineBuffer::Read(int this,char *filename)
///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114d0
bool kclib::kcFileLineBuffer::Read(IN const char *filename)
{
    HGLOBAL hMemLines = nullptr;
    HGLOBAL hMemOffsets = nullptr;
    char *lineBuffer = nullptr;
    unsigned int *lineOffsets = nullptr;
    unsigned int lineCount = 0;
    
    // all this just to read bufferSize... nice
    kcFileInfo *fileinfo = new kcFileInfo(filename);
    fileinfo->SetReadMode();
    unsigned int bufferSize = fileinfo->GetSize();
    delete fileinfo;

    // allocate MemoryLines and copy lines into buffer,
    //  nullterminated with newlines and control chars removed
    if ((int)bufferSize > 0)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        
        bufferSize += 0x10; // extra space, for unknown reasons. Could be useful? *shrug*
        FILE *file = nullptr; // text file (mode "rt")

        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemLines = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, bufferSize);

        if (hMemLines != nullptr)
        {
            lineBuffer = (char *)::GlobalLock(hMemLines);
        }

        if (lineBuffer != nullptr)
        {
            file = std::fopen(filename, "rt");
        }

        // reading file (final 'if' in setup)
        if (file != nullptr)
        {
            int offset = 0;
            while (!std::feof(file) && std::fgets(&lineBuffer[offset], bufferSize - offset, file))
            {
                // trim control chars from end of line (excluding whitespace)
                int length = (int)std::strlen(&lineBuffer[offset]);
                for (; length > 0; length--)
                {
                    if ((unsigned int)lineBuffer[offset + length - 1] >= 0x20 || lineBuffer[offset + length - 1] == '\t')
                        break;
                }
                lineBuffer[offset + length] = '\0';
                offset += length + 1; // +1 for null-termination
                lineCount++;
            }
            // cleanup file
            std::fclose(file);
        }

        // cleanup
        if (lineBuffer != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }
        // done
    }

    // No lines in the file? That's a paddlin' (return FALSE)
    if (lineCount < 1)
    {
        if (hMemLines != nullptr)
        {
            ::GlobalFree(hMemLines);
        }
        return false;
    }
    // allocate MemoryOffsets and add byte offsets of each line in MemoryLines (return TRUE)
    else //if (lineCount >= 1)
    {
        // the original source was likely a goto-failure control flow,
        //  decompiled it's just layers upon layers of if statements, so it has been cleaned up
        this->LineCount = lineCount;
        this->BufferSize = bufferSize;
        this->MemoryLines = hMemLines;
        lineBuffer = nullptr;
        lineOffsets = nullptr;
        //0x42 (GHND, GMEM_MOVEABLE | GMEM_ZEROINIT)
        hMemOffsets = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, lineCount * sizeof(unsigned int));
        if (hMemOffsets != nullptr)
        {
            lineBuffer = (char *)::GlobalLock(hMemLines);
        }
        if (lineBuffer != nullptr)
        {
            lineOffsets = (unsigned int *)::GlobalLock(hMemOffsets);
        }
        
        // reading file (final 'if' in setup)
        if (lineOffsets != nullptr)
        {
            // unsigned int offset = 0;
            for (unsigned int line = 0, offset = 0; line < this->LineCount; line++)
            {
                lineOffsets[line] = offset;
                offset += (unsigned int)std::strlen(&lineBuffer[offset]) + 1; // +1 for null-termination
            }
            ::GlobalUnlock(hMemOffsets);
        }

        // cleanup
        if (lineBuffer != nullptr)
        {
            ::GlobalUnlock(hMemLines);
        }

        // done
        this->MemoryOffsets = hMemOffsets;
        return true;
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411410
bool kclib::kcFileLineBuffer::LockBuffers()
{
    if (this->LineBuffer == nullptr || this->LineOffsets == nullptr) // 0x8, 0xc
    {
        if (this->MemoryLines != nullptr) // 0x414
        {
            this->LineBuffer = (char *)::GlobalLock(this->MemoryLines); // 0x8, 0x414
        }
        if (this->MemoryOffsets != nullptr) // 0x418
        {
            this->LineOffsets = (unsigned int *)::GlobalLock(this->MemoryOffsets); // 0xc, 0x418
        }
    }
    
    if (this->LineBuffer == nullptr || this->LineOffsets == nullptr) // 0x8, 0xc
    {
        this->UnlockBuffers();
        return false;
    }
    return true;
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004113b0
void kclib::kcFileLineBuffer::UnlockBuffers()
{
    if (this->LineBuffer != nullptr) // 0x8
    {
        ::GlobalUnlock(this->MemoryLines); // 0x414
        this->LineBuffer = nullptr; // 0x8
    }
    if (this->LineOffsets != nullptr) // 0xc
    {
        ::GlobalUnlock(this->MemoryOffsets); // 0x418
        this->LineOffsets = nullptr; // 0xc
    }
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_004114a0
bool kclib::kcFileLineBuffer::IsLocked()
{
    return (this->LineBuffer != nullptr && this->LineOffsets != nullptr);
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411920
const char * kclib::kcFileLineBuffer::GetLineAt(int index)
{
    if (!this->IsLocked())
        return nullptr; // not locked

    if (index < 0 || index >= this->LineCount)
        return nullptr; // out of range
    
    return &this->LineBuffer[this->LineOffsets[index]];
}

///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411970
bool kclib::kcFileLineBuffer::HasLineAt(int index)
{
    return (this->GetLineAt(index) != nullptr);
}

// Copy the next line (or continued lines) from the buffer at index
//  writes to outBuffer, and updates inoutIndex (and +1 for each '\\' line continuation)
///FLAG: ALLOW_LINE_CONTINUE  0x1
///FID:cs2_full_v401/system/scene/mc.exe: FUN_00411a30
unsigned int kclib::kcFileLineBuffer::CopyLineAt(OUT char *outBuffer, int bufferSize, IN OUT int *inoutIndex, unsigned int flags)
{
    int index = *inoutIndex;
    char *out_ptr = outBuffer;
    bool lineContinued = false;
    while (true)
    {
        if (false) // probably a preprocessor flag (that's changed based on the needs of a specific script type)
            break;
        const char *line_ptr = this->GetLineAt(index);
        if (line_ptr == nullptr)
            break; // no more lines, finish

        index++; // increment index (moved up for visibility)
        if (this->LastLineMultibyteContinue != FALSE)
        {
            this->LastLineMultibyteContinue = FALSE; // only set back to false if a line at index exists
        }

        //TRIM: when continuing, trim (skip) control chars and whitespace from start of line
        while (lineContinued && line_ptr[0] != '\0' && (unsigned char)line_ptr[0] <= 0x20) // '\0', 0x21
        {
            line_ptr++;
        }

        //COPY: copy current line into buffer
        int line_len = (int)std::strlen(line_ptr);
        if (line_len >= bufferSize)
        {
            std::memcpy(out_ptr, line_ptr, bufferSize - 1U);
            out_ptr += (bufferSize - 1U); // lineEnd = out_ptr + (bufferSize - 1U);
            break; // end of buffer reached, finish here
        }
        std::memcpy(out_ptr, line_ptr, line_len);
        ///FIXME: Buffer size subtraction doesn't account for -1 from skipping '\\' (non-fatal, minor issue)
        bufferSize -= line_len;
        
        //CONTINUE?: some flags (it's that "allow line comment/continue?" flag again!)
        if ((flags & 1) == 0 || line_len < 1 || out_ptr[line_len - 1] != '\\')
            break; // no line continuation, finish

        //CHECK: A VERY rudimentary scan of multibyte characters in the line to see if
        //        the final '\\' isn't actually part of a double-byte Shift_JIS character.
        int i, char_width;
        for (i = 0, char_width = 0; i + 1 < line_len; i += char_width) // while (line_len >= 2)
        {
            if ((unsigned char)out_ptr[i] >= 0x80)
                char_width = 2;
            else
                char_width = 1;
        }
        if (i == line_len && char_width == 2 && out_ptr[i - 1] == '\\') // 0x5c
        {
            this->LastLineMultibyteContinue = TRUE; // *(unk4 *)0x10 = 1
            break; // multi-byte ending in '\\' (0x5c) not a line continuation
        }

        //NEXT: normal line continuation, keep going and copy next line
        lineContinued = true;
        out_ptr += line_len - 1; // -1 to overwrite (ignore) '\\' line continuation char
    }

    *inoutIndex = index;
    out_ptr[0] = '\0'; // null-terminate
    ///PTRMATH: string distance
    return (unsigned int)(out_ptr - outBuffer);
}

///CUSTOM: function for potentially inlined access to Filename member
unsigned int kclib::kcFileLineBuffer::GetLineCount() const
{
    return this->LineCount;
}

///CUSTOM: function for potentially inlined access to Filename member
const char * kclib::kcFileLineBuffer::GetFilename() const
{
    return &this->Filename[0];
}

#endif
