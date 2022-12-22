//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <base/std/container/vector.h>
#include <core/registry/Registry.h>
#include <render/imgui/UIRenderSystem.h>
#include <scene/Camera.h>

namespace cc::exp {

class RenderScene {
public:
    RenderScene() = default;
    ~RenderScene() = default;

    void tick(float time);

private:
    // component data
    ccstd::vector<Registry::Entity> entities;
    Registry registry;
};


}
