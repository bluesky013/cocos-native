//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <render/imgui/UIContextData.h>
#include <memory>

namespace cc::exp {

struct UIRenderData {
    bool inited = false;

    uint32_t width = 1;
    uint32_t height = 1;

    uint64_t currentVertexSize = 0;
    uint64_t currentIndexSize = 0;

    IntrusivePtr<gfx::Buffer> vertexBuffer;
    IntrusivePtr<gfx::Buffer> indexBuffer;
    IntrusivePtr<gfx::Buffer> uniformBuffer;
    IntrusivePtr<gfx::Buffer> localSet;
    IntrusivePtr<gfx::InputAssembler> ia;
    gfx::InputAssemblerInfo iaInfo = {};
};

struct UIContext {
    std::unique_ptr<UIContextData> data;
    UIRenderData renderData;
};

}
