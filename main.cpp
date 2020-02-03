#include <iostream>
#include "float.h"
#include <glm/glm/gtx/norm.hpp>

#include "HittableList.h"
#include "Sphere.h"
#include "Image.h"
#include "Camera.h"
#include "Random.h"

// Searches a point in the unit sphere, by rejecting all points which squared length of the origin vector exceeds 1.0
glm::vec3 randomInUnitSphere()
{
    glm::vec3 p;
    do {
        p = glm::vec3(randomDoubleCPP(), randomDoubleCPP(), randomDoubleCPP());
        p = 2.0f * p - glm::vec3(1,1,1);
    } while(glm::length2(p) >= 1.0f);
    return p;
}

// Calculates pixel color. Background is a color gradient from skyblue to white.
glm::vec3 color(const Ray &ray, Hittable *world, int depth)
{
    hitRecord rec;
    if (world->hit(ray, 0.001f, FLT_MAX, rec))
    {
        if (depth < 5000)
        {
        glm::vec3 target = rec.p + rec.n + randomInUnitSphere();
        return 0.5f*color(Ray(rec.p, target - rec.p), world, depth+1);
        }
        else {
            return glm::vec3(0,0,0);
        }
    } else
    {
        glm::vec3 unit_direction = glm::normalize(ray.direction());
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

int main() {
    const int pic_x = 2000;
    const int pic_y = 1000;
    const int pic_s = 10;

    Image image(pic_x, pic_y);

    Hittable *list[2];
    list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5);
    list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100);
    Hittable *world = new HittableList(list, 2);
    Camera camera;

    for (int j = 0; j < pic_y; j++) {
        for (int i = 0; i < pic_x; i++) {
            glm::vec3 col(0,0,0);
            // Anti-aliasing: multiple rays per pixel, then average
            for(int s = 0; s < pic_s; s++) {
                float u = float(i + randomDoubleC()) / float(pic_x);
                float v = float(j + randomDoubleC()) / float(pic_y);

                Ray ray = camera.getRay(u,v);
                col += color(ray, world, 0);
            }
            col /= pic_s;
            col = glm::sqrt(col); // Gamma 2 correction: (color)^(1/gamma) equals taking the square root with gamma = 2


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
