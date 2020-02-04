#pragma once

#include <glm/glm/gtx/norm.hpp>

#include "Ray.h"
#include "Hittable.h"
#include "Random.h"

// Reflects an incoming vector about a normal vector. Only for completeness. We use glm::reflect which does the same thing.
glm::vec3 reflect(glm::vec3 vecIn, glm::vec3 normal)
{
    return vecIn - 2 * glm::dot(vecIn, normal) * normal;
}

// Searches a point in the unit sphere, by rejecting all points which squared length of the origin vector exceeds 1.0
glm::vec3 randomInUnitSphere()
{
    glm::vec3 p;
    do {
        p = glm::vec3(randomDoubleCPP(), randomDoubleCPP(), randomDoubleCPP());
        p = 2.0f * p - glm::vec3(1,1,1);
    } while(glm::length2(p) >= 1.0f);
    return p;
}

class Material
{
public:
    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3 a) : _albedo(a) {}

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
    {
        glm::vec3 target = rec.p + rec.n + randomInUnitSphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = _albedo;
        return true;
    }


private:
    glm::vec3 _albedo;
};

class Metal : public Material
{
public:
    Metal(const glm::vec3 a) : _albedo(a) {}

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const {
        glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.direction()), rec.n);
        scattered = Ray(rec.p, reflected);
        attenuation = _albedo;
        return (dot(scattered.direction(), rec.n) > 0);
    }

private:
    glm::vec3 _albedo;

};