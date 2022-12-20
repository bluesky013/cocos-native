//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <string_view>
#include <mutex>

#include <core/util/TypeInfo.h>

namespace cc::exp {

template <typename T>
class Singleton {
public:
    static constexpr std::string_view NAME = TypeInfo<T>::name();
    static constexpr uint32_t ID = TypeInfo<T>::hash();

    static void init() {
        getPtr(true);
    }

    static T *get() {
        return getPtr(false);
    }

    static void destroy() {
        auto &instance = getPtr(false);
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }

protected:
    static T *&getPtr(bool create) {
        static T *instance = nullptr;
        static std::mutex mutex;
        if (instance == nullptr && create) {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr) {
                instance = new T();
            }
        }
        return instance;
    }

    Singleton() = default;
    virtual ~Singleton() = default;
};

}
