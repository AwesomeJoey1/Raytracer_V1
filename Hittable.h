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

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> objPtr, const glm::vec3& displacement)
        : _objPtr(objPtr), _offset(displacement)  {}

    bool boundingBox(float t0, float t1, AABB& outputBox) const override;
    bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

private:
    std::shared_ptr<Hittable> _objPtr;
    glm::vec3 _offset;
};

bool Translate::boundingBox(float t0, float t1, AABB &outputBox) const
{
    if (!_objPtr->boundingBox(t0, t1, outputBox))
        return false;

    outputBox = AABB(outputBox.min() + _offset, outputBox.max() + _offset);

    return true;
}

bool Translate::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    Ray translatedRay = Ray(ray.origin() - _offset, ray.direction(), ray.time());

    if (!_objPtr->hit(translatedRay, tMin, tMax, rec))
        return false;

    rec.p = rec.p + _offset;
    rec.setFaceNormal(translatedRay, rec.n);

    return true;
}

class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> obj, float angle);

    bool boundingBox(float t0, float t1, AABB& outputBox) const override;
    bool hit(const Ray& ray, float tMin ,float tMax, hitRecord& rec) const override;

private:
    std::shared_ptr<Hittable> _objPtr;
    float _sinTheta, _cosTheta;
    bool _hasBox;
    AABB _bbox;
};

RotateY::RotateY(std::shared_ptr<Hittable> objPtr, float angle) :_objPtr(objPtr)
{
    auto radians = degreeToRadians(angle);
    _sinTheta = sin(radians);
    _cosTheta = cos(radians);
    _hasBox = _objPtr->boundingBox(0, 1, _bbox);

    glm::vec3 min(c_Infinity);
    glm::vec3 max(c_Infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++)
            {
                auto x = i*_bbox.max().x + (1-i)*_bbox.min().x;
                auto y = i*_bbox.max().y + (1-i)*_bbox.min().y;
                auto z = i*_bbox.max().z + (1-i)*_bbox.min().z;

                auto newX = _cosTheta*x + _sinTheta*z;
                auto newZ = -_sinTheta*x + _cosTheta*z;

                glm::vec3 temp(newX, y, newZ);

                for (int c = 0; c < 3; c++)
                {
                    min[c] = ffmin(min[c], temp[c]);
                    max[c] = ffmax(max[c], temp[c]);
                }
            }
        }
    }
    _bbox = AABB(min, max);
}

bool RotateY::boundingBox(float t0, float t1, AABB &outputBox) const
{
    outputBox = _bbox;
    return _hasBox;
}

bool RotateY::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    auto origin = ray.origin();
    auto direction = ray.direction();

    origin.x = _cosTheta * ray.origin().x - _sinTheta * ray.origin().z;
    origin.z = _sinTheta * ray.origin().x + _cosTheta * ray.origin().z;

    direction.x = _cosTheta * ray.direction().x - _sinTheta * ray.direction().z;
    direction.z = _sinTheta * ray.direction().x + _cosTheta * ray.direction().z;

    Ray rotatedRay(origin, direction, ray.time());

    if (!_objPtr->hit(rotatedRay, tMin, tMax, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.n;

    p.x = _cosTheta * rec.p.x + _sinTheta * rec.p.z;
    p.z = -_sinTheta * rec.p.x + _cosTheta * rec.p.z;

    normal.x = _cosTheta * rec.n.x + _sinTheta * rec.n.z;
    normal.z = -_sinTheta * rec.n.x + _cosTheta * rec.n.z;

    rec.p = p;
    rec.setFaceNormal(rotatedRay, normal);

    return true;
}