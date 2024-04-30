#ifndef point_light_dir_H
#define point_light_dir_H

#include "SceneElement.h"
#include "scene/SceneContext.h"

namespace EditorScene {
    class PointLightDir : public SceneElement {
    public:
        /// NOTE: Must be unique per element type, as it is used to select generators,
        ///       so if you are creating a new element type make sure to change this to a new unique name.
        static constexpr const char* ELEMENT_TYPE_NAME = "Direction Light";

        // Local transformation
        glm::vec3 position;
        bool visible = true;
        float visual_scale = 1.0f;
        // PointLight and Entity will store World position
        std::shared_ptr<PointLight> light;
        std::shared_ptr<PointLightDirection> light_dir;

        std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_dir;

        PointLightDir(const ElementRef& parent, std::string name, glm::vec3 position, std::shared_ptr<PointLightDirection> light_dir, std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_dir) :
            SceneElement(parent, std::move(name)), position(position), light(std::move(light)), light_sphere_dir(std::move(light_sphere_dir)) {}

        static std::unique_ptr<PointLightDir> new_default(const SceneContext& scene_context, ElementRef parent);
        static std::unique_ptr<PointLightDir> from_json(const SceneContext& scene_context, ElementRef parent, const json& j);

        [[nodiscard]] json into_json() const override;

        void add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) override;

        void update_instance_data() override;

        void add_to_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.insert_entity(light_sphere_dir);
            target_render_scene.insert_light(light_dir);
        }

        void remove_from_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.remove_entity(light_sphere_dir);
            target_render_scene.remove_light(light_dir);
        }

        [[nodiscard]] const char* element_type_name() const override;
    };
}

std::unique_ptr<EditorScene::PointLightDir> EditorScene::PointLightDir::new_default(const SceneContext& scene_context, EditorScene::ElementRef parent) {
    auto light_element = std::make_unique<PointLightDir>(
        parent,
        "New Point Light",
        glm::vec3{0.0f, 1.0f, 0.0f},
        PointLight::create(
            glm::vec3{}, // Set via update_instance_data()
            glm::vec4{1.0f}
        ),
        EmissiveEntityRenderer::Entity::create(
            scene_context.model_loader.load_from_file<EmissiveEntityRenderer::VertexData>("cone.obj"),
            EmissiveEntityRenderer::InstanceData{
                glm::mat4{}, // Set via update_instance_data()
                EmissiveEntityRenderer::EmissiveEntityMaterial{
                    glm::vec4{1.0f}
                }
            },
            EmissiveEntityRenderer::RenderData{
                scene_context.texture_loader.default_white_texture()
            }
        )
    );

    light_element->update_instance_data();
    return light_element;
}

std::unique_ptr<EditorScene::PointLightDir> EditorScene::PointLightDir::from_json(const SceneContext& scene_context, EditorScene::ElementRef parent, const json& j) {
    auto light_element = new_default(scene_context, parent);

    light_element->position = j["position"];
    light_element->light->colour = j["colour"];
    light_element->visible = j["visible"];
    light_element->visual_scale = j["visual_scale"];

    light_element->update_instance_data();
    return light_element;
}

json EditorScene::PointLightDir::into_json() const {
    return {
        {"position",     position},
        {"colour",       light->colour},
        {"visible",      visible},
        {"visual_scale", visual_scale},
    };
}

void EditorScene::PointLightDir::add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) {
    ImGui::Text("Directional Light");
    SceneElement::add_imgui_edit_section(render_scene, scene_context);

    ImGui::Text("Local Transformation");
    bool transformUpdated = false;
    transformUpdated |= ImGui::DragFloat3("Translation", &position[0], 0.01f);
    ImGui::DragDisableCursor(scene_context.window);
    ImGui::Spacing();

    ImGui::Text("Light Properties");
    transformUpdated |= ImGui::ColorEdit3("Colour", &light->colour[0]);
    ImGui::Spacing();
    ImGui::DragFloat("Intensity", &light->colour.a, 0.01f, 0.0f, FLT_MAX);
    ImGui::DragDisableCursor(scene_context.window);

    ImGui::Spacing();
    ImGui::Text("Visuals");

    transformUpdated |= ImGui::Checkbox("Show Visuals", &visible);
    transformUpdated |= ImGui::DragFloat("Visual Scale", &visual_scale, 0.01f, 0.0f, FLT_MAX);
    ImGui::DragDisableCursor(scene_context.window);

    if (transformUpdated) {
        update_instance_data();
    }
}

void EditorScene::PointLightDir::update_instance_data() {
    transform = glm::translate(position);

    if (!EditorScene::is_null(parent)) {
        // Post multiply by transform so that local transformations are applied first
        transform = (*parent)->transform * transform;
    }

    light->position = glm::vec3(transform[3]); // Extract translation from matrix
    if (visible) {
        light_sphere_dir->instance_data.model_matrix = transform * glm::scale(glm::vec3{0.1f * visual_scale});
    } else {
        // Throw off to infinity as a hacky way to make model invisible
        light_sphere_dir->instance_data.model_matrix = glm::scale(glm::vec3{std::numeric_limits<float>::infinity()}) * glm::translate(glm::vec3{std::numeric_limits<float>::infinity()});
    }

    glm::vec3 normalised_colour = glm::vec3(light->colour) / glm::compMax(glm::vec3(light->colour));
    light_sphere_dir->instance_data.material.emission_tint = glm::vec4(normalised_colour, light_sphere_dir->instance_data.material.emission_tint.a);
}

const char* EditorScene::PointLightDir::element_type_name() const {
    return ELEMENT_TYPE_NAME;
}


#endif //point_light_dir_H
