
// OOP-specific definition of ac.exe main

#include "common.h"
#ifdef KCLIB_OOP

#include "TokenParser_functional.h"
#include "kclib_common.h"
#include "kcAnmScript.h"
#include "ghidra_types_min.h"
#include <windows.h>
// #include <vector>

namespace kclib {
    namespace ac {
        ///FID:cs2_full_v401/tool/ac.exe: FUN_00405bd0
        void CoUninitializeKclibCleanup(void)

        {
            // dummy func
        }

        ///FID:cs2_full_v401/tool/ac.exe: FUN_00405b10
        bool CoInitializeKclibSetup(unsigned int param_1, int param_2)

        {
            // This likely sets up some globals of importance, like for detecting memory leaks.
            // 
            // We can see that absolute path with kcBaseClass.h in it, which is likely for an
            // STL std::list<kcBaseClass> or std::vector<kcBaseClass> (probably the first)

            // dummy func
            return true;
        }
    }
}

#ifdef UNICODE

extern "C" int wmain(int argc, wchar_t *argv[]) //, wchar_t *envp[])

{
    if (argc < 2)
    {
        ///JP: std::wprintf(L"アニメーションスクリプトコンバータ\n");
        // std::wprintf(L"Animation script converter\n");
        // std::wprintf(L"usage : ac [file] [file] ...\n");

        // std::wprintf(L"Animation script converter\n");
        std::wprintf(L"CatSystem 2 Animation Script compiler\n");
        std::wprintf(L"compile:   ac_tool [txtfile] [txtfile] ...\n");
        std::wprintf(L"decompile: ac_tool /d [anmfile] [anmfile] ...\n");
        std::wprintf(L"\n");
        std::wprintf(L"arguments:\n");
        std::wprintf(L"  txtfile   text file names to compile (with extension)\n");
        std::wprintf(L"\n");
        std::wprintf(L"  /d        decompile mode\n");
        std::wprintf(L"  anmfile   anm file names to decompile (with extension)\n");
        std::wprintf(L"\n");

//         CatSystem 2 Z-compressed Package File creation tool
// pack:   ztpack <outfile> <file> [file] ...
// unpack: ztpack /d <ztfile> <outdir>

// arguments:
//   file      file name(s) to convert (with extension)
//   outfile   output file name (with extension)
//   file      file name(s) or folder(s) to pack

//   /d        unpack mode
//   ztfile    input zt file to unpack
//   outdir    output directory to unpack files to
        return 1;
    }

    kclib::kcAnmScript anmscr;

    kclib::ac::CoInitializeKclibSetup(0x7f, 0); //FUN_00405b10(0x7f,0); CoInitialize and setup

    bool decompileMode = false;
    if (std::wcslen(argv[1]) == 2 && (argv[1][0] == L'/' || argv[1][0] == L'-'))
    {
        if (argv[1][1] == L'd' || argv[1][1] == L'D')
        {
            decompileMode  = true;
        }
    }

    for (int i = (decompileMode ? 2 : 1); i < argc; i++)
    {
        wchar_t argFullpath[MAX_PATH + 4]; //char local_35c [264];
        wchar_t argParentdir[MAX_PATH + 4]; //char local_254 [264];
        wchar_t outFullpath[MAX_PATH + 4]; //char local_35c [264];
        kclib::GetAbsolutePath(argv[i], argFullpath, (wchar_t **)nullptr); // FUN_00411ff0(argv[i], local_35c, (LPSTR *)0x0);
        kclib::GetParentDirectory(argFullpath, argParentdir); // FUN_00412520(local_35c, local_254);

        WIN32_FIND_DATAW findData;
        HANDLE hFindFile = FindFirstFileW(argv[i], (LPWIN32_FIND_DATAW)&findData);
        if (hFindFile == INVALID_HANDLE_VALUE)
        {
            ///JP: std::wprintf(L"ファイルが見つかりません : %s\n", argv[i]);
            std::wprintf(L"File not found : %ls\n", argv[i]);
            continue;
        }
        
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // swprintf_s(argFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                _snwprintf(argFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                // int iVar1 = ac_parseLines(argFullpath);
                // if (iVar == 0)

                if (!decompileMode)
                {

                    // original source expects 0 for parse success (as in 0 errorCount)
                    //   we changed it to bool (true for success)
                    unsigned int errorCount = 0;
                    if (anmscr.OpenScript(argFullpath, &errorCount))
                    {
                        // swprintf_s(argFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                        _snwprintf(outFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                        kclib::ChangeExtension(outFullpath, L"out-anm"); //FUN_004120e0(argFullpath,(undefined4 *)"anm");
                        
                        // ac_WriteAnmFile(argFullpath); //FUN_00403990(argFullpath);
                        if (anmscr.WriteFile(outFullpath))
                        {
                            std::fwprintf(stdout, L" anm out -> %ls\n", outFullpath);
                        }
                        else
                        {

                        }
                        // std::fwprintf(stdout, L"\n");
                        // anmscr.PrintScript(stdout);
                    }
                    else
                    {
                        std::fwprintf(stdout, L" error parsing txt file\n");
                    }
                    std::fflush(stdout);
                    anmscr.Close();
                }
                else
                {
                    // original source expects 0 for parse success (as in 0 errorCount)
                    //   we changed it to bool (true for success)
                    if (anmscr.OpenFile(argFullpath))
                    {
                        // sprintf_s(argFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                        _snwprintf(outFullpath, MAX_PATH, L"%ls\\%ls", argParentdir, findData.cFileName);
                        kclib::ChangeExtension(outFullpath, L"out-txt");
                        
                        if (anmscr.WriteScript(outFullpath))
                        {
                            std::fwprintf(stdout, L" txt out -> %ls\n", outFullpath);
                        }
                        else
                        {

                        }
                        // std::fwprintf(stdout, L"\n");
                        // anmscr.PrintScript(stdout);
                    }
                    else
                    {
                        std::fwprintf(stdout, L" error reading anm file\n");
                    }
                    std::fflush(stdout);
                    anmscr.Close();
                }
            }
        } while (FindNextFileW(hFindFile, (LPWIN32_FIND_DATAW)&findData));

        FindClose(hFindFile);
    }
    kclib::ac::CoUninitializeKclibCleanup();
    return 0;
}

#else

///FID:cs2_full_v401/tool/ac.exe: FUN_00401fe0
extern "C" int main(int argc, char* argv[])

{
    if (argc < 2)
    {
        ///JP: std::printf("アニメーションスクリプトコンバータ\n");
        // std::printf("Animation script converter\n");
        // std::printf("usage : ac [file] [file] ...\n");

        // std::printf("Animation script converter\n");
        std::printf("CatSystem 2 Animation Script compiler\n");
        std::printf("compile:   ac_tool [txtfile] [txtfile] ...\n");
        std::printf("decompile: ac_tool /d [anmfile] [anmfile] ...\n");
        std::printf("\n");
        std::printf("arguments:\n");
        std::printf("  txtfile   text file names to compile (with extension)\n");
        std::printf("\n");
        std::printf("  /d        decompile mode\n");
        std::printf("  anmfile   anm file names to decompile (with extension)\n");
        std::printf("\n");

//         CatSystem 2 Z-compressed Package File creation tool
// pack:   ztpack <outfile> <file> [file] ...
// unpack: ztpack /d <ztfile> <outdir>

// arguments:
//   file      file name(s) to convert (with extension)
//   outfile   output file name (with extension)
//   file      file name(s) or folder(s) to pack

//   /d        unpack mode
//   ztfile    input zt file to unpack
//   outdir    output directory to unpack files to
        return 1;
    }

    kclib::kcAnmScript anmscr;

    kclib::ac::CoInitializeKclibSetup(0x7f, 0); //FUN_00405b10(0x7f,0); CoInitialize and setup

    bool decompileMode = false;
    if (std::strlen(argv[1]) == 2 && (argv[1][0] == '/' || argv[1][0] == '-'))
    {
        if (argv[1][1] == 'd' || argv[1][1] == 'D')
        {
            decompileMode  = true;
        }
    }

    for (int i = (decompileMode ? 2 : 1); i < argc; i++)
    {
        char argFullpath[MAX_PATH + 4]; //char local_35c [264];
        char argParentdir[MAX_PATH + 4]; //char local_254 [264];
        char outFullpath[MAX_PATH + 4]; //char local_35c [264];
        kclib::GetAbsolutePath(argv[i], argFullpath, nullptr); // FUN_00411ff0(argv[i], local_35c, (LPSTR *)0x0);
        kclib::GetParentDirectory(argFullpath, argParentdir); // FUN_00412520(local_35c, local_254);

        WIN32_FIND_DATAA findData;
        HANDLE hFindFile = FindFirstFileA(argv[i], (LPWIN32_FIND_DATAA)&findData);
        if (hFindFile == INVALID_HANDLE_VALUE)
        {
            ///JP: std::printf("ファイルが見つかりません : %s\n", argv[i]);
            std::printf("File not found : %s\n", argv[i]);
            continue;
        }
        
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                std::snprintf(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                // int iVar1 = ac_parseLines(argFullpath);
                // if (iVar == 0)

                if (!decompileMode)
                {

                    // original source expects 0 for parse success (as in 0 errorCount)
                    //   we changed it to bool (true for success)
                    unsigned int errorCount = 0;
                    if (anmscr.OpenScript(argFullpath, &errorCount))
                    {
                        // sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                        std::snprintf(outFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                        kclib::ChangeExtension(outFullpath, "out-anm"); //FUN_004120e0(argFullpath,(undefined4 *)"anm");
                        
                        // ac_WriteAnmFile(argFullpath); //FUN_00403990(argFullpath);
                        if (anmscr.WriteFile(outFullpath))
                        {
                            std::fprintf(stdout, " anm out -> %s\n", outFullpath);
                        }
                        else
                        {

                        }
                        // std::fprintf(stdout, "\n");
                        // anmscr.PrintScript(stdout);
                    }
                    else
                    {
                        std::fprintf(stdout, " error parsing txt file\n");
                    }
                    std::fflush(stdout);
                    anmscr.Close();
                }
                else
                {
                    // original source expects 0 for parse success (as in 0 errorCount)
                    //   we changed it to bool (true for success)
                    if (anmscr.OpenFile(argFullpath))
                    {
                        // sprintf_s(argFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                        std::snprintf(outFullpath, MAX_PATH, "%s\\%s", argParentdir, findData.cFileName);
                        kclib::ChangeExtension(outFullpath, "out-txt");
                        
                        if (anmscr.WriteScript(outFullpath))
                        {
                            std::fprintf(stdout, " txt out -> %s\n", outFullpath);
                        }
                        else
                        {

                        }
                        // std::fprintf(stdout, "\n");
                        // anmscr.PrintScript(stdout);
                    }
                    else
                    {
                        std::fprintf(stdout, " error reading anm file\n");
                    }
                    std::fflush(stdout);
                    anmscr.Close();
                }
            }
        } while (FindNextFileA(hFindFile, (LPWIN32_FIND_DATAA)&findData));

        FindClose(hFindFile);
    }
    kclib::ac::CoUninitializeKclibCleanup();
    return 0;
}

#endif

#endif
