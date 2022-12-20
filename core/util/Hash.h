//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <cstdint>
#include <string_view>

namespace cc::exp {

constexpr uint32_t FNVPrime32      = 0x01000193;
constexpr uint32_t FNVOffsetBias32 = 0x811c9dc5;

constexpr uint64_t FNVPrime64      = 0x00000100000001B3;
constexpr uint64_t FNVOffsetBias64 = 0xcbf29ce484222325;

inline constexpr uint32_t fnv1a32(std::string_view str)
{
    uint32_t res  = FNVOffsetBias32;
    uint32_t size = static_cast<uint32_t>(str.size());
    for (uint32_t i = 0; i < size; ++i) {
        res ^= str[i];
        res *= FNVPrime32;
    }
    return res;
}

inline constexpr uint64_t fnv1a64(std::string_view str)
{
    uint64_t res  = FNVOffsetBias64;
    uint64_t size = str.size();
    for (uint32_t i = 0; i < size; ++i) {
        res ^= str[i];
        res *= FNVPrime64;
    }
    return res;
}

}
