#pragma once

#include "Common.h"

inline float trilinearInterpolation(glm::vec3 c[2][2][2], float u, float v, float w)
{
    auto uu = u*u * (3-2*u);
    auto vv = v*v * (3-2*v);
    auto ww = w*w * (3-2*w);

    auto accum = 0.0f;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                glm::vec3 weightV(u-i, v-j, w-k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         glm::dot(c[i][j][k], weightV);
            }
    return accum;
}

class Perlin
{
public:
    Perlin();
    ~Perlin();

    float noise(const glm::vec3& p) const;
    float turbulence(const glm::vec3& p, int depth=7) const;

private:
    static const int pointCount = 256;
    glm::vec3* _randVec;
    int* _permX;
    int* _permY;
    int* _permZ;

    static int* perlinGeneratePermutation();
    static void permute(int* p, int n);
};

Perlin::Perlin()
{
    _randVec = new glm::vec3[pointCount];
    for (int i = 0; i < pointCount; i++)
    {
        _randVec[i] = glm::normalize(randomVec3(-1, 1));
    }

    _permX = perlinGeneratePermutation();
    _permY = perlinGeneratePermutation();
    _permZ = perlinGeneratePermutation();
}

Perlin::~Perlin()
{
    delete[] _randVec;
    delete[] _permX;
    delete[] _permY;
    delete[] _permZ;
}

float Perlin::noise(const glm::vec3 &p) const
{
    auto u = p.x - glm::floor(p.x);
    auto v = p.y - glm::floor(p.y);
    auto w = p.z - glm::floor(p.z);

    auto i = static_cast<int>(glm::floor(p.x));
    auto j = static_cast<int>(glm::floor(p.y));
    auto k = static_cast<int>(glm::floor(p.z));
    glm::vec3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = _randVec[
                        _permX[(i+di) & 255] ^
                        _permY[(j+dj) & 255] ^
                        _permZ[(k+dk) & 255]];


    return trilinearInterpolation(c, u, v, w);
}

float Perlin::turbulence(const glm::vec3& p, int depth) const {
    auto accum = 0.0f;
    glm::vec3 tempP = p;
    float weight = 1.0f;

    for (int i = 0; i < depth; i++)
    {
        accum += weight*noise(tempP);
        weight *= 0.5;
        tempP *= 2;
    }

    return fabsf(accum);
}

int* Perlin::perlinGeneratePermutation()
{
    auto p = new int[pointCount];

    for (int i = 0; i < Perlin::pointCount; i++)
        p[i] = i;

    permute(p, pointCount);

    return p;
}

void Perlin::permute(int* p, int n)
{
    for (int i = n-1; i > 0; i--)
    {
        int target = randomInt(0, i);
        int temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}