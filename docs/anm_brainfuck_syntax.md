# Brainfuck to ANM script syntax

*Because why not? ... :)*


### Rules

1. `ptr` address range is `0-61`, *not a power of 2 :(*
2. Standard wrapping rules apply for overflow
3. `*ptr` value range is `0-0xffffffff`
4. Printing `.` will change the current animation frame
5. There is no equivalent for input `,`

### Variables

|Var|Usage|
|:-|:-|
|`0-61`|User variables
|`62`|Pointed-to value *(for printing)*|
|`63`|Pointer variable|


## Conversion table

|Description|Brainfuck|ANM script|
|:-|:-|:-|
|(program start)| |`set 62 0`<br>`set 63 0`|
|`++ptr;`|`>`|`ifn 63 61 add_%d`<br>`set 63 0xffffffff`<br>`#add_%d`<br>`add 63 1`<br>*(`set 62 @63`)*|
|`--ptr;`|`<`|`ifn 63 0 sub_%d`<br>`set 63 62`<br>`#sub_%d`<br>`sub 63 1`<br>*(`set 62 @63`)*|
|`++*ptr;`|`+`|`add @63 1`|
|`--*ptr;`|`-`|`sub @63 1`|
|`putchar(*ptr);`|`.`|*(`set 62 @63`)*<br>`@62 1`|
|`*ptr=getchar();`|`,`|*no equivalent*|
|`while (*ptr) {`|`[`|`#while_%d`|
|`}`|`]`|`if 63 while_%d`|


***

## See also

### External links

* [Wikipedia: Brainfuck Commands](https://en.wikipedia.org/wiki/Brainfuck#Commands)
