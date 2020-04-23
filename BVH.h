#pragma once

#include <algorithm>
#include "Common.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
public:

    BVHNode();

    BVHNode(HittableList& list, float time0, float time1) :
        BVHNode(list._objects, 0, list._objects.size(), time0, time1) {}

    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, float time0, float time1);

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

private:
    std::shared_ptr<Hittable> _left;
    std::shared_ptr<Hittable> _right;
    AABB _box;
};

inline bool boxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
    AABB boxA, boxB;
    if(!a->boundingBox(0,0, boxA) || !b->boundingBox(0,0, boxB))
        std::cerr << "No bounding box in BVH Node constructor.\n";

    return boxA.min()[axis] < boxB.min()[axis];
}

bool boxCompareX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 0);
}

bool boxCompareY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 1);
}

bool boxCompareZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 2);
}

 BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end, float time0, float time1)
 {
     int axis = randomInt(0, 2);
     auto comparator = (axis == 0) ? boxCompareX
                     : (axis == 1) ? boxCompareY
                                   : boxCompareZ;

     size_t objectSpan = end - start;

     if (objectSpan == 1) {
         _left = _right = objects[start];
     } else if (objectSpan == 2) {
         if(comparator(objects[start], objects[start+1])) {
             _left = objects[start];
             _right = objects[start+1];
         } else {
             _left = objects[start+1];
             _right = objects[start];
         }
     } else {
         std::sort(objects.begin()+start, objects.begin()+end, comparator);
         auto mid = start + objectSpan / 2;
         _left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
         _right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);
     }

     AABB boxLeft, boxRight;
     if(!_left->boundingBox(time0, time1, boxLeft) ||  !_right->boundingBox(time0, time1, boxRight))
     {
         std::cerr << "No bounding box in BVH Node constructor \n";
     }

     _box = surroundingBox(boxLeft,boxRight);
 }

 bool BVHNode::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
 {
     if(!_box.hit(ray, tMin, tMax)) return false;

     bool hitLeft = _left->hit(ray, tMin, tMax, rec);
     bool hitRight = _right->hit(ray, tMin, hitLeft ? rec.t : tMax, rec);

     return hitLeft || hitRight;
 }

bool BVHNode::boundingBox(float t0, float t1, AABB &outputBox) const
{
    outputBox = _box;
    return true;
}