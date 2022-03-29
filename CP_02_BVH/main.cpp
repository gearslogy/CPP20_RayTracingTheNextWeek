#include <iostream>
#include "Core/Vec.hpp"
#include "Core/Ray.hpp"
#include "Core/Utils.hpp"
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Core/Kernel.hpp"
#include "Core/Hittable.hpp"
#include "primitives/PrimSphere.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/PerfectReflection.hpp"
#include <chrono>
#include "Core/parallel.h"
#include "Materials/Refract.hpp"
#include "Core/BVH.hpp"
Vec3 ray_color(const Ray& r, const HittableList & hitList, int depth) {
    if(depth < 0) return color_black;
    HitRecord rec{};

    if(hitList.hit(r,0.0001, infinity, rec)){
        Ray scattered;
        Color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, hitList, depth-1);
        return color_black;
    }

    Vec3 unit_direction = normalize(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}


inline HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color (0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0,-1000,0), 1000, ground_material));



    const int loopCount = 11;
    for (int a = -loopCount; a < loopCount; a++) {
        for (int b = -loopCount; b < loopCount; b++) {
            auto choose_mat = random_double();
            Vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ( auto temp =  center - Vec3(4, 0.2, 0); length(temp) > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vector<Vec3>() * random_vector<Vec3>();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0,.5), 0);
                    world.add(std::make_shared<MovingSphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));

                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vector<Color>(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<PerfectReflection>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }


    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<PerfectReflection>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    return world;
    //return HittableList(std::make_shared<BVH>(world, 0.0, 1.0));
}



int main() {

    const auto aspect_ratio = 16.0/9.0;
    Vec3 lookfrom(13,2,3);
    Vec3 lookat(0,0,0);

    auto dist_to_focus = length(lookfrom - lookat);
    std::cout << "focus:"<<dist_to_focus << std::endl;
    const auto shutterOpen = 0.0;
    const auto shutterClose = 1.0;
    Camera cam(lookfrom, lookat,0.1, 20, aspect_ratio, dist_to_focus, shutterOpen,shutterClose );
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const auto max_depth = 50;
    const auto max_samples = 50;


    auto world = random_scene();


    std::vector<char> imageData; // R/G/B array
    imageData.resize(image_width * image_height * 3);
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::function<void ()>> funcs;
    for (int j = image_height-1; j >= 0; --j){
        for (int i = 0; i < image_width; ++i){
            auto func = [i, j, image_width, image_height, &cam, &world, max_depth, max_samples, &imageData]() {
                Vec3 pixel_color{0, 0, 0};
                for (int s = 0; s < max_samples; s++) {
                    auto u = (double(i) + random_double()) / (image_width - 1);
                    auto v = 1 - (double(j) + random_double()) / (image_height - 1);    // left cornel is 0,0
                    auto ray = cam.getRay(u, v);
                    ray.d = normalize(ray.d);
                    pixel_color  += ray_color(ray, world, max_depth);
                }
                pixel_color /= max_samples;


                auto idx = j * image_width + i;
                auto idx_r = idx * 3 + 0; // R idx
                auto idx_g = idx * 3 + 1; // G idx
                auto idx_b = idx * 3 + 2; // B idx


                auto r = std::pow( pixel_color.x(),1.0/2.2);
                auto g = std::pow( pixel_color.y(),1.0/2.2);
                auto b = std::pow( pixel_color.z(),1.0/2.2);

                r = clamp(r,0,1);
                g = clamp(g,0,1);
                b = clamp(b,0,1);

                //std::cout <<"idx:::"<< idx <<"   "<<idx_r << " " << idx_g << " " << idx_b << std::endl;
                imageData[idx_r] = static_cast<char>(255.999 * r);
                imageData[idx_g] = static_cast<char>(255.999 * g);
                imageData[idx_b] = static_cast<char>(255.999 * b);
            };
            funcs.emplace_back(func);
        }
    }
    parallel_for_each_async_pool(funcs.begin(), funcs.end(), [](auto &func){func();});
    auto end = std::chrono::high_resolution_clock::now();
    auto dt =  std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "dt:" << dt << std::endl;



    stbi_write_jpg("image.jpg",image_width, image_height,3, imageData.data(),100);

    return 0;
}
