//
// Created by star on 2/17/2022.
//

#ifndef ONE_WEEKEND_PERFECTREFLECTION_H
#define ONE_WEEKEND_PERFECTREFLECTION_H
#include "Core/Material.hpp"
#include "Core/Hittable.hpp"
#include "Core/Sampler.hpp"
class PerfectReflection : public Material{
public:
    explicit PerfectReflection(const Color& a) : albedo{a} {}
    PerfectReflection(const Color &a, float glossyAmount): albedo{a} , glossy{glossyAmount}{}
    bool scatter(const Ray& in, const HitRecord& rec, Vec3 & attenuation, Ray& scattered) const override{
        auto v = normalize(in.d);
        auto r = reflect(v, rec.normal);
        r += glossy * SphereScatter<Vec3>::surface();
        r = normalize(r);
        scattered = {rec.p, r, in.time()};
        attenuation = albedo;
        return dot(rec.normal, r) > 0;
    }
private:
    Color albedo{0,0,0};
    float glossy{0.0f};
};






#endif //ONE_WEEKEND_PERFECTREFLECTION_H
