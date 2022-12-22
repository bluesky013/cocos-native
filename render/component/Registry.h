//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <core/singleton/Singleton.h>
#include <entt/entt.hpp>

namespace cc::exp {

class Registry : public Singleton<Registry> {
public:
    Registry() = default;
    ~Registry() = default;

    using Entity = entt::registry::entity_type;

    entt::registry reg;
};

inline Registry::Entity createEntity() {
    return Registry::get()->reg.create();
}

inline Registry::Entity destroyEntity(Registry::Entity entity) {
    Registry::get()->reg.destroy(entity);
}

}
