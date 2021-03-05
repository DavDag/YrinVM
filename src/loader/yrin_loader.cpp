#include "../runtime/yrin_vm.hpp"
#include "../common/yrin_error.hpp"
#include "../common/yrin_code_metadata.hpp"
#include <fstream>

void Yrin::VM::read(const char *filePath) {
    // Sanity checks
    static_assert(sizeof(BYTE) == 1, "Instruction size must be 8 bit.\n");

    // Open file
    std::ifstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.good()) {
        ERROR_LOG("Unable to open file %s\n", filePath);
        throw Yrin::Error::YvmException(Yrin::Error::Code::UNABLE_TO_OPEN_FILE);
    }

    // 1. Read metadata
    Yrin::Meta::Metadata fileMetadata{};
    file.read(reinterpret_cast<char *>(&fileMetadata), sizeof(Yrin::Meta::Metadata));

    // 1.1 Version check
    DEBUG_LOG("Current YVM version is %c.%c.%c-%c.\n",
              Yrin::Meta::CURRENT_VERSION.major, Yrin::Meta::CURRENT_VERSION.minor,
              Yrin::Meta::CURRENT_VERSION.type, Yrin::Meta::CURRENT_VERSION.release);
    if (fileMetadata.version != Yrin::Meta::CURRENT_VERSION) {
        ERROR_LOG("Version %d.%d.%c-%d is not supported\n",
                  fileMetadata.version.major, fileMetadata.version.minor,
                  fileMetadata.version.type, fileMetadata.version.release);
        throw Yrin::Error::YvmException(Error::Code::VERSION_NOT_SUPPORTED);
    }

    // 1.2 Read constants
    LOADER_DEBUG_LOG("Constant pool size: %d bytes\n", fileMetadata.constantPoolSize);

    // 2. Read instructions list
    BYTE byte;
    file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
    while (!file.eof()) {
        LOADER_DEBUG_LOG("%d ", byte);
        code.push_back(byte);
        file.read(reinterpret_cast<char *>(&byte), sizeof(BYTE));
    }

    // 3. Insert starting instruction
    ips.push(0);
}
