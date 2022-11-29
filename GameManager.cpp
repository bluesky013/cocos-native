#include "GameManager.h"

#include "pipeline/NativePipelineSample.h"
#include "GameApp.h"

namespace cc {

void GameManager::init() {
    _currentGame = std::make_unique<NativePipelineSample>();
    _app->setCurrentGame(_currentGame.get());
}

}
