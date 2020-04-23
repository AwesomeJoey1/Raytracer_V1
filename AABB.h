#pragma once

#include "Common.h"

class AABB
{
public:
    AABB() {};
    AABB(const glm::vec3& a, const glm::vec3& b) { _min = a; _max = b; }

    glm::vec3 min() { return _min; }
    glm::vec3 max() { return _max; }

    bool hit(const Ray& ray, float tMin, float tMax) const;

private:
    glm::vec3 _min, _max;
};

bool AABB::hit(const Ray& ray, float tMin, float tMax) const
{
    // f(t) = a+ t*b --> x0 = a_x + t*b_x --> t = (x0 - a_x) / b_x
    for (int i = 0; i < 3; i++)
    {
        auto t0 = ffmin((_min[i] - ray.origin()[i]) / ray.direction()[i],
                        (_max[i] - ray.origin()[i]) / ray.direction()[i]);
        auto t1 = ffmax((_min[i] - ray.origin()[i]) / ray.direction()[i],
                        (_max[i] - ray.origin()[i]) / ray.direction()[i]);
        tMin = ffmax(t0, tMin);
        tMax = ffmin(t1, tMax);
        if (tMax <= tMin)
            return false;
    }
    return true;
}

AABB surroundingBox(AABB box1, AABB box2)
{
    glm::vec3 small(ffmin(box1.min().x, box2.min().x),
                    ffmin(box1.min().y, box2.min().y),
                    ffmin(box1.min().z, box2.min().z));

    glm::vec3 big(ffmax(box1.max().x, box2.max().x),
                  ffmax(box1.max().y, box2.max().y),
                  ffmax(box1.max().z, box2.max().z));

    return AABB(small, big);
}