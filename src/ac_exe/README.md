# ac_tool.exe

An open-source reconstruction of `ac.exe` from the CatSystem 2 toolset. Features an English translation, unicode filepath support, and decompiling `.anm` files back to `.txt` scripts.

This tool aims to keep all odd/wonkey behavior related to token parsing **intact**. This means you can go wild, with label strings: `#"Hello label!"`, 0% whitespace runs: `loop@0x1"Hello Label!"`, single-quote literals: `1 6 'A` / `1 'A' 6`, keywords as label names: `#jump\n  jump jump //around now`, and many other exciting syntaxes that defy logic!!


## Building

Refer to the compiler settings in `/.vscode/tasks.json` and `/.vscode/c_cpp_properties.json`

You **will** need to change the filepath and other settings to match your compiler. The current C++ standard is probably higher than necessary.

**Requirements:** WINAPI (MinGW or Visual Studio), C++`??`


## Usage

```cmd
CatSystem 2 Animation Script compiler
compile:   ac_tool [txtfile] [txtfile] ...
decompile: ac_tool /d [anmfile] [anmfile] ...

arguments:
  txtfile   text file names to compile (with extension)

  /d        decompile mode
  anmfile   anm file names to decompile (with extension)

```
