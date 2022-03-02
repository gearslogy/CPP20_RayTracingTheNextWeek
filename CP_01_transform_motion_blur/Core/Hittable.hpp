//
// Created by star on 2/14/2022.
//

#ifndef ONE_WEEKEND_HITTABLE_HPP
#define ONE_WEEKEND_HITTABLE_HPP
#include "Kernel.hpp"
#include <memory>


class Material;
struct HitRecord{
    Vec3 p{0,0,0};
    Vec3 normal{0,0,0};
    double t{0.0};
    bool front_face{true};
    std::shared_ptr<Material> matPtr;
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};


class Hittable{
public:
    virtual bool hit(const Ray &ray , double t_min, double t_max, HitRecord & rec) const = 0;
};


// HittableList
class HittableList : public Hittable {
public:
    HittableList() = default;
    explicit HittableList(std::shared_ptr<Hittable> object) { add( std::move(object)); }
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.emplace_back(std::move(object)); }
    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
public:
    std::vector<std::shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}


#endif //ONE_WEEKEND_HITTABLE_HPP
