#pragma once

#ifndef KCLIB_KCCLASSBASE_CLASS_H
#define KCLIB_KCCLASSBASE_CLASS_H

// #include "../ac_exe/common.h"
#include <cstdlib>
#include <list>
#include <new>

// An example showing what is probably going on with the header filepaths:
//   "e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h"
// that are litered EVERYWHERE in CatSystem2 assemblies, they're used for tracking memory leaks by
// throwing the class allocation into a global std::list, and is removed when the delete operator is called.
// 
// Looking at this, it makes it even easier to spot kclib-specific classes in assembly. And you'll
// notice that more often than not, classes are rarely allocated to the heap, unless they're going into
// global storage, need virtual function calls (like with kcFileBase stuff), etc.
//  but that's normal for C++...?  It's been awhile...
// 
// when CatSystem2 closes(?), the list is cleared and each item in the list will log an error.
// 
// I don't actually think this tracker is specific to kcClassBase, or kclib, but I've only noticed it there,
//  and possibly with std::boost::expression (regex classes), which have also had absolute filepaths
//  litered in code (TODO: check on that)
// 
// Everything in this file is based off ac.exe source, however other assemblies show similar usage
//  The Happiness 2: Sakura Celebration assembly seems not to use this feature
//   (so it may be a debug / or outdated thing)

namespace kclib
{
    class kcTracker
    {
        // extern global std::list for tracking all allocations
        static std::list<kclib::kcTracker> g_kcStatic_global_list;

        /*$0,104*/  char Filename[0x104]; // MAX_PATH ("e:\\prg\\vs2008\\kclib-dx9\\include\\..\\kcClassBase\\kcClassBase.h", param_2)
        /*$104,4*/  int LineNumber; // classLineNumber (0x27 (line 39), param_3)
        /*$108,4*/  std::size_t ClassSize; // classSize (param_1)
        /*$10c,4*/  void *ClassPtr; // ptr to allocated class (class cannot see the tracker)
        /*$110*/
    public:
        static void * Create(std::size_t size, const char *filename, int line);
        static void Release(void *ptr);
        static void Cleanup(); // called at end of program runtime(?)
    };

    // our main protagonist, the root of all kclib, kcClassBase!
    class kcClassBase
    {
    public:
        inline void * operator new(std::size_t size)
        {
            // these macros will resolve to the current filename and line number respectively (at compile time)
            //  whether the new operator overload is actually the method used is unknown, but these macros
            //  are very likely to have been used.
            return kclib::kcTracker::Create(size, __FILE__, __LINE__); // pretend we're on line 39, like in ac.exe
        }
        inline void operator delete(void *ptr)
        {
            kclib::kcTracker::Release(ptr);
        }
    };


    //////////////////////////////////////////////
    ///// EXAMPLE CLASS USAGE

    // Example kclib class from ac.exe (not particularly useful, but one that's easy to showcase)
    // 
    // This class uses Windows Dynamic Data Exchange (DDE) for CatSystem2 to communicate and
    //  co-operate with the cs2conf.dll engine config, among other programs (at least that's the theory)
    class kcDDE : public kclib::kcClassBase
    {
        /*$0,4*/   unsigned int DdeUnk0;
        /*$4,4*/   unsigned int InstanceID; // DDE pidInst from DdeInitializeA
        /*$8,4*/   void* DdeUnk2; // *pidInst,"KCDDE_TOPIC",0x3ec
        /*$c,4*/   unsigned int* DdeUnk3; // [0x1c]
        /*$10,4*/  unsigned int* DdeUnk4; // [0x1c]
        /*$14,4*/  unsigned int* DdeUnk5; // [0x1c]
        /*$18*/
    public:
        kcDDE();
        ~kcDDE();

        // HDDEDATA InitCallback(
        //     UINT wType,
        //     UINT wFmt,
        //     HCONV hConv,
        //     HSZ hsz1,
        //     HSZ hsz2,
        //     HDDEDATA hData,
        //     ULONG_PTR dwData1,
        //     ULONG_PTR dwData2
        // );
    };
    //0x1062 | XTYP_CONNECT
    //XTYP_CONNECT (0x0060 | XCLASS_BOOL | XTYPF_NOBLOCK)
    //0x20b0 | XTYP_REQUEST
    //XTYP_REQUEST (0x00B0 | XCLASS_DATA)
}


#endif /* end include guard */
