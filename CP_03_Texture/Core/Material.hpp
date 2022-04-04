//
// Created by star on 2/15/2022.
//

#ifndef ONE_WEEKEND_MATERIAL_HPP
#define ONE_WEEKEND_MATERIAL_HPP

#include "Kernel.hpp"
#include <memory>

class HitRecord;
class Material {
public:
    virtual bool scatter(const Ray& in, const HitRecord& rec, Vec3 & attenuation, Ray& scattered) const = 0;
};
using MaterialPtr = std::shared_ptr<Material>;



#endif //ONE_WEEKEND_MATERIAL_HPP
