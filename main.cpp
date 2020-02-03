#include <iostream>
#include "float.h"

#include "HittableList.h"
#include "Sphere.h"
#include "Image.h"
#include "Camera.h"
#include "Random.h"

// background color that is the linear interpolation depending on the y value of the direction of the ray
glm::vec3 color(Ray &ray, Hittable *world)
{
    hitRecord rec;
    if (world->hit(ray, 0.001f, FLT_MAX, rec))
    {
        return 0.5f*glm::vec3(rec.n.x + 1, rec.n.y + 1, rec.n.z + 1);
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
    const int pic_s = 100;

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
                col += color(ray, world);
            }
            col /= pic_s;


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
