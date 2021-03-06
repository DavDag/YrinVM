#include "yvm.hpp"

#include <fstream>

namespace Yrin {

    void VM::read(const char *filePath) {
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

        // 2. Read instructions list
        BYTE byte;
        file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
        while (!file.eof()) {
            // LOADER_DEBUG_LOG("%d ", byte);
            code.push_back(byte);
            file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
        }
        // LOADER_DEBUG_LOG("\n");

        // 3. Insert starting instruction
        ips.push(0);
    }

} // Yrin

