//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <entt/entt.hpp>

namespace cc::exp {

struct Registry {
public:
    Registry() = default;
    ~Registry() = default;

    using Entity = entt::registry::entity_type;

    entt::registry registry;
};

}
