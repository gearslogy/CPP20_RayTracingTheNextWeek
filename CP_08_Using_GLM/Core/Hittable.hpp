//
// Created by star on 2/14/2022.
//

#ifndef ONE_WEEKEND_HITTABLE_HPP
#define ONE_WEEKEND_HITTABLE_HPP
#include "Kernel.hpp"
#include <memory>
#include <optional>

class Material;
struct HitRecord{
    Vec3 p{0,0,0};
    Vec3 normal{0,0,0};
    double t{0.0};
    bool front_face{true};
    double u;
    double v;
    std::shared_ptr<Material> matPtr;
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};


class Hittable{
public:
    [[nodiscard]] virtual bool hit(const Ray &ray , double t_min, double t_max, HitRecord & rec) const = 0;
    [[nodiscard]] virtual AABB bbox(double time0, double time1) const = 0;
};

using HittablePtr = std::shared_ptr<Hittable>;


// HittableList
class HittableList : public Hittable {
public:
    HittableList() = default;
    explicit HittableList(HittablePtr object) { add( std::move(object)); }
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.emplace_back(std::move(object)); }
    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    AABB bbox(double t0, double t1) const override;
public:
    std::vector<HittablePtr> objects;
};


inline bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
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

inline AABB HittableList::bbox(double t0, double t1) const {
    AABB ret;
    if(objects.empty()) return ret;
    AABB temp;
    bool firstBox = true;
    for(const auto &obj : objects){
        auto perBbox = obj->bbox(t0, t1);
        ret = firstBox? temp : surroundingBox(ret, temp ) ;
        firstBox = false;
    }
    return ret;
}



#endif //ONE_WEEKEND_HITTABLE_HPP
