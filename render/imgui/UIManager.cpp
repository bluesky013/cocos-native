//
// Created by yjrj on 2022/12/20.
//

#include "UIManager.h"
#include "imgui.h"
#include "gfx-base/GFXDevice.h"
#include "engine/EngineEvents.h"

namespace cc::exp {

void UIManager::initUI() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    initFontTexture();
    mapKey();
}

void UIManager::update(float time) {
    ImGuiIO &io  = ImGui::GetIO();
    io.DeltaTime = time;

    ImGui::NewFrame();
    auto *context = ImGui::GetCurrentContext();

    {
        // todo
    }

    ImGui::Render();
}

void UIManager::initFontTexture() {
    ImGuiIO       &io = ImGui::GetIO();
    unsigned char *pixels;
    int            width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    gfx::TextureInfo textureInfo = {};
    textureInfo.usage = gfx::TextureUsageBit::TRANSFER_DST | gfx::TextureUsageBit::SAMPLED;
    textureInfo.width = static_cast<uint32_t>(width);
    textureInfo.height = static_cast<uint32_t>(height);
    textureInfo.format = gfx::Format::RGBA8;

    fontTexture = gfx::Device::getInstance()->createTexture(textureInfo);
    gfx::BufferDataList buffers{pixels};
    gfx::BufferTextureCopyList regions = {{0U,
                                           0U,
                                           0U,
                                           {0, 0, 0},
                                           {textureInfo.width, textureInfo.height, 1U},
                                           {0U, 0U, 1U}}};

    auto *device = gfx::Device::getInstance();
    device->copyBuffersToTexture(buffers, fontTexture, regions);

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(fontTexture.get()));
}

void UIManager::mapKey() {
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

void UIManager::initRenderer() {
    renderer = ccnew UIRenderer();
    renderer->init();
}
}
