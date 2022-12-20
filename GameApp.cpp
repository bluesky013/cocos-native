#include "GameApp.h"
#include "interfaces/modules/ISystemWindowManager.h"
#include <core/jobsystem/JobSystem.h>
#include <core/asset/DataBase.h>
#include "FileUtils.h"
#include "ProjectRoot.h"

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
  CocosApplication::init();
  _gameManager.init();

  exp::JobSystem::init();
  FileUtils::getInstance()->setDefaultResourceRootPath(exp::PROJECT_ROOT);
  exp::DataBase::init();
  exp::DataBase::get()->initDataBase(exp::PROJECT_ROOT + "/pc/db.sqlite");

  return 0;
}

void GameApp::setCurrentGame(IGame *game) {
    _game = game;
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
