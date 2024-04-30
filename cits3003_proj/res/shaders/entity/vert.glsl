#version 410 core
#include "../common/lights.glsl"

// Per vertex data
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

out vec3 ws_position;
out vec3 ws_normal;
out vec2 vertex_out_texture_coordinate;

// Per instance data
uniform mat4 model_matrix;
uniform mat3 normal_matrix;


uniform vec3 diffuse_tint;
uniform vec3 specular_tint;
uniform vec3 ambient_tint;
uniform float shininess;
uniform float texture_scale;

// Global data
uniform vec3 ws_view_position;
uniform mat4 projection_view_matrix;




void main() {
    // Transform vertices
    ws_position = (model_matrix * vec4(vertex_position, 1.0f)).xyz;
    ws_normal = normalize(normal_matrix * normal);
    vertex_out_texture_coordinate = texture_coordinate * texture_scale;

    gl_Position = projection_view_matrix * vec4(ws_position, 1.0f);

    vec3 ws_view_dir = normalize(ws_view_position - ws_position);
    // LightCalculatioData light_calculation_data = LightCalculatioData(ws_position, ws_view_dir, ws_normal);
    // Material material = Material(diffuse_tint, specular_tint, ambient_tint, shininess);

    // LightingResult vertex_out_lighting_result = total_light_calculation(light_calculation_data, material
    //     #if NUM_PL > 0
    //     ,point_lights
    //     #else
    //     ,direction_lights
    //     #endif
    // );


}
