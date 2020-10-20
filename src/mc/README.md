### What is this?
This is a fully decompiled version of the cs2 script compiler (build date 2016-07-06), taken from the public devkit.  
Yes, the code looks horrible and there are about four billion warnings! That's a side effect of the decompilation process, which is very much imperfect.  
Also, there is a buffer access without bounds checks just about every three lines, don't blame me for that :P  

### Differences to the original
This is a decompiled version of the original executable. During the compilation process, all symbol names have been stripped, therefore the names are probably different to the original.  
In addition, I'm using a different library (version) for compression. The original compiler used some sort of object-oriented compressor class, whereas I'm simply using a recent build of the [zlib](https://zlib.net/) library.  
The difference in compression means that the generated files are not exactly the same, but they load into the engine with no problems.  
If you specify the `-x` command line flag, an uncompressed version of the script will be generated that will be exactly the same as produced by the original compiler.  

### Notes
I've eliminated some errors that came up in the decompilation process, but it is possible there are some more - especially in the macro expansion logic, since I haven't tested that. Keep an eye out for `signed`/`unsigned` conversions, those are a common source of errors when decompiling.  

Also, please keep in mind that -- while tool-assisted -- a lot of manual labor went into this, so human error is always possible.
