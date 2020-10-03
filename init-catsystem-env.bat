@echo off

:: TODO: Configure additional paths for CatSystem 1+2 command line tools here
:: NEEDS ATTENTION: Is quoting the %PATH% variable correct?
set "PATH=%~dp0tool;%PATH%"

:: Utility Python scripts and batch files
set "PATH=%~dp0tool\script;%PATH%"

:: Personal toolsets, with many quirks and bugs with certain filepath inputs.
::   Many of these have no existing equivalent, however, near all of this is
::   decumented in the wiki:
::   <https://github.com/trigger-segfault/TriggersTools.CatSystem2/wiki>
:: cmd tools: cs2_tool.exe, cs2_decompile.exe, zt_tool.exe
::   from the included tools under /tool/trigger, most originated from the
::   TriggersTools.CatSystem2 library but never had a stable version control,
::   and later had most improvements added via dnSpy... Horrifying! right??
set "PATH=%~dp0tool\trigger;%PATH%"

:: Not included in this repo, but not directly used / or hard to find

:: Tools from cs2_full_v301 and cs2_full_v401 toolkits, publicly available
:: on <http://cs2.suki.jp>
:: cmd tools: ac.exe, mc.exe, fes.exe, ztpack.exe, MakeInt.exe, MakeCGList.exe
:: gui tools: pcmtag.exe, wsd.exe, WGC.exe, StView.exe
::         (+StView.exe v2 from cs2_full_v301/tool/StView.exe, which is slightly
::          newer, all other 3 versions in the toolkits are the same older version)
REM set PATH="%~dp0tool\cs2;%PATH%"

:: UI translations of cs2 tools using Resource Hacker, Google Translate, and
::   enough knowledge of the Cs2 engine and history to get by.
:: gui tools: wsd_en.exe, WGC_en.exe, StView_en.exe (the older StView.exe version)
set "PATH=%~dp0tool\cs2-translate;%PATH%"

:: Original Asmodean tools for ripping CS2 .int archives and .hg2/.hg3 image assets
:: Most tools have been surpassed by cs2_tool.exe, cs2_decompile.exe, zt_tool.exe
::   from the included tools under /tool/trigger, most originated from the
::   TriggersTools.CatSystem2 library but never had a stable version control,
::   and later had most improvements added via dnSpy... Horrifying! right??
:: Asmodean tools <http://asmodean.reverse.net/pages/exkifint.html>
REM set PATH="%~dp0tool\asmodean;%PATH%"


:: Extraction tool for *.lzh / *.lha, for older JP resource downloads,
::   WinRAR also supports most of these features, making this obsolete
REM set PATH="%~dp0tool\lha;%PATH%"

:: Emulate JP locale more easily than locale emulator, but with less support features. (helpful, but non-essential with Locale Emulator, below)
:: cmd tools: ntleac.exe (+ntleah.dll)
REM set PATH="%~dp0tool\ntlea;%PATH%"

:: Best tol for JP locale emulation, essential for numerous cs2 tools like ac.exe,
::   mc.exe, fes.exe, etc...
:: <https://github.com/xupefei/Locale-Emulator>
REM set PATH="%~dp0tool\locale-emulator;%PATH%"

:: PYTHON ENVIRONMENT

:: TODO: Set your Python (3.6+) environment executable path
set PYTHON="%~dp0tool\python.bat"

:: NEEDS ATTENTION: Is quoting the %PYTHONPATH% variable correct?
:: NEEDS ATTENTION: Can paths in %PYTHONPATH% end with a slash?
set PYTHONPATH="%~dp0src;%PYTHONPATH%"

:: TODO: Make a useful setup script with common helper functions for now, catsys.reverse will do.
set PYTHONSTARTUP="%~dp0src\catsys\reversing.py"


REM PYTHONSTARTUP: file executed on interactive startup (no default)
REM PYTHONPATH   : ';'-separated list of directories prefixed to the
REM                default module search path.  The result is sys.path.
REM PYTHONHOME   : alternate <prefix> directory (or <prefix>;<exec_prefix>).
REM                The default module search path uses <prefix>/lib/pythonX.X.
REM PYTHONCASEOK : ignore case in 'import' statements (Windows).
REM PYTHONUTF8: if set to 1, enable the UTF-8 mode.
