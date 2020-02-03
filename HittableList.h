#pragma once

#include "Hittable.h"

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(Hittable **l, int n) { _list = l; _listSize = n; }

    virtual bool hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const;

private:
    Hittable **_list;
    int _listSize;
};

bool HittableList::hit(const Ray &ray, float tmin, float tmax, hitRecord &rec) const
{
    hitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = tmax;
    for (int i = 0; i < _listSize; i++)
    {
        if(_list[i]->hit(ray, tmin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}