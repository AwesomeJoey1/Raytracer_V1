#pragma once
#include <glm/glm/glm.hpp>

class Ray {
public:
    Ray() {};
    Ray(const glm::vec3 &origin, const glm::vec3 &direction, float time=0.0f) : _origin(origin), _direction(direction), _time(time) {}
    glm::vec3 origin() const { return _origin; }
    glm::vec3 direction() const { return _direction; }
    float time() const { return _time; }
    glm::vec3 pointAtParam(float t) const { return _origin + _direction * t; }

private:
    glm::vec3 _origin, _direction;
    float _time;
};

