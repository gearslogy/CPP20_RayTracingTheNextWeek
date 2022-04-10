//
// Created by star on 3/9/2022.
//

#ifndef ONE_WEEKEND_AABB_H
#define ONE_WEEKEND_AABB_H
#include <utility>
template<typename vec_t, typename ray_t>
struct AABB_T{
public:
    AABB_T() = default;
    AABB_T(const vec_t &min , const vec_t &max): minimum{min}, maximum{max}{}

    constexpr inline auto min() const{
        return minimum;
    }
    constexpr inline auto max() const{
        return maximum;
    }
    constexpr inline auto center() const{
        return (maximum - minimum) / 2.0;
    }
    constexpr inline void expand(auto var) const{
        minimum -= var;
        maximum += var;
    }
    // An Optimized AABB Hit Method
    bool hit(const ray_t &r, double t_min , double t_max) const{
        for (int a = 0; a < 3; a++) {
            double t0 = std::min((minimum[a] - r.origin()[a]) / r.direction()[a],
                           (maximum[a] - r.origin()[a]) / r.direction()[a]);
            double t1 = std::max((minimum[a] - r.origin()[a]) / r.direction()[a],
                           (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = std::max(t0, t_min);
            t_max = std::min(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    vec_t minimum{};
    vec_t maximum{};
};

template<typename vec_t, typename ray_t>
inline auto surroundingBox(const AABB_T<vec_t,ray_t> &box0, const AABB_T<vec_t, ray_t> &box1){
    static_assert(vec_t::dim == 3, "only support DIM==3");


    vec_t small{std::min(box0.min().x(), box1.min().x()),
                std::min(box0.min().y(), box1.min().y()),
                std::min(box0.min().z(), box1.min().z())};

    vec_t big{std::max(box0.max().x(), box1.max().x()),
              std::max(box0.max().y(), box1.max().y()),
              std::max(box0.max().z(), box1.max().z())};

    return AABB_T<vec_t,ray_t>{small,big};

}






#endif //ONE_WEEKEND_AABB_H
