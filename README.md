# Yrin VM
Early preview. [YrinLang repo link](https://github.com/tommymarto/YrinLang).<br>
It's a stack-based VM (Virtual Machine) which runs special bytecode.

### Bytecode format
Instructions are 8 bytes wide.

1 byte | 1 bytes | 2 bytes | 4 bytes
------ | ------- | ------- | -------
OpCode | Flags | ExtData | Data
A | B | C | D


### Instructions list
There's a legend at the end of the table which explains 'Data' format.<br>
- RS (Runtime stack)
- DS (Data Stack)

OpCode | OpName | Data | Info
------ | ------ | ---- | -----
0 | Reserved | 0 |
1 | Push | Number | Add 'Number' to RS
2 | Pop | 0 | Pop top element from RS
3 | Store | 0 | Pop top element from RS and push it into the DS
4 | Load | Index | Copy element 'Index' position from DS top into RS
5 | Add | 0 | Pop two elements from RS and push their sum back to RS
6 | Sub | 0 | Pop two elements from RS and push their subtraction back to RS
7 | Mul | 0 | Pop two elements from RS and push their multiplication back to RS
8 | Div | 0 | Pop two elements from RS and push their division back to RS
9 | Mod | 0 | Pop two elements from RS and push their integer division back to RS

Flags table and Data format table

Name | Position | Values
---- | -------- | ------
DATA_TYPE | B1 | DECIMAL (0) or INTEGER(1)

Name | Flags to set | Data
---- | ----- | ----
Number | DATA_TYPE (0 or 1) | DECIMAL / INTEGER
Index | 0 | INTEGER (offset from stack's top)
