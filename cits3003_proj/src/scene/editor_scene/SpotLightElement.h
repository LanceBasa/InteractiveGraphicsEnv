#ifndef SPOT_LIGHT_ELEMENT_H
#define SPOT_LIGHT_ELEMENT_H

#include "SceneElement.h"
#include "scene/SceneContext.h"

namespace EditorScene  {
    class SpotLightElement : public SceneElement {
    public:
        /// NOTE: Must be unique per element type, as it is used to select generators,
        ///       so if you are creating a new element type make sure to change this to a new unique name.
        static constexpr const char* ELEMENT_TYPE_NAME = "Spot Light";

        // Local transformation
        glm::vec3 position;
        
        bool visible = true;
        float visual_scale = 1.0f;
        float pitch = -45.0f;
        float yaw = 0.0f;
        // PointLight and Entity will store World position
        //std::shared_ptr<PointLight> light;
        std::shared_ptr<SpotLight> slight;
        glm::vec3 direction;

        std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_spot;

        SpotLightElement(const ElementRef& parent, std::string name, glm::vec3 position, std::shared_ptr<SpotLight> slight, std::shared_ptr<EmissiveEntityRenderer::Entity> light_sphere_spot) :
        SceneElement(parent, std::move(name)), position(position) ,slight(std::move(slight)), light_sphere_spot(std::move(light_sphere_spot)) {}

        static std::unique_ptr<SpotLightElement> new_default(const SceneContext& scene_context, ElementRef parent);
        static std::unique_ptr<SpotLightElement> from_json(const SceneContext& scene_context, ElementRef parent, const json& j);

        [[nodiscard]] json into_json() const override;

        void add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) override;

        void update_instance_data() override;

        void add_to_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.insert_entity(light_sphere_spot);
            target_render_scene.insert_spot_light(slight);
        }

        void remove_from_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.remove_entity(light_sphere_spot);
            target_render_scene.remove_light_spot(slight);
        }

        [[nodiscard]] const char* element_type_name() const override;
    };
}

#endif