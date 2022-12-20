//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <core/singleton/Singleton.h>
#include <gfx-base/GFXTexture.h>
#include <render/imgui/UIRenderer.h>
#include <base/Ptr.h>

namespace cc::exp {

class UIManager : public Singleton<UIManager> {
public:
    UIManager() = default;
    ~UIManager() = default;

    void initUI();
    void update(float time);

private:
    void initFontTexture();
    void mapKey();
    void initRenderer();

    IntrusivePtr<gfx::Texture> fontTexture = nullptr;
    IntrusivePtr<UIRenderer> renderer = nullptr;
};

}
