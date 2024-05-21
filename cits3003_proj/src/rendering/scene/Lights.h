#ifndef LIGHTS_H
#define LIGHTS_H

#include <memory>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

/// A representation of a PointLight render scene element
struct PointLight {
    PointLight() = default;

    PointLight(const glm::vec3& position, const glm::vec4& colour) :
        position(position), colour(colour) {}

    static PointLight off() {
        return {glm::vec3{}, glm::vec4{}};
    }

    static std::shared_ptr<PointLight> create(const glm::vec3& position, const glm::vec4& colour) {
        return std::make_shared<PointLight>(position, colour);
    }

    glm::vec3 position{};
    // Alpha components are just used to store a scalar that is applied before passing to the GPU
    glm::vec4 colour{};

    // On GPU format
    // alignas used to conform to std140 for direct binary usage with glsl
    struct Data {
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 colour;
    };
};

struct PointLightDirection {
    PointLightDirection() = default;

    PointLightDirection(const glm::vec3& position, const glm::vec4& colour) :
        position(position), colour(colour) {}

    static PointLightDirection off() {
        return {glm::vec3{}, glm::vec4{}};
    }

    static std::shared_ptr<PointLightDirection> create(const glm::vec3& position, const glm::vec4& colour) {
        return std::make_shared<PointLightDirection>(position, colour);
    }

    glm::vec3 position{};
    glm::vec3 direction{};
    // Alpha components are just used to store a scalar that is applied before passing to the GPU
    glm::vec4 colour{};

    // On GPU format
    // alignas used to conform to std140 for direct binary usage with glsl
    struct Data {
        alignas(16) glm::vec3 direction;
        alignas(16) glm::vec3 colour;
    };
};


struct SpotLight {
    SpotLight() = default;

    SpotLight(const glm::vec3& position,const glm::vec3& direction ,const glm::vec4& colour) :
        position(position), direction(direction),colour(colour) {}

    static SpotLight off() {
        return {glm::vec3{}, glm::vec3{},glm::vec4{}};
    }

    static std::shared_ptr<SpotLight> create(const glm::vec3& position, const glm::vec3 direction, const glm::vec4& colour) {
        return std::make_shared<SpotLight>(position, direction, colour);
    }

    glm::vec3 position{};
    glm::vec3 direction{};
    // Alpha components are just used to store a scalar that is applied before passing to the GPU
    glm::vec4 colour{};

    // On GPU format
    // alignas used to conform to std140 for direct binary usage with glsl
    struct Data {
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 direction;
        alignas(16) glm::vec3 colour;
    };
};

/// A collection of each light type, with helpers that allow for selecting a subset of
/// those lights on a proximity basis, since processing an unbounded number of lights on the GPU is bad idea.


/// A collection of each light type, with helpers that allow for selecting a subset of
/// those lights on a proximity basis, since processing an unbounded number of lights on the GPU is bad idea.
struct LightScene {
    std::unordered_set<std::shared_ptr<PointLight>> point_lights;

    std::unordered_set<std::shared_ptr<PointLightDirection>> point_lights_dir;
    std::unordered_set<std::shared_ptr<SpotLight>> spot_lights;

    /// Will return up to `max_count` nearest point lights to `target`.
    /// It returns less than `max_count` if there are not that many point lights,
    /// in which case it will end up returning all point lights.
    ///
    /// If a `min_count` > 0 is provided, it will provide at least that many, with filling empty
    /// slots with a "Black" light.
    ///
    /// NOTE: This is VERY inefficient as is, since it currently (possibly) sorts all lights,
    ///       then copies out up to `max_count` point lights, so is O(n log(n)) for
    ///       n being the total number of point lights.
    ///
    ///       So an improvement would be to use a data structure which can accelerate this,
    ///       as well as support incrementally getting the `k` nearest.
    ///
    std::vector<PointLightDirection> get_nearest_point_lights_dir(glm::vec3 target, size_t max_count, size_t min_count = 0) const;


    std::vector<SpotLight> get_nearest_spot_lights(glm::vec3 target, size_t max_count, size_t min_count = 0) const;



    /// Will return up to `max_count` nearest point lights to `target`.
    /// It returns less than `max_count` if there are not that many point lights,
    /// in which case it will end up returning all point lights.
    ///
    /// If a `min_count` > 0 is provided, it will provide at least that many, with filling empty
    /// slots with a "Black" light.
    ///
    /// NOTE: This is VERY inefficient as is, since it currently (possibly) sorts all lights,
    ///       then copies out up to `max_count` point lights, so is O(n log(n)) for
    ///       n being the total number of point lights.
    ///
    ///       So an improvement would be to use a data structure which can accelerate this,
    ///       as well as support incrementally getting the `k` nearest.
    ///
    std::vector<PointLight> get_nearest_point_lights(glm::vec3 target, size_t max_count, size_t min_count = 0) const;

private:
    template<typename Light>
    static std::vector<Light> get_nearest_lights(const std::unordered_set<std::shared_ptr<Light>>& lights, glm::vec3 target, size_t max_count, size_t min_count = 0);
    template<typename Light>
    static std::vector<Light> get_nearest_lights_dir(const std::unordered_set<std::shared_ptr<Light>>& lights, glm::vec3 target, size_t max_count, size_t min_count = 0);
    template<typename Light>
    static std::vector<Light> get_nearest_lights_spot(const std::unordered_set<std::shared_ptr<Light>>& lights, glm::vec3 target, size_t max_count, size_t min_count = 0);
};

/// A representation of a PointLightDirection render scene element
#endif //LIGHTS_H
