#include "loader.hpp"
#include <fstream>

namespace YVM::Loader {

    const CODE::Data &load(int argc, char **argv) {
        // Sanity checks
        static_assert(sizeof(CODE::Instruction) == 8, "Instruction size must be 8.\n");
        static_assert(sizeof(CODE::Metadata) % 8 == 0, "Metadata must be factor of 8.\n");

        // Check argument count
        if (argc == 0) {
            ERROR_LOG("Pass the file path as first argument\n");
            throw Exception::YvmLoaderException(YVM::Exception::ERRORS::INVALID_CMD_LINE_ARGUMENT);
        }

        // Retrieve file_path and open file
        const char *filePath = argv[1];
        std::ifstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.good()) {
            ERROR_LOG("Unable to open file %s\n", filePath);
            throw Exception::YvmLoaderException(Exception::ERRORS::UNABLE_TO_OPEN_FILE);
        }

        // 1. Read metadata
        CODE::Metadata fileMetadata{};
        file.read(reinterpret_cast<char *>(&fileMetadata), sizeof(CODE::Metadata));

        // 1.1 Version check
        printf("===============================\nCurrent YVM version is %d.%d.%c-%d.\n===============================\n",
               CODE::CURRENT_VERSION.major, CODE::CURRENT_VERSION.minor,
               CODE::CURRENT_VERSION.type, CODE::CURRENT_VERSION.release);
        if (fileMetadata.version != CODE::CURRENT_VERSION) {
            ERROR_LOG("Version %d.%d.%c-%d is not supported\n",
                      fileMetadata.version.major, fileMetadata.version.minor,
                      fileMetadata.version.type, fileMetadata.version.release);
            throw Exception::YvmLoaderException(Exception::ERRORS::INVALID_CMD_LINE_ARGUMENT);
        }

        auto *data = new CODE::Data{
                .version = fileMetadata.version,
                .instructions = std::vector<CODE::Instruction>()
        };

        // 2. Read instructions list
        CODE::Instruction instruction{};
        file.read(reinterpret_cast<char *>(&instruction), sizeof(CODE::Instruction));
        while (!file.eof()) {
            LOADER_DEBUG_LOG("%05u: 0x%02X 0x%06X %+9d\n", data->instructions.size(), instruction.opcode,
                             instruction.flags,
                             instruction.data_as_int);
            check_instruction(instruction);
            data->instructions.push_back(instruction);
            file.read(reinterpret_cast<char *>(&instruction), sizeof(CODE::Instruction));
        }

        // CODE representation
        return *data;
    }

    void check_instruction(const CODE::Instruction &i) {
        // TODO
    }

} // YVM::Loader
