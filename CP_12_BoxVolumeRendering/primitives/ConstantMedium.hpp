//
// Created by star on 6/24/2022.
//

#ifndef ONE_WEEKEND_CONSTANTMEDIUM_H
#define ONE_WEEKEND_CONSTANTMEDIUM_H

#include "Core/Hittable.hpp"
#include "Core/Material.hpp"
#include "Core/GLM_Operators.hpp"
class ConstantMedium: public Hittable{
public:
    ConstantMedium(const HittablePtr& bound, MaterialPtr mat, double density):boundary{bound}, phase_function{mat},constantDensity{density}    {
    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {

        const bool enableDebug = true;
        const bool debugging = enableDebug && random_double() < 0.00001;
        HitRecord rec1;
        HitRecord rec2;

        if(!boundary->hit(ray, -infinity, infinity, rec1))
            return false;

        if(!boundary->hit(ray, rec1.t + 0.0001, infinity, rec2)){
            return false;
        }

        if (rec1.t < t_min) rec1.t = t_min;
        if (rec2.t > t_max) rec2.t = t_max;

        if(rec1.t >= rec2.t) return false;
        if (rec1.t < 0)
            rec1.t = 0;

        auto rayInsideDistance = rec2.t - rec1.t;
        auto hitStepDistance =  - 1 / constantDensity * std::log(random_double()) ;


        if (hitStepDistance > rayInsideDistance)
            return false;

        if(debugging){
            //std::cout << "rayInsideDistance:" << rayInsideDistance << " hitStepDistance:" << hitStepDistance << std::endl;
        }

        rec.t = rec1.t + hitStepDistance ;
        rec.p = ray.at(rec.t);
        rec.normal = Vec3{1,0,0};  // arbitrary
        rec.front_face = true;     // also arbitrary
        rec.matPtr = phase_function;
        return true;
    }

    AABB bbox(double time0, double time1) const override {
        return boundary->bbox(time0, time1);
    }


    MaterialPtr phase_function;

private:
    double constantDensity;
    HittablePtr boundary;
};










#endif //ONE_WEEKEND_CONSTANTMEDIUM_H
