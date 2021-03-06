#pragma once

#include <glm/glm/gtx/norm.hpp>

#include "Ray.h"
#include "Hittable.h"

// Calculates the polynomial approximation for angle dependend reflecticity probability by Christophe Schlick
float schlick(float cosine, float refIdx)
{
    float r0 = (1-refIdx) / (1+refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*glm::pow((1 - cosine),5);
}

// Reflects an incoming vector about a normal vector. Only for completeness. We use glm::reflect which does the same thing.
glm::vec3 reflect(glm::vec3& vecIn, glm::vec3& normal)
{
    return vecIn - 2 * glm::dot(vecIn, normal) * normal;
}

// Refracts an incoming vector at an intersection of a dielectric. The two refractive indices are combined in niOverNt.
glm::vec3 refract(const glm::vec3 &vecIn, const glm::vec3 &n, float niOverNt)
{
    // vecIn must be normalized
    float cosTheta = glm::dot(-vecIn, n);

    // outgoing vector consist of a parallel and perpendicular part
    glm::vec3 outParallel = niOverNt * (vecIn + cosTheta * n);
    glm::vec3 outPerpendicular = -glm::sqrt(1.0f - glm::length2(outParallel)) * n;

    return outParallel + outPerpendicular;
}

// returns a random reflection referring to a Lambertian diffuse material
glm::vec3 randomUnitVector()
{
    auto a = randomDouble(0, 2*c_Pi);
    auto z = randomDouble(-1, 1);
    auto r = glm::sqrt(1-z*z);
    return glm::vec3(r*cos(a),r*sin(a),z);
}


class Material
{
public:
    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3& albedo) : _albedo(albedo) {}

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
    {
        glm::vec3 scatterDirection = rec.p + rec.n + randomUnitVector();
        scattered = Ray(rec.p, scatterDirection - rec.p);
        attenuation = _albedo;
        return true;
    }

private:
    glm::vec3 _albedo;
};

class Metal : public Material
{
public:
    Metal(const glm::vec3& albedo, float fuzz) : _albedo(albedo), _fuzz(fuzz < 1.0f ? fuzz : 1.0f) {}

    virtual bool scatter(const Ray& rayIn, const hitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
    {
        glm::vec3 reflectedDirection = reflect(glm::normalize(rayIn.direction()), rec.n);
        // Fuzziness as pertubation of the material. 0 = no pertubation
        scattered = Ray(rec.p, reflectedDirection + _fuzz*randomUnitVector());
        attenuation = _albedo;
        return (glm::dot(scattered.direction(), rec.n) > 0.0f);
    }

private:
    glm::vec3 _albedo;
    float _fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(float ri) : _refIdx(ri) {}

    virtual bool scatter(const Ray &rayIn, const hitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
    {
        attenuation = glm::vec3(1.0, 1.0, 1.0);
        float niOverNt = (rec.frontFace) ? (1.0f / _refIdx) : (_refIdx);

        glm::vec3 unitDirection = glm::normalize(rayIn.direction());
        float cosTheta = ffmin(glm::dot(-unitDirection, rec.n), 1.0f);
        double sinTheta = glm::sqrt(1.0f - cosTheta*cosTheta);

        if (niOverNt*sinTheta > 1.0f)
        {
            glm::vec3 reflected = reflect(unitDirection, rec.n);
            scattered = Ray(rec.p, reflected);
            return true;
        }

        if (randomDouble() < schlick(cosTheta, niOverNt))
        {
            glm::vec3 reflected = reflect(unitDirection, rec.n);
            scattered = Ray(rec.p, reflected);
            return true;
        }

        glm::vec3 refracted = refract(unitDirection, rec.n, niOverNt);
        scattered = Ray(rec.p, refracted);
        return true;
    }

private:
    float _refIdx;
};