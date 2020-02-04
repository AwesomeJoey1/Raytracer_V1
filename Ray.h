#pragma once
#include <glm/glm/glm.hpp>

class Ray {
public:
    Ray() {};
    Ray(const glm::vec3 &origin, const glm::vec3 &direction) : _origin(origin), _direction(direction) {}
    glm::vec3 origin() const { return _origin; }
    glm::vec3 direction() const { return _direction; }
    glm::vec3 pointAtParam(float t) const { return _origin + _direction * t; }

private:
    glm::vec3 _origin, _direction;
};

