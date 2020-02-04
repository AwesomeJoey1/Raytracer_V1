#pragma once

#include "Ray.h"

class Material;

struct hitRecord {
    float t;
    glm::vec3 p;
    glm::vec3 n;
    Material *materialPtr;
};

class Hittable {
public:
    virtual bool hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const = 0;
};