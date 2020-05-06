#pragma once

#include "Hittable.h"

class xyRect : public Hittable
{
public:
    xyRect() {}

    xyRect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> mat) :
        _x0(x0), _x1(x1), _y0(y0), _y1(y1), _k(k), _materialPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

private:
    std::shared_ptr<Material> _materialPtr;
    float _x0, _x1, _y0, _y1, _k;

};

class xzRect : public Hittable
{
public:
    xzRect() {}

    xzRect(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> mat)
            : _x0(x0), _x1(x1), _z0(z0), _z1(z1), _k(k), _materialPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

private:
    float _x0, _x1, _z0, _z1, _k;
    std::shared_ptr<Material> _materialPtr;
};

class yzRect : public Hittable{
public:
    yzRect() {}

    yzRect(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> mat)
        : _y0(y0), _y1(y1), _z0(z0), _z1(z1), _k(k), _materialPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const;

private:
    float _y0, _y1, _z0, _z1, _k;
    std::shared_ptr<Material> _materialPtr;
};

bool xyRect::boundingBox(float t0, float t1, AABB &outputBox) const
{
    // Bounding box must have a non-zero width in each dimension --> padding in z dimension
    outputBox = AABB(glm::vec3(_x0, _y0, _k-0.0001f), glm::vec3(_x1, _y1, _k+0.0001f));
    return true;
}

bool xyRect::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    // f(t) = A + t*B;
    // k = A_z + t*B_z --> t = (k - A_z) / B_z
    float t = (_k - ray.origin().z) / ray.direction().z;
    if (t < tMin || t > tMax)
        return false;

    float x = ray.origin().x + t * ray.direction().x;
    float y = ray.origin().y + t * ray.direction().y;

    if (x < _x0 || x > _x1 || y < _y0 || y > _y1)
        return false;

    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (y - _y0) / (_y1 - _y0);
    rec.t = t;
    auto outwardNormal = glm::vec3(0,0,1);
    rec.setFaceNormal(ray, outwardNormal);
    rec.materialPtr = _materialPtr;
    rec.p = ray.pointAtParam(t);
    return true;
}

bool xzRect::boundingBox(float t0, float t1, AABB &outputBox) const
{
    outputBox = AABB(glm::vec3(_x0, _k-0.0001f, _z0), glm::vec3(_x1, _k+0.0001, _z1));
    return true;
}

bool xzRect::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    float t = (_k - ray.origin().y) / ray.direction().y;
    if (t < tMin || t > tMax)
        return false;

    float x = ray.origin().x + t * ray.direction().x;
    float z = ray.origin().z + t * ray.direction().z;

    if (x < _x0 || x > _x1 || z < _z0 || z > _z1)
        return false;

    rec.u = (x - _x0) / (_x1 - _x0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.t = t;
    auto outwardNormal = glm::vec3( 0 , 1, 0);
    rec.setFaceNormal(ray, outwardNormal);
    rec.materialPtr = _materialPtr;
    rec.p = ray.pointAtParam(t);
    return true;
}

bool yzRect::boundingBox(float t0, float t1, AABB &outputBox) const
{
    outputBox = AABB(glm::vec3(_k-0.0001f, _y0, _z0), glm::vec3(_k+0.0001, _y1, _z1));
    return true;
}

bool yzRect::hit(const Ray &ray, float tMin, float tMax, hitRecord &rec) const
{
    float t = (_k - ray.origin().x) / ray.direction().x;
    if (t < tMin || t > tMax)
        return false;

    float y = ray.origin().y + t * ray.direction().y;
    float z = ray.origin().z + t * ray.direction().z;

    if (y < _y0 || y > _y1 || z < _z0 || z > _z1)
        return false;

    rec.u = (y - _y0) / (_y1 - _y0);
    rec.v = (z - _z0) / (_z1 - _z0);
    rec.t = t;
    auto outwardNormal = glm::vec3(1, 0, 0);
    rec.setFaceNormal(ray, outwardNormal);
    rec.materialPtr = _materialPtr;
    rec.p = ray.pointAtParam(t);
    return true;
}