//
// Created by star on 2/17/2022.
//

#ifndef ONE_WEEKEND_REFRACT_HPP
#define ONE_WEEKEND_REFRACT_HPP

#include "Core/Material.hpp"
#include "Core/Hittable.hpp"
#include "Core/Sampler.hpp"

class Dielectric: public Material{
public:
    explicit Dielectric(double eta) :ior{eta}{}
    Dielectric(double outIOR, double inIOR ): ior{outIOR/inIOR}{}
   
    bool scatter(const Ray& in, const HitRecord& rec, Vec3 & attenuation, Ray& scattered) const override{
        attenuation = Color{1.0, 1.0, 1.0};
        double refraction_ratio = rec.front_face ? (1.0/ior) : ior;
        auto unit_direction = normalize(in.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction{0,0,0};
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = glm::reflect(unit_direction, rec.normal);
        else
            direction = glm::refract(unit_direction, rec.normal, float(refraction_ratio));

        scattered = {rec.p, direction, in.time()};
        return true;
    }

private:
    double ior{1.0/1.3};
private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};







#endif //ONE_WEEKEND_REFRACT_HPP
