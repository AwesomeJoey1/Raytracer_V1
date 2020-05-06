#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { _objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { _objects.push_back(object); }

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float tMin, float tMax, AABB& outputBox) const;

    std::vector<std::shared_ptr<Hittable>> _objects;
};

bool HittableList::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const
{
    hitRecord tempRec;
    bool hitAnything = false;
    float closestSoFar = tMax;
    for (const auto& object : _objects)
    {
        if(object->hit(ray, tMin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}

bool HittableList::boundingBox(float tMin, float tMax, AABB& outputBox) const
{
    if (_objects.empty()) return false;

    AABB tempBox;
    bool firstBox = true;

    for (const auto& object : _objects)
    {
        if(!object->boundingBox(tMin, tMax, tempBox)) return false;

        outputBox = firstBox ? tempBox : surroundingBox(outputBox, tempBox);
        firstBox = false;
    }
    return true;
}