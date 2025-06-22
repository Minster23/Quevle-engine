#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <limits>

namespace QuavleEngine {

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction; // Should be normalized
};

struct RaycastHit {
    bool hit = false;
    float distance = std::numeric_limits<float>::max();
    glm::vec3 point = glm::vec3(0.0f);
    int objectIndex = -1;
};

// Axis-Aligned Bounding Box
struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

// Ray-AABB intersection
inline bool rayIntersectsAABB(const Ray& ray, const AABB& box, float& tmin, float& tmax) {
    tmin = (box.min.x - ray.origin.x) / ray.direction.x;
    tmax = (box.max.x - ray.origin.x) / ray.direction.x;
    if (tmin > tmax) std::swap(tmin, tmax);
    float tymin = (box.min.y - ray.origin.y) / ray.direction.y;
    float tymax = (box.max.y - ray.origin.y) / ray.direction.y;
    if (tymin > tymax) std::swap(tymin, tymax);
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    float tzmin = (box.min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (box.max.z - ray.origin.z) / ray.direction.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    return tmax >= std::max(0.0f, tmin);
}

// Raycast against a list of AABBs, returns the closest hit
inline RaycastHit raycast(const Ray& ray, const std::vector<AABB>& objects) {
    RaycastHit result;
    for (size_t i = 0; i < objects.size(); ++i) {
        float tmin, tmax;
        if (rayIntersectsAABB(ray, objects[i], tmin, tmax) && tmin >= 0.0f && tmin < result.distance) {
            result.hit = true;
            result.distance = tmin;
            result.point = ray.origin + tmin * ray.direction;
            result.objectIndex = static_cast<int>(i);
        }
    }
    return result;
}

} // namespace QuavleEngine

