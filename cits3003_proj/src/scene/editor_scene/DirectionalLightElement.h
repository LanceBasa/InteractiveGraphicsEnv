#ifndef point_light_dir_H
#define point_light_dir_H

#include "SceneElement.h"
#include "scene/SceneContext.h"

namespace EditorScene  {
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
        //std::shared_ptr<PointLight> light;
        std::shared_ptr<PointLightDirection> light_dir;

        std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_dir;

        PointLightDir(const ElementRef& parent, std::string name, glm::vec3 position, std::shared_ptr<PointLightDirection> light_dir, std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_dir) :
            SceneElement(parent, std::move(name)), position(position), light_dir(std::move(light_dir)), light_sphere_dir(std::move(light_sphere_dir)) {}

        static std::unique_ptr<PointLightDir> new_default(const SceneContext& scene_context, ElementRef parent);
        static std::unique_ptr<PointLightDir> from_json(const SceneContext& scene_context, ElementRef parent, const json& j);

        [[nodiscard]] json into_json() const override;

        void add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) override;

        void update_instance_data() override;

        void add_to_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.insert_entity(light_sphere_dir);
            target_render_scene.insert_light_dir(light_dir);
        }

        void remove_from_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.remove_entity(light_sphere_dir);
            target_render_scene.remove_light_dir(light_dir);
        }

        [[nodiscard]] const char* element_type_name() const override;
    };
}

#endif