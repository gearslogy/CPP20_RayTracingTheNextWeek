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
        auto t = (k-ray.origin().z) / ray.direction().z;
        if (t < t_min || t > t_max)
            return false;
        auto x = ray.origin().x + t*ray.direction().x;
        auto y = ray.origin().y + t*ray.direction().y;
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



class YZ_Rect: public Hittable{
public:
    YZ_Rect(double y0, double y1, double z0, double z1, double xPos,
            MaterialPtr matPtr)
            : m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), k(xPos), mat(matPtr) {};

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        auto t = (k-ray.origin().x) / ray.direction().x;
        if (t < t_min || t > t_max)
            return false;
        auto y = ray.origin().y + t*ray.direction().y;
        auto z = ray.origin().z + t*ray.direction().z;
        if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
            return false;
        rec.u = (y-m_y0)/(m_y1-m_y0);
        rec.v = (z-m_z0)/(m_z1-m_z0);
        rec.t = t;
        auto outward_normal = Vec3{1, 0, 0};
        rec.set_face_normal(ray, outward_normal);
        rec.matPtr = mat;
        rec.p = ray.at(t);
        return true;
    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override {
        return AABB{Vec3(k-0.0001, m_y0, m_z0), Vec3(k+0.0001, m_y1, m_z1)};
    }

    double m_y0;
    double m_y1;
    double m_z0;
    double m_z1;
    double k;
    MaterialPtr mat;
};

class XZ_Rect: public Hittable{
public:

    XZ_Rect(double x0, double x1, double z0, double z1, double yPos,
            MaterialPtr mat)
            : m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), k(yPos), mat(mat) {};

    bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override {
        auto t = (k-ray.origin().y) / ray.direction().y;
        if (t < t_min || t > t_max)
            return false;
        auto x = ray.origin().x + t*ray.direction().x;
        auto z = ray.origin().z + t*ray.direction().z;
        if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
            return false;
        rec.u = (x-m_x0)/(m_x1-m_x0);
        rec.v = (z-m_z0)/(m_z1-m_z0);
        rec.t = t;
        auto outward_normal = Vec3{0, 1, 0};
        rec.set_face_normal(ray, outward_normal);
        rec.matPtr = mat;
        rec.p = ray.at(t);
        return true;
    }

    [[nodiscard]] AABB bbox(double time0, double time1) const override {
        return AABB{Vec3{m_x0,k-0.0001,m_z0}, Vec3{m_x1, k+0.0001, m_z1}};
    }

public:
    MaterialPtr mat;
    double m_x0, m_x1, m_z0, m_z1, k;
};



#endif //ONE_WEEKEND_PRIMRECT_HPP
