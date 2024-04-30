#version 410 core
#include "../common/lights.glsl"


layout(location = 0) out vec4 out_colour;

in vec3 ws_position;
in vec3 ws_normal;
in vec2 vertex_out.texture_coordinate;

// Material properties
uniform vec3 diffuse_tint;
uniform vec3 specular_tint;
uniform vec3 ambient_tint;
uniform float shininess;
uniform float texture_scale;


// Global Data
uniform float inverse_gamma;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_map_texture;

void main() {

    vec3 ws_view_dir = normalize(ws_view_position - ws_position);
    LightCalculatioData light_calculation_data = LightCalculatioData(ws_position, ws_view_dir, ws_normal);
    Material material = Material(diffuse_tint, specular_tint, ambient_tint, shininess);


    // Resolve the per vertex lighting with per fragment texture sampling.
    vec3 resolved_lighting = resolve_textured_light_calculation(frag_in.lighting_result, diffuse_texture, specular_map_texture, frag_in.texture_coordinate);

    out_colour = vec4(resolved_lighting, 1.0f);
    out_colour.rgb = pow(out_colour.rgb, vec3(inverse_gamma));
}