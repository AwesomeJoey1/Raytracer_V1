#pragma once

#include "HittableList.h"

class Box : public Hittable
{
public:
    Box() {}
    Box(const glm::vec3& p0, const glm::vec3& p1, std::shared_ptr<Material> matPtr);

    bool boundingBox(float t0, float t1, AABB& outputBox) const override;
    bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

private:
    glm::vec3 _boxMin, _boxMax;
    HittableList _sides;
};

Box::Box(const glm::vec3& p0, const glm::vec3& p1, std::shared_ptr<Material> matPtr)
{
    _boxMin = p0;
    _boxMax = p1;

    _sides.add(std::make_shared<xyRect>(p0.x, p1.x, p0.y, p1.y, p1.z, matPtr));
    _sides.add(std::make_shared<FlipFace>(std::make_shared<xyRect>(p0.x, p1.x, p0.y, p1.y, p0.z, matPtr)));
    _sides.add(std::make_shared<xzRect>(p0.x, p1.x, p0.z, p1.z, p1.y, matPtr));
    _sides.add(std::make_shared<FlipFace>(std::make_shared<xzRect>(p0.x, p1.x, p0.z, p1.z, p0.y, matPtr)));
    _sides.add(std::make_shared<yzRect>(p0.y, p1.y, p0.z, p1.z, p1.x, matPtr));
    _sides.add(std::make_shared<FlipFace>(std::make_shared<yzRect>(p0.y, p1.y, p0.z, p1.z, p0.x, matPtr)));
}

bool Box::boundingBox(float t0, float t1, AABB &outputBox) const
{
    outputBox = AABB(_boxMin, _boxMax);
    return true;
}

bool Box::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    return _sides.hit(ray, tMin, tMax, rec);
}