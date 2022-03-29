//
// Created by star on 2/15/2022.
//

#ifndef ONE_WEEKEND_LAMBERTIAN_H
#define ONE_WEEKEND_LAMBERTIAN_H

#include "Core/Material.hpp"
#include "Core/Hittable.hpp"
#include "Core/Sampler.hpp"
class Lambertian: public Material{
public:
    explicit Lambertian(const Vec3 diffuse): diffuseColor(diffuse){}

    bool scatter(const Ray& in, const HitRecord& rec, Vec3 & attenuation, Ray& scattered) const override{
        auto scatterDirection = rec.normal + HemisphereScatter<Vec3>::surface(rec.normal);
        // Catch degenerate scatter direction
        if (isZeroVector(scatterDirection))
            scatterDirection = rec.normal;
        scatterDirection = normalize(scatterDirection);
        scattered = Ray{rec.p, scatterDirection, in.time()};
        attenuation = diffuseColor;
        return true;
    }
private:
    Vec3 diffuseColor;
};




#endif //ONE_WEEKEND_LAMBERTIAN_H
