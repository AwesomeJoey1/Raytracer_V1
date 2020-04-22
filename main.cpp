#include <iostream>
#include <float.h>
#include <time.h>

#include "Common.h"
#include "Camera.h"
#include "HittableList.h"
#include "Image.h"
#include "Material.h"
#include "Sphere.h"

// Calculates pixel color. Background is a rayColor gradient from skyblue to white.
glm::vec3 rayColor(const Ray& ray, const Hittable& world, int depth)
{
    hitRecord rec;

    if(depth <=0)
        return glm::vec3(0.0f,0.0f,0.0f);

    if (world.hit(ray, 0.001f, FLT_MAX, rec))
    {
        Ray scattered;
        glm::vec3 attenuation;
        if (rec.materialPtr->scatter(ray, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth - 1);
        return glm::vec3(0.0f,0.0f,0.0f);
    }

    glm::vec3 unit_direction = glm::normalize(ray.direction());
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

HittableList randomScene()
{
    HittableList world;
    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float chooseMat = randomDouble();
            glm::vec3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());
            if(glm::length(center-glm::vec3(4,0.2,0)) > 0.9) {
                if(chooseMat < 0.8)
                {
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(glm::vec3(randomDouble() * randomDouble(),
                                                                                                           randomDouble() * randomDouble(),
                                                                                                           randomDouble() * randomDouble()))));
                }
                else {
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(glm::vec3(0,1,0), 1.0, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(glm::vec3(-4,1,0), 1.0, std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1))));
    world.add(std::make_shared<Sphere>(glm::vec3(4,1,0), 1.0, std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main() {
    const int picX = 1200;
    const int picY = 800;
    const int pic_s = 10;
    const int maxDepth = 50;
    size_t t_start, t_stop, t_end;

    Image image(picX, picY);

    std::cout << "Creating scene...";
    t_start = clock();
    HittableList world;
    /*world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5, std::make_shared<Lambertian>(glm::vec3(0.1, 0.2, 0.5))));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100, std::make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.0))));
    world.add(std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5, std::make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 0.0)));
    world.add(std::make_shared<Sphere>(glm::vec3(-1,0,-1), 0.5, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(glm::vec3(-1,0,-1), -0.49, std::make_shared<Dielectric>(1.5)));*/
    world = randomScene();
    t_stop = clock();
    std::cout << "\t\t\t" << (t_stop - t_start) <<"ms\n";

    glm::vec3 camOrigin(13 ,2,3);
    glm::vec3 lookAt(0,0,0);
    float distToFocus = 10.0f;
    float aperture = 0.1f;
    Camera camera(camOrigin, lookAt, glm::vec3(0,1,0),20, float(picX) / float(picY), aperture, distToFocus);

    std::cout << "Rendering...\n";
    t_stop = clock();
    for (int j = 0; j < picY; j++) {
        std::cerr << "\rScanlines remaining: " << picY - j << ' ' << std::flush;
        for (int i = 0; i < picX; i++) {
            glm::vec3 col(0,0,0);
            // Anti-aliasing: multiple rays per pixel, then average
            for(int s = 0; s < pic_s; s++) {
                float u = float(i + randomDouble()) / float(picX);
                float v = float(j + randomDouble()) / float(picY);

                Ray ray = camera.getRay(u,v);
                col += rayColor(ray, world, maxDepth);
            }
            col /= pic_s;
            col = glm::sqrt(col); // Gamma 2 correction: (rayColor)^(1/gamma) equals taking the square root with gamma = 2


            int ir = int(255.99 * col.r);
            int ig = int(255.99 * col.g);
            int ib = int(255.99 * col.b);

            image.setPixel(i, j, glm::vec3(ir,ig,ib));
        }
    }
    t_stop = clock() - t_stop;

    std::cout << "Saving image...\n";
    image.save();
    t_end = clock();
    std::cout << "Rendering:\t\t"<< t_stop << "ms\n";
    std::cout << "Full program:\t\t"<< (t_end-t_start) << "ms\n";
    return 0;
}
