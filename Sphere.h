#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(const glm::vec3 &center, float radius) : _center(center), _radius(radius) {};

    virtual bool hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const;

private:
    glm::vec3 _center;
    float _radius;
};

bool Sphere::hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const {
    glm::vec3 oc = ray.origin() - _center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2.0f * glm::dot(ray.direction(), oc);
    float c = glm::dot(oc, oc) - _radius * _radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float temp = (-b - glm::sqrt(discriminant)) / (2*a);
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            rec.n = (rec.p - _center) / _radius;
            return true;
        }
        temp = (-b + glm::sqrt(discriminant)) / (2*a);
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            rec.n = (rec.p - _center) / _radius;
            return true;
        }
    }
    return false;
}

