//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <SFMT.h>
#include <mutex>


namespace cc::exp {

class SFMTRandom {
public:
    SFMTRandom();
    SFMTRandom(uint32_t s);
    ~SFMTRandom() = default;

    uint32_t genU32();

    uint64_t genU64();

private:
    uint32_t           seed = 0;
    sfmt_t             sfmt;
    mutable std::mutex mutex;
};

}
