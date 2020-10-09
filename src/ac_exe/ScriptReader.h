#pragma once

#ifndef KCLIB_SCRIPT_READER_H
#define KCLIB_SCRIPT_READER_H

// OOP-only file, functional decompiled versions of functions for ScriptReader
//  are in "TokenParser_functional.*"
//
// functional SCRIPT_READER type moved to "ScriptReader.h"

#include "common.h"

#ifdef KCLIB_OOP
namespace kclib
{
#endif

    #pragma pack(push, 1)

    #ifndef KCLIB_OOP

    // Similar structure to SCRIPT_DECODER, only holds basic positional info
    // This could possibly be std::ifstream or something.
    // Meaning the SCRIPT_DECODER could be the same or std::sstream (std::stringstream)
    struct SCRIPT_READER
    {
        /*$0,4*/    char *Buffer;
        /*$4,4*/    unsigned int Length; // text buffer length
        /*$8,4*/    unsigned int Position; // text buffer index of current/(end?) line
        /*$c,4*/    unsigned int RdrUnk3; // set to 0 in ctor, never observed elsewhere
        // LineNumber may not be part of this reader structure, if so, size is definite $10
        /*$10*/
        // /*$10,4*/   unsigned int LineNumber; // position of text buffer? End of line? Start of line?
        // /*$14*/
    };

    #else
    
    class ScriptReader
    {
        /*$0,4*/    char *Buffer;
        /*$4,4*/    unsigned int Length; // text buffer length
        /*$8,4*/    unsigned int Position; // text buffer index of current/(end?) line
        /*$c,4*/    unsigned int RdrUnk3; // set to 0 in ctor, never observed elsewhere
        // LineNumber may not be part of this reader structure, if so, size is definite $10
        /*$10*/
        // /*$10,4*/   unsigned int LineNumber; // position of text buffer? End of line? Start of line?
        // /*$14*/

    public:
        ScriptReader();
        ~ScriptReader();

        bool OpenFile(IN const char *filename);
        bool OpenFile(IN const wchar_t *filename);
        void Close();
        bool IsOpen() const;

        bool IsEOF() const;

        const char * GetBuffer() const;
        bool OpenBuffer(IN const char *newBuffer);

        unsigned int GetPosition() const;
        unsigned int GetLength() const;
        void SetPosition(unsigned int newPosition);

        void ConvertToLF();
        void NextLine(OPTIONAL OUT char *lineBuffer);

    private:
        unsigned int FUN_00411c60();
    };

    #endif

    #pragma pack(pop)

#ifdef KCLIB_OOP
}
#endif

#endif /* end include guard */
