#pragma once

#include "common/IGame.h"

#include "base/std/container/unordered_map.h"
#include "base/std/container/vector.h"
#include "base/Ptr.h"

#include "core/scene-graph/Node.h"
#include "core/scene-graph/Scene.h"

#include "renderer/pipeline/custom/RenderInterfaceTypes.h"

#include "scene/RenderWindow.h"
#include "scene/RenderScene.h"
#include "scene/Camera.h"
#include "scene/Model.h"

// core
#include "core/Root.h"

// exp
#include <render/scene/RenderScene.h>

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
    scene::Model *addModel(const ccstd::string &key);

    void setActive(scene::Camera *camera);
    void setDeActive(scene::Camera *camera);

    void initScene();
    void initRenderWindow(uint32_t windowId, ISystemWindow *window);
    void initWindowEvent();
    void initPipeline();

    using CameraPtr = IntrusivePtr<scene::Camera>;
    using ModelPtr = IntrusivePtr<scene::Model>;

    Root *_root = nullptr;

    gfx::Device *_device = nullptr;
    scene::Camera *_mainCamera = nullptr;

    std::unique_ptr<render::Pipeline> _ppl;
    ccstd::vector<gfx::Swapchain *> _swapChains;
    events::WindowDestroy::Listener _windowDestroyListener;
    events::WindowRecreated::Listener _windowRecreatedListener;

    // scene
    IntrusivePtr<scene::RenderScene> _scene;
    IntrusivePtr<scene::RenderWindow> _mainRenderWindow;

    ccstd::vector<scene::Camera *> _activeCameras;
    ccstd::unordered_map<ccstd::string, CameraPtr> _cameras;
    ccstd::unordered_map<ccstd::string, ModelPtr> _models;
    ModelPtr uiModel = nullptr;

    IntrusivePtr<Scene> _world;
    ccstd::unordered_map<ccstd::string, IntrusivePtr<Node>> _nodes;

    // ecs
    exp::RenderScene _renderScene;
};


}
