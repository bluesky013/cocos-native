#include "GameApp.h"
#include "interfaces/modules/ISystemWindowManager.h"

namespace cc {
int GameApp::init() {
#if CC_PLATFORM == CC_PLATFORM_WINDOWS || CC_PLATFORM == CC_PLATFORM_LINUX || CC_PLATFORM == CC_PLATFORM_QNX || CC_PLATFORM == CC_PLATFORM_MACOS
  std::call_once(_windowCreateFlag, [&]() {
    ISystemWindowInfo info;
    info.title = "CocosNative";
    info.x = 50;
    info.y = 50;
    info.width = 800;
    info.height = 600;
    info.flags = cc::ISystemWindow::CC_WINDOW_SHOWN |
                 cc::ISystemWindow::CC_WINDOW_RESIZABLE |
                 cc::ISystemWindow::CC_WINDOW_INPUT_FOCUS;

    ISystemWindowManager* windowMgr = CC_GET_PLATFORM_INTERFACE(ISystemWindowManager);
    windowMgr->createWindow(info);
  });
#endif

  _tickEvent.bind([this](float time) {
    tick(time);
  });
  _gameManager.init();
  return CocosApplication::init();
}

void GameApp::setCurrentGame(IGame *game) {
    _game = game;
}

void GameApp::onStart() {
  if (_game != nullptr) {
      _game->onStart();
  }
  CocosApplication::onStart();
}

void GameApp::onPause() {
  if (_game != nullptr) {
      _game->onPause();
  }
  CocosApplication::onPause();
}

void GameApp::onResume() {
  if (_game != nullptr) {
      _game->onResume();
  }
  CocosApplication::onResume();
}

void GameApp::onClose() {
  if (_game != nullptr) {
      _game->onClose();
  }
  CocosApplication::onClose();
}

void GameApp::tick(float time) {
    if (_game != nullptr) {
        _game->onTick(time);
    }
}
}
CC_REGISTER_APPLICATION(cc::GameApp);
