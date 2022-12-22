//
// Created by yjrj on 2022/12/22.
//

#include <render/scene/RenderScene.h>
#include "render/imgui/UIRenderSystem.h"

namespace cc::exp {

void RenderScene::tick(float time) {
    UIRenderSystem().ViewSceneData(registry, time);
}

}
