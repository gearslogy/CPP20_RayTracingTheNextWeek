//
// Created by star on 4/17/2022.
//
#ifndef ONE_WEEKEND_PRIMRECT_HPP
#define ONE_WEEKEND_PRIMRECT_HPP
#include "Core/Hittable.hpp"
#include "Core/Material.hpp"
class XY_Rect: public Hittable{
public:
    XY_Rect(double x0, double x1 , double y0, double y1, double zPos, MaterialPtr matPtr):
            m_x0{x0},m_x1{x1}, m_y0{y0},m_y1{y1}, k{zPos}, mat{matPtr}{
    }

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        auto t = (k-ray.origin().z()) / ray.direction().z();
        if (t < t_min || t > t_max)
            return false;
        auto x = ray.origin().x() + t*ray.direction().x();
        auto y = ray.origin().y() + t*ray.direction().y();
        if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
            return false;
        rec.u = (x-m_x0)/(m_x1-m_x0);
        rec.v = (y-m_y0)/(m_y1-m_y0);
        rec.t = t;

        auto outward_normal = Vec3(0, 0, 1);
        rec.set_face_normal(ray, outward_normal);
        rec.matPtr = mat ;
        rec.p = ray.at(t);
        return true;

    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override {
        return AABB{Vec3{m_x0, m_y0, k - 0.0001}, Vec3{m_x1, m_y1, k + 0.0001}};
    }

    double m_x0;
    double m_x1;
    double m_y0;
    double m_y1;
    double k;
    MaterialPtr mat;
};





#endif //ONE_WEEKEND_PRIMRECT_HPP
