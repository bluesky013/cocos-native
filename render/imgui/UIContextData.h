//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <imgui.h>
#include <base/Ptr.h>
#include <gfx-base/GFXTexture.h>
#include <engine/EngineEvents.h>

namespace cc::exp {

struct UIWidget {
    std::function<void()> fn;
};

struct UIContextData {
    void init();

    uint32_t _relatedWindowID = 0;
    ImGuiContext *_context = nullptr;
    IntrusivePtr<gfx::Texture> _fontTexture = nullptr;

    ccstd::vector<UIWidget> widgets;

    events::Keyboard::Listener _windowKeyboardListener;
    events::Mouse::Listener _windowMouseListener;
    events::WindowRecreated::Listener _windowRecreatedListener;

private:
    void initFontTexture();
    void mapKey();
    void initWindowEvent();
};

}
