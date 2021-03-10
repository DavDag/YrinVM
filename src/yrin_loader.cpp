#include "yvm.hpp"

#include <fstream>

namespace Yrin {

    int verify_instruction(const BYTE &opcode, const std::vector<BYTE> &bytestream, int index) {
        switch (opcode) {
            case OP_RESERVED_CODE:
            case OP_RETURN_CODE:
            case OP_PUSH_B0_CODE:
            case OP_PUSH_B1_CODE:
            case OP_POP_CODE:
            case OP_STORE_CODE:
            case OP_ADD_I_CODE:
            case OP_ADD_L_CODE:
            case OP_ADD_F_CODE:
            case OP_ADD_D_CODE:
            case OP_SUB_I_CODE:
            case OP_SUB_L_CODE:
            case OP_SUB_F_CODE:
            case OP_SUB_D_CODE:
            case OP_MUL_I_CODE:
            case OP_MUL_L_CODE:
            case OP_MUL_F_CODE:
            case OP_MUL_D_CODE:
            case OP_DIV_I_CODE:
            case OP_DIV_F_CODE:
            case OP_DIV_L_CODE:
            case OP_DIV_D_CODE:
            case OP_MOD_I_CODE:
                // Expect 0 bytes
                return 0;

            case OP_LOAD_CODE:
                // Expect 2 bytes
                return 2 * sizeof(BYTE);

            case OP_PUSH_I_CODE:
                // Expect sizeof(int) bytes
                return sizeof(int);

            case OP_PUSH_L_CODE:
                // Expect sizeof(long long) bytes
                return sizeof(long long);

            case OP_PUSH_F_CODE:
                // Expect sizeof(float) bytes
                return sizeof(float);

            case OP_PUSH_D_CODE:
                // Expect sizeof(double) bytes
                return sizeof(double);

            case OP_PUSH_C_CODE:
                // Expect sizeof(char) bytes
                return sizeof(char);

            case OP_PUSH_S_CODE:
                // TODO !
                return 0;

            default:
                // Unhandled operation code
                break;
        }
        return 0;
    }

    void VM::read(const char *filePath) {
        LOADER_DEBUG_LOG("=================================\n"
                         "||           LOADER            ||\n"
                         "=================================\n");

        // Sanity checks
        static_assert(sizeof(BYTE) == 1, "Instruction size must be 8 bit.\n");

        // Open file
        std::ifstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.good()) {
            ERROR_LOG("Unable to open file %s\n", filePath);
            throw Error::YvmException(Error::Code::UNABLE_TO_OPEN_FILE);
        }

        // 1. Read metadata
        Meta::Metadata fileMetadata{};
        file.read(reinterpret_cast<char *>(&fileMetadata), sizeof(Meta::Metadata));

        // 1.1 Version check
        DEBUG_LOG("Current YVM version is %c.%c.%c-%c.\n",
                  Meta::CURRENT_VERSION.major, Meta::CURRENT_VERSION.minor,
                  Meta::CURRENT_VERSION.type, Meta::CURRENT_VERSION.release);
        if (fileMetadata.version != Meta::CURRENT_VERSION) {
            ERROR_LOG("Version %d.%d.%c-%d is not supported\n",
                      fileMetadata.version.major, fileMetadata.version.minor,
                      fileMetadata.version.type, fileMetadata.version.release);
            throw Error::YvmException(Error::Code::VERSION_NOT_SUPPORTED);
        }

        // 1.2 Read constants
        LOADER_DEBUG_LOG("Constant pool size: %d bytes\n", fileMetadata.constantPoolSize);

        // 2. Read bytestream
        LOADER_DEBUG_LOG("Read instructions\n");
        BYTE byte;
        file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
        while (!file.eof()) {
            LOADER_DEBUG_LOG("%d.", byte);
            bytestream.push_back(byte);
            file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
        }
        LOADER_DEBUG_LOG("\n");

        // 3. Code packing
        LOADER_DEBUG_LOG("Code packing\n");
        int i = 0;
        while (i < bytestream.size()) {
            LOADER_DEBUG_LOG("%llu: %d, ", code.size(), i);
            code.push_back(i);
            const BYTE &b = bytestream[i++];
            i += verify_instruction(b, bytestream, i);
        }
        LOADER_DEBUG_LOG("\n");
        LOADER_DEBUG_LOG("\n");

        // 4. Insert starting instruction index
        ips.push(0);
    }

} // Yrin

