//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <core/util/Uuid.h>

namespace cc::exp {

template <typename T>
struct AssetTrait {
    using DataType = std::vector<uint8_t>;

    static bool loadFromFile(const std::string& path, DataType &out) {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        auto size = static_cast<uint32_t>(file.tellg());
        out.resize(size);
        file.seekg(0);
        file.read((char *)out.data(), size);
        file.close();
    }

    static bool writeToFile(const std::string& path, const char* data, uint32_t size) {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        file.write(data, size);
    }
};

template <typename T>
class Asset {
public:
    Asset() = default;
    ~Asset() = default;

    using DataType = typename AssetTrait<T>::DataType;

private:
    Uuid uuid;
    std::vector<Uuid> dependencies;

    DataType rawData;
};

}
