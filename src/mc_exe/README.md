# mc_tool.exe

**This reconstruction is still on-going. Most information listed below either isn't relevant yet. Or is planned once the program reaches a buildable stage.**

An open-source reconstruction of `mc.exe` from the CatSystem 2 toolset. Features an English translation, non-Japanese locale support, unicode filepath support, and decompiling `.cst` files back to `.txt` scripts.

This tool aims to keep all odd/wonkey behavior related to token parsing **intact**. <!--This means you can go wild, with label strings: `#"Hello label!"`, 0% whitespace runs: `loop@0x1"Hello Label!"`, single-quote literals: `1 6 'A` / `1 'A' 6`, keywords as label names: `#jump\n  jump jump //around now`, and many other exciting syntaxes that defy logic!! -->


## Building

Refer to the compiler settings in `/.vscode/tasks.json` and `/.vscode/c_cpp_properties.json`

You **will** need to change the filepath and other settings to match your compiler. The current C++ standard is probably higher than necessary.

**Requirements:** WINAPI (MinGW or Visual Studio), C++`??`


## Usage

```cmd
CatSystem 2 Message Script compiler
compile:   mc_tool [/u] [/l] [/x] [txtfile] [txtfile] ...
decompile: mc_tool /d [cstfile] [cstfile] ...

arguments:
  txtfile   text file names to compile (with extension)
  /u        update only newer files (uses last modified time)
  /l        add debug lines (may break 3rd party tools!)
  /x        no compression (will break 3rd party tools!)

  /d        decompile mode
  cstfile   cst file names to decompile (with extension)

```

## Script features

> **Note:** These features are identical to the original mc.exe

### Macro definitions

The file `&macro.txt` (or `%macro.txt` if other not found) are automatically compiled with any txt scene scripts. They define optional macro commands (prefixed with `%`).

> **TODO:** Finish section on macros

#### \%echo command

The macro name `%echo` is reserved, and has a special function of outputting a message when encountered during the compilation process in txt scripts.

*myscene.txt*
```
#myscene_00_1
\charname \fn["This] [is] [a] [dialogue] [message."]\fn

    %echo This is a compile message.

    \fn[This] [is] [a] [monologue] [message.]\fn
```

During compilation, you will see:
```cmd
myscene.txt : echo This is a compile message.
```

### Include files

> **TODO:** Write section on includes

### Script name blocks

Lines starting with `#somescriptname` determine the cst output file name. Unlike other tools, one txt script file may output multiple cst files. So remembering to include a *(or change the)* name block is important.

> **TODO:** Finish section on script name blocks

