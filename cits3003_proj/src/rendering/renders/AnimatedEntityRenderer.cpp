#include "AnimatedEntityRenderer.h"

AnimatedEntityRenderer::AnimatedEntityShader::AnimatedEntityShader() :
    BaseLitEntityShader("Animated Entity", "animated_entity/vert.glsl", "animated_entity/frag.glsl", {{"BONE_TRANSFORMS", BONE_TRANSFORMS_STR}}) {

    get_uniforms_set_bindings();
}

void AnimatedEntityRenderer::AnimatedEntityShader::get_uniforms_set_bindings() {
    BaseLitEntityShader::get_uniforms_set_bindings(); // Call the base implementation to load all the common uniforms
    bone_transforms_location = get_uniform_location("bone_transforms");
}

void AnimatedEntityRenderer::AnimatedEntityShader::set_model_matrix(const glm::mat4& model_matrix) {
    glProgramUniformMatrix4fv(id(), model_matrix_location, 1, GL_FALSE, &model_matrix[0][0]);
}

void AnimatedEntityRenderer::AnimatedEntityShader::set_bone_transforms(const std::vector<glm::mat4>& bone_transforms) {
    glProgramUniformMatrix4fv(id(), bone_transforms_location, std::min(BONE_TRANSFORMS, (int) bone_transforms.size()), GL_FALSE, &bone_transforms[0][0][0]);
}

AnimatedEntityRenderer::AnimatedEntityRenderer::AnimatedEntityRenderer() : shader() {}

void AnimatedEntityRenderer::AnimatedEntityRenderer::render(const RenderScene& render_scene, const LightScene& light_scene) {
    shader.use();
    shader.set_global_data(render_scene.global_data);

    for (const auto& entity : render_scene.entities) {
        shader.set_instance_data(entity->instance_data);

        glm::vec3 position = entity->instance_data.model_matrix[3];

        // Set Point Lights
        shader.set_point_lights(light_scene.get_nearest_point_lights(position, BaseLitEntityShader::MAX_PL, 1));

        // Set Directional Lights if present
        #if NUM_DL > 0
        shader.set_directional_lights(light_scene.get_nearest_direction_lights(position, BaseLitEntityShader::MAX_DL, 1));
        #endif

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entity->render_data.diffuse_texture->get_texture_id());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, entity->render_data.specular_map_texture->get_texture_id());

        entity->mesh_hierarchy->calculate_animation(entity->animation_id, entity->animation_time_seconds);
        entity->mesh_hierarchy->visit_nodes([this, &entity](const MeshHierarchyNode& node, glm::mat4 accumulated_transformation) {
            for (const auto& mesh_id : node.meshes) {
                const auto& mesh = entity->mesh_hierarchy->meshes[mesh_id];

                shader.set_model_matrix(entity->instance_data.model_matrix * accumulated_transformation);
                if (!mesh.bone_transforms.empty()) shader.set_bone_transforms(mesh.bone_transforms);

                glBindVertexArray(mesh.model->get_vao());
                glDrawElementsBaseVertex(GL_TRIANGLES, mesh.model->get_index_count(), GL_UNSIGNED_INT, nullptr, mesh.model->get_vertex_offset());
            }
        });
    }
}





void AnimatedEntityRenderer::VertexData::setup_attrib_pointers() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, texture_coordinate));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, bone_weights));
    glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(VertexData), (void*) offsetof(VertexData, bone_indices)); // Note the `I` in the function name, needed to have ints work as expected
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}