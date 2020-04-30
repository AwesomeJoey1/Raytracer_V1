#include <float.h>
#include <time.h>

#include "BVH.h"
#include "Common.h"
#include "Camera.h"
#include "HittableList.h"
#include "Image.h"
#include "Material.h"
#include "MovingSphere.h"
#include "Sphere.h"
#include "Texture.h"


glm::vec3 background(const glm::vec3& rayDir, int backgroundTheme)
{
    glm::vec3 background(0);
    switch(backgroundTheme)
    {
        case 0:
            break;
        case 1:
            glm::vec3 unit_direction = glm::normalize(rayDir);
            float t = 0.5f * (unit_direction.y + 1.0f);
            background = (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
            break;
    }
    return background;
}

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

    return background(ray.direction(), 1);
}

HittableList randomScene()
{
    HittableList world;
    auto checkerTex = std::make_shared<CheckerTexture>(
            std::make_shared<ConstantTexture>(glm::vec3(0.2, 0.3, 0.1)),
            std::make_shared<ConstantTexture>(glm::vec3(0.9, 0.9, 0.9)));

    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000,
            std::make_shared<Lambertian>(checkerTex)));
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
                    world.add(std::make_shared<MovingSphere>(center, center + glm::vec3(0, randomDouble(0.0, 0.5), 0), 0.0f, 1.0f,  0.2,
                            std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(randomVec3()))));
                }
                else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = glm::vec3(randomDouble(.5, 1), randomDouble(.5, 1), randomDouble(.5, 1));
                    auto fuzz = randomDouble(0, .5);
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else {
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(glm::vec3(0,1,0), 1.0,
            std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(glm::vec3(-4,1,0), 1.0,
            std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.4, 0.2, 0.1)))));
    world.add(std::make_shared<Sphere>(glm::vec3(4,1,0), 1.0,
            std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0)));

    return HittableList(std::make_shared<BVHNode>(world, 0.0, 1.0));
}

HittableList twoPerlinSpheres()
{
    HittableList objects;

    auto perlinTex = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000,
                                         std::make_shared<Lambertian>(perlinTex)));

    objects.add(std::make_shared<Sphere>(glm::vec3(0, 2, 0), 2,
                                         std::make_shared<Lambertian>(perlinTex)));

    return HittableList(std::make_shared<BVHNode>(objects, 0.0, 1.0));
}

HittableList earth()
{
    HittableList objects;

    auto earthPic = std::make_shared<Image>("../Textures/earthmap.jpg");
    auto earthSurface = std::make_shared<Lambertian>(std::make_shared<ImageTexture>(earthPic));
    objects.add(std::make_shared<Sphere>(glm::vec3(0,0,0), 2, earthSurface));

    return objects;
}

int main() {
    const int picX = 1200;
    const int picY = 600;
    const int pic_s = 100;
    const int maxDepth = 50;
    const auto aspectRatio = float(picX) / float(picY);
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
    t_stop = clock();


    glm::vec3 camOrigin, lookAt;
    float vfov = 40.0f;
    float distToFocus = 10.0f;
    float aperture = 0.1f;
    int mode = 2;
    switch (mode)
    {
        case 0:
            world = randomScene();
            camOrigin = glm::vec3(13 ,2,3);
            lookAt = glm::vec3(0);
            vfov = 20.0f;
            break;
        case 1:
            world = twoPerlinSpheres();
            camOrigin = glm::vec3(13 ,2,3);
            lookAt = glm::vec3(0);
            vfov = 20.0f;
            break;
        case 2:
            world = earth();
            camOrigin = glm::vec3(0,0,12);
            lookAt = glm::vec3(0,0,0);
            vfov = 20.0f;
            break;
    }
    std::cout << "\t\t\t" << (t_stop - t_start) <<"ms\n";


    Camera camera(camOrigin, lookAt, glm::vec3(0,1,0),vfov, aspectRatio, aperture, distToFocus, 0.0f, 1.0f);

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
