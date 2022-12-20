//
// Created by yjrj on 2022/12/20.
//

#include "UIRenderer.h"
#include "imgui.h"
#include "gfx-base/GFXDevice.h"

namespace cc::exp {

//struct Attribute {
//    ccstd::string name;
//    Format format{Format::UNKNOWN};
//    bool isNormalized{false};
//    uint32_t stream{0};
//    bool isInstanced{false};
//    uint32_t location{0};
//
//    EXPOSE_COPY_FN(Attribute)
//};

void UIRenderer::init() {
    iaInfo.attributes = {
        {"inPos", gfx::Format::RG32F, false, 0, false, 0},
        {"inUv", gfx::Format::RG32F, false, 0, false, 1},
        {"inColor", gfx::Format::RGBA8, false, 0, false, 2},
    };
}

void UIRenderer::render() {
    ImDrawData *drawData = ImGui::GetDrawData();
    if (drawData == nullptr || drawData->TotalVtxCount == 0) {
        return;
    }
    uint64_t vertexSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
    uint64_t indexSize  = drawData->TotalIdxCount * sizeof(ImDrawIdx);
    checkBufferSize(vertexSize, indexSize);

//    auto *vertexDst = reinterpret_cast<ImDrawVert *>(vertexBuffer->GetMappedAddress());
//    auto *indexDst  = reinterpret_cast<ImDrawIdx *>(indexBuffer->GetMappedAddress());
//
//    for (int i = 0; i < drawData->CmdListsCount; ++i) {
//        const ImDrawList *cmdList = drawData->CmdLists[i];
//        memcpy(vertexDst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
//        memcpy(indexDst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
//        vertexDst += cmdList->VtxBuffer.Size;
//        indexDst += cmdList->IdxBuffer.Size;
//    }

    ImVec2 clipOff   = drawData->DisplayPos;
    ImVec2 clipScale = drawData->FramebufferScale;

//    GuiConstants currentConstant;
//    currentConstant.scale.x     = 2.0f / drawData->DisplaySize.x;
//    currentConstant.scale.y     = 2.0f / drawData->DisplaySize.y;
//    currentConstant.translate.x = -1.0f - drawData->DisplayPos.x * currentConstant.scale[0];
//    currentConstant.translate.y = -1.0f - drawData->DisplayPos.y * currentConstant.scale[1];
//    primitive->constants->WriteData(currentConstant, 0);
//    primitive->dc.clear();

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
            if (clipMax.x > static_cast<float>(width)) {
                clipMax.x = static_cast<float>(width);
            }
            if (clipMax.y > static_cast<float>(height)) {
                clipMax.y = static_cast<float>(height);
            }
            if (clipMax.x < clipMin.x || clipMax.y < clipMin.y) {
                continue;
            }

//            GuiPrimitive::DrawCall drawCall{};
//            drawCall.scissor.offset.x      = (int32_t)(clipMin.x);
//            drawCall.scissor.offset.y      = (int32_t)(clipMin.y);
//            drawCall.scissor.extent.width  = (uint32_t)(clipMax.x - clipMin.x);
//            drawCall.scissor.extent.height = (uint32_t)(clipMax.y - clipMin.y);
//
//            drawCall.indexed.indexCount   = pcmd->ElemCount;
//            drawCall.indexed.firstIndex   = pcmd->IdxOffset + idxOffset;
//            drawCall.indexed.vertexOffset = static_cast<int32_t>(pcmd->VtxOffset + vtxOffset);
//            primitive->dc.emplace_back(drawCall);
        }
        idxOffset += cmdList->IdxBuffer.Size;
        vtxOffset += cmdList->VtxBuffer.Size;
    }

//    const auto &views = scene.GetViews();
//    for (const auto &view : views) {
//        view->AddRenderPrimitive(primitive.get());
//    }
}

void UIRenderer::checkBufferSize(uint64_t vertexSize, uint64_t indexSize) {
    auto *device = gfx::Device::getInstance();

    bool updateIA = false;
    if (vertexSize > currentVertexSize) {
        gfx::BufferInfo bufferInfo = {};
        bufferInfo.usage = gfx::BufferUsageBit::VERTEX | gfx::BufferUsageBit::TRANSFER_DST;
        bufferInfo.memUsage = gfx::MemoryUsageBit::DEVICE | gfx::MemoryUsageBit::HOST;
        bufferInfo.size = vertexSize;
        vertexBuffer = device->createBuffer(bufferInfo);
        updateIA = true;
    }

    if (indexSize > currentIndexSize) {
        gfx::BufferInfo bufferInfo = {};
        bufferInfo.usage = gfx::BufferUsageBit::INDEX | gfx::BufferUsageBit::TRANSFER_DST;
        bufferInfo.memUsage = gfx::MemoryUsageBit::DEVICE | gfx::MemoryUsageBit::HOST;
        bufferInfo.size = indexSize;
        indexBuffer = device->createBuffer(bufferInfo);
        updateIA = true;
    }

    if (updateIA) {
        iaInfo.vertexBuffers.clear();
        iaInfo.vertexBuffers.emplace_back(vertexBuffer);
        iaInfo.indexBuffer = indexBuffer;
        ia = device->createInputAssembler(iaInfo);
    }
}
}
