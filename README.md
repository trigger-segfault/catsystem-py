# catsystem-py ![](docs/img/cs2/icon-manju-cheeky-top-computer-cs2-bottom--edit-red-blush.png)  ![](docs/img/cs2/icon-cs1-yui_taiken_dl.png)

<!--
![](docs/img/cs2/icon-manju-happy--edit-noleaf-fix--32x32px.png)
![](docs/img/cs2/icon-manju-cheeky-top-computer-cs2-bottom--edit-red-blush.png)
![](docs/img/cs2/icon-cs1-yui_taiken_dl.png)
-->

Work-in-progress library created in unison with reversing the CatSystem 1 and 2 VN engines.

**Python Package Name:** `catsys`<br>
**VSCode Extension Name:** `vscode-catsystem2`

**Warning:** The default branch is named `unstable` for a reason. This library has gone through countless changes, refactors, and rewrites. Some fueled by new knowledge about the Python language, and others purely because of OCD (yes literally).


This library is heavily tied to [TriggersTools.CatSystem2](https://github.com/trigger-segfault/TriggersTools.CatSystem2), and more importantly, the repo's [Wiki knowledge base](https://github.com/trigger-segfault/TriggersTools.CatSystem2/wiki).

***

## Contents

* Semi-functional Python library for Cs2 file formats, and other aspects. (`src/catsys/`)
* Open source reconstruction of ac.exe for better understanding of Cs2 script parsing, and decompiling support. (`scr/ac_exe/`)
* Unpublished tools used for decompiling script formats, extracting int archives with inline decompile options, and extracting and displaying cs2.exe decrypted vcodes and keycodes. (`tool/trigger/`)
* Rough English UI translations of official Cs2 toolset programs for creating and extracting HG2 and HG3 images. (`tool/cs2-translate/`)
* VSCode extensions for Cs2 script format syntax highlighting, WIP, CST is the most functional. (`plugin/vscode-catsystem2/`)
* Expanded detailed notes on the HG image encoding patent filed in the late 80's/early 90's. It nearly describes the format and compression in HG images down to smallest details. (`docs/document_US4982282A_*.md`, also see image encoding in Python library for heavy amount of comments)
* Various organized and unorganized notes, scribbles, and decompiled functions and structures from CatSystem 1 and 2, written over the last year and a half. (`docs/`)


***

This is the work of reverse engineering the CatSystem 2 (and then additionally CatSystem 1) engine. Although the original implementations were not in Python. Python and the interactive shell have been essential in interpreting raw data and most importantly prototyping everything that comes after.

There is no external documentation to this library, and there never will be. Good luck...
