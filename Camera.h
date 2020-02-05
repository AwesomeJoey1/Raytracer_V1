#pragma once

#include "Ray.h"

class Camera {
public:
    Camera(float vfov, float aspect)
    {
        //vfoc
        float theta = vfov * M_PI/180;
        float halfHeight = tan(theta/2);
        float halfWidth = aspect * halfHeight;
        leftCorner = glm::vec3(-halfWidth, -halfHeight, -1.0);
        horizontal = glm::vec3(2 * halfWidth, 0.0, 0.0);
        vertical = glm::vec3(0.0, 2 * halfHeight, 0.0);
        origin = glm::vec3(0.0, 0.0, 0.0);
    }

    Ray getRay(float u, float v)
    {
        return Ray(origin, leftCorner + u*horizontal + v*vertical - origin);
    }

private:
    glm::vec3 leftCorner, horizontal, vertical, origin;
};
