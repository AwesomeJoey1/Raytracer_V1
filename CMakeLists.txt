cmake_minimum_required(VERSION 3.15)
project(Raytracer_V1)

set(CMAKE_CXX_STANDARD 14)

include_directories("$ENV{Libs}")

add_executable(Raytracer_V1 main.cpp Image.cpp Image.h Ray.h Hittable.h Sphere.h HittableList.h Camera.h Material.h Common.h MovingSphere.h AABB.h BVH.h Texture.h Perlin.h)
