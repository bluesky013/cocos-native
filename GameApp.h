#pragma once

#include "cocos/application/CocosApplication.h"
#include "cocos/application/ApplicationManager.h"
#include "common/IGame.h"
#include "GameManager.h"

namespace cc {
class GameApp : public CocosApplication {
public:
  GameApp() : _gameManager(this) {}
  ~GameApp() = default;

  GameApp(const GameApp&) = delete;
  GameApp &operator=(const GameApp&) = delete;

  void setCurrentGame(IGame *);

private:
  int init() override;
  void onStart() override;
  void onPause() override;
  void onResume() override;
  void onClose() override;

  void tick(float time);

  events::Tick::Listener _tickEvent;
  std::once_flag _windowCreateFlag;
  GameManager _gameManager;
  IGame *_game = nullptr;
};

} // namespace cc
