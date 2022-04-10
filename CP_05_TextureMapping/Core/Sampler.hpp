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
            auto p = QubeScatter<vec_t>::interior();
            if (length_squared(p)>= 1) continue;
            return p;
        }
    }
    // sample point on unit sphere surface
    static inline vec_t surface() {
        return normalize(interior());  // 只要把原来的球内改成normalize就可以了
    }
};
template<typename vec_t>
class HemisphereScatter{
public:
    // sample point in unit hemisphere
    static inline auto interior(const vec_t& normal) {
        auto in_unit_sphere = SphereScatter<vec_t>::interior();
        if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return in_unit_sphere;
        else
            return -in_unit_sphere;
    }
    // sample point on unit hemisphere surface
    static inline auto surface(const vec_t& normal){
        return normalize(interior(normal));
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
