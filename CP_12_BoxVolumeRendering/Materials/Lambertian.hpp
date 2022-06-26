//
// Created by star on 2/15/2022.
//

#ifndef ONE_WEEKEND_LAMBERTIAN_H
#define ONE_WEEKEND_LAMBERTIAN_H

#include "Core/Material.hpp"
#include "Core/Hittable.hpp"
#include "Core/Sampler.hpp"
#include "Core/Texture.hpp"

class Lambertian: public Material{
public:
    explicit Lambertian(const Vec3 diffuse): diffuseColorTex{ std::make_shared<SolidColorTex>(diffuse) } {}
    explicit Lambertian(TexturePtr tex): diffuseColorTex{tex}{}
    bool scatter(const Ray& in, const HitRecord& rec, Vec3 & attenuation, Ray& scattered) const override{
        auto scatterDirection =    rec.normal + SphereScatter<Vec3>::surface();
        //auto scatterDirection =   HemisphereScatter<Vec3>::surface(rec.normal);

        // Catch degenerate scatter direction
        if (isZeroVector(scatterDirection))
            scatterDirection = rec.normal;
        scatterDirection = normalize(scatterDirection);
        scattered = Ray{rec.p + 0.00001* rec.normal  , scatterDirection, in.time()};
        attenuation = diffuseColorTex->value(rec.u,rec.v,rec.p);
        return true;
    }
private:
    TexturePtr diffuseColorTex;
};




#endif //ONE_WEEKEND_LAMBERTIAN_H
