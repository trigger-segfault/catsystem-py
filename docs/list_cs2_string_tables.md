# CatSystem2 Name Tables

<!-- ^\|`[^`]*(8|sj)` -->

## Struct('<I 16s') @ `0x0071e290`

Match: `exe.find(pack('<I16s', 1, b'add(+)'))`

### Table Entries

|Symbol|ID|
|:-----|:-|
|`add(+)`|`0x01`|
|`sub(-)`|`0x02`|
|`mul(*)`|`0x03`|
|`div(/)`|`0x04`|
|`mod(%)`|`0x05`|
|`and(&)`|`0x06`|
|`or(|)`|`0x07`|
|`not(~)`|`0x08`|
|`xor(^)`|`0x09`|
|`shl(<<)`|`0x0a`|
|`shr(>>)`|`0x0b`|
|`neg(!)`|`0x0c`|
|`iand(&&)`|`0x0d`|
|`ior(||)`|`0x0e`|
|`big(>)`|`0x0f`|
|`sml(<)`|`0x10`|
|`beq(>=)`|`0x11`|
|`seq(<=)`|`0x12`|
|`eq(==)`|`0x13`|
|`neq(!=)`|`0x14`|
|`mov(=)`|`0x15`|
|`inc(++)`|`0x16`|
|`dec(--)`|`0x17`|
|`incp(++)`|`0x18`|
|`decp(--)`|`0x19`|
|`csgn(-)`|`0x1a`|
|`mode(%=)`|`0x1b`|
|`ande(&=)`|`0x1c`|
|`mule(*=)`|`0x1d`|
|`adde(+=)`|`0x1e`|
|`sube(-=)`|`0x1f`|
|`dive(/=)`|`0x20`|
|`shle(<<=)`|`0x21`|
|`shre(>>=)`|`0x22`|
|`xore(^=)`|`0x23`|
|`ore(|=)`|`0x24`|
|`push`|`0x25`|
|`pop`|`0x26`|
|`jz`|`0x27`|
|`jnz`|`0x28`|
|`jmp`|`0x29`|
|`call`|`0x2a`|
|`gcall`|`0x2b`|
|`ret`|`0x2c`|
|`popn`|`0x2d`|
|`adr`|`0x2e`|
|`pnt`|`0x2f`|
|`cpy`|`0x30`|
|`adst`|`0x31`|
|`sbst`|`0x32`|
|`popax`|`0x33`|
|`jeax`|`0x34`|
|`jrax`|`0x35`|
|`utof`|`0x36`|
|`itof`|`0x37`|
|`ftou`|`0x38`|
|`ftoi`|`0x39`|
|`cpst`|`0x3a`|
|`exit`|`0x3b`|
|`end`|`0x3c`|
|`otog`|`0x3d`|
|`padd`|`0x3e`|
|`break`|`0x3f`|
|`continue`|`0x40`|
|`d_line`|`0x41`|
|`d_func`|`0x42`|
|`d_push`|`0x43`|
|`d_pop`|`0x44`|
|` `|`0xffffffff`|

## Struct ('<I 16s') @ `0x00714570`

Match: `exe.find(pack('<I16sI16s', 0x12,b'if', 0x13,b'else'))`

### Table Entries

|Symbol|ID|
|:-----|:-|
|`if`|`0x12`|
|`else`|`0x13`|
|`while`|`0x14`|
|`do`|`0x15`|
|`for`|`0x16`|
|`switch`|`0x17`|
|`case`|`0x18`|
|`range`|`0x19`|
|`default`|`0x1a`|
|`goto`|`0x1b`|
|`return`|`0x1c`|
|`break`|`0x1d`|
|`continue`|`0x1e`|
|`struct`|`0x1f`|
|`union`|`0x20`|
|`enum`|`0x21`|
|`function`|`0x23`|
|`global`|`0x25`|
|`heap`|`0x26`|
|`@`|`0x27`|
|`void`|`0x29`|
|`char`|`0x2a`|
|`byte`|`0x2b`|
|`short`|`0x2c`|
|`word`|`0x2d`|
|`long`|`0x2e`|
|`dword`|`0x2f`|
|`float`|`0x30`|
|` `|`0xbd`|


#### MANUAL ENTRY

|Symbol|ID|Notes|
|:-----|:-|:----|
|` `|`0x00`|NOT FOUND *(NULL?*)|
|`#`|`0x01`|Number Sign|
|`$`|`0x02`|Dollar Sign|
|`$$`|`0x03`|Double Dollar Sign|
|`\`|`0x04`|Backslash|
|`/\n|\r\n?/`|`0x05`|Newline|
|`/ |\t/`|`0x06`|Whitespace|
| |`0x07`|???|
|`\'`|`0x0d`|Integer/Single Quote (Char String?)|
| |`0x0e`|Float|
|`\"`|`0x0f`|Double Quote (String?)|
| |`0x10`|
| |`0x11`|
|||
|`!`|`0x80`|Logical NOT|
|`!=`|`0x81`|Not Equal to|
|`%`|`0x82`|Remainder|
|`&`|`0x83`|Bitwise AND|
|`&&`|`0x84`|Logical AND|
|`(`|`0x85`|Open Parentheses|
|`)`|`0x86`|Close Parentheses|
|`*`|`0x87`|Multiply|
|`+`|`0x88`|Add|
|`++`|`0x89`|Increment|
| |`0x8a`|
|`,`|`0x8b`|Comma|
|`-`|`0x8c`|Subtract|
|`--`|`0x8d`|Decrement|
| |`0x8e`|
|`->`|`0x8f`|Point to|
|`.`|`0x90`|Dot|
|`...`|`0x91`|Ellipsis|
|`/`|`0x92`|Divide|
|`:`|`0x93`|Colon|
|`;`|`0x94`|Semicolon|
|`<`|`0x95`|Less than|
|`<<`|`0x96`|Bitwise LSHIFT|
|`<=`|`0x97`|Less or Equal|
|`=`|`0x98`|Assignment
|`%=`|`0x99`|Remainder assignment|
|`&=`|`0x9a`|Bitwise AND assignment|
|`*=`|`0x9b`|Multiply assignment|
|`+=`|`0x9c`|Add assignment|
|`-=`|`0x9d`|Subtract assignment|
|`/=`|`0x9e`|Divide assignment|
|`<<=`|`0x9f`|Bitwise LSHIFT assignment|
|`==`|`0xa0`|Equal to|
|`>>=`|`0xa1`|Bitwise RSHIFT assignment|
|`^=`|`0xa2`|Bitwise XOR assignment|
|`|=`|`0xa3`|Bitwise OR assignment|
|`>`|`0xa4`|Greater than|
|`>=`|`0xa5`|Greater or Equal|
|`>>`|`0xa6`|Bitwise RSHIFT|
|`?`|`0xa7`|Question Mark|
|`[`|`0xa8`|Open Bracket|
|`]`|`0xa9`|Close Bracket|
|`^`|`0xaa`|Bitwise XOR|
|`{`|`0xab`|Open Brace|
|`|`|`0xac`|Bitwise OR|
|`||`|`0xad`|Logical OR|
|`}`|`0xae`|Close Brace|
|`~`|`0xaf`|Bitwise NOT|
| |`0xb0`|
| |`0xb1`|
| |`0xb2`|
| |`0xb3`|
|`sizeof`|`0xb4`|Operator with no table entry|
| |`0xb5`|
| |`0xb6`|
| |`0xb7`|
| |`0xb8`|
| |`0xb9`|
| |`0xba`|
| |`0xbb`|
| |`0xbc`|
|` `|`0xbd`|END OF TABLE *(invalid?)*|



## Python Helpers

```py
def writetabl(tabl, file, codekey=0, symkey=1):
    with open(file, 'a') as f:
        f.write('\n')
        f.write('|Symbol|ID|\n')
        f.write('|:-----|:-|\n')
        for instr in tabl:
            f.write('|`{0!s}`|`0x{1:02x}`|\n'.format(instr[symkey], instr[codekey]))
        f.write('\n')
        f.flush()

def writeaddrtabl(tabl, file, addrkey=0, symkey=1):
    with open(file, 'a') as f:
        f.write('\n')
        f.write('|Symbol|Func\*|\n')
        f.write('|:-----|:-----|\n')
        for instr in tabl:
            f.write('|`{0!s}`|`0x{1:08x}`|\n'.format(instr[symkey], instr[addrkey]))
        f.write('\n')
        f.flush()

def writelist(tabl, file):
    with open(file, 'a') as f:
        f.write('\n')
        f.write('|Symbol|\n')
        f.write('|:-----|\n')
        for item in tabl:
            f.write('|`{0!s}`|\n'.format(item))
        f.write('\n')
        f.flush()

def writeu3tabl(tabl, file, code1key=0, code2key=1, code3key=2):
    with open(file, 'a') as f:
        f.write('\n')
        f.write('|Code 1|Code 2|Code 3|\n')
        f.write('|-----:|-----:|-----:|\n')
        for instr in tabl:
            f.write('|{3}`0x{0:x}`|{4}`0x{1:x}`|{5}`0x{2:x}`|\n'.format(instr[code1key], instr[code2key], instr[code3key], ' '*(10-len(hex(instr[code1key]))), ' '*(10-len(hex(instr[code2key]))), ' '*(10-len(hex(instr[code3key]))) ))
        f.write('\n')
        f.flush()

def readfile(path):
    with open(path, 'rb') as f:
        return f.read()

hap2exe = readfile(r"path\to\cs2.exe")

FESFUNC = struct.Struct('<32s I')
instrtabl5, tabloff = [], hap2exe.find(b'if\x00\x00' + bytes(28))
instr = FESFUNC.unpack_from(hap2exe, tabloff)
while instr[1] != 0x00000000:
    instrtabl5.append((instr[1], instr[0].rstrip(b'\x00').decode('cp932')))
    tabloff += FESFUNC.size
    instr = FESFUNC.unpack_from(hap2exe, tabloff)

# def readtabl(file, locationexitfunc)
```

## Struct('<I 16s') @ `0x006d2680`

Match: `exe.find(pack('<I16s', 0, b'FILE'))`

### Table Entries

`(FILE|IDN|ID|POS|PL|BLEND|PARENT|BASE|SIZE|VRAM|PRI|PRI_P|DISP|ENABLE|DEFAULT|COLOR|STYPE|CHANNEL|KEY_U|KEY_D|KEY_L|KEY_R|POS2|SIZE2|TYPE|KEYBLOCK|TABLE|GROUP|SYSKEY|MASK|COUNT|BTYPE|DIV|EXPOS|PATHMODE|VIEWPORT)`


|Symbol|ID|
|:-----|:-|
|`FILE`|`0x00`|
|`IDN`|`0x01`|
|`ID`|`0x02`|
|`POS`|`0x22`|
|`PL`|`0x23`|
|`BLEND`|`0x24`|
|`PARENT`|`0x25`|
|`BASE`|`0x26`|
|`SIZE`|`0x27`|
|`VRAM`|`0x28`|
|`PRI`|`0x29`|
|`PRI_P`|`0x2a`|
|`DISP`|`0x2b`|
|`ENABLE`|`0x2c`|
|`DEFAULT`|`0x2d`|
|`COLOR`|`0x2e`|
|`STYPE`|`0x2f`|
|`CHANNEL`|`0x30`|
|`KEY_U`|`0x31`|
|`KEY_D`|`0x32`|
|`KEY_L`|`0x33`|
|`KEY_R`|`0x34`|
|`POS2`|`0x35`|
|`SIZE2`|`0x36`|
|`TYPE`|`0x37`|
|`KEYBLOCK`|`0x38`|
|`TABLE`|`0x39`|
|`GROUP`|`0x3a`|
|`SYSKEY`|`0x3b`|
|`MASK`|`0x3c`|
|`COUNT`|`0x3d`|
|`BTYPE`|`0x3e`|
|`DIV`|`0x3f`|
|`EXPOS`|`0x40`|
|`PATHMODE`|`0x41`|
|`VIEWPORT`|`0x42`|
|` `|`0x43`|

## Struct('<I 8s') @ `0x006d2968`

Match: `exe.find(pack('<I8sI8s', 0, b'p', 1, b'BX'))`

### Table Entries

|Symbol|ID|
|:-----|:-|
|`p`|`0x00`|
|`BX`|`0x01`|
|`bx`|`0x02`|
|`BY`|`0x03`|
|`by`|`0x04`|
|`BZ`|`0x05`|
|`bz`|`0x06`|
|`PX`|`0x07`|
|`px`|`0x08`|
|`PY`|`0x09`|
|`py`|`0x0a`|
|`PZ`|`0x0b`|
|`pz`|`0x0c`|
|`VX`|`0x0d`|
|`vx`|`0x0e`|
|`VY`|`0x0f`|
|`vy`|`0x10`|
|`VZ`|`0x11`|
|`vz`|`0x12`|
|`RX`|`0x13`|
|`rx`|`0x14`|
|`RY`|`0x15`|
|`ry`|`0x16`|
|`RZ`|`0x17`|
|`rz`|`0x18`|
|`SX`|`0x19`|
|`sx`|`0x1a`|
|`SY`|`0x1b`|
|`sy`|`0x1c`|
|`SZ`|`0x1d`|
|`sz`|`0x1e`|
|`CR`|`0x1f`|
|`cr`|`0x20`|
|`CG`|`0x21`|
|`cg`|`0x22`|
|`CB`|`0x23`|
|`cb`|`0x24`|
|`CA`|`0x25`|
|`ca`|`0x26`|
|`CX`|`0x27`|
|`cx`|`0x28`|

## Struct('<32s I') @ `0x004bec78`

Match: `exe.find(pack('<32s', b'if'))`

### Table Entries

|Symbol|Func\*|
|:-----|:-----|
|`if`|`0x00748c60`|
|`endif`|`0x00748d00`|
|`wait`|`0x00748da0`|
|`next`|`0x00748d10`|
|`keyskip`|`0x007490d0`|
|`exit`|`0x007492b0`|
|`execfes`|`0x00749360`|
|`endfes`|`0x007494f0`|
|`reset_frame`|`0x00749560`|
|`baseimg`|`0x00749590`|
|`execkcs`|`0x007496e0`|
|`endkcs`|`0x007497d0`|
|`cgreg_init`|`0x00749840`|
|`cgreg`|`0x00749a30`|
|`cgreg_t`|`0x00749b20`|
|`cgregnum`|`0x00749c50`|
|`cgregmax`|`0x00749d80`|
|`cgregtbl`|`0x00749eb0`|
|`saveexist`|`0x00749fa0`|
|`call`|`0x0074a190`|
|`datasave`|`0x0074a250`|
|`config_open`|`0x0074a310`|
|`automove`|`0x0074a370`|
|`restoremove`|`0x0074a550`|
|`react`|`0x0074a5b0`|
|`checktitle`|`0x0074a610`|
|`break`|`0x0074a740`|
|`config`|`0x0074a7f0`|
|`rand`|`0x0074a8e0`|
|`randtbl`|`0x0074a9b0`|
|`basebtnpri`|`0x0074ab60`|
|`send`|`0x0074ac10`|
|`fileexist`|`0x0074ada0`|
|`sysvoice`|`0x0074ae40`|
|`strvar`|`0x0074afa0`|
|`setstbl`|`0x0074b1e0`|
|`getstbl`|`0x0074b400`|
|`getconf`|`0x0074b540`|
|`setconf`|`0x0074c4b0`|
|`resetactive`|`0x0074d510`|
|`result`|`0x0074ec70`|
|`deletebgbutton`|`0x0074d560`|
|`rettitle`|`0x0074d5c0`|
|`opensite`|`0x0074d610`|
|`clipboard`|`0x0074d750`|
|`savedelete`|`0x0074d840`|
|`saveexchg`|`0x0074d910`|
|`savecopy`|`0x0074da30`|
|`getweek`|`0x0074db50`|
|`newsave`|`0x0074dc50`|
|`lockactive`|`0x0074de80`|
|`decode`|`0x0074df30`|
|`input_text`|`0x0074e090`|
|`saveapend_str`|`0x0074e260`|
|`saveget_str`|`0x0074e3e0`|
|`messagebox`|`0x0074e540`|
|`messagebox8`|`0x0074e690`|
|`setexvar`|`0x00748b50`|
|`getexvar`|`0x00748bd0`|
|`debug`|`0x0074df90`|
|`dbgtime`|`0x0074dff0`|
|`fw_init`|`0x00754f40`|
|`fw_hash`|`0x00754fa0`|
|`fw_limitpos`|`0x00755090`|
|`fw_auth`|`0x00755150`|
|`fw_activate`|`0x00755240`|
|`fw_actcode`|`0x007552a0`|
|`fw_uniquecode`|`0x00755360`|
|`fw_status`|`0x007553c0`|
|`fw_destroy`|`0x00755420`|
|`maketask`|`0x0074ea10`|
|`deletetask`|`0x0074eb80`|
|`fontcom`|`0x0074ece0`|
|`sendbtn`|`0x0074f160`|
|`rdraw`|`0x0074f1d0`|
|`rwipe`|`0x0074f3d0`|
|`xchgcursor`|`0x0074e7e0`|
|`force_redraw`|`0x0074f630`|
|`wndmenu`|`0x0074e960`|
|`baseimg_pri`|`0x0074f650`|
|`wbreak`|`0x0074f700`|
|`dic_find`|`0x0074fa40`|
|`dic_appear`|`0x0074fbb0`|
|`dic_title`|`0x0074fc70`|
|`dic_ind`|`0x0074fd80`|
|`dic_max_appear`|`0x0074fe60`|
|`dic_update`|`0x0074ff20`|
|`dic_update_clear`|`0x00750030`|
|`dic_id`|`0x007500c0`|
|`dbgstr`|`0x00750180`|
|`dbgstru8`|`0x007501e0`|
|`bitcount`|`0x00750250`|
|`btnsimulate`|`0x007502e0`|
|`automoveex`|`0x007503e0`|
|`rwmos`|`0x00750500`|
|`rfmos`|`0x00750760`|
|`sortsave`|`0x00750a00`|
|`sortsave2`|`0x00750cc0`|
|`enablebgbutton`|`0x00750ff0`|
|`fps_enable`|`0x007510a0`|
|`fps_get`|`0x00751120`|
|`vram_get`|`0x007511f0`|
|`isdebug`|`0x007512e0`|
|`_redirect`|`0x00751350`|
|`_redirect_erase`|`0x007516a0`|
|`_redirect_clear`|`0x007517b0`|
|`gettime`|`0x007517c0`|
|`touch_kill`|`0x00751970`|
|`touch_enable`|`0x007519d0`|
|`check_install`|`0x00751a80`|
|`exit_boot`|`0x00751cf0`|
|`strcmp`|`0x00751ef0`|
|`getsndconfex`|`0x00751fd0`|
|`setsndconfex`|`0x00752970`|
|`sysvoiceex`|`0x00753620`|
|`psversion`|`0x00753a90`|
|`cursormove`|`0x00753b00`|
|`opensiteenc`|`0x00753bd0`|
|`saveget_date`|`0x00754280`|
|`inputdevice`|`0x00754550`|
|`replacespace`|`0x007545b0`|
|`steam_init`|`0x00754750`|
|`steam_achievement`|`0x00754760`|
|`steam_achievement_reset`|`0x007547c0`|
|`steam_isbroadcast`|`0x007547e0`|
|`steam_isenable`|`0x00754860`|
|`steam_enablescreenshot`|`0x007548e0`|
|`cgreg_getlist`|`0x00754950`|
|`clearstrvar`|`0x00754ae0`|
|`strvar_beta`|`0x00754b90`|



## Struct('<16s I') @ `0x004bfee8`

Match: `exe.find(pack('<16s', b'setid'))`

### Table Entries

|Symbol|Func\*|
|:-----|:-----|
|`setid`|`0x00738a10`|
|`getid`|`0x00738b30`|
|`enable`|`0x00738c20`|
|`disp`|`0x00738d30`|
|`draw`|`0x00738f20`|
|`wipe`|`0x00739300`|
|`getpos`|`0x00739920`|
|`stop`|`0x00739b40`|
|`blend`|`0x00739c10`|
|`load`|`0x00739e80`|
|`play`|`0x0073a390`|
|`copy`|`0x0073a4b0`|
|`delete`|`0x0073a6e0`|
|`size`|`0x0073a740`|
|`color`|`0x0073a8c0`|
|`noact`|`0x0073ab00`|
|`sndtbl`|`0x0073abf0`|
|`next`|`0x0073ad60`|
|`rev`|`0x0073ae50`|
|`repeat`|`0x0073af40`|
|`shuffle`|`0x0073afe0`|
|`settbl`|`0x0073b0c0`|
|`loadlist`|`0x0073b250`|
|`loadlist2`|`0x0073b410`|
|`dispid`|`0x0073b5d0`|
|`ptype`|`0x0073b770`|
|`setnum`|`0x0073c020`|
|`setsave`|`0x0073c130`|
|`pos`|`0x0073c250`|
|`pl`|`0x0073c670`|
|`getpos2`|`0x0073c7a0`|
|`setact`|`0x0073c970`|
|`page`|`0x0073ca60`|
|`getsize`|`0x0073cc90`|
|`ref`|`0x0073ce80`|
|`clip`|`0x0073cf90`|
|`lock`|`0x0073d110`|
|`exist`|`0x0073d1f0`|
|`max`|`0x0073d3b0`|
|`keypos`|`0x0073d7b0`|
|`attach`|`0x0073d940`|
|`getflag`|`0x0073dab0`|
|`arc`|`0x0073db80`|
|`mcarc`|`0x0073de10`|
|`userfont`|`0x0073e0a0`|
|`channel`|`0x0073e5a0`|
|`stype`|`0x0073e690`|
|`tone`|`0x0073e780`|
|`tonetbl`|`0x0073e920`|
|`gettonetbl`|`0x0073e9f0`|
|`coordmode`|`0x0073ebb0`|
|`pend`|`0x0073ed10`|
|`mcpend`|`0x0073efb0`|
|`setanm`|`0x0073f200`|
|`anm`|`0x0073f320`|
|`keyblock`|`0x0073f420`|
|`layout`|`0x0073f580`|
|`str`|`0x00743340`|
|`strsj`|`0x00743580`|
|`reset`|`0x007437e0`|
|`locator`|`0x00743940`|
|`pos2`|`0x00743a10`|
|`baseopt`|`0x00743bc0`|
|`init`|`0x00743ca0`|
|`param`|`0x00743df0`|
|`start`|`0x00743f20`|
|`apend`|`0x00743f90`|
|`apendmark`|`0x007441d0`|
|`clear`|`0x00744290`|
|`isdraw`|`0x00744360`|
|`isdrawmark`|`0x00744430`|
|`skip`|`0x00744500`|
|`refex`|`0x007445c0`|
|`setlog`|`0x00744720`|
|`blockinfo`|`0x00744820`|
|`linecolor`|`0x00744b40`|
|`blockcolor`|`0x00744cb0`|
|`lineinfo`|`0x00744de0`|
|`endpos`|`0x00744fc0`|
|`attr`|`0x00745220`|
|`refparentpl`|`0x00745500`|
|`mag`|`0x00745610`|
|`time`|`0x00745750`|
|`userfontobj`|`0x00745920`|
|`delay`|`0x00745b20`|
|`ismot`|`0x00745be0`|
|`insert`|`0x00745ce0`|
|`name`|`0x00745d60`|
|`getdiv`|`0x00745f40`|
|`redraw`|`0x00745fd0`|
|`refcap`|`0x00746050`|
|`filter`|`0x00746180`|
|`pri`|`0x007462f0`|
|`linecnt`|`0x007464d0`|
|`getbase`|`0x007465c0`|
|`getoffset`|`0x00746790`|
|`drag`|`0x00746950`|
|`bmode`|`0x00746a30`|
|`mode`|`0x00746b20`|
|`getfile`|`0x007471d0`|
|`m2eff`|`0x00746c00`|
|`m2att`|`0x00746c20`|
|`m2fspeed`|`0x00746c40`|
|`m2prmscale`|`0x00746c60`|
|`m2body`|`0x00746c80`|
|`m2iner`|`0x00746d60`|
|`m2enable_blink`|`0x00746d80`|
|`m2enable_lip`|`0x00746da0`|
|`m2splay`|`0x00746dc0`|
|`m2draw`|`0x00746de0`|
|`m2enable_eff`|`0x00746e00`|
|`sndtag`|`0x00746ee0`|
|`strid`|`0x00747330`|
|`get`|`0x007476b0`|
|`pathinfo`|`0x00747b60`|
|`getsavestr`|`0x00747d60`|
|`loop`|`0x00747e50`|
|`blur`|`0x00747ee0`|
|`make`|`0x00748020`|
|`remake`|`0x007482f0`|
|`getsavedate`|`0x00748460`|
|`pause`|`0x00748630`|
|`crossfade`|`0x00748670`|
|`drawarea`|`0x007487a0`|

# Front End Script (FES)

## Struct('<I 32s') @ `0x006d1050`

Match: `exe.find(pack('<I32sI32s', 0, b'BASE', 1, b'CIMAGE'))`

### Table Entries
<!-- 
|BASE|BGTABLE2?|BUTTON|CHTABLE[23]?|CIMAGE|CPLANE|DRAW|

|IMAGE|PLANE|SAVEPANEL|SCROLL2?|SLIDER|SOUND|SNDTABLE|VIEW|WIPE -->

|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
BASE|CIMAGE|IMAGE|CPLANE|PLANE|VIEW|BUTTON|DRAW|WIPE|SOUND|SNDTABLE|PANEL|SAVEPANEL|SCROLL|SCROLL2|SLIDER|BGTABLE|BGTABLE2|CHTABLE|CHTABLE2|CHTABLE3|STRING|IMGWND|PARTICLE|LOGSTRING|DIFIMAGE|RDRAW|RWIPE|NUMIMG|NVSTRING|WBREAK|RWMOS|RFMOS|STRTABLE|FILELIST|M2PLANE|EFKPLANE|STRING8|LOGSTRING8|NVSTRING8|VIEWPORT


|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|20|21|22|23|24|25|26|27|28|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
BASE|C?IMAGE||C?PLANE||VIEW|BUTTON|R?DRAW|R?WIPE|SOUND|SNDTABLE|PANEL|SAVEPANEL|SCROLL2?||SLIDER|BGTABLE2?||CHTABLE[23]?|||STRING8?|IMGWND|PARTICLE|LOGSTRING8?|DIFIMAGE|||NUMIMG|NVSTRING8?|WBREAK|RWMOS|RFMOS|STRTABLE|FILELIST|M2PLANE|EFKPLANE||||VIEWPORT

`(BASE|C?IMAGE|C?PLANE|VIEW|BUTTON|R?DRAW|R?WIPE|SOUND|SNDTABLE|PANEL|SAVEPANEL|SCROLL2?|SLIDER|BGTABLE2?|CHTABLE[23]?|STRING8?|IMGWND|PARTICLE|LOGSTRING8?|DIFIMAGE|NUMIMG|NVSTRING8?|WBREAK|RWMOS|RFMOS|STRTABLE|FILELIST|M2PLANE|EFKPLANE|VIEWPORT)`

|Symbol|ID|
|:-----|:-|
|`BASE`|`0x00`|
|`CIMAGE`|`0x01`|
|`IMAGE`|`0x02`|
|`CPLANE`|`0x03`|
|`PLANE`|`0x04`|
|`VIEW`|`0x05`|
|`BUTTON`|`0x06`|
|`DRAW`|`0x07`|
|`WIPE`|`0x08`|
|`SOUND`|`0x09`|
|`SNDTABLE`|`0x0a`|
|`PANEL`|`0x0b`|
|`SAVEPANEL`|`0x0c`|
|`SCROLL`|`0x0d`|
|`SCROLL2`|`0x0e`|
|`SLIDER`|`0x0f`|
|`BGTABLE`|`0x10`|
|`BGTABLE2`|`0x11`|
|`CHTABLE`|`0x12`|
|`CHTABLE2`|`0x13`|
|`CHTABLE3`|`0x14`|
|`STRING`|`0x15`|
|`IMGWND`|`0x16`|
|`PARTICLE`|`0x17`|
|`LOGSTRING`|`0x18`|
|`DIFIMAGE`|`0x19`|
|`RDRAW`|`0x1a`|
|`RWIPE`|`0x1b`|
|`NUMIMG`|`0x1c`|
|`NVSTRING`|`0x1d`|
|`WBREAK`|`0x1e`|
|`RWMOS`|`0x1f`|
|`RFMOS`|`0x20`|
|`STRTABLE`|`0x22`|
|`FILELIST`|`0x23`|
|`M2PLANE`|`0x21`|
|`EFKPLANE`|`0x24`|
|`STRING8`|`0x25`|
|`LOGSTRING8`|`0x26`|
|`NVSTRING8`|`0x27`|
|`VIEWPORT`|`0x28`|
|` `|`0x29`|


## Struct('<3I') @ `0x004bd350`

Match: `exe.find(pack('<3I3I', 0,0,0, 1,0x1e,0x1f))`

### Table Entries

|Code 1      |Code 2      |Code 3      |
|-----------:|-----------:|-----------:|
|       `0x0`|       `0x0`|       `0x0`|
|       `0x1`|      `0x1e`|      `0x1f`|
|       `0x2`|      `0x1e`|      `0x1f`|
|       `0x4`|      `0x1e`|      `0x1f`|
|       `0x3`|      `0x1e`|      `0x1f`|
|      `0x80`|      `0x1c`|      `0x1d`|
|      `0x81`|      `0x13`|      `0x12`|
|      `0x82`|      `0x1b`|      `0x1a`|
|      `0x83`|      `0x11`|      `0x10`|
|      `0x84`|       `0xb`|       `0xa`|
|      `0x85`|       `0x0`|      `0x21`|
|      `0x86`|      `0x20`|       `0x0`|
|      `0x87`|      `0x1b`|      `0x1a`|
|      `0x88`|      `0x19`|      `0x18`|
|      `0x89`|      `0x1d`|      `0x1c`|
|      `0x8a`|      `0x1c`|      `0x1d`|
|      `0x8c`|      `0x19`|      `0x18`|
|      `0x8d`|      `0x1d`|      `0x1c`|
|      `0x8e`|      `0x1c`|      `0x1d`|
|      `0x92`|      `0x1b`|      `0x1a`|
|      `0x94`|       `0x1`|       `0x0`|
|      `0x95`|      `0x15`|      `0x14`|
|      `0x96`|      `0x17`|      `0x16`|
|      `0x97`|      `0x15`|      `0x14`|
|      `0x98`|       `0x4`|       `0x5`|
|      `0x99`|       `0x4`|       `0x5`|
|      `0x9a`|       `0x4`|       `0x5`|
|      `0x9b`|       `0x4`|       `0x5`|
|      `0x9c`|       `0x4`|       `0x5`|
|      `0x9d`|       `0x4`|       `0x5`|
|      `0x9e`|       `0x4`|       `0x5`|
|      `0x9f`|       `0x4`|       `0x5`|
|      `0xa0`|      `0x13`|      `0x12`|
|      `0xa1`|       `0x4`|       `0x5`|
|      `0xa2`|       `0x4`|       `0x5`|
|      `0xa3`|       `0x4`|       `0x5`|
|      `0xa4`|      `0x15`|      `0x14`|
|      `0xa5`|      `0x15`|      `0x14`|
|      `0xa6`|      `0x17`|      `0x16`|
|      `0xa8`|       `0x0`|      `0x21`|
|      `0xa9`|      `0x20`|       `0x0`|
|      `0xaa`|       `0xf`|       `0xe`|
|      `0xac`|       `0xd`|       `0xc`|
|      `0xad`|       `0x9`|       `0x8`|
|      `0xaf`|       `0x0`|      `0x1d`|
|      `0xb0`|      `0x1c`|      `0x1d`|
|      `0xb1`|      `0x1c`|      `0x1d`|
|      `0xbe`|`0xffffffff`|       `0x0`|
|      `0x12`|       `0x0`|       `0x1`|
|`0xffffffff`|`0xffffffff`|`0xffffffff`|


## Struct('<16s') @ '0x00717a98'

Match: `exe.find(pack('<16s16s', b'include', b'define'))`

### Table Entries

|Symbol|
|:-----|
|`include`|
|`define`|
|`ifdef`|
|`ifndef`|
|`else`|
|`endif`|
|`entry`|
|`stack`|
|`version`|


# `tooltips_class32`



|Symbol|ID|Notes|
|:-----|:-|:----|
|` `|`0x00`|NOT FOUND *(NULL?*)|
|`#`|`0x01`|Number Sign|
|`$`|`0x02`|Dollar Sign|
|`$$`|`0x03`|Double Dollar Sign|
|`\`|`0x04`|Backslash|
|`/\n|\r\n?/`|`0x05`|Newline|
|`/ |\t/`|`0x06`|Whitespace|
| |`0x07`|???|
|`\'`|`0x0d`|Integer/Single Quote (Char String?)|
| |`0x0e`|Float|
|`\"`|`0x0f`|Double Quote (String?)|
| |`0x10`|
| |`0x11`|
||||
|`if`|`0x12`|If statement|
|`else`|`0x13`|Else statement|
|`while`|`0x14`|While Loop statement|
|`do`|`0x15`|Do while Loop statement|
|`for`|`0x16`|For Loop statement|
|`switch`|`0x17`|Switch statement|
|`case`|`0x18`|Case statement|
|`range`|`0x19`|Range case statement|
|`default`|`0x1a`|Default case statement|
|`goto`|`0x1b`|Goto statement|
|`return`|`0x1c`|Return statement|
|`break`|`0x1d`|Break statement|
|`continue`|`0x1e`|Continue statement|
|`struct`|`0x1f`|Struct data declaration|
|`union`|`0x20`|Union data declaration|
|`enum`|`0x21`|Enum data declaration|
| |`0x22`|
|`function`|`0x23`|Function keyword|
| |`0x24`|
|`global`|`0x25`|Global Storage keyword (API identifier)|
|`heap`|`0x26`|Heap Storage keyword(?)|
|`@`|`0x27`|
| |`0x28`|
|`void`|`0x29`|Void type|
|`char`|`0x2a`|Signed Int8 type|
|`byte`|`0x2b`|Unsigned Int8 type|
|`short`|`0x2c`|Signed Int16 type|
|`word`|`0x2d`|Unsigned Int16 type|
|`long`|`0x2e`|Signed Int32 type|
|`dword`|`0x2f`|Unsigned Int16 type|
|`float`|`0x30`|Single Floating Point type|
|||
|`!`|`0x80`|Logical NOT|
|`!=`|`0x81`|Not Equal to|
|`%`|`0x82`|Remainder|
|`&`|`0x83`|Bitwise AND|
|`&&`|`0x84`|Logical AND|
|`(`|`0x85`|Open Parentheses|
|`)`|`0x86`|Close Parentheses|
|`*`|`0x87`|Multiply|
|`+`|`0x88`|Add|
|`++`|`0x89`|Increment|
| |`0x8a`|
|`,`|`0x8b`|Comma|
|`-`|`0x8c`|Subtract|
|`--`|`0x8d`|Decrement|
| |`0x8e`|
|`->`|`0x8f`|Point to|
|`.`|`0x90`|Dot|
|`...`|`0x91`|Ellipsis|
|`/`|`0x92`|Divide|
|`:`|`0x93`|Colon|
|`;`|`0x94`|Semicolon|
|`<`|`0x95`|Less than|
|`<<`|`0x96`|Bitwise LSHIFT|
|`<=`|`0x97`|Less or Equal|
|`=`|`0x98`|Assignment
|`%=`|`0x99`|Remainder assignment|
|`&=`|`0x9a`|Bitwise AND assignment|
|`*=`|`0x9b`|Multiply assignment|
|`+=`|`0x9c`|Add assignment|
|`-=`|`0x9d`|Subtract assignment|
|`/=`|`0x9e`|Divide assignment|
|`<<=`|`0x9f`|Bitwise LSHIFT assignment|
|`==`|`0xa0`|Equal to|
|`>>=`|`0xa1`|Bitwise RSHIFT assignment|
|`^=`|`0xa2`|Bitwise XOR assignment|
|`|=`|`0xa3`|Bitwise OR assignment|
|`>`|`0xa4`|Greater than|
|`>=`|`0xa5`|Greater or Equal|
|`>>`|`0xa6`|Bitwise RSHIFT|
|`?`|`0xa7`|Question Mark|
|`[`|`0xa8`|Open Bracket|
|`]`|`0xa9`|Close Bracket|
|`^`|`0xaa`|Bitwise XOR|
|`{`|`0xab`|Open Brace|
|`|`|`0xac`|Bitwise OR|
|`||`|`0xad`|Logical OR|
|`}`|`0xae`|Close Brace|
|`~`|`0xaf`|Bitwise NOT|
| |`0xb0`|
| |`0xb1`|
| |`0xb2`|
| |`0xb3`|
|`sizeof`|`0xb4`|Operator with no table entry|
| |`0xb5`|
| |`0xb6`|
| |`0xb7`|
| |`0xb8`|
| |`0xb9`|
| |`0xba`|
| |`0xbb`|
| |`0xbc`|
|` `|`0xbd`|END OF TABLE *(invalid?)*|


```cpp
typedef enum {
    TOKEN_NONE    = 0x00,   //

    // SYMBOL/OPERATOR FUNCTION

    TOKEN_POUND   = 0x01,   // "#"
    TOKEN_DOLLAR  = 0x02,   // "$"
    TOKEN_DOLLAR2 = 0x03,   // "$$"
    TOKEN_BACKSLASH = 0x04, // "\\" 
    TOKEN_NEWLINE = 0x05,   // "\n", "\r", "\r\n"
    TOKEN_WHITESPACE = 0x06, // " ", "\t"

    // KEYWORD TABLE

    TOKEN_IF_KEYWORD = 0x12, // "if"
    TOKEN_ELSE_KEYWORD = 0x13, // "else"
    TOKEN_WHILE_KEYWORD = 0x14, // "while"
    TOKEN_DO_KEYWORD = 0x15, // "do"
    TOKEN_FOR_KEYWORD = 0x16, // "for"
    TOKEN_SWITCH_KEYWORD = 0x17, // "switch"
    TOKEN_CASE_KEYWORD = 0x18, // "case"
    TOKEN_RANGE_KEYWORD = 0x19, // "range"
    TOKEN_DEFAULT_KEYWORD = 0x1a, // "default"
    TOKEN_GOTO_KEYWORD = 0x1b, // "goto"
    TOKEN_RETURN_KEYWORD = 0x1c, // "return"
    TOKEN_BREAK_KEYWORD = 0x1d, // "break"
    TOKEN_CONTINUE_KEYWORD = 0x1e, // "continue"
    TOKEN_STRUCT_KEYWORD = 0x1f, // "struct"
    TOKEN_UNION_KEYWORD = 0x20, // "union"
    TOKEN_ENUM_KEYWORD = 0x21, // "enum"
    
    TOKEN_FUNCTION_KEYWORD = 0x23, // "function"
    
    TOKEN_GLOBAL_KEYWORD = 0x25, // "global"
    TOKEN_HEAP_KEYWORD = 0x26, // "heap"
    TOKEN_ATSIGN_KEYWORD = 0x27, // "@"
    TOKEN_ATSIGN = 0x27, // "@"

    TOKEN_VOID_KEYWORD = 0x29, // "void"
    TOKEN_CHAR_KEYWORD = 0x2a, // "char"
    TOKEN_BYTE_KEYWORD = 0x2b, // "byte"
    TOKEN_SHORT_KEYWORD = 0x2c, // "short"
    TOKEN_WORD_KEYWORD = 0x2d, // "word"
    TOKEN_LONG_KEYWORD = 0x2e, // "long"
    TOKEN_DWORD_KEYWORD = 0x2f, // "dword"
    TOKEN_FLOAT_KEYWORD = 0x30, // "float"

    // SYMBOL/OPERATOR FUNCTION

    TOKEN_LOGICALNOT_OPERATOR = 0x80, // "!"
    TOKEN_NOTEQUAL_OPERATOR = 0x81, // "!="
    
    TOKEN_PERCENT_OPERATOR = 0x82, // "%"
    TOKEN_MODULO_OPERATOR = 0x82, // "%"
    TOKEN_MODULUS_OPERATOR = 0x82, // "%"
    TOKEN_REMAINDER_OPERATOR = 0x82, // "%"

    TOKEN_BITWISEAND_OPERATOR = 0x83, // "&"
    TOKEN_LOGICALAND_OPERATOR = 0x84, // "&&"
    TOKEN_OPENPAREN_OPERATOR = 0x85, // "("
    TOKEN_CLOSEPAREN_OPERATOR = 0x86, // ")"
    TOKEN_MULTIPLY_OPERATOR = 0x87, // "*"
    TOKEN_ADD_OPERATOR = 0x88, // "+"
    TOKEN_INCREMENT_OPERATOR = 0x89, // "++"
    
    TOKEN_COMMA_OPERATOR = 0x8b, // ","
    TOKEN_SUBTRACT_OPERATOR = 0x8c, // "-"
    TOKEN_DECREMENT_OPERATOR = 0x8d, // "--"

    TOKEN_POINTTO_OPERATOR = 0x8f, // "->"
    TOKEN_DOT_OPERATOR = 0x90, // "."
    TOKEN_ELLIPSIS_OPERATOR = 0x91, // "..."
    TOKEN_DIVIDE_OPERATOR = 0x92, // "/"
    TOKEN_COLON_OPERATOR = 0x93, // ":"
    TOKEN_SEMICOLON_OPERATOR = 0x94, // ";"
    TOKEN_LESS_OPERATOR = 0x95, // "<"
    TOKEN_LSHIFT_OPERATOR = 0x96, // "<<"
    TOKEN_LESSEQUAL_OPERATOR = 0x97, // "<="
    TOKEN_ASSIGN_OPERATOR = 0x98, // "="
    TOKEN_MODULO_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_MODULUS_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_REMAINDER_ASSIGN_OPERATOR = 0x99, // "%="
    TOKEN_BITWISEAND_ASSIGN_OPERATOR = 0x9a, // "&="
    TOKEN_MULTIPLY_ASSIGN_OPERATOR = 0x9b, // "*="
    TOKEN_ADD_ASSIGN_OPERATOR = 0x9c, // "+="
    TOKEN_SUBTRACT_ASSIGN_OPERATOR = 0x9d, // "-="
    TOKEN_DIVIDE_ASSIGN_OPERATOR = 0x9e, // "/="
    TOKEN_LSHIFT_ASSIGN_OPERATOR = 0x9f, // "<<="
    TOKEN_EQUAL_OPERATOR = 0xa0, // "=="
    TOKEN_RSHIFT_ASSIGN_OPERATOR = 0xa1, // ">>="
    TOKEN_BITWISEXOR_ASSIGN_OPERATOR = 0xa2, // "^="
    TOKEN_BITWISEOR_ASSIGN_OPERATOR = 0xa3, // "|="
    TOKEN_GREATER_OPERATOR = 0xa4, // ">"
    TOKEN_GREATEREQUAL_OPERATOR = 0xa5, // ">="
    TOKEN_RSHIFT_OPERATOR = 0xa6, // ">>"
    TOKEN_QUESTIONMARK = 0xa7, // "?"
    TOKEN_OPENBRACKET_OPERATOR = 0xa8, // "["
    TOKEN_CLOSEBRACKET_OPERATOR = 0xa9, // "]"
    TOKEN_BITWISEXOR_OPERATOR = 0xaa, // "^"
    TOKEN_OPENBRACE_OPERATOR = 0xab, // "{"
    TOKEN_BITWISEOR_OPERATOR = 0xac, // "|"
    TOKEN_LOGICALOR_OPERATOR = 0xad, // "||"
    TOKEN_CLOSEBRACE_OPERATOR = 0xae, // "}"
    TOKEN_BITWISENOT_OPERATOR = 0xaf, // "~"

    // STANDALONE

    TOKEN_SIZEOF_OPERATOR = 0xb4, // "sizeof" (no table entry)

    TOKEN_END = 0xbd,
} TOKEN_TYPE;

```
