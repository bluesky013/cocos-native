//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include "base/Macros.h"
#include "base/Ptr.h"
#include "base/RefCounted.h"
#include <base/std/container/vector.h>
#include <scene/Camera.h>
#include <scene/Model.h>
#include <render/component/Registry.h>
#include <variant>

namespace cc::exp {

struct SceneViewData {
    IntrusivePtr<scene::RenderScene> scene;
    IntrusivePtr<scene::Camera> camera;

    IntrusivePtr<gfx::DescriptorSet> globalSet;
};

class RenderScene {
public:
    RenderScene() = default;
    ~RenderScene() = default;

    void addCamera(scene::Camera *camera);
    void removeCamera(scene::Camera *camera);

    void addModel(scene::Model *model);
    void removeModel(scene::Model *model);

private:
};

}
