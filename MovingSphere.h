#pragma once

#include "Common.h"
#include "Hittable.h"

class MovingSphere : public Hittable
{
public:
    MovingSphere() {}
    MovingSphere(glm::vec3 center0, glm::vec3 center1, float t0, float t1, float radius,
            std::shared_ptr<Material> material) :
            _center0(center0), _center1(center1), _time0(t0), _time1(t1), _radius(radius), _materialPtr(material) {};

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

    glm::vec3 center(float t) const;

private:
    glm::vec3 _center0, _center1;
    float _time0, _time1, _radius;
    std::shared_ptr<Material> _materialPtr;
};

bool MovingSphere::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const
{
    glm::vec3 oc = ray.origin() - center(ray.time());
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
           glm::vec3 outwardNormal = (rec.p - center(ray.time())) / _radius;
           rec.setFaceNormal(ray, outwardNormal);
           rec.materialPtr = _materialPtr;
           return true;
        }

        temp = (-halfB + root) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            glm::vec3 outwardNormal = (rec.p - center(ray.time())) / _radius;
            rec.setFaceNormal(ray, outwardNormal);
            rec.materialPtr = _materialPtr;
            return true;
        }
    }
    return false;
}

bool MovingSphere::boundingBox(float t0, float t1, AABB& outputBox) const
{
    AABB box1(center(t0) - _radius, center(t0) + _radius);
    AABB box2(center(t1) - _radius, center(t1) + _radius);
    outputBox = surroundingBox(box1, box2);
    return true;
}

glm::vec3 MovingSphere::center(float t) const {
    return _center0 + ((t-_time0)/(_time1 - _time0)) * (_center1 - _center0);
}