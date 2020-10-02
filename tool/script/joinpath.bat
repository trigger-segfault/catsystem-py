@echo off
call :joinpath2 %*
echo Joined path is "%Result%"
call :joinpath joinedpath %*
echo Joined path is "%joinedpath%"
echo Args are %*
goto :eof



:joinpath2
    set Path1=%~1
    set Path2=%~2
    if {%Path1:~-1,1%}=={\} (set "Result=%Path1%%Path2%") else (set "Result=%Path1%\%Path2%")
    goto :eof

:: usage: call :joinpath <varname> <path1> <path2>
:: return: joined-backslash-noquotes-path -> %<varname>%
::
:: modified from: <https://stackoverflow.com/a/3114237>
:joinpath
    setlocal EnableExtensions
    call :backslashpath joinpath_v0 "%~2"
    call :backslashpath joinpath_v1 "%~3"
    if "%joinpath_v0%" == ""  set "joinpath_rtn=%joinpath_v1%" && goto :joinpath_10
    if "%joinpath_v1%" == ""  set "joinpath_rtn=%joinpath_v0%" && goto :joinpath_10
    if "%joinpath_v1:~0,1%" == "\" set "joinpath_rtn=%joinpath_v1%" && goto :joinpath_10
    if "%joinpath_v0:~-1,1%" == "\" ( set "joinpath_rtn=%joinpath_v0%%joinpath_v1%") else ( set "joinpath_rtn=%joinpath_v0%\%joinpath_v1%")
:joinpath_10
    endlocal && set "%1=%joinpath_rtn%"
    goto :eof

REM :: usage: call :joinpath <varname> <path1> <path2>
REM :: return: joined-backslash-noquotes-path -> %<varname>%
REM ::
REM :: modified from: <https://stackoverflow.com/a/3114237>
REM :joinpath
REM     REM setlocal EnableExtensions EnableDelayedExpansion
REM     setlocal EnableExtensions
REM     REM echo "%~2" "%~3"
REM     set "joinpath_v0=%~2"
REM     set "joinpath_v1=%~3"
REM     call :backslashpath joinpath_v0 "%joinpath_v0%"
REM     call :backslashpath joinpath_v1 "%joinpath_v1%"
REM     if "%joinpath_v0%" == "" set "joinpath_rtn=%joinpath_v1%" && goto :joinpath_10
REM     if "%joinpath_v1%" == "" set "joinpath_rtn=%joinpath_v0%" && goto :joinpath_10
REM     if "%joinpath_v1:~0,1%" == "\" set "joinpath_rtn=%joinpath_v1%" && goto :joinpath_10
REM     if "%joinpath_v0:~-1,1%" == "\" set "joinpath_v0=%joinpath_v0:~0,-1%"
REM     if not "%joinpath_v0:~-1,1%" == "\" set "joinpath_v0=%joinpath_v0%/"
REM     if "%joinpath_v0:~-1,1%" == "\" ( set "joinpath_rtn=%joinpath_v0%%joinpath_v1%") else ( set "joinpath_rtn=%joinpath_v0%\%joinpath_v1%")
REM :joinpath_10
REM     endlocal && set "%1=%joinpath_rtn%"
REM     goto :eof

:: usage: call :backslashpath <varname> <path>
:: return: backslashed-noquotes-path -> %<varname>%
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
