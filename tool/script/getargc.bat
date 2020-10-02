@echo off
call :getargc argc %*
echo Count is %argc%
echo Args are %*
goto :eof

:: usage: call :getargc <varname> <%*>
:: return: argc -> %<varname>%
::
:: modified from: <https://stackoverflow.com/a/1292079>
:getargc
    setlocal EnableExtensions
    set getargc_v0=%1
    set /a "getargc_rtn = 0"
:getargc_l0
    if not x%2x==xx (
        shift
        set /a "getargc_rtn = getargc_rtn + 1"
        goto :getargc_l0
    )
    endlocal && set /a "%getargc_v0% = %getargc_rtn%"
    goto :eof

