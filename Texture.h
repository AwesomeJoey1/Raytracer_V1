#pragma once

#include "Common.h"
#include "Perlin.h"

class Texture
{
public:
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() {}
    ConstantTexture(glm::vec3 color) : _color(color) {}

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const { return _color; }

private:
    glm::vec3 _color;
};

class CheckerTexture: public Texture
{
public:
    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture> tex0, std::shared_ptr<Texture> tex1) : _odd(tex0), _even(tex1) {}

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const
    {
        auto sines = glm::sin(10*p.x)*glm::sin(10*p.y)*glm::sin(10*p.z);
        if (sines < 0)
        {
            return _odd->value(u, v, p);
        } else {
            return _even->value(u, v, p);
        }
    }

private:
    std::shared_ptr<Texture> _odd;
    std::shared_ptr<Texture> _even;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(float scale) : _scale(scale) {}

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const
    {
        return glm::vec3(1) * 0.5f * (1.0f + glm::sin(_scale*p.z + 10*perlinNoise.turbulence(p)));
    }

private:
    Perlin perlinNoise;
    float _scale;
};