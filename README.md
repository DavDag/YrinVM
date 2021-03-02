# Yrin VM
Early preview.
It's a stack-based VM (Virtual Machine) which runs particular bytecode.

### Bytecode format
Instructions are 8 bytes wide.

1 byte | 1 bytes | 2 bytes | 8 bytes
------ | ------- | ------- | -------
OpCode | Flags | ExtData | Data
A | B | C | D


### Instructions list
There's a legend at the end of the table which explains 'Data' format.

OpCode | OpName | Data
------ | ------ | ----
0 | Reserved | 0
1 | Push | Number
2 | Pop | 0
3 | Store | 0
4 | Load | Index
5 | Add | 0
6 | Sub | 0
7 | Mul | 0
8 | Div | 0
9 | Mod | 0

Flags table and Data format table

Name | Position | Values
---- | -------- | ------
DATA_TYPE | B1 | DECIMAL (0) or INTEGER(1)

Name | Flags to set | Data
---- | ----- | ----
Number | DATA_TYPE (0 or 1) | DECIMAL / INTEGER
Index | 0 | INTEGER (offset from stack's top)
