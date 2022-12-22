//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <core/registry/Registry.h>

namespace cc::exp {

class RenderScene {
public:
    RenderScene() = default;
    ~RenderScene() = default;

private:
    Registry registry;
};


}
