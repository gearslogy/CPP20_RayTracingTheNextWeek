//
// Created by star on 6/26/2022.
//

#ifndef ONE_WEEKEND_ISOTROPIC_HPP
#define ONE_WEEKEND_ISOTROPIC_HPP
#include "Core/Material.hpp"
#include "Core/Sampler.hpp"
#include "Core/Texture.hpp"
#include "Core/Hittable.hpp"
class Isotropic: public Material{
public:
    Isotropic(Vec3 color): albedo{std::make_shared<SolidColorTex>(color)}{}


    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        scattered = Ray{rec.p , SphereScatter<Vec3>::interior(), in.time()};
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    std::shared_ptr<Texture> albedo;
};







#endif //ONE_WEEKEND_ISOTROPIC_HPP
