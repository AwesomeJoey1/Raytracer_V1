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

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const override { return _color; }

private:
    glm::vec3 _color;
};

class CheckerTexture: public Texture
{
public:
    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture> tex0, std::shared_ptr<Texture> tex1) : _odd(tex0), _even(tex1) {}

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const override
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

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        return glm::vec3(1) * 0.5f * (1.0f + glm::sin(_scale*p.z + 10*perlinNoise.turbulence(p)));
    }

private:
    Perlin perlinNoise;
    float _scale;
};

class ImageTexture : public Texture {
public:
    ImageTexture() {}
    ImageTexture(std::shared_ptr<Image> img) : _image(img) {}

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        // if not texture data is available render cyan as debbuging aid
        if (!_image->hasData())
        {
            return glm::vec3(0,1,1);
        }

        auto i = static_cast<int>(u*_image->width());
        auto j = static_cast<int>((1-v)*_image->height()-0.0001);

        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > _image->width()-1) i = _image->width()-1;
        if (j > _image->height()-1) j = _image->height()-1;

        return (_image->getPixel(i,j) / 255.0f);
    }

private:
    std::shared_ptr<Image> _image;
};