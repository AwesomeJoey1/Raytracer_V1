#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> materialPtr) :
            _center(center), _radius(radius), _materialPtr(materialPtr) {};

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

private:
    glm::vec3 _center;
    float _radius;
    std::shared_ptr<Material> _materialPtr;
};

bool Sphere::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    glm::vec3 oc = ray.origin() - _center;
    auto a = glm::dot(ray.direction(), ray.direction());
    auto halfB = glm::dot(oc, ray.direction());
    auto c = glm::dot(oc, oc) - _radius*_radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant > 0) {
        auto root = glm::sqrt(discriminant);

        auto temp = (-halfB - root) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            glm::vec3 outwardNormal = (rec.p - _center) / _radius;
            rec.setFaceNormal(ray, outwardNormal);
            rec.materialPtr = _materialPtr;
            return true;
        }

        temp = (-halfB + root) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            glm::vec3 outwardNormal = (rec.p - _center) / _radius;
            rec.setFaceNormal(ray, outwardNormal);
            rec.materialPtr = _materialPtr;
            return true;
        }
    }
    return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB& outputBox) const {
    if (_center.y < -200)
    {
        int a = 1;
    }
    outputBox = AABB(_center - _radius, _center + _radius);
    return true;
}

