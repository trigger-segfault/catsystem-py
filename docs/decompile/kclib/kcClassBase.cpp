#include "kcClassBase.h"
#include <cstdio>
#include <cstring>


std::list<kclib::kcTracker> kclib::kcTracker::g_kcStatic_global_list;

void * kclib::kcTracker::Create(std::size_t size, const char *filename, int line)
{
    void *ptr = (kclib::kcClassBase *)std::malloc(size);
    if (ptr)
    {
        kclib::kcTracker kc;
        std::strcpy(&kc.Filename[0], filename);
        kc.LineNumber = line;
        kc.ClassSize = size;
        kc.ClassPtr = ptr;

        ///CUSTOM: logging
        std::printf("%s(%d) : new [adr=0x%016llx  size=%lld]\n", kc.Filename, kc.LineNumber, (unsigned long long)kc.ClassPtr, (long long)kc.ClassSize);

        kclib::kcTracker::g_kcStatic_global_list.push_back(kc);
    }
    return ptr;
}

void kclib::kcTracker::Release(void *ptr)
{
    // list iterators were definitely present in the ac.exe source
    std::list<kclib::kcTracker>::iterator it;
    for (it = kclib::kcTracker::g_kcStatic_global_list.begin(); it != kclib::kcTracker::g_kcStatic_global_list.end(); ++it)
    {
        if (it->ClassPtr == ptr)
            break;
    }

    ///CUSTOM: logging
    std::printf("%s(%d) : delete [adr=0x%016llx  size=%lld]\n", it->Filename, it->LineNumber, (unsigned long long)it->ClassPtr, (long long)it->ClassSize);
    
    it->ClassPtr = nullptr; // not present in assembly (added)
    kclib::kcTracker::g_kcStatic_global_list.erase(it);
    std::free(ptr);

}

void kclib::kcTracker::Cleanup()
{
    std::list<kclib::kcTracker>::iterator it;
    for (it = kclib::kcTracker::g_kcStatic_global_list.begin(); it != kclib::kcTracker::g_kcStatic_global_list.end(); ++it)
    {
        ///ORIG: kclib::LogSendMessage("%s(%d) : no release [adr=0x%08x  size=%d]", it->Filename, it->LineNumber, (unsigned int)it->ClassPtr, (int)it->ClassSize);
        
        // the environment being tested in is x64 (so the ll's, 16, long long, etc. are side effects of that, ignore them)
        std::printf("%s(%d) : no release [adr=0x%016llx  size=%lld]\n", it->Filename, it->LineNumber, (unsigned long long)it->ClassPtr, (long long)it->ClassSize);

        ///EXAMPLE: e:\prg\vs2008\kclib-dx9\include\..\kcClassBase\kcClassBase.h(39) : no release [adr=0x43056388  size=24]
    }

    ///ORIG: kclib::LogSendMessage("leak count = %d", (int)kclib::kcTracker::g_kcStatic_global_list.size());
    std::printf("leak count = %lld", (long long)kclib::kcTracker::g_kcStatic_global_list.size());
}


kclib::kcDDE::kcDDE()
{
    ///CUSTOM: logging, no implementation
    std::printf("kcDDE()\n");
    // DdeInitializeA(&this->InstanceID, )
    // DdeCreateStringHandleA(this->InstanceID, "KCDDE_TOPIC", CP_WINANSI) //(CP_WINANSI == 0x3ec (1004))
}
kclib::kcDDE::~kcDDE()
{
    ///CUSTOM: logging, no implementation
    std::printf("~kcDDE()\n");
}
