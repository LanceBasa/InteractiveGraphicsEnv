#include "MasterRenderScene.h"

void MasterRenderScene::use_camera(const CameraInterface& camera_interface) {
    entity_scene.global_data.use_camera(camera_interface);
    animated_entity_scene.global_data.use_camera(camera_interface);
    emissive_entity_scene.global_data.use_camera(camera_interface);
}

void MasterRenderScene::insert_entity(std::shared_ptr<EntityRenderer::Entity> entity) {
    entity_scene.entities.insert(std::move(entity));
}

void MasterRenderScene::insert_entity(std::shared_ptr<AnimatedEntityRenderer::Entity> entity) {
    animated_entity_scene.entities.insert(std::move(entity));
}

void MasterRenderScene::insert_entity(std::shared_ptr<EmissiveEntityRenderer::Entity> entity) {
    emissive_entity_scene.entities.insert(std::move(entity));
}

bool MasterRenderScene::remove_entity(const std::shared_ptr<EntityRenderer::Entity>& entity) {
    return entity_scene.entities.erase(entity) != 0;
}

bool MasterRenderScene::remove_entity(const std::shared_ptr<AnimatedEntityRenderer::Entity>& entity) {
    return animated_entity_scene.entities.erase(entity) != 0;
}

bool MasterRenderScene::remove_entity(const std::shared_ptr<EmissiveEntityRenderer::Entity>& entity) {
    return emissive_entity_scene.entities.erase(entity) != 0;
}

void MasterRenderScene::insert_light(std::shared_ptr<PointLight> point_light) {
    light_scene.point_lights.insert(std::move(point_light));
}

bool MasterRenderScene::remove_light(const std::shared_ptr<PointLight>& point_light) {
    return light_scene.point_lights.erase(point_light) != 0;
}

void MasterRenderScene::insert_light_dir(std::shared_ptr<PointLightDirection> point_light_dir) {
    light_scene.point_lights_dir.insert(std::move(point_light_dir));
}

bool MasterRenderScene::remove_light_dir(const std::shared_ptr<PointLightDirection>& point_light_dir) {
    return light_scene.point_lights_dir.erase(point_light_dir) != 0;
}

void MasterRenderScene::insert_spot_light(std::shared_ptr<SpotLight> spot_light) {
    light_scene.spot_lights.insert(std::move(spot_light));
}

bool MasterRenderScene::remove_light_spot(const std::shared_ptr<SpotLight>& spot_light) {
    return light_scene.spot_lights.erase(spot_light) != 0;
}
