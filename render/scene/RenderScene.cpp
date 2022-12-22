//
// Created by yjrj on 2022/12/22.
//

#include <render/scene/RenderScene.h>

namespace cc::exp {

//template <typename T>
//void emplace(ccstd::vector<RenderScene::Entity<T>> &entities, T *data) {
//    entities.emplace_back(RenderScene::Entity<T>{data, createEntity()});
//}
//
//template <typename T>
//void erase(ccstd::vector<RenderScene::Entity<T>> &entities, T *data) {
//    entities.erase(std::remove_if(entities.begin(), entities.end(), [data](auto &entity) {
//        return entity.data == data;
//    }), entities.end());
//}

void RenderScene::addCamera(scene::Camera *camera) {
//    emplace(_cameras, camera);
}

void RenderScene::removeCamera(scene::Camera *camera) {
//    erase(_cameras, camera);
}

void RenderScene::addModel(scene::Model *model) {
//    emplace(_models, model);
}

void RenderScene::removeModel(scene::Model *model) {
//    erase(_models, model);
}


}
