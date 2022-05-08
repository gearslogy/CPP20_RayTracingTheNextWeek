//
// Created by star on 2/13/2022.
//

#ifndef ONE_WEEKEND_RAY_HPP
#define ONE_WEEKEND_RAY_HPP
#include <iostream>
template<typename Vec_t >
struct Ray_t{
    constexpr Ray_t() =default;
    constexpr Ray_t(const Vec_t &rayOrigin, const Vec_t &rayDir, const double time) noexcept: o{rayOrigin}, d{rayDir},rt{time}{}

    constexpr auto at(double t) const{
        return o + t * d;
    }
    constexpr auto direction()const{return d;}
    constexpr auto origin() const{return o;}
    constexpr auto time() const{return rt;}


    Vec_t o; // xyz o
    Vec_t d; // xyz dir
    double rt{0}; // ray time;
};

template< typename Vec_t>
inline std::ostream  & operator <<(std::ostream & os , const Ray_t<Vec_t> & ray){
    os << "ray origin:" << ray.o <<" direction:" << ray.d;
    return os;
}

#endif //ONE_WEEKEND_RAY_HPP
