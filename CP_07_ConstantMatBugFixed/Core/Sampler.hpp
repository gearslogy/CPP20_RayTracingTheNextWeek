//
// Created by star on 2/15/2022.
//

#ifndef ONE_WEEKEND_SAMPLER_HPP
#define ONE_WEEKEND_SAMPLER_HPP


#include "Utils.hpp"

template<typename vec_t>
class QubeScatter{
public:
    // random points in [-1 , 1] qube
    static inline auto interior(){
        vec_t ret{random_double(-1,1), random_double(-1,1), random_double(-1,1)};
        return ret;
    };
};

template<typename vec_t>
class DiskScatter{
public:
    // reject method. get sample point on disk
    static inline auto surface() {
        while (true) {
            auto p = vec_t(random_double(-1,1), random_double(-1,1), 0);
            if (length_squared(p) >= 1) continue;
            return p;
        }
    }
};

template<typename vec_t>
class SphereScatter{
public:
    // sample point inside sphere, use reject method
    static inline auto interior(){
        while (true) {
            auto x = random_double(-1,1);
            auto y = random_double(-1,1);
            auto z = random_double(-1,1);
            auto p = vec_t{x,y,z};
            if (length_squared(p)>= 1) continue;
            return p;
        }
    }
    // sample point on unit sphere surface
    static inline vec_t surface() {
        auto a = random_double(0, 2*pi);
        auto z = random_double(-1, 1);
        auto r = sqrt(1 - z*z);
        return vec_t(r*std::cos(a), r*std::sin(a), z);
    }
};
template<typename vec_t>
class HemisphereScatter{
public:
    // sample point in unit hemisphere
    static inline auto surface(const vec_t& normal) {
        vec_t dir{};
        auto in_unit_sphere = SphereScatter<vec_t>::surface();
        if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            dir = in_unit_sphere;
        else
            dir = -in_unit_sphere;
        return dir;
    }
};


template<typename vec_t>
inline auto random_vector(double low = 0 , double high = 1){
    auto x = random_double(low, high);
    auto y = random_double(low, high);
    auto z = random_double(low, high);
    return vec_t{x,y,z};
}

#endif //ONE_WEEKEND_SAMPLER_HPP
