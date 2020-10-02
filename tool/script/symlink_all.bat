@echo off
setlocal EnableExtensions

REM set DEBUG=1

call :getargc argc %*
if "%argc%"=="0" goto :symlink_all_usage

call :symlink_all %*
endlocal
goto :eof

:: usage: call :symlink_all [/R] [/Q] [[/F] | [/H]] [[/D] | [/J]] Link Target [Pattern]
:symlink_all
    setlocal EnableExtensions EnableDelayedExpansion

    set "symlink_recurse=0"
    set "symlink_overwrite=0"
    set "symlink_file=0"
    set "symlink_dir=0"
    set "symlink_fileopt="
    set "symlink_diropt="

:symlink_all_parseLoopStart
    set "symlink_opt=%~1"
    if "%symlink_opt:~0,1%" == "-" set "symlink_opt=/%symlink_opt:~1%"
    if not "%symlink_opt:~0,1%" == "/" goto :symlink_all_parseLoopEnd
    if not "%symlink_opt:~2,1%" == ""  goto :symlink_all_parseLoopEnd

    if "%symlink_opt%" == "/R" set "symlink_recurse=1"
    REM if "%symlink_opt%" == "/T" set "symlink_recurse=0"

    REM if "%symlink_opt%" == "/Y" set "symlink_overwrite=1"
    REM if "%symlink_opt%" == "/N" set "symlink_overwrite=0"
    if "%symlink_opt%" == "/Q" set "symlink_overwrite=1"

    if "%symlink_opt%" == "/F" set "symlink_fileopt="   && set "symlink_file=1"
    if "%symlink_opt%" == "/H" set "symlink_fileopt=/H" && set "symlink_file=1"
    if "%symlink_opt%" == "/D" set "symlink_diropt=/D"  && set "symlink_dir=1"
    if "%symlink_opt%" == "/J" set "symlink_diropt=/J"  && set "symlink_dir=1"

    shift
    goto :symlink_all_parseLoopStart
    REM if "%symlink_opt:~0,1%" == "-" set "symlink_opt=/%symlink_opt:~1%"
    REM if "%symlink_opt:~0,1%" == "/" ( if "%symlink_opt:~2,1%" == "" ( shift ) else ( set "symlink_opt=" ) ) else ( set "symlink_opt=" )

:symlink_all_parseLoopEnd
    if "%symlink_dir%" == "1"      set "symlink_recurse=0"
    if "%symlink_dir%" == "0"      set "symlink_file=1"
    REM if "%symlink_file%%symlink_dir%" == "00" set "symlink_file=1" && set "symlink_dir=0"

    if "%DEBUG%"=="1" echo symlink_recurse   = %symlink_recurse%,
    if "%DEBUG%"=="1" echo symlink_overwrite = %symlink_overwrite%,
    if "%DEBUG%"=="1" echo symlink_file      = %symlink_file%,%symlink_fileopt%,
    if "%DEBUG%"=="1" echo symlink_dir       = %symlink_dir%,%symlink_diropt%,
    REM if "%symlink_file%" == "/F" ( set "symlink_fileopt=" ) else ( set "symlink_fileopt=%symlink_file%" )

    set /a "symlink_add=0"
    set /a "symlink_repl=0"
    set /a "symlink_skip=0"

    REM set "symlink_opt=%~1"
    REM REM if not "%symlink_opt:~0,1%" == "/" set "symlink_opt="
    REM REM if not "%symlink_opt:~2,1%" == ""  set "symlink_opt="
    REM REM if     "%symlink_opt:~0,1%" == "/" if "%symlink_opt:~2,1%" == "" shift
    REM if "%symlink_opt:~0,1%" == "/" ( if "%symlink_opt:~2,1%" == "" ( shift ) else ( set "symlink_opt=" ) ) else ( set "symlink_opt=" )

    REM set "symlink_link=%~dpnx1"
    REM set "symlink_target=%~dpnx2"
    call :backslashpath symlink_link   "%~dpnx1"
    call :backslashpath symlink_target "%~dpnx2"
    if "%symlink_link%" == ""   set "symlink_link=%cd%"
    if "%symlink_target%" == "" set "symlink_target=%cd%"
    REM set "symlink_pattern=%~3"
    REM if "%~3" == "" set "symlink_pattern=*"
    if "%~3" == "" ( set "symlink_pattern=*" ) else ( set "symlink_pattern=%~3" )
    REM echo symlink_opt       = %symlink_opt%,
    if "%DEBUG%"=="1" echo symlink_link      = %symlink_link%,
    if "%DEBUG%"=="1" echo symlink_target    = %symlink_target%,
    if "%DEBUG%"=="1" echo symlink_pattern   = %symlink_pattern%,
    cd "%symlink_target%"
    if "%DEBUG%"=="1" echo call :symlink_all_recurseTree "%symlink_opt%" "%symlink_link%\" "%symlink_target%\" "%symlink_pattern%"
    call :symlink_all_recurseTree "%symlink_opt%" "%symlink_link%\" "%symlink_target%\" "%symlink_pattern%"
    
    if not "%symlink_add%" == "0"  echo Created %symlink_add% links
    if not "%symlink_repl%" == "0" echo Replaced %symlink_repl% links
    if not "%symlink_skip%" == "0" echo Skipped %symlink_skip% links
    REM echo symlink_add  = %symlink_add%,
    REM echo symlink_repl = %symlink_repl%,
    REM echo symlink_skip = %symlink_skip%,
    endlocal
    goto :eof

:: ******** SUB ROUTINES ********

:: usage: call :symlink_recurseTree <Opt:""| [/D] | [/H] | [/J]> <Link\> <Target\> <Pattern>
:: requires current directory to be set
::
:: heavily modified from: <https://stackoverflow.com/a/8398621>
:symlink_all_recurseTree
    setlocal EnableExtensions EnableDelayedExpansion
    REM if "%symtree_opt:~0,1%" == "/" ( if "%symtree_opt:~2,1%" == "" ( shift ) else ( set "symtree_opt=") ) else ( set "symtree_opt=")
    if "%symlink_file%" == "1" (
        for %%f in ("%~4") do (
            if "%DEBUG%"=="1" echo f = %%f
            cd "%~2"
            if exist "%~2%%f" (
                if "%symlink_overwrite%" == "0" (
                    set /a "symlink_skip = symlink_skip + 1"
                ) else (
                    if "%DEBUG%"=="1" echo del /Q "%~2%%f"
                    if "%DEBUG%"=="1" echo mklink %symlink_fileopt% "%%f" "%~3%%f"
                    if not "%DEBUG%"=="1" del /Q "%~2%%f"
                    if not "%DEBUG%"=="1" mklink %symlink_fileopt% "%%f" "%~3%%f"
                    set /a "symlink_repl = symlink_repl + 1"
                )
            ) else (
                if "%DEBUG%"=="1" echo mklink %symlink_fileopt% "%%f" "%~3%%f"
                if not "%DEBUG%"=="1" mklink %symlink_fileopt% "%%f" "%~3%%f"
                set /a "symlink_add = symlink_add + 1"
            )
            cd "%~3"
        )
    )
    if "%symlink_dir%" == "1" (
        for /D %%d in (*) do (
            if "%DEBUG%"=="1" echo d = %%d
            cd "%~2"
            if exist "%~2%%d" (
                if "%symlink_overwrite%" == "0" (
                    set /a "symlink_skip = symlink_skip + 1"
                ) else (
                    if "%DEBUG%"=="1" echo rmdir /S /Q "%~2%%d"
                    if "%DEBUG%"=="1" echo mklink %symlink_diropt% "%%d" "%~3%%d"
                    if not "%DEBUG%"=="1" rmdir /S /Q "%~2%%d"
                    if not "%DEBUG%"=="1" mklink %symlink_diropt% "%%d" "%~3%%d"
                    set /a "symlink_repl = symlink_repl + 1"
                )
            ) else (
                if "%DEBUG%"=="1" echo mklink %symlink_diropt% "%%d" "%~3%%d"
                if not "%DEBUG%"=="1" mklink %symlink_diropt% "%%d" "%~3%%d"
                set /a "symlink_add = symlink_add + 1"
            )
            cd "%~3"
        )
    ) else (
        if "%symlink_recurse%" == "1" (
            for /D %%d in (*) do (
                if "%DEBUG%"=="1" echo d = %%d
                if "%DEBUG%"=="1" if not exist "%~2%%d" echo mkdir "%~2%%d"
                if not "%DEBUG%"=="1" if not exist "%~2%%d" mkdir "%~2%%d"
                cd %%d
                REM if "%DEBUG%"=="1" echo call :symlink_all_recurseTree "%~1" "%~2%%d\" "%~3%%d\" %4
                call :symlink_all_recurseTree "%~1" "%~2%%d\" "%~3%%d\" %4
                cd ..
            )
        )
    )
    endlocal && set /a "symlink_add = %symlink_add%" && set /a "symlink_repl = %symlink_repl%" && set /a "symlink_skip = %symlink_skip%"
    goto :eof


:: usage: goto :symlink_all_usage
:symlink_all_usage
    echo Creates symbolic links for directory contents. An extension of mklink.
    echo All links are created as absolute paths.
    echo.
    echo %~nx0 [/R] [/Q] [[/F] ^| [/D]] [[/H] ^| [/J]] Link Target [Pattern]
    echo.
    echo     /R      Recursively links every file in sub-directories.  Disabled
    echo             when creating directory links.
    echo     /Q      Quiet mode, do not ask if ok to replace a file or directory.
    echo.
    echo     /F      Creates file symbolic links.  Default mode.
    echo     /H      Creates hard links instead of symbolic links.
    echo     /D      Creates directory symbolic links.
    echo     /J      Creates Directory Junctions.
    echo.
    echo     Link    Specifies the new symbolic link directory location.
    echo     Target  Specifies the directory location (relative or absolute) that
    echo             the new links refer to.
    echo     Pattern Specifies the target and link names or wildcard patterns.
    echo             Default is "*" to match all contents.
    goto :eof


REM :: usage: goto :echo_usage
REM :symlink_usage
REM     echo Creates a symbolic link.
REM     echo.
REM     REM echo SYMLINK_TOP.BAT [[/D] ^| [/H] ^| [/J]] [[/T] ^| [/R]] Link Target Name
REM     echo %~nx0 [[/D] ^| [/H] ^| [/J]] Link Target Name
REM     echo.
REM     echo     /D      Creates a directory symbolic link.  Default is a file
REM     echo             symbolic link.
REM     echo     /H      Creates a hard link instead of a symbolic link.
REM     echo     /J      Creates a Directory Junction.
REM     REM echo     /T      Links every file in the top-level directory.
REM     REM echo     /R      Recursively links every file in the directory.
REM     echo     Link    Specifies the new symbolic link directory location.
REM     echo     Target  Specifies the directory location (relative or absolute) that
REM     echo             the new link refers to.
REM     echo     Pattern Specifies the target and link file name or wildcard pattern.
REM     goto :eof


:: ******** HELPER ROUTINES ********

:: usage: call :backslashpath <varname> <path>
:: return: backslashed-noquotes-path -> %<varname>%
::
:: modified from: <https://stackoverflow.com/a/23543817>
:backslashpath
    setlocal EnableExtensions EnableDelayedExpansion
    set backslashpath_rtn=%2
    for %%f in (%2) do (
        set backslashpath_v0=%%f
        set backslashpath_rtn=!backslashpath_v0:/=\!
    )
    call :stripquotes backslashpath_rtn %backslashpath_rtn%
    endlocal && set "%1=%backslashpath_rtn%"
    goto :eof

:: usage: call :stripquotes <varname> <path>
:: return: noquotes-path -> %<varname>%
:stripquotes
    set "%1=%~2"
    goto :eof

:: usage: call :getargc <varname> <%*>
:: return: argc -> %<varname>%
::
:: modified from: <https://stackoverflow.com/a/1292079>
:getargc
    setlocal EnableExtensions
    set getargc_v0=%1
    set /a "joinpath_rtn = 0"
:getargc_l0
    if not x%2x==xx (
        shift
        set /a "joinpath_rtn = joinpath_rtn + 1"
        goto :getargc_l0
    )
    endlocal && set /a "%getargc_v0% = %joinpath_rtn%"
    goto :eof
