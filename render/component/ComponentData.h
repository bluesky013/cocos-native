//
// Created by yjrj on 2022/12/22.
//

#pragma once

#include <core/geometry/AABB.h>
#include <scene/Camera.h>
#include <scene/Model.h>

namespace cc::exp {
// SceneView
struct SceneView {
    uint32_t windowId;
};

// Model
struct ModelUpdateTag {};

struct BondingBox {
    geometry::AABB localBounds;
    geometry::AABB worldBounds;
};

struct ModelMeshData {
    IntrusivePtr<Mesh> source;
};

struct ModelData {
    bool castShadow;
    bool receiveShadow;
};

struct ModelBufferData {
    IntrusivePtr<gfx::Buffer> localBuffer;
    IntrusivePtr<gfx::Buffer> localSHBuffer;
    IntrusivePtr<gfx::Buffer> worldBoundBuffer;
};

// Camera
struct CameraUpdateTag {};

struct CameraPhyData {
    scene::CameraAperture aperture{scene::CameraAperture::F16_0};
    float apertureValue{0.F};
    scene::CameraShutter shutter{scene::CameraShutter::D125};
    float shutterValue{0.F};
    scene::CameraISO iso{scene::CameraISO::ISO100};
    float isoValue{0.F};
    float ec = {0.F};
};

struct CameraData {
    scene::CameraProjection proj{scene::CameraProjection::UNKNOWN};
    scene::CameraFOVAxis fovAxis{scene::CameraFOVAxis::VERTICAL};
    float aspect{0.F};
    float orthoHeight{10.0F};
    float fov{static_cast<float>(mathutils::toRadian(45.F))};
    float nearClip{1.0F};
    float farClip{1000.0F};
    uint32_t width{1};
    uint32_t height{1};

    Vec3 forward;
    Vec3 position;
    geometry::Frustum frustum;
};

struct CameraProjectData {
    Mat4 matView;
    Mat4 matProj;
    Mat4 matProjInv;
    Mat4 matViewProj;
    Mat4 matViewProjInv;
};
}
