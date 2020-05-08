#pragma once

#include "Texture.h"
#include "Material.h"

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, float density, std::shared_ptr<Texture> tex)
        : _boundary(boundary), _negInvDensity(-1.0f/density)
    {
        phaseFunction = std::make_shared<Isotropic>(tex);
    }

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override;
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;


private:
    std::shared_ptr<Hittable> _boundary;
    std::shared_ptr<Material> phaseFunction;
    float _negInvDensity;
};

bool ConstantMedium::boundingBox(float t0, float t1, AABB &outputBox) const
{
    return _boundary->boundingBox(t0, t1, outputBox);
}

bool ConstantMedium::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    hitRecord rec1, rec2;

    if (!_boundary->hit(ray, -c_Infinity, c_Infinity, rec1))
        return false;

    if (!_boundary->hit(ray, rec1.t+0.0001f, c_Infinity, rec2))
        return false;

    rec1.t = (rec1.t < tMin) ? tMin : rec1.t;
    rec2.t = (rec2.t > tMax) ? tMax : rec2.t;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto rayLength = glm::length(ray.direction());
    const auto distInBoundary = (rec2.t - rec1.t) * rayLength;
    const auto hitDistance = _negInvDensity * glm::log(randomDouble());

    if (hitDistance > distInBoundary)
        return false;

    rec.t = rec1.t + hitDistance / rayLength;
    rec.p = ray.pointAtParam(rec.t);
    rec.n = glm::vec3(1, 0, 0);
    rec.frontFace = true;
    rec.materialPtr = phaseFunction;
    return true;
}