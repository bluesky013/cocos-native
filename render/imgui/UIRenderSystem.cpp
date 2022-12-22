//
// Created by yjrj on 2022/12/20.
//

#include "UIRenderSystem.h"
#include "UIDataType.h"
#include "gfx-base/GFXDevice.h"
#include "imgui.h"
#include "render/component/ComponentData.h"

namespace cc::exp {

struct UILocalData {
    ImVec2 scale;
    ImVec2 translate;
};

static void initRenderData(UIRenderData &data) {
    data.iaInfo.attributes = {
        {"inPos", gfx::Format::RG32F, false, 0, false, 0},
        {"inUv", gfx::Format::RG32F, false, 0, false, 1},
        {"inColor", gfx::Format::RGBA8, false, 0, false, 2},
    };

    gfx::BufferInfo bufferInfo = {};
    bufferInfo.usage = gfx::BufferUsageBit::UNIFORM | gfx::BufferUsageBit::TRANSFER_DST;
    bufferInfo.memUsage = gfx::MemoryUsageBit::HOST | gfx::MemoryUsageBit::DEVICE;
    bufferInfo.size = sizeof(UILocalData);
    bufferInfo.flags = gfx::BufferFlagBit::ENABLE_STAGING_WRITE;
    data.uniformBuffer = gfx::Device::getInstance()->createBuffer(bufferInfo);

    data.inited = true;
}

static void checkBufferSize(UIRenderData &data, ImDrawData *drawData) {
    auto *device = gfx::Device::getInstance();

    bool updateIA = false;
    uint64_t vertexSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
    uint64_t indexSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);
    if (vertexSize > data.currentVertexSize) {
        gfx::BufferInfo bufferInfo = {};
        bufferInfo.usage = gfx::BufferUsageBit::VERTEX | gfx::BufferUsageBit::TRANSFER_DST;
        bufferInfo.memUsage = gfx::MemoryUsageBit::DEVICE | gfx::MemoryUsageBit::HOST;
        bufferInfo.size = vertexSize;
        data.vertexBuffer = device->createBuffer(bufferInfo);
        updateIA = true;
    }

    if (indexSize > data.currentIndexSize) {
        gfx::BufferInfo bufferInfo = {};
        bufferInfo.usage = gfx::BufferUsageBit::INDEX | gfx::BufferUsageBit::TRANSFER_DST;
        bufferInfo.memUsage = gfx::MemoryUsageBit::DEVICE | gfx::MemoryUsageBit::HOST;
        bufferInfo.size = indexSize;
        data.indexBuffer = device->createBuffer(bufferInfo);
        updateIA = true;
    }

    if (updateIA) {
        data.iaInfo.vertexBuffers.clear();
        data.iaInfo.vertexBuffers.emplace_back(data.vertexBuffer);
        data.iaInfo.indexBuffer = data.indexBuffer;
        data.ia = device->createInputAssembler(data.iaInfo);
    }
}

static void updateBuffer(UIRenderData &renderData, ImDrawData *drawData) {
    // primitive buffer
    uint32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    for (int i = 0; i < drawData->CmdListsCount; ++i) {
        const ImDrawList *cmdList = drawData->CmdLists[i];

        renderData.vertexBuffer->write(reinterpret_cast<const uint8_t *>(cmdList->VtxBuffer.Data),
                                       vertexOffset,
                                       cmdList->VtxBuffer.Size * sizeof(ImDrawVert));

        renderData.indexBuffer->write(reinterpret_cast<const uint8_t *>(cmdList->IdxBuffer.Data),
                                      indexOffset,
                                      cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));

        vertexOffset += cmdList->VtxBuffer.Size;
        indexOffset += cmdList->IdxBuffer.Size;
    }

    // uniform buffer
    UILocalData currentConstant;
    currentConstant.scale.x = 2.0f / drawData->DisplaySize.x;
    currentConstant.scale.y = 2.0f / drawData->DisplaySize.y;
    currentConstant.translate.x = -1.0f - drawData->DisplayPos.x * currentConstant.scale[0];
    currentConstant.translate.y = -1.0f - drawData->DisplayPos.y * currentConstant.scale[1];
    renderData.uniformBuffer->write(currentConstant, 0);


    renderData.vertexBuffer->update();
    renderData.indexBuffer->update();
    renderData.uniformBuffer->update();
}

static void prepareUIDrawInfo(UIRenderData &renderData) {
    ImDrawData *drawData = ImGui::GetDrawData();
    if (drawData == nullptr || drawData->TotalVtxCount == 0) {
        return;
    }

    checkBufferSize(renderData, drawData);

    updateBuffer(renderData, drawData);

    ImVec2 clipOff = drawData->DisplayPos;
    ImVec2 clipScale = drawData->FramebufferScale;
    uint32_t vtxOffset = 0;
    uint32_t idxOffset = 0;
    for (int i = 0; i < drawData->CmdListsCount; i++) {
        const ImDrawList *cmdList = drawData->CmdLists[i];
        for (int j = 0; j < cmdList->CmdBuffer.Size; j++) {
            const ImDrawCmd *pcmd = &cmdList->CmdBuffer[j];

            ImVec2 clipMin((pcmd->ClipRect.x - clipOff.x) * clipScale.x, (pcmd->ClipRect.y - clipOff.y) * clipScale.y);
            ImVec2 clipMax((pcmd->ClipRect.z - clipOff.x) * clipScale.x, (pcmd->ClipRect.w - clipOff.y) * clipScale.y);

            if (clipMin.x < 0.0f) {
                clipMin.x = 0.0f;
            }
            if (clipMin.y < 0.0f) {
                clipMin.y = 0.0f;
            }
            if (clipMax.x > static_cast<float>(renderData.width)) {
                clipMax.x = static_cast<float>(renderData.width);
            }
            if (clipMax.y > static_cast<float>(renderData.height)) {
                clipMax.y = static_cast<float>(renderData.height);
            }
            if (clipMax.x < clipMin.x || clipMax.y < clipMin.y) {
                continue;
            }

//            GuiPrimitive::DrawCall drawCall{};
//            drawCall.scissor.offset.x = (int32_t)(clipMin.x);
//            drawCall.scissor.offset.y = (int32_t)(clipMin.y);
//            drawCall.scissor.extent.width = (uint32_t)(clipMax.x - clipMin.x);
//            drawCall.scissor.extent.height = (uint32_t)(clipMax.y - clipMin.y);
//
//            drawCall.indexed.indexCount = pcmd->ElemCount;
//            drawCall.indexed.firstIndex = pcmd->IdxOffset + idxOffset;
//            drawCall.indexed.vertexOffset = static_cast<int32_t>(pcmd->VtxOffset + vtxOffset);
//            primitive->dc.emplace_back(drawCall);
        }
        idxOffset += cmdList->IdxBuffer.Size;
        vtxOffset += cmdList->VtxBuffer.Size;
    }
}

void UIRenderSystem::ViewSceneData(Registry &registry, float time) {
    registry.reg.view<UIContext>().each([time](UIContext &context) {
        if (!context.data) {
            return;
        }
        ImGui::SetCurrentContext(context.data->_context);

        ImGuiIO &io = ImGui::GetIO();
        io.DeltaTime = time;

        ImGui::NewFrame();

        for (auto &widget : context.data->widgets) {
            widget.fn();
        }

        ImGui::Render();

        if (!context.renderData.inited) {
            initRenderData(context.renderData);
        }
        prepareUIDrawInfo(context.renderData);
    });
}
}
