#pragma once

#include "Ray.h"

class Camera {
public:
    Camera()
    {
        leftCorner = glm::vec3(-2.0, -1.0, -1.0);
        horizontal = glm::vec3(4.0, 0.0, 0.0);
        vertical = glm::vec3(0.0, 2.0, 0.0);
        origin = glm::vec3(0.0, 0.0, 0.0);
    }

    Ray getRay(float u, float v)
    {
        return Ray(origin, leftCorner + u*horizontal + v*vertical - origin);
    }

private:
    glm::vec3 leftCorner, horizontal, vertical, origin;
};
