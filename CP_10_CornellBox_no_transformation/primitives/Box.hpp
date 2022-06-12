//
// Created by star on 6/12/2022.
//

#ifndef ONE_WEEKEND_BOX_HPP
#define ONE_WEEKEND_BOX_HPP
#include "Core/Hittable.hpp"
#include "Core/Material.hpp"
#include "PrimRect.hpp"

class Box: public Hittable{
public:
    Box(Vec3 p0, Vec3 p1, MaterialPtr mat): minPos(p1), maxPos(p1), matPtr(mat){
        sides.add(make_shared<XY_Rect>(p0.x, p1.x, p0.y, p1.y, p1.z, mat)); // front
        sides.add(make_shared<XY_Rect>(p0.x, p1.x, p0.y, p1.y, p0.z, mat)); // back
        sides.add(make_shared<XZ_Rect>(p0.x, p1.x, p0.z, p1.z, p1.y, mat));        //  top
        sides.add(make_shared<XZ_Rect>(p0.x, p1.x, p0.z, p1.z, p0.y, mat));        // bottom
        sides.add(make_shared<YZ_Rect>(p0.y, p1.y, p0.z, p1.z, p1.x, mat)); // right
        sides.add(make_shared<YZ_Rect>(p0.y, p1.y, p0.z, p1.z, p0.x, mat)); // left
    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        return sides.hit(ray,t_min,t_max,rec);
    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override {
        return AABB{minPos, maxPos};
    }

public:
    Vec3 minPos;
    Vec3 maxPos;
    HittableList sides;
    MaterialPtr matPtr;
};





#endif //ONE_WEEKEND_BOX_HPP
