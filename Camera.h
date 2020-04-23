#pragma once
#include "Common.h"
#include "Ray.h"


glm::vec3 randomInUnitDisk()
{
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(randomDouble(), randomDouble(), 0) - glm::vec3(1,1,0);
    } while (glm::dot(p,p) >= 1.0); // Every sqrt bigger than 1 is also bigger than 1, so only check the square length.

    return p;
}

class Camera {
public:
    Camera(glm::vec3 camOrigin, glm::vec3 lookAt, glm::vec3 up,
            float vfov,
            float aspect, float aperture, float focusDist, float t0=0.0f, float t1=0.0f)
    {
        _lensRadius = aperture / 2;     // aperture defines the size of the lens
        _time0 = t0;                    // shutter open time
        _time1 = t1;                    // shutter close time
        float theta = vfov * c_Pi/180;  // vfov given as the vertical angle of the camera
        float halfHeight = tan(theta/2);
        float halfWidth = aspect * halfHeight;

        _origin = camOrigin;
        _w = glm::normalize(camOrigin - lookAt); // z axis
        _u = glm::normalize(glm::cross(up, _w));  // x axis
        _v = glm::cross(_w, _u);                   // y axis

        // because of orthonormal basis the left corner can be specified by simple vector math
        _leftCorner = _origin
                      - halfWidth * focusDist * _u
                      - halfHeight * focusDist * _v
                      - focusDist * _w;
        _horizontal = 2 * halfWidth * focusDist * _u;
        _vertical = 2 * halfHeight * focusDist * _v;
    }

    Ray getRay(float s, float t)
    {
        glm::vec3 rd = _lensRadius * randomInUnitDisk();
        glm::vec3 offset = _u * rd.x + _v * rd.y;
        return Ray(_origin + offset, _leftCorner + s*_horizontal + t*_vertical - _origin - offset,
                randomDouble(_time0, _time1));
    }

private:
    glm::vec3 _leftCorner, _horizontal, _vertical, _origin, _u, _v, _w;
    float _lensRadius;
    float _time0, _time1;
};
