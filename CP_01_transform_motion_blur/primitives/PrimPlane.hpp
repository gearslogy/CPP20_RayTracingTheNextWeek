//
// Created by star on 2/26/2022.
//

#ifndef ONE_WEEKEND_PRIMPLANE_H
#define ONE_WEEKEND_PRIMPLANE_H

#include "Core/Kernel.hpp"
#include "Core/Hittable.hpp"
#include "Core/Material.hpp"


class Plane: public Hittable{
public:
    Plane(Vec3 center, Vec3 normal, MaterialPtr mat) : cen{center}, N{ normalize(normal)}, matPtr{mat} {}

    bool hit( const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
        double t =  dot(cen - r.origin(), N) / dot(normalize(r.d), N);
        if( t> t_min && t < t_max ) {
            rec.matPtr = matPtr;
            rec.normal = N;
            rec.p = r.o + t * r.d;
            return true;
        }
        return false;
    }
private:
    Vec3 cen;
    Vec3 N;
    MaterialPtr matPtr;
};


#endif //ONE_WEEKEND_PRIMPLANE_H
