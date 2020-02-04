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

bool refract(const glm::vec3 &vecIn, const glm::vec3 &n, float niOverNt, glm::vec3 &refracted)
{
    glm::vec3 uv = glm::normalize(vecIn);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0f - niOverNt*niOverNt*(1-dt*dt);
    if (discriminant > 0)
    {
        refracted = niOverNt*(uv - n*dt) - n*glm::sqrt(discriminant);
        return true;
    } else
        return false;
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
    Metal(const glm::vec3 a, float f) : _albedo(a) { if(f < 1.0f) _fuzziness = f; else _fuzziness = 1.0f; }

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const {
        glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.direction()), rec.n);
        // Fuzziness as pertubation of the material. 0 = no pertubation
        scattered = Ray(rec.p, reflected + _fuzziness*randomInUnitSphere());
        attenuation = _albedo;
        return (dot(scattered.direction(), rec.n) > 0);
    }

private:
    glm::vec3 _albedo;
    float _fuzziness;

};

class Dielectric : public Material {
public:
    Dielectric(float ri) : _refIdx(ri) {}

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const  {
        glm::vec3 outward_normal;
        glm::vec3 reflected = reflect(rayIn.direction(), rec.n);
        float niOverNt;
        attenuation = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 refracted;

        // normal calculation. If ray hits outer boundary
        if(glm::dot(rayIn.direction(), rec.n) > 0)
        {
            outward_normal = -rec.n;
            niOverNt = _refIdx;
        }
        else { // If ray hits inner boundary
            outward_normal = rec.n;
            niOverNt = 1.0f / _refIdx;
        }

        if(refract(rayIn.direction(), outward_normal, niOverNt, refracted))
        {
            scattered = Ray(rec.p, refracted);
        } else {
            scattered = Ray(rec.p, reflected);
            return false;
        }
        return true;
    }

private:
    float _refIdx;
};