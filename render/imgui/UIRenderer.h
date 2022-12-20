//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include "base/Ptr.h"
#include "base/RefCounted.h"
#include "gfx-base/GFXInputAssembler.h"
#include "gfx-base/GFXBuffer.h"

namespace cc::exp {

class UIRenderer : public RefCounted {
public:
    UIRenderer() = default;
    ~UIRenderer() = default;

    void init();
    void render();

private:
    void checkBufferSize(uint64_t vertexSize, uint64_t indexSize);

    IntrusivePtr<gfx::Buffer> vertexBuffer;
    IntrusivePtr<gfx::Buffer> indexBuffer;
    IntrusivePtr<gfx::InputAssembler> ia;
    gfx::InputAssemblerInfo iaInfo = {};

    uint64_t currentVertexSize = 0;
    uint64_t currentIndexSize = 0;
    uint32_t width = 1;
    uint32_t height = 1;
};

}
