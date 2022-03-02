//
// Created by star on 2/13/2022.
//

#ifndef ONE_WEEKEND_UTILS_HPP
#define ONE_WEEKEND_UTILS_HPP

#include <random>
#include <iostream>
#include <numeric>
#include <numbers>
#include <limits>
#include <cstdlib>

constexpr double pi = std::numbers::pi;
constexpr double infinity = std::numeric_limits<double>::infinity();

constexpr inline double degrees_to_radians(double degrees) {
    return degrees * std::numbers::pi / 180.0;
}

constexpr inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

template<typename T>
class Singleton
{
public:
    Singleton(Singleton const &) = delete;
    Singleton & operator = (Singleton const &)= delete;
    static T& instance(){
        static T t;
        return t;
    }

protected:
    Singleton()= default;

};


class RandEngine: public Singleton<RandEngine>{
public:
    std::random_device rd;
    std::mt19937_64 generator{rd()};
};


// default generate 0-1 numbers
inline double random_double(double low= 0, double high = 1) {
    auto &instance = RandEngine::instance();
    std::uniform_real_distribution<double> distribution(low, high);
    return distribution(instance.generator);
}

template<typename RGB_t, typename Color_T>
constexpr inline RGB_t vec_to_rgb( const Color_T & pixel_color){
    return {static_cast<int>(255.999 * pixel_color.x()) ,
            static_cast<int>(255.999 * pixel_color.y()),
            static_cast<int>(255.999 * pixel_color.z()) };
}

#endif //ONE_WEEKEND_UTILS_HPP
