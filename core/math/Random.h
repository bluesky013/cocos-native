//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <cstdint>

namespace cc::exp {

class Random {
public:
    Random()  = default;
    ~Random() = default;

    static bool gen(void *data, uint32_t dataSize);
};


}
