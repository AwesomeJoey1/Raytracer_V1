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
    void sphereUV(const glm::vec3& p, float& u, float& v) const;

    glm::vec3 _center;
    float _radius;
    std::shared_ptr<Material> _materialPtr;
};

bool Sphere::boundingBox(float t0, float t1, AABB& outputBox) const {
    outputBox = AABB(_center - _radius, _center + _radius);
    return true;
}

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
            sphereUV((rec.p - _center)/_radius, rec.u, rec.v);
            return true;
        }

        temp = (-halfB + root) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = ray.pointAtParam(rec.t);
            glm::vec3 outwardNormal = (rec.p - _center) / _radius;
            rec.setFaceNormal(ray, outwardNormal);
            rec.materialPtr = _materialPtr;
            sphereUV((rec.p - _center)/_radius, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

void Sphere::sphereUV(const glm::vec3 &p, float &u, float &v) const
{
    /* Parametric description of a point on a unit sphere
     * x = cos(phi)cos(theta)
     * y = sin(phi)cos(theta)
     * y = sin(theta)
     */
    auto phi = atan2(p.z, p.x);         // atan returns in range [-pi, pi]
    auto theta = asin(p.y);             // asin returns in range [-pi/2, pi/2]
    u = 1.0f-(phi+c_Pi)/(2*c_Pi);
    v = (theta + c_Pi/2.0) / c_Pi;
}
