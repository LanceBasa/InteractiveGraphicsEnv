#version 410 core
#include "../common/lights.glsl"

// Per vertex data
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;


// Per instance data
uniform mat4 model_matrix;
uniform mat3 normal_matrix;


// Light Data
#if NUM_PL > 0
layout (std140) uniform PointLightArray {
    PointLightData point_lights[NUM_PL];
};
#endif

// Global data
uniform vec3 ws_view_position;
uniform mat4 projection_view_matrix;

out vec3 ws_position;
out vec3 ws_normal;
out vec2 vertex_outtexture_coordinate;

void main() {
    // Transform vertices
    vec3 ws_position = (model_matrix * vec4(vertex_position, 1.0f)).xyz;
    vec3 ws_normal = normalize(normal_matrix * normal);
    vertex_out.texture_coordinate = texture_coordinate * texture_scale;

    gl_Position = projection_view_matrix * vec4(ws_position, 1.0f);


}