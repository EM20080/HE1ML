#pragma once
#include <vector>

namespace LibGens {
    class File;
    std::vector<unsigned char> endianSwapHKX(File* file);
}