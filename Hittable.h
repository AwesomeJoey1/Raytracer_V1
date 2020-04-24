#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;

struct hitRecord {
    float t;
    glm::vec3 p;
    glm::vec3 n;
    float u;
    float v;
    bool frontFace;
    std::shared_ptr<Material> materialPtr;

    inline void setFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
    {
        frontFace = dot(ray.direction(), outwardNormal) < 0;
        n = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const = 0;
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const = 0;
};