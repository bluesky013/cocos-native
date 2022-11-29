#pragma once

#include "common/IGame.h"

namespace cc {

class NativePipelineSample : public IGame {
public:
    NativePipelineSample() = default;
    ~NativePipelineSample() = default;

private:
    void onStart() override;
    void onPause() override;
    void onResume() override;
    void onClose() override;
    void onTick(float time) override;
};


}
