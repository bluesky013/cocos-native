//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include "base/Ptr.h"
#include "gfx-base/GFXInputAssembler.h"
#include "gfx-base/GFXBuffer.h"
#include "render/component/Registry.h"

namespace cc::exp {

struct UIRenderSystem {
    void ViewSceneData(Registry &registry, float time);
};

}
