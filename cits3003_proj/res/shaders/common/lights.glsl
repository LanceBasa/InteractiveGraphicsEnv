#ifndef NUM_PL
#define NUM_PL 0
#endif

#ifndef NUM_PL_DIR
#define NUM_PL_DIR 0
#endif

#ifndef NUM_SL
#define NUM_SL 0
#endif

// Material Properties
struct Material {
    vec3 diffuse_tint;
    vec3 specular_tint;
    vec3 ambient_tint;
    float shininess;
};

// Light Data
struct LightCalculatioData {
    vec3 ws_frag_position;
    vec3 ws_view_dir;
    vec3 ws_normal;
};

struct PointLightData {
    vec3 position;
    vec3 colour;
};

// Directional Light
struct DirectionalLightData {
    //vec3 position;
    vec3 direction;
    vec3 colour;
};

// Directional Light
struct SpotLightData {
    vec3 position;
    vec3 direction;
    vec3 colour;
};

// Total Calculation

struct LightingResult {
    vec3 total_diffuse;
    vec3 total_specular;
    vec3 total_ambient;
};



// Calculations

const float ambient_factor = 0.002f;

// Point Lights
void point_light_calculation(PointLightData point_light, LightCalculatioData calculation_data, float shininess, inout vec3 total_diffuse, inout vec3 total_specular, inout vec3 total_ambient) {


    // using this we can get the light distance to the object
    vec3 ws_light_offset = point_light.position - calculation_data.ws_frag_position; 
    float distance = length(ws_light_offset);       // length is a function that calculates the length from Euclidean distance between the fragment position and
    float lossRate=0.2;                        // Rate of quickly the light loss with distance.
    float attenuation = 1.0 / (1.0 + lossRate * distance + (lossRate* distance*distance));        // From lecture calculating Light attenuation with distance


    // Ambient
    vec3 ambient_component = ambient_factor * point_light.colour; // ambiance doesnt require attenuation (distance)

    // Diffuse
    vec3 ws_light_dir = normalize(ws_light_offset);
    float diffuse_factor = max(dot(ws_light_dir, calculation_data.ws_normal), 0.0f);
    vec3 diffuse_component = diffuse_factor * point_light.colour * attenuation;

    // Specular
    vec3 ws_halfway_dir = normalize(ws_light_dir + calculation_data.ws_view_dir);
    float specular_factor = pow(max(dot(calculation_data.ws_normal, ws_halfway_dir), 0.0f), shininess);
    vec3 specular_component = specular_factor * point_light.colour *attenuation;

    total_diffuse += diffuse_component;
    total_specular += specular_component;
    total_ambient += ambient_component;
}

// Directional Lights
void point_light_dir_calculation(DirectionalLightData dir_light, LightCalculatioData calculation_data, float shininess, inout vec3 total_diffuse, inout vec3 total_specular, inout vec3 total_ambient) {


    // using this we can get the light distance to the object
    //vec3 ws_light_offset = dir_light.direction - calculation_data.ws_frag_position; 
    vec3 ws_light_offset = normalize(-dir_light.direction);
    //float distance = length(ws_light_offset);       // length is a function that calculates the length from Euclidean distance between the fragment position and
    float lossRate=0.2;                        // Rate of quickly the light loss with distance.
    float attenuation = 1.0 / (1.0 + lossRate   + (lossRate));        // From lecture calculating Light attenuation with distance
    //float attenuation =1.0;

    // Ambient
    vec3 ambient_component = ambient_factor * dir_light.colour; // ambiance doesnt require attenuation (distance)

    // Diffuse
    vec3 ws_light_dir = normalize(ws_light_offset);
    float diffuse_factor = max(dot(ws_light_dir, calculation_data.ws_normal), 0.0f);
    vec3 diffuse_component = diffuse_factor * dir_light.colour * attenuation;

    // Specular
    vec3 ws_halfway_dir = normalize(ws_light_dir + calculation_data.ws_view_dir);
    float specular_factor = pow(max(dot(calculation_data.ws_normal, ws_halfway_dir), 0.0f), shininess);
    vec3 specular_component = specular_factor * dir_light.colour *attenuation;

    total_diffuse += diffuse_component;
    total_specular += specular_component;
    total_ambient += ambient_component;
}

// Directional Lights
void point_light_spot_calculation(SpotLightData spot_light, LightCalculatioData calculation_data, float shininess, inout vec3 total_diffuse, inout vec3 total_specular, inout vec3 total_ambient) {

    
    // else  // else, use ambient light so scene isn't completely dark outside the spotlight.
    // color = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);


    // using this we can get the light distance to the object
    vec3 ws_light_offset = spot_light.position - calculation_data.ws_frag_position; 
    float distance = length(ws_light_offset);       // length is a function that calculates the length from Euclidean distance between the fragment position and
    float lossRate=0.2;                        // Rate of quickly the light loss with distance.
    float attenuation = 1.0 / (1.0 + lossRate * distance + (lossRate* distance*distance));        // From lecture calculating Light attenuation with distance


    // Ambient
    vec3 ambient_component = ambient_factor * spot_light.colour; // ambiance doesnt require attenuation (distance)

    // Diffuse
    vec3 ws_light_dir = normalize(ws_light_offset);
    float diffuse_factor = max(dot(ws_light_dir, calculation_data.ws_normal), 0.0f);
    vec3 diffuse_component = diffuse_factor * spot_light.colour * attenuation;

    // Specular
    vec3 ws_halfway_dir = normalize(ws_light_dir + calculation_data.ws_view_dir);
    float specular_factor = pow(max(dot(calculation_data.ws_normal, ws_halfway_dir), 0.0f), shininess);
    vec3 specular_component = specular_factor * spot_light.colour *attenuation;

    float angle = 0.52;

    float theta = dot(ws_light_dir, normalize(spot_light.direction));
    
    if(theta > angle) 
    {       
        total_diffuse += diffuse_component;
        total_specular += specular_component;
        total_ambient += ambient_component;
    }
}



LightingResult total_light_calculation(LightCalculatioData light_calculation_data, Material material
        #if NUM_PL > 0
        ,PointLightData point_lights[NUM_PL]

        #endif

        #if NUM_PL_DIR > 0
        ,DirectionalLightData dir_lights[NUM_PL_DIR]
        #endif

        #if NUM_SL > 0
        ,SpotLightData spot_light[NUM_SL]
        #endif
    ) {

    vec3 total_diffuse = vec3(0.0f);
    vec3 total_specular = vec3(0.0f);
    vec3 total_ambient = vec3(0.0f);

    #if NUM_PL > 0
    for (int i = 0; i < NUM_PL; i++) {
        point_light_calculation(point_lights[i], light_calculation_data, material.shininess, total_diffuse, total_specular, total_ambient);
    }
    #endif

    #if NUM_PL_DIR > 0
    for (int i = 0; i < NUM_PL_DIR; i++) {
        point_light_dir_calculation(dir_lights[i], light_calculation_data, material.shininess, total_diffuse, total_specular, total_ambient);
    }
    #endif

    #if NUM_SL > 0
    for (int i = 0; i < NUM_SL; i++) {
        point_light_spot_calculation(spot_light[i], light_calculation_data, material.shininess, total_diffuse, total_specular, total_ambient);
    }
    #endif

    #if NUM_PL > 0
    total_ambient /= float(NUM_PL);
    #endif

    #if NUM_PL_DIR > 0
    total_ambient /= float(NUM_PL_DIR);
    #endif

    #if NUM_SL > 0
    total_ambient /= float(NUM_SL);
    #endif

    total_diffuse *= material.diffuse_tint;
    total_specular *= material.specular_tint;
    total_ambient *= material.ambient_tint;

    return LightingResult(total_diffuse, total_specular, total_ambient);
}

vec3 resolve_textured_light_calculation(LightingResult result, sampler2D diffuse_texture, sampler2D specular_map, vec2 texture_coordinate) {
    vec3 texture_colour = texture(diffuse_texture, texture_coordinate).rgb;
    vec3 specular_map_sample = texture(specular_map, texture_coordinate).rgb;

    vec3 textured_diffuse = result.total_diffuse * texture_colour;
    vec3 sampled_specular = result.total_specular * specular_map_sample;
    vec3 textured_ambient = result.total_ambient * texture_colour;

    // Mix the diffuse and ambient so that there is no ambient in bright scenes
    return max(textured_diffuse, textured_ambient) + sampled_specular;
}




