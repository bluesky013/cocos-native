#include "GameManager.h"

#include "pipeline/NativePipelineSample.h"
#include "GameApp.h"

namespace cc {

void GameManager::init() {
    auto *sample = ccnew NativePipelineSample();
    sample->init();

    _currentGame.reset(sample);
    _app->setCurrentGame(_currentGame.get());
}

}
