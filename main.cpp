#include <iostream>

#include "Ray.h"
#include "Image.h"


float hitSphere(const glm::vec3 & center, float radius, const Ray &ray)
{
    glm::vec3 oc = ray.origin() - center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2.0f * glm::dot(ray.direction(), oc);
    float c = glm::dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0)
    {
        return -1.0f;
    } else{
        return (-b - glm::sqrt(discriminant)) / (2.0f * a);
    }
}

// background color that is the linear interpolation depending on the y value of the direction of the ray
glm::vec3 color(Ray ray)
{
    float t = hitSphere(glm::vec3(0,0,-1), 0.5, ray);
    if (t > 0.0f)
    {
        glm::vec3 normal = glm::normalize(ray.pointAtParam(t) - glm::vec3(0,0,-1));
        return 0.5f*glm::vec3(normal.x + 1, normal.y + 1, normal.z + 1);
    }

    glm::vec3 unit_direction = glm::normalize(ray.direction());
    t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main() {
    const int pic_x = 2000;
    const int pic_y = 1000;

    Image image(pic_x, pic_y);

    glm::vec3 lower_left(-2.0, -1.0, -1.0);
    glm::vec3 horizontal(4.0, 0.0, 0.0);
    glm::vec3 vertical(0.0, 2.0, 0.0);
    glm::vec3 origin(0.0, 0.0, 0.0);
    for (int j = 0; j < pic_y; j++) {
        for (int i = 0; i < pic_x; i++) {
            float u = float(i) / float(pic_x);
            float v = float(j) / float(pic_y);

            Ray ray(origin, lower_left + u*horizontal + v*vertical);
            glm::vec3 col = color(ray);

            int ir = int(255.99 * col.r);
            int ig = int(255.99 * col.g);
            int ib = int(255.99 * col.b);

            image.setPixel(i, j, glm::vec3(ir,ig,ib));
        }
    }

    std::cout << "Hello, World!" << std::endl;
    image.save();
    return 0;
}
