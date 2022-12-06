#include "pipeline/NativePipelineSample.h"

// framework
#include "application/ApplicationManager.h"
#include "interfaces/modules/ISystemWindow.h"
#include "interfaces/modules/ISystemWindowManager.h"

// base
#include "base/StringUtil.h"

// render
#include "scene/RenderWindow.h"

// gfx
#include "gfx-base/GFXDevice.h"

namespace cc {

void NativePipelineSample::initScene() {
    scene::IRenderSceneInfo sceneInfo = {"MainScene"};
    _scene = ccnew scene::RenderScene();
    _scene->initialize(sceneInfo);

    _mainCamera = addCamera("MainCamera");
    _mainCamera->changeTargetWindow(_mainRenderWindow);
    setActive(_mainCamera);
}

void NativePipelineSample::initWindowEvent() {
    _windowDestroyListener.bind([this](uint32_t windowId) -> void {
        _mainRenderWindow->onNativeWindowDestroy(windowId);
    });

    _windowRecreatedListener.bind([this](uint32_t windowId) -> void {
        _mainRenderWindow->onNativeWindowResume(windowId);
    });
}

void NativePipelineSample::initPipeline() {
    _ppl.reset(render::Factory::createPipeline());
    auto *layoutGraph = _ppl->getLayoutGraphBuilder();
    _ppl->beginSetup();
    uint32_t cleanScreenId = layoutGraph->addRenderStage("clean");
    uint32_t forwardQueueId = layoutGraph->addRenderPhase("Queue", cleanScreenId);

    layoutGraph->compile();
    layoutGraph->print();

    _ppl->addRenderTexture("output1", gfx::Format::BGRA8, 800, 600, _mainRenderWindow);
    _ppl->addRenderTarget("output2", gfx::Format::RGBA8, 800, 600);
    auto *passBuilder = _ppl->addRasterPass(800, 600, "clean");
    passBuilder->addRasterView("output1", {" ",
                                           render::AccessType::WRITE,
                                           render::AttachmentType::RENDER_TARGET,
                                           gfx::LoadOp::CLEAR,
                                           gfx::StoreOp::STORE,
                                           gfx::ClearFlagBit::ALL,
                                           gfx::Color{1, 0, 0, 1}});

    passBuilder->addRasterView("output2", {" ",
                                           render::AccessType::WRITE,
                                           render::AttachmentType::RENDER_TARGET,
                                           gfx::LoadOp::CLEAR,
                                           gfx::StoreOp::STORE,
                                           gfx::ClearFlagBit::ALL,
                                           gfx::Color{0, 1, 0, 1}});

    _ppl->presentAll();
    _ppl->endSetup();

    if (!_ppl->activate(_swapChains[0])) {
        _ppl->destroy();
        _ppl.reset();
    }
}

void NativePipelineSample::initRenderWindow(uint32_t windowId, ISystemWindow *window) {
    auto handle = window->getWindowHandle();
    const auto &size = window->getViewSize();

    gfx::SwapchainInfo info;
    info.width = static_cast<uint32_t>(size.width);
    info.height = static_cast<uint32_t>(size.height);
    info.windowHandle = reinterpret_cast<void *>(handle);
    info.windowId = window->getWindowId();

    gfx::Swapchain *swapChain = gfx::Device::getInstance()->createSwapchain(info);
    _swapChains.emplace_back(swapChain);

    scene::IRenderWindowInfo windowInfo = {};
    windowInfo.title = StringUtil::format("renderWindow_%d", windowId);
    windowInfo.width = swapChain->getWidth();
    windowInfo.height = swapChain->getHeight();
    windowInfo.swapchain = swapChain;

    auto &passInfo = windowInfo.renderPassInfo;
    gfx::ColorAttachment colorAttachment;
    colorAttachment.format = swapChain->getColorTexture()->getFormat();
    passInfo.colorAttachments.emplace_back(colorAttachment);

    auto &depthStencilAttachment = passInfo.depthStencilAttachment;
    depthStencilAttachment.format = swapChain->getDepthStencilTexture()->getFormat();
    depthStencilAttachment.depthStoreOp = gfx::StoreOp::DISCARD;
    depthStencilAttachment.stencilStoreOp = gfx::StoreOp::DISCARD;
    _mainRenderWindow = ccnew scene::RenderWindow();
    _mainRenderWindow->initialize(_device, windowInfo);
}

void NativePipelineSample::init() {
    _device = gfx::Device::getInstance();
    auto *windowMgr = CC_GET_PLATFORM_INTERFACE(ISystemWindowManager);
    const auto &windows = windowMgr->getWindows();
    for (auto &[winId, sysWindow] : windows) {
        initRenderWindow(winId, sysWindow.get());
        break;
    }

    initWindowEvent();
    initScene();
    initPipeline();
}

void NativePipelineSample::onPause() {

}

void NativePipelineSample::onResume() {

}

void NativePipelineSample::onClose() {
    _windowDestroyListener.reset();
    _windowRecreatedListener.reset();

    _scene = nullptr;
    _mainRenderWindow = nullptr;

    _cameras.clear();
    _ppl = nullptr;
    for (auto *swapchain : _swapChains) {
        CC_SAFE_DELETE(swapchain);
    }
    _swapChains.clear();
}

void NativePipelineSample::onTick(float time) {
    if (_ppl != nullptr && !_activeCameras.empty()) {
        std::stable_sort(_activeCameras.begin(), _activeCameras.end(), [](const auto *a, const auto *b) {
            return a->getPriority() < b->getPriority();
        });

        _device->acquire(_swapChains);

        _ppl->beginFrame();
        _ppl->render(_activeCameras);
        _ppl->endFrame();

        _device->present();
    }
}


scene::Camera *NativePipelineSample::addCamera(const ccstd::string &key) {
    auto *camera = ccnew scene::Camera(_device);
    _cameras.emplace(key, camera);
    return camera;
}

void NativePipelineSample::setActive(scene::Camera *camera) {
    _activeCameras.emplace_back(camera);
}

void NativePipelineSample::setDeActive(scene::Camera *camera) {
    _activeCameras.erase(std::remove(_activeCameras.begin(), _activeCameras.end(), camera), _activeCameras.end());
}
}
