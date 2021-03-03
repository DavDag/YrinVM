#include "loader.hpp"
#include "operations.hpp"
#include <fstream>

namespace YVM::Loader {

    const Bytecode::Data &load(const char *filePath) {
        // Sanity checks
        static_assert(sizeof(Bytecode::Instruction) == 8, "Instruction size must be 8.\n");
        static_assert(sizeof(Bytecode::Metadata) % 8 == 0, "Metadata must be factor of 8.\n");

        // Open file
        std::ifstream file(filePath, std::ios::out | std::ios::binary);
        if (!file.good()) {
            ERROR_LOG("Unable to open file %s\n", filePath);
            throw Exception::YvmLoaderException(Exception::Errors::UNABLE_TO_OPEN_FILE);
        }

        // 1. Read metadata
        Bytecode::Metadata fileMetadata{};
        file.read(reinterpret_cast<char *>(&fileMetadata), sizeof(Bytecode::Metadata));

        // 1.1 Version check
        DEBUG_LOG("Current YVM version is %d.%d.%c-%d.\n",
                  Bytecode::CURRENT_VERSION.major, Bytecode::CURRENT_VERSION.minor,
                  Bytecode::CURRENT_VERSION.type, Bytecode::CURRENT_VERSION.release);
        if (fileMetadata.version != Bytecode::CURRENT_VERSION) {
            ERROR_LOG("Version %d.%d.%c-%d is not supported\n",
                      fileMetadata.version.major, fileMetadata.version.minor,
                      fileMetadata.version.type, fileMetadata.version.release);
            throw Exception::YvmLoaderException(Exception::Errors::VERSION_NOT_SUPPORTED);
        }

        auto *data = new Bytecode::Data{
            .version = fileMetadata.version,
            .instructions = std::vector<Bytecode::Instruction>()
        };

        // 2. Read instructions list
        Bytecode::Instruction instruction{};
        file.read(reinterpret_cast<char *>(&instruction), sizeof(Bytecode::Instruction));
        while (!file.eof()) {
            LOADER_DEBUG_LOG("%05u: 0x%02X 0x%06X %+9d\n", data->instructions.size(), instruction.opcode,
                             instruction.flags,
                             instruction.data_as_int);
            // TODO: check_instruction(instruction);
            data->instructions.push_back(instruction);
            file.read(reinterpret_cast<char *>(&instruction), sizeof(Bytecode::Instruction));
        }

        // CODE representation
        return *data;
    }

} // YVM::Loader
