//
// Created by yjrj on 2022/12/20.
//

#pragma once

#ifdef _MSC_VER
    #define PRETTY_FUNC __FUNCSIG__
#else
    #define PRETTY_FUNC __PRETTY_FUNCTION__
#endif

#include <core/util/Hash.h>
#include <string_view>
#include <type_traits>

namespace cc::exp {

template <typename T>
constexpr std::string_view prettyFunc()
{
    return PRETTY_FUNC;
}

template <typename T>
struct TypeInfo {

    static constexpr std::string_view name()
    {
        return prettyFunc<T>();
    }

    static constexpr uint32_t hash()
    {
        return fnv1a32(name());
    }
};

}
