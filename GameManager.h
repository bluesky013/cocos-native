#pragma once

#include "common/IGame.h"
#include <memory>

namespace cc {
class GameApp;

class GameManager {
public:
    explicit GameManager(GameApp *app) : _app(app) {}
    ~GameManager() = default;

    GameManager(const GameManager&) = delete;
    GameManager &operator=(const GameManager&) = delete;

    void init();

private:
    GameApp *_app;
    std::unique_ptr<IGame> _currentGame;
};

}
