#pragma once

#ifndef YRINVM_YRIN_CODE_METADATA_HPP
#define YRINVM_YRIN_CODE_METADATA_HPP

#include "yrin_defs.hpp"

namespace Yrin::Meta {

    // Version X.Y.Z-W
    struct Version {
        int major: 8;   // X
        int minor: 8;   // Y
        int type: 8;    // Z
        int release: 8; // W

        // Operator overloading for equals
        bool operator!=(const Version &rhs) const {
            return major != rhs.major || minor != rhs.minor; /* || type != rhs.type || release != rhs.release;*/
        }
    };

    // Bytecode metadata
    struct Metadata {
        Version version;
        int constantPoolSize: 32;
    };

    // Current YVM version
    constexpr Version CURRENT_VERSION = {
        .major = CURRENT_YVM_VERSION_MAJOR,
        .minor = CURRENT_YVM_VERSION_MINOR,
        .type = CURRENT_YVM_VERSION_RTYPE,
        .release = CURRENT_YVM_VERSION_COUNT
    };

} // Yrin::Meta

#endif //YRINVM_YRIN_CODE_METADATA_HPP
