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
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const = 0;
    virtual bool hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const = 0;
};

class FlipFace : public Hittable {
public:
    FlipFace(std::shared_ptr<Hittable> objectPtr) : _objectPtr(objectPtr) {}

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override
    {
        return _objectPtr->boundingBox(t0, t1, outputBox);

    }
    virtual bool hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const override
    {
        if (!_objectPtr->hit(ray, tMin, tMax, rec))
            return false;

        rec.frontFace = !rec.frontFace;
        return true;
    }

private:
    std::shared_ptr<Hittable> _objectPtr;
};