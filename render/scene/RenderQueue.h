//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <gfx-base/GFXDevice.h>

namespace cc::exp {

// DrawData
struct DrawInstanceData {
    IntrusivePtr<gfx::PipelineState> pso;
    IntrusivePtr<gfx::DescriptorSet> localSet;
    IntrusivePtr<gfx::DescriptorSet> matSet;
    IntrusivePtr<gfx::InputAssembler> ia;
    std::vector<gfx::DrawInfo> drawInfos;

    float depth{0};
};

}
