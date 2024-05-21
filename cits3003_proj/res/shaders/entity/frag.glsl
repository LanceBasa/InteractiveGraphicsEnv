#version 410 core
#include "../common/lights.glsl"


layout(location = 0) out vec4 out_colour;


// Light Data
#if NUM_PL > 0
layout (std140) uniform PointLightArray {
    PointLightData point_lights[NUM_PL];
};
#endif

// Direction Data
#if NUM_PL_DIR > 0
layout (std140) uniform PointLightDirectionArray {
    DirectionalLightData direction_lights[NUM_PL_DIR];
};
#endif

// Direction Data
#if NUM_SL > 0
layout (std140) uniform SpotLightArray {
    SpotLightData spot_lights[NUM_SL];
};
#endif


// Global Data
uniform float inverse_gamma;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_map_texture;

in vec3 ws_position;
in vec3 ws_normal;
in vec2 vertex_out_texture_coordinate;

uniform vec3 diffuse_tint;
uniform vec3 specular_tint;
uniform vec3 ambient_tint;
uniform float shininess;
uniform float texture_scale;

uniform vec3 ws_view_position;
// uniform mat4 projection_view_matrix;

void main() {
        // Per vertex lighting
    vec3 ws_view_dir = normalize(ws_view_position - ws_position);
    LightCalculatioData light_calculation_data = LightCalculatioData(ws_position, ws_view_dir, ws_normal);
    Material material = Material(diffuse_tint, specular_tint, ambient_tint, shininess);


    LightingResult vertex_out_lighting_result = total_light_calculation(light_calculation_data, material
        #if NUM_PL > 0
        ,point_lights
        #endif
        #if NUM_PL_DIR > 0
        ,direction_lights
        #endif
        #if NUM_SL > 0
        ,spot_lights
        #endif
    );

        // Resolve the per vertex lighting with per fragment texture sampling.
    vec3 resolved_lighting = resolve_textured_light_calculation(vertex_out_lighting_result, diffuse_texture, specular_map_texture, vertex_out_texture_coordinate * texture_scale); 
    out_colour = vec4(resolved_lighting, 1.0f);
    out_colour.rgb = pow(out_colour.rgb, vec3(inverse_gamma));
}
