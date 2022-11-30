#pragma once

#include "common/IGame.h"

#include "base/std/container/unordered_map.h"
#include "base/std/container/vector.h"
#include "base/Ptr.h"
#include "renderer/pipeline/custom/RenderInterfaceTypes.h"
#include "scene/RenderWindow.h"
#include "scene/RenderScene.h"
#include "scene/Camera.h"

namespace cc {

class NativePipelineSample : public IGame {
public:
    NativePipelineSample() = default;
    ~NativePipelineSample() = default;

    void init();

private:
    void onPause() override;
    void onResume() override;
    void onClose() override;
    void onTick(float time) override;

    scene::Camera *addCamera(const ccstd::string &key);
    void setActive(scene::Camera *camera);
    void setDeActive(scene::Camera *camera);

    void initScene();
    void initRenderWindow(uint32_t windowId, ISystemWindow *window);
    void initWindowEvent();
    void initPipeline();

    using CameraPtr = IntrusivePtr<scene::Camera>;

    gfx::Device *_device = nullptr;
    scene::Camera *_mainCamera = nullptr;
    ccstd::vector<scene::Camera *> _activeCameras;
    ccstd::unordered_map<ccstd::string, CameraPtr> _cameras;
    std::unique_ptr<render::Pipeline> _ppl;
    ccstd::vector<gfx::Swapchain *> _swapChains;
    IntrusivePtr<scene::RenderScene> _scene;
    IntrusivePtr<scene::RenderWindow> _mainRenderWindow;
    events::WindowDestroy::Listener _windowDestroyListener;
    events::WindowRecreated::Listener _windowRecreatedListener;
};


}
