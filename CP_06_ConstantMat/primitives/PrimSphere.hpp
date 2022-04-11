//
// Created by star on 2/14/2022.
//

#ifndef ONE_WEEKEND_PRIMSPHERE_HPP
#define ONE_WEEKEND_PRIMSPHERE_HPP



#include "Core/Kernel.hpp"
#include "Core/Hittable.hpp"
#include "Core/Material.hpp"

inline void computeSphereUV(const Vec3 &p, double &u, double &v){
    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi;
    u = phi / (2*pi);
    v = theta / pi;
}


class Sphere : public Hittable {
public:
    Sphere() =default;
    Sphere(Vec3 cen, double r) : center(cen), radius(r) {};
    Sphere(Vec3 cen, double r, const MaterialPtr &ptr): center(cen), radius(r), matPtr(ptr){}
    inline void setMaterial(const MaterialPtr &ptr){ matPtr = ptr; }
    [[nodiscard]] inline auto getMaterial() const{return matPtr;}

    bool hit( const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
        Vec3 oc = r.origin() - center;
        auto a = r.d.x() * r.d.x() + r.d.y() * r.d.y()+ r.d.z() * r.d.z();
        auto half_b = dot(oc, r.direction());
        auto c = length_squared(oc) - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        computeSphereUV(outward_normal, rec.u , rec.v);
        rec.matPtr = matPtr;
        return true;
    }

    [[nodiscard]] AABB bbox(double t0, double t1) const override{
        AABB ret{};
        ret = AABB{center - Vec3{radius, radius, radius},
                    center + Vec3{radius,radius,radius}};
        return ret;
    }

public:
    Vec3 center;
    double radius{};
    MaterialPtr matPtr;
};

class MovingSphere: public Hittable{
public:
    Vec3 cen0; // position0 of time
    Vec3 cen1; // position1 of time
    double t0{0};
    double t1{1};
    double radius{};
    MaterialPtr matPtr;

    MovingSphere() =default;

    MovingSphere(Vec3 pos1, Vec3 pos2,
                 double time0,double time1,
                 double r,
                 const MaterialPtr &ptr): cen0{pos1},
                 cen1{pos2},t0{time0}, t1{time1}, radius(r), matPtr(ptr){
    }

    inline void setMaterial(const MaterialPtr &ptr){ matPtr = ptr; }
    [[nodiscard]] inline auto getMaterial() const{return matPtr;}


    auto center(double rayTime)const{
        return cen1 + (rayTime - t0)/ (t1-t0)  * (cen1- cen0);
    }


    bool hit( const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
        Vec3 oc = r.origin() - center(r.time());
        auto a = r.d.x() * r.d.x() + r.d.y() * r.d.y()+ r.d.z() * r.d.z();
        auto half_b = dot(oc, r.direction());
        auto c = length_squared(oc) - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center(r.time())) / radius;
        rec.set_face_normal(r, outward_normal);
        computeSphereUV(outward_normal, rec.u , rec.v);
        rec.matPtr = matPtr;
        return true;
    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override{

        AABB ret;
        AABB time0BBox{center(t0) - Vec3{radius, radius, radius},
                       center(t0)  + Vec3{radius,radius,radius}};
        AABB time1BBox{center(t1)  - Vec3{radius, radius, radius},
                       center(t1)  + Vec3{radius, radius, radius}};
        ret = surroundingBox(time0BBox, time1BBox);
        return ret;
    }

};



#endif //ONE_WEEKEND_PRIMSPHERE_HPP
