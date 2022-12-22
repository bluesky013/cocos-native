//
// Created by yjrj on 2022/12/20.
//

#include <BasePlatform.h>
#include <interfaces/modules/ISystemWindow.h>
#include <interfaces/modules/ISystemWindowManager.h>
#include "UIContextData.h"
#include "engine/EngineEvents.h"
#include "gfx-base/GFXDevice.h"
#include "render/imgui/UIDataType.h"

namespace cc::exp {

void UIContextData::init() {
    IMGUI_CHECKVERSION();
    _context = ImGui::CreateContext();

    initFontTexture();
    mapKey();
    initWindowEvent();
}

void UIContextData::initFontTexture() {
    ImGuiIO       &io = ImGui::GetIO();
    unsigned char *pixels;
    int            width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    gfx::TextureInfo textureInfo = {};
    textureInfo.usage = gfx::TextureUsageBit::TRANSFER_DST | gfx::TextureUsageBit::SAMPLED;
    textureInfo.width = static_cast<uint32_t>(width);
    textureInfo.height = static_cast<uint32_t>(height);
    textureInfo.format = gfx::Format::RGBA8;

    _fontTexture = gfx::Device::getInstance()->createTexture(textureInfo);
    gfx::BufferDataList buffers{pixels};
    gfx::BufferTextureCopyList regions = {{0U,
                                           0U,
                                           0U,
                                           {0, 0, 0},
                                           {textureInfo.width, textureInfo.height, 1U},
                                           {0U, 0U, 1U}}};

    auto *device = gfx::Device::getInstance();
    device->copyBuffersToTexture(buffers, _fontTexture, regions);

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(_fontTexture.get()));
}

void UIContextData::mapKey() {
    ImGuiIO       &io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab]         = static_cast<int>(cc::KeyCode::TAB);
    io.KeyMap[ImGuiKey_LeftArrow]   = static_cast<int>(cc::KeyCode::ARROW_LEFT);
    io.KeyMap[ImGuiKey_RightArrow]  = static_cast<int>(cc::KeyCode::ARROW_RIGHT);
    io.KeyMap[ImGuiKey_UpArrow]     = static_cast<int>(cc::KeyCode::ARROW_UP);
    io.KeyMap[ImGuiKey_DownArrow]   = static_cast<int>(cc::KeyCode::ARROW_DOWN);
    io.KeyMap[ImGuiKey_PageUp]      = static_cast<int>(cc::KeyCode::PAGE_UP);
    io.KeyMap[ImGuiKey_PageDown]    = static_cast<int>(cc::KeyCode::PAGE_DOWN);
    io.KeyMap[ImGuiKey_Home]        = static_cast<int>(cc::KeyCode::HOME);
    io.KeyMap[ImGuiKey_End]         = static_cast<int>(cc::KeyCode::END);
    io.KeyMap[ImGuiKey_Insert]      = static_cast<int>(cc::KeyCode::INSERT);
    io.KeyMap[ImGuiKey_Delete]      = static_cast<int>(cc::KeyCode::DELETE_KEY);
    io.KeyMap[ImGuiKey_Backspace]   = static_cast<int>(cc::KeyCode::BACKSPACE);
    io.KeyMap[ImGuiKey_Space]       = static_cast<int>(cc::KeyCode::SPACE);
    io.KeyMap[ImGuiKey_Enter]       = static_cast<int>(cc::KeyCode::ENTER);
    io.KeyMap[ImGuiKey_Escape]      = static_cast<int>(cc::KeyCode::ESCAPE);
    io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(cc::KeyCode::NUMPAD_ENTER);
    io.KeyMap[ImGuiKey_A]           = 'A';
    io.KeyMap[ImGuiKey_C]           = 'C';
    io.KeyMap[ImGuiKey_V]           = 'V';
    io.KeyMap[ImGuiKey_X]           = 'X';
    io.KeyMap[ImGuiKey_Y]           = 'Y';
    io.KeyMap[ImGuiKey_Z]           = 'Z';
}

static uint32_t mapMouseButton(uint16_t button) {
    // IMGUI: 0=left, 1=right, 2=middle
    // COCOS: 0=left, 1=middle, 2=right
    static constexpr uint32_t map[3] = {
        0, 2, 1
    };
    return map[button];
}

void UIContextData::initWindowEvent() {
    // resize
    _windowRecreatedListener.bind([this](uint32_t windowId) -> void {
        if (_relatedWindowID != windowId) {
            return;
        }

        auto *windowMgr = BasePlatform::getPlatform()->getInterface<ISystemWindowManager>();
        auto *sysWindow = windowMgr->getWindow(_relatedWindowID);
        auto extent = sysWindow->getViewSize();
        ImGui::SetCurrentContext(_context);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(extent.width, extent.height);
    });

    _windowKeyboardListener.bind([this](const cc::KeyboardEvent &event) -> void {
        if (_relatedWindowID != event.windowId) {
            return;
        }

        ImGui::SetCurrentContext(_context);
        ImGuiIO &io = ImGui::GetIO();
        if (event.action == KeyboardEvent::Action::RELEASE) {
            io.KeysDown[event.key] = false;
        } else if (event.action == KeyboardEvent::Action::PRESS) {
            io.KeysDown[event.key] = true;
        }
    });

    _windowMouseListener.bind([this](const cc::MouseEvent &event) -> void {
        if (_relatedWindowID != event.windowId) {
            return;
        }

        ImGui::SetCurrentContext(_context);
        ImGuiIO &io = ImGui::GetIO();
        if (event.type == cc::MouseEvent::Type::UP) {
            io.MouseDown[mapMouseButton(event.button)] = false;
        } else if(event.type == cc::MouseEvent::Type::DOWN) {
            io.MouseDown[mapMouseButton(event.button)] = true;
        } else if(event.type == cc::MouseEvent::Type::MOVE) {
            io.MousePos.x = event.x;
            io.MousePos.y = event.y;
        } else if (event.type == cc::MouseEvent::Type::WHEEL) {
            if (event.x > 0) {
                io.MouseWheelH += 1;
            }
            if (event.x < 0) {
                io.MouseWheelH -= 1;
            }
            if (event.y > 0) {
                io.MouseWheel += 1;
            }
            if (event.y < 0) {
                io.MouseWheel -= 1;
            }
        }
    });
}
}
