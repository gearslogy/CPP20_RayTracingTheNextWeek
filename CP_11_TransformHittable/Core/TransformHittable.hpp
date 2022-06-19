//
// Created by star on 6/12/2022.
//

#ifndef ONE_WEEKEND_TRANSFORMHITTABLE_H
#define ONE_WEEKEND_TRANSFORMHITTABLE_H

#include "Hittable.hpp"
class Hittable_Translate: public Hittable{
public:
    Hittable_Translate(const HittablePtr &geo, Vec3 move):ptr{geo}, offset{move}{}

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        Ray movedRay{ray.o- offset, ray.d, ray.time()}; // reverse transform ray!
        if (!ptr->hit(movedRay, t_min, t_max, rec)){
            return false;
        }
        rec.p += offset;
        rec.set_face_normal(movedRay, rec.normal);
        return true;

    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override {
        return AABB{ptr->bbox(time0,time1).minimum+offset, ptr->bbox(time0,time1).maximum+offset};
    }

    HittablePtr ptr;
    Vec3 offset;
};

class Hittable_Rotate_X: public Hittable{
public:
    Hittable_Rotate_X(const HittablePtr &geo, float angle ): ptr{geo},rotateAngle{angle}{

    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        return false;
    }

    AABB bbox(double time0, double time1) const override {
        return calBox;
    }

    HittablePtr ptr;
    float rotateAngle{0};
    AABB calBox;
};


class Hittable_Rotate_Y: public Hittable{
public:
    Hittable_Rotate_Y(const HittablePtr &geo, float angle ): ptr{geo}, rad{ glm::radians(angle)}{
        auto box = ptr->bbox(0,1);
        auto boxMin = box.min();
        auto boxMax = box.max();

        rotMatrix = glm::rotate(rotMatrix, rad, Vec3{0,1,0});
        Vec3 rotBoxMin = rotMatrix *  glm::vec4{rotBoxMin, 1.0f};
        Vec3 rotBoxMax = rotMatrix *  glm::vec4{rotBoxMax, 1.0f};

        auto newBoxMin = glm::min(boxMin, rotBoxMin);
        auto newBoxMax = glm::min(boxMax, rotBoxMax);
        calBox = AABB{newBoxMin, newBoxMax};
    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        // reverse rotate ray
        auto origin = ray.origin();
        auto dir = ray.direction();

        auto inverseRotMatrix = glm::inverse(rotMatrix);
        origin = inverseRotMatrix * glm::vec4{origin, 1.0f};
        dir = inverseRotMatrix * glm::vec4{dir, 1.0f};

        Ray rotated_r{origin, dir, ray.time()};
        if(!ptr->hit(rotated_r, t_min, t_max, rec))
            return false;

        auto p = rec.p;
        auto normal = rec.normal;
        rec.p = rotMatrix * glm::vec4{p,1.0f};
        rec.normal = rotMatrix * glm::vec4{normal,1.0f};
        rec.set_face_normal(rotated_r, normal);
        return true;
    }

    AABB bbox(double time0, double time1) const override {
        return calBox;
    }

    HittablePtr ptr;
    float rad{0};
    AABB calBox;
    glm::mat4 rotMatrix{1.0f};
};

class Hittable_Rotate_Z: public Hittable{
public:
    Hittable_Rotate_Z(const HittablePtr &geo, float angle ): ptr{geo},rotateAngle{angle}{

    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        return false;
    }

    AABB bbox(double time0, double time1) const override {
        return calBox;
    }

    HittablePtr ptr;
    float rotateAngle{0};
    AABB calBox;


};




#endif //ONE_WEEKEND_TRANSFORMHITTABLE_H
