#pragma once

namespace cc {

struct IGame {
  IGame() = default;
  virtual ~IGame() = default;

  virtual void onPause() = 0;
  virtual void onResume() = 0;
  virtual void onClose() = 0;
  virtual void onTick(float time) = 0;
};

}
