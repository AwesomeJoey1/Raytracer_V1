#include <float.h>
#include <time.h>

#include "AARect.h"
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
glm::vec3 rayColor(const Ray& ray, const Hittable& world, int backgroundTheme, int depth)
{
    hitRecord rec;

    if(depth <= 0)
        return glm::vec3(0);

    if (!world.hit(ray, 0.001f, FLT_MAX, rec))
        return background(ray.direction(), backgroundTheme);

    Ray scattered;
    glm::vec3 attenuation;
    glm::vec3 emitted = rec.materialPtr->emitted(rec.u, rec.v, rec.p);
    if (!rec.materialPtr->scatter(ray, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * rayColor(scattered, world, backgroundTheme, depth - 1);

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

HittableList simpleLight()
{
    HittableList objects;

    auto perlinTex = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, std::make_shared<Lambertian>(perlinTex)));
    objects.add(std::make_shared<Sphere>(glm::vec3(0,2,0), 2, std::make_shared<Lambertian>(perlinTex)));

    auto diffLight = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(4,4, 4)));
    objects.add(std::make_shared<xyRect>(3, 5, 1, 3, -2, diffLight));
    objects.add(std::make_shared<Sphere>(glm::vec3(0, 7, 0), 2, diffLight));

    return objects;
}

HittableList cornellBox()
{
    HittableList objects;

    auto red = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.65f, 0.05f, 0.05f)));
    auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f)));
    auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.12f, 0.45f, 0.15f)));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(5)));

    objects.add(std::make_shared<FlipFace>(std::make_shared<yzRect>(0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<yzRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xzRect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<xzRect>(0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<xzRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<xyRect>(0, 555, 0, 555, 555, white)));

    return objects;
}

int main() {
    const int picX = 600;
    const int picY = 600;
    const int picS = 100;
    const int maxDepth = 50;
    const auto aspectRatio = float(picX) / float(picY);
    size_t t_start, t_stop, t_end;

    Image image(picX, picY);

    std::cout << "Creating scene...";
    t_start = clock();
    HittableList world;
    int backgroundTheme;
    t_stop = clock();


    glm::vec3 camOrigin, lookAt;
    float vfov = 40.0f;
    float distToFocus = 10.0f;
    float aperture = 0.1f;
    switch (0)
    {
        case 1:

            world = randomScene();
            backgroundTheme = 1;
            camOrigin = glm::vec3(13 ,2,3);
            lookAt = glm::vec3(0);
            vfov = 20.0f;
            break;
        case 2:
            world = twoPerlinSpheres();
            backgroundTheme = 1;
            camOrigin = glm::vec3(13 ,2,3);
            lookAt = glm::vec3(0);
            vfov = 20.0f;
            break;

        case 3:
            world = earth();
            backgroundTheme = 1;
            camOrigin = glm::vec3(0,0,12);
            lookAt = glm::vec3(0,0,0);
            vfov = 20.0f;
            break;

        case 4:
            world = simpleLight();
            backgroundTheme = 0;
            camOrigin = glm::vec3(26 ,3,6);
            lookAt = glm::vec3(0,2,0);
            vfov = 20.0f;
            distToFocus = 1;
            aperture = 0;
            break;
        default:
         case 5:
            world = cornellBox();
            backgroundTheme = 0;
            camOrigin = glm::vec3(278, 278, -800);
            lookAt = glm::vec3 (278, 278, 0);
            distToFocus = 10.0f;
            aperture = 0.0;
    }
    std::cout << "\t\t\t" << (t_stop - t_start) <<"ms\n";
    if (backgroundTheme < 0 || backgroundTheme > 1) {
        std::cerr << "BACKGROUND ERROR: Theme " << backgroundTheme << " not found!\n";
        exit(1);
    }

    Camera camera(camOrigin, lookAt, glm::vec3(0,1,0),vfov, aspectRatio, aperture, distToFocus, 0.0f, 1.0f);

    std::cout << "Rendering...\n";
    t_stop = clock();
    for (int j = 0; j < picY; j++) {
        std::cerr << "\rScanlines remaining: " << picY - j << ' ' << std::flush;
        for (int i = 0; i < picX; i++) {
            glm::vec3 col(0,0,0);
            // Anti-aliasing: multiple rays per pixel, then average
            for(int s = 0; s < picS; s++) {
                float u = float(i + randomDouble()) / float(picX);
                float v = float(j + randomDouble()) / float(picY);

                Ray ray = camera.getRay(u,v);
                col += rayColor(ray, world, backgroundTheme, maxDepth);
            }
            col /= picS;
            col = glm::sqrt(col); // Gamma 2 correction: (rayColor)^(1/gamma) equals taking the square root with gamma = 2


            int ir = static_cast<int>(256 * clamp(col.r, 0.0f, 0.9999f));
            int ig = static_cast<int>(256 * clamp(col.g, 0.0f, 0.9999f));
            int ib = static_cast<int>(256 * clamp(col.b, 0.0f, 0.9999f));

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
