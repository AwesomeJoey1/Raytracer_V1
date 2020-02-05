#include <iostream>
#include "float.h"

#include "Camera.h"
#include "HittableList.h"
#include "Image.h"
#include "Material.h"
#include "Random.h"
#include "Sphere.h"

// Calculates pixel color. Background is a color gradient from skyblue to white.
glm::vec3 color(const Ray &ray, Hittable *world, int depth)
{
    hitRecord rec;
    if (world->hit(ray, 0.001f, FLT_MAX, rec))
    {
        Ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && rec.materialPtr->scatter(ray, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth+1);
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
    const int picX = 2000;
    const int picY = 1000;
    const int pic_s = 10;

    Image image(picX, picY);

    Hittable *list[4];
    /*list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5, new Lambertian(glm::vec3(0.1, 0.2, 0.5)));
    list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100, new Lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(glm::vec3(1, 0, -1), 0.5, new Metal(glm::vec3(0.8, 0.6, 0.2), 0.0));
    //list[3] = new Sphere(glm::vec3(-1,0,-1), 0.5, new Dielectric(1.5));
    list[3] = new Sphere(glm::vec3(-1,0,-1), 0.5, new Dielectric(-0.45));*/
    float R = cos(M_PI/4);
    list[0] = new Sphere(glm::vec3(-R,0,-1), R, new Lambertian(glm::vec3(0,0,1)));
    list[1] = new Sphere(glm::vec3(R,0,-1), R, new Lambertian(glm::vec3(1,0,0)));
    Hittable *world = new HittableList(list, 2);
    Camera camera(90, float(picX) / float(picY));

    for (int j = 0; j < picY; j++) {
        for (int i = 0; i < picX; i++) {
            glm::vec3 col(0,0,0);
            // Anti-aliasing: multiple rays per pixel, then average
            for(int s = 0; s < pic_s; s++) {
                float u = float(i + randomDoubleC()) / float(picX);
                float v = float(j + randomDoubleC()) / float(picY);

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
