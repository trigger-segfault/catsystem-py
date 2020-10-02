@echo off
setlocal EnableExtensions

REM set DEBUG=1

call :getargc argc %*
if "%argc%"=="0" goto :symlink_one_usage

call :symlink_one %*
endlocal
goto :eof

:: usage: call :symlink_one [/Q] [[/F] | [/H] | [/D] | [/J]] Link Target File
:symlink_one
    setlocal EnableExtensions EnableDelayedExpansion

    set "symlink_overwrite=0"
    set "symlink_type=FILE"
    set "symlink_typeopt="

:symlink_one_parseLoopStart
    set "symlink_opt=%~1"
    if "%symlink_opt:~0,1%" == "-" set "symlink_opt=/%symlink_opt:~1%"
    if not "%symlink_opt:~0,1%" == "/" goto :symlink_one_parseLoopEnd
    if not "%symlink_opt:~2,1%" == ""  goto :symlink_one_parseLoopEnd

    REM if "%symlink_opt%" == "/Y" set "symlink_overwrite=1"
    REM if "%symlink_opt%" == "/N" set "symlink_overwrite=0"
    if "%symlink_opt%" == "/Q" set "symlink_overwrite=1"

    if "%symlink_opt%" == "/F" set "symlink_typeopt="   && set "symlink_type=FILE"
    if "%symlink_opt%" == "/H" set "symlink_typeopt=/H" && set "symlink_type=FILE"
    if "%symlink_opt%" == "/D" set "symlink_typeopt=/D" && set "symlink_type=DIR"
    if "%symlink_opt%" == "/J" set "symlink_typeopt=/J" && set "symlink_type=DIR"

    shift
    goto :symlink_one_parseLoopStart
    REM if "%symlink_opt:~0,1%" == "-" set "symlink_opt=/%symlink_opt:~1%"
    REM if "%symlink_opt:~0,1%" == "/" ( if "%symlink_opt:~2,1%" == "" ( shift ) else ( set "symlink_opt=" ) ) else ( set "symlink_opt=" )

:symlink_one_parseLoopEnd
    if "%symlink_dir%" == "1"      set "symlink_recurse=0"
    if "%symlink_dir%" == "0"      set "symlink_file=1"
    REM if "%symlink_file%%symlink_dir%" == "00" set "symlink_file=1" && set "symlink_dir=0"

    if "%DEBUG%"=="1" echo symlink_overwrite = %symlink_overwrite%,
    if "%DEBUG%"=="1" echo symlink_type      = %symlink_type%,%symlink_typeopt%,

    set /a "symlink_add=0"
    set /a "symlink_repl=0"
    set /a "symlink_skip=0"

    call :backslashpath symlink_link   "%~dpnx1"
    call :backslashpath symlink_target "%~dpnx2"
    set "symlink_name=%~3"
    if "%symlink_link%" == ""   set "symlink_link=%cd%"
    if "%symlink_target%" == "" set "symlink_target=%cd%"
    REM if "%symlink_name%" == ""   set "symlink_name=."
    REM echo symlink_opt       = %symlink_opt%,
    if "%DEBUG%"=="1" echo symlink_link      = %symlink_link%,
    if "%DEBUG%"=="1" echo symlink_target    = %symlink_target%,
    if "%DEBUG%"=="1" echo symlink_name      = %symlink_name%,

    if "%symlink_type%" == "FILE" (
        if exist "%symlink_link%\%symlink_name%" (
            if "%symlink_overwrite%" == "0" (
                echo File already exists at link
                set /a "symlink_skip = symlink_skip + 1"
                goto :symlink_one_end
            ) else (
                echo Replacing directory at link
                if "%DEBUG%"=="1" echo del /Q "%symlink_link%\%symlink_name%"
                if not "%DEBUG%"=="1" del /Q "%symlink_link%\%symlink_name%"
                set /a "symlink_repl = symlink_repl + 1"
                goto :symlink_one_mklink
            )
        ) else (
            set /a "symlink_add = symlink_add + 1"
        )
    ) else (
        if exist "%symlink_link%\%symlink_name%\" (
            if "%symlink_overwrite%" == "0" (
                echo Directory already exists at link
                set /a "symlink_skip = symlink_skip + 1"
                goto :symlink_one_end
            ) else (
                echo Replacing directory at link
                if "%DEBUG%"=="1" echo rmdir /S /Q "%symlink_link%\%symlink_name%"
                if not "%DEBUG%"=="1" rmdir /S /Q "%symlink_link%\%symlink_name%"
                set /a "symlink_repl = symlink_repl + 1"
                goto :symlink_one_mklink
            )
        ) else (
            set /a "symlink_add = symlink_add + 1"
        )
    )

:symlink_one_mklink
    cd "%symlink_link%"
    if "%DEBUG%"=="1" echo mklink %symlink_typeopt% "%symlink_name%" "%symlink_target%\%symlink_name%"
    if not "%DEBUG%"=="1" mklink %symlink_typeopt% "%symlink_name%" "%symlink_target%\%symlink_name%"

:symlink_one_end
    if not "%symlink_add%" == "0"  echo Created %symlink_add% links
    if not "%symlink_repl%" == "0" echo Replaced %symlink_repl% links
    if not "%symlink_skip%" == "0" echo Skipped %symlink_skip% links

    endlocal
    goto :eof

:: ******** SUB ROUTINES ********

:: usage: goto :symlink_one_usage
:symlink_one_usage
    echo Creates a symbolic link. An extension of mklink.
    echo All links are created as absolute paths.
    echo.
    echo %~nx0 [/Q] [[/F] ^| [/H] ^| [/D] ^| [/J]] Link Target File
    echo.
    echo     /Q      Quiet mode, do not ask if ok to replace a file or directory.
    echo.
    echo     /F      Creates a file symbolic link.  Default mode.
    echo     /H      Creates a hard link instead of a symbolic link.
    echo     /D      Creates a directory symbolic link.
    echo     /J      Creates a Directory Junction.
    echo.
    echo     Link    Specifies the new symbolic link directory location.
    echo     Target  Specifies the directory location (relative or absolute) that
    echo             the new links refer to.
    echo     File    Specifies the target and link file name.
    goto :eof

:: usage: goto :mklink_usage
:mklink_usage
    echo Creates a symbolic link.
    echo.
    echo %~nx0 [[/D] | [/H] | [/J]] Link Target
    echo.
    echo     /D      Creates a directory symbolic link.  Default is a file
    echo             symbolic link.
    echo     /H      Creates a hard link instead of a symbolic link.
    echo     /J      Creates a Directory Junction.
    echo     Link    Specifies the new symbolic link name.
    echo     Target  Specifies the path (relative or absolute) that the new link
    echo             refers to.
    goto :eof

:: usage: goto :symlink_all_usage
:symlink_all_usage
    echo Creates symbolic links for directory contents.
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
