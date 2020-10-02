# Animation (.anm) Commands

At the time of writing, every command type is known, as their names were ripped from the ac.exe binary file and then tested via compiling.

Every command after `if` is undocumented and untested, for the `ifx` family of commands, [var] and [value] may be in the opposite order.

`x = command name`<br/>
`[x] = required parameter`<br/>
`(x) = optional parameter`

|Command|Val|Description|
|:------|:--|:----------|
|[ID] [min] (max)|`0`|Display image Id for range of frames|
|set [var] [min] (max)|`1`|Set variable from range of values|
|loop [var] [label]|`2`|Jump to label if variable-- > 0|
|jump [label]|`3`|Jump to label|
|if [var] [label]|`4`|Jump to label if variable > 0|
|ife [var] [value] [label]|`5`|Jump to label if variable == value|
|ifn [var] [value] [label]|`6`|Jump to label if variable != value|
|ifg [var] [value] [label]|`7`|Jump to label if variable > value|
|ifs [var] [value] [label]|`8`|Jump to label if variable < value|
|ifge [var] [value] [label]|`9`|Jump to label if variable >= value|
|ifse [var] [value] [label]|`10`|Jump to label if variable <= value|
|max [param]|`11`|Unknown functionality|
|blend [param]|`12`|Unknown functionality|
|disp [bool]|`13`|Set visibility of image?|
|pos [x] [y]|`14`|Set position of image?|
|wait [min] [max]|`15`|Wait for range of frames?|
|add [var] [value]|`16`|Add value to variable?|
|sub [var] [value]|`17`|Subtract value from variable?|


## About Script Parameters

The three types of parameter are:
* `14` (a number)
* `@5` (a variable)
* `label` (a label name)

Each parameter type can be used for any paremeter of any command. If a label is used for a parameter not asking for a label, then the result will be a parameter with its value set to the label index. (Which is what happens even with parameters asking for labels.)

