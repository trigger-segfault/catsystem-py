# Trigger's CatSystem2 Tools


some bugs still exist - all require support for native dlls

**cs2\_tool version:** `cs2_tool-new_WIP-2020-08-20_new3-ILedit.exe`

Current iteration of cs2_tool.exe: `cs2_tool-new_WIP-2020-08-20_new3-ILedit.exe`

Absolutely horrifying version control, yet again. *yikes!*


## cs2_tool.exe

**Note:** This tool does *not* support *(or detect)* early CatSystem 2 `.int` archive formats. Unexpected behavior is likely.

```cmd
CatSystem 2 .int Archive Unpacker
usage: cs2_tool.exe [ARCHIVE=*] [ENTRY=*] [-i INPUTDIR] [-o OUTDIR] [-m WILDCARD|-r REGEX|-R REGEX] [-b EXE|-v V_CODE2] [-g EXE|-G] [-e|-d|-D|-l] [-s|-S]
alt usage: cs2_tool.exe <-g EXE|-G>

parameters:
  ARCHIVE       archive file or wildcard pattern
  ENTRY         archive entry files or wildcard pattern (same as -m WILDCARD)
  -i INPUTDIR   search for archives in directory path (default: CURRENT)
  -o OUTPUTDIR  output directory for extracted files (default: 'extracted')
  -m WILDCARD   only process matching entry names in archives (wildcard ?*)
  -r REGEX      only process matching entry names in archives (regular expression)
  -R REGEX      Same as -r, but do not ignore case
  -b EXE        game binary to extract V_CODE2 from (may be .bin file)
  -v V_CODE2    V_CODE2 string used to decrypt archive filenames
  -g EXE        get V_CODE2 from game executable (or .bin file)
  -a STYLE      append archive subdir to path, see below (default: name)
                empty (./), name (config/), file (config.int/)
  -A            don't append archive subdir to path (./) (alias for -a empty)
  -x STYLE      extension format for decompiled files, see below (default: both)
                empty (), orig (.cst), text (.txt), both (.cst.txt), join (.cst-txt), script (.cst-script)
  -X            don't add original extension for decompiled files (.txt) (alias for -x text)

switches:
  -e   extract raw files only
  -d   decompile/extract appropriate files without extacting raw
  -D   decompile/extract appropriate files AND extract raw files
  -l   only list filenames, but do not extract
  -L   only list number of files, but do not extract
  -s   silent, no output besides errors
  -S   verbose, list every extracted file
  -G   find and get V_CODE2 from all game executables (shorthand for '-g *'
  -f   vertically flip extracted images (sometimes stored like that)
  -F   do not vertically flip extracted images (default)
```

### Examples

#### Extract "config.int" using encryption keys in "Grisaia.exe"

```cmd
cs2_tool config.int -b Grisaia.exe
```

#### Extract "config.int" using encryption V\_CODE2 "FW-AW4YFHG8"

*Not a real V\_CODE*

```cmd
cs2_tool config.int -v "FW-AW4YFHG8"
```


#### Show V\_CODEs for "Grisaia.exe"

```cmd
cs2_tool -g Grisaia.exe
```


#### Show V\_CODEs of all CatSystem 2 executables in current directory

```cmd
cs2_tool -G
```


## cs2_decompile.exe

```cmd
CatSystem2 Script Decompiler
Usage: cs2_decompile [-i] <inputs...> [-o <output_dir>] [-u|-hr]
Options:
  -h|--help   Display more help information
 [-i|--input] <inputs...>  Input files or patterns to decompile
  -o|--output <output_dir>  Output directory for decompiled files
  -u|--utf8   Output file is UTF-8 (BOM), otherwise Shift JIS
 -hr|--human  Output CST file is human-readable dialogue (as UTF-8)
 -md|--markdown  Output CST file is human-readable markdown (as UTF-8)
```

### Examples

#### Decompile scripts in the fes folder

## zt_tool.exe

```cmd
CatSystem2 ZT Pack Extractor
Usage: zt_tool [-i] <inputs...> [-o <output_dir>] [-s|-l] [-v]
Options:
  -h|--help    Display more help information
 [-i|--input] <inputs...>  Input zt pack files or patterns to extract
  -o|--output <output_dir> Output directory of extracted files
  -s|--subdir  Extract to subdirs using name of zt pack file
  -l|--list    Do not extract files, only list them
  -v|--verbose List sizes of each file
```



## cs2key_tool

Tool for generating various types of keys needed by CatSystem 2. Most notably keys to enabled debug mode.

Can be zipped up and run as a standalone python file (i.e. `python cs2key_tool.zip ...`).

**Requires:** Python >= 3.6

**PyPI modules:** [pefile](https://pypi.org/project/pefile/)


### Examples

```bash
# for help
python cs2key_tool -h

# Generate debug key file "cs2_debug_key.dat" using cs2.exe for the encryption codes.
python cs2key_tool debug cs2.exe

# Generate direct key file "direct_NEW.dat" using cs2.exe for the encryption codes.
python cs2key_tool direct cs2.exe -o direct_NEW.dat


# Generate CD key file "key.dat" using cs2.exe for the encryption codes.
# NOTE: This type of key must be generated on the system intended to run the game.
#       This is because CD keys have extra encryption that includes a serial number
#        for the drive/volume holding the %WINDOWS% directory.
python cs2key_tool cd cs2.exe


# Generate global key file "cs2_gk.dat" (v1) using Djibril4.exe for the encryption codes, and listing the tamper-proof files: "Djibril4.exe", and "install.exe"
python cs2key_tool global Djibril4.exe -w Djibril4.exe install.exe

# Display the names/MD5 hashes of the existing global key file "cs2_gk.dat" (v1) using using Djibril4.exe for the encryption codes.
python cs2key_tool global Djibril4.exe -r cs2_gk.dat
```


## cstl_tool

Tool for generating and manipulating CatSystem 2 CSTL scene localization files.

Can be zipped up and run as a standalone python file (i.e. `python cstl_tool.zip ...`).

**Requires:** Python >= 3.6


### Examples

```bash
# for help
python cstl_tool -h

# decompile a CSTL file and output to INI
python cstl_tool -d c00_09.cstl -o c00_09.ini

# compile an INI file and output to CSTL
python cstl_tool -c c00_09.ini -o c00_09-NEW.cstl

# add new languages (he, llo) to the list of an INI file (all messages will be empty for that language)
# `-t ini` tells to output to the ini format, regardless of what format was input
python cstl_tool -c c00_09.ini -t ini -o c00_09-LANG.ini --add-langs he llo

# build a CSTL file from a basic CST CatScene script, and designate the default original langage as "jp"
python cstl_tool -b c03_06_h.cst -t ini -o c03_06_h.ini --orig-lang jp
```
