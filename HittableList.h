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


    virtual bool hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const;

private:
    std::vector<std::shared_ptr<Hittable>> _objects;
};

bool HittableList::hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const
{
    hitRecord tempRec;
    bool hitAnything = false;
    float closestSoFar = tmax;
    for (const auto& object : _objects)
    {
        if(object->hit(ray, tmin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}