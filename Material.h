#pragma once

#include <glm/glm/gtx/norm.hpp>

#include "Ray.h"
#include "Hittable.h"
#include "Random.h"

// Calculates the polynomial approximation for angle dependend reflecticity probability by Christophe Schlick
float schlick(float cosine, float refIdx)
{
    float r0 = (1-refIdx) / (1+refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*glm::pow((1 - cosine),2);
}

// Reflects an incoming vector about a normal vector. Only for completeness. We use glm::reflect which does the same thing.
glm::vec3 reflect(glm::vec3 vecIn, glm::vec3 normal)
{
    return vecIn - 2 * glm::dot(vecIn, normal) * normal;
}

// Refracts an incoming vector at an intersection of a dielectric. The two refractive indices are combined in niOverNt.
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

        float reflect_prob;
        float cosine;

        // normal calculation. If ray hits outer boundary
        if(glm::dot(rayIn.direction(), rec.n) > 0)
        {
            outward_normal = -rec.n;
            niOverNt = _refIdx;
            //cosine = _refIdx * glm::dot(rayIn.direction(), rec.n) / glm::length(rayIn.direction());
            cosine = dot(rayIn.direction(), rec.n) / rayIn.direction().length();
            cosine = sqrt(1 - _refIdx*_refIdx*(1-cosine*cosine));
        }
        else { // If ray hits inner boundary
            outward_normal = rec.n;
            niOverNt = 1.0f / _refIdx;
            cosine = -glm::dot(rayIn.direction(), rec.n) / glm::length(rayIn.direction());
        }

        if(refract(rayIn.direction(), outward_normal, niOverNt, refracted))
        {
            reflect_prob = schlick(cosine, _refIdx);
        }
        else {
            reflect_prob = 1.0f;
        }

        if (randomDoubleC() < reflect_prob)
        {
            scattered = Ray(rec.p, reflected);
        } else {
            scattered = Ray(rec.p, refracted);
        }
        return true;
    }

private:
    float _refIdx;
};