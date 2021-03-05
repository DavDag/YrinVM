import json
import sys
import ctypes


# Pre assemble checks
def metadata_checks(meta, byte):
    # Check bytecode version
    byte_version = byte["metadata"]["version"]
    meta_version = meta["metadata"]["version"]
    if meta_version != byte_version:
        print("Version " + byte_version + " is not supported")
        exit(1)


# Assemble bytecode
def assemble(meta, byte):
    # Instructions Table
    instrTable = meta["instructions"]

    # Instructions List
    instrList = byte["main"]

    # Dump
    with open(byte["out-filename"], "wb") as of:
        b = bytes([])
        for i in instrList:
            b += bytes_for_instruction(i, instrTable)
        of.write(b)


# Return instruction as bytes
def bytes_for_instruction(instr, table):
    name = instr[0]
    opcode = bytes(ctypes.c_byte(table[name]))
    if name == "PUSHb0" or name == "PUSHb1":
        return opcode
    elif name[:4] == "PUSH":
        return opcode + bytes_for_data(name[4::], instr[1])
    elif name == "LOAD":
        return opcode + bytes(ctypes.c_int16(instr[1]))
    else:
        return opcode


# Return data as bytes
def bytes_for_data(typ, data):
    if typ == "i":
        return bytes(ctypes.c_int(data))
    elif typ == "f":
        return bytes(ctypes.c_float(data))
    elif typ == "d":
        return bytes(ctypes.c_double(data))
    elif typ == "c":
        return bytes(ctypes.c_char(data))
    elif typ == "s":
        return bytes(ctypes.c_int16(data.size())) + bytes(data)
    else:
        print("type: " + typ + " not supported.")
        exit(1)


# Assembler main
if __name__ == "__main__":
    # BYTECODE configs
    metadataFile = "yvm_01b1.json"

    # File to assemble
    fileToAssemble = sys.argv[1]

    # Load file for config
    with open(metadataFile, "r") as mf:
        yvmMetadata = json.load(mf)

    # Load file to assemble
    with open(fileToAssemble, "r") as bf:
        yvmBytecode = json.load(bf)

    # 1. Checks
    metadata_checks(yvmMetadata, yvmBytecode)

    # 2. Assemble
    assemble(yvmMetadata, yvmBytecode)
