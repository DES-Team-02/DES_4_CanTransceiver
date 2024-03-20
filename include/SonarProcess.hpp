#pragma once

#include <vector>

class RpmProcess {
public:
    static RpmData process(const std::vector<uint8_t>& data);
};

