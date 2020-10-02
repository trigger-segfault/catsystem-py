# jpcmd - Japanese Command Prompt ![AppIcon](https://i.imgur.com/r31XqH5.png)

[![Latest Release](https://img.shields.io/github/release-pre/trigger-death/jpcmd.svg?style=flat&label=version)](https://github.com/trigger-death/jpcmd/releases/latest)
[![Latest Release Date](https://img.shields.io/github/release-date-pre/trigger-death/jpcmd.svg?style=flat&label=released)](https://github.com/trigger-death/jpcmd/releases/latest)
[![Total Downloads](https://img.shields.io/github/downloads/trigger-death/jpcmd/total.svg?style=flat)](https://github.com/trigger-death/jpcmd/releases)
[![Creation Date](https://img.shields.io/badge/created-march%202019-A642FF.svg?style=flat)](https://github.com/trigger-death/jpcmd/commit/9b6ecfe8875a2fbd472d3fce3dfebfd80ef53910)
[![Discord](https://img.shields.io/discord/436949335947870238.svg?style=flat&logo=discord&label=chat&colorB=7389DC&link=https://discord.gg/vB7jUbY)](https://discord.gg/vB7jUbY)

A Command Prompt launcher that sets the codepage to [Shift JIS (932)](https://en.wikipedia.org/wiki/Shift_JIS) for proper Japanese program output.

All command line arguments passed to `jpcmd.exe` are passed directly to `cmd.exe`.

Setting the path to [Locale Emulator's](https://github.com/xupefei/Locale-Emulator) `LEProc.exe` in the `config.ini` file will allow launching the program through `jpcmdle.exe` to make sure all programs launched from `jpcmd.exe` keep their Japanese locale. If `jpcmd.exe` is running properly in the Japanese locale then the console title will be *Japanese Command Prompt (LE)*.

The reason for the above feature is that even when running `cmd.exe` through [Locale Emulator](https://github.com/xupefei/Locale-Emulator), the codepage will not be set to [Shift JIS (932)](https://en.wikipedia.org/wiki/Shift_JIS), making the output practically useless.

![Preview](https://i.imgur.com/bos9XOe.png)
