//
// Created by star on 4/5/2022.
//

#ifndef ONE_WEEKEND_PERLIN_H
#define ONE_WEEKEND_PERLIN_H
#include <vector>
#include <algorithm>
#include <ranges>
#include "Core/Utils.hpp"
class Perlin {
public:
    static inline constexpr int pointCount = 256;
    Perlin(){
        randFloat.resize(pointCount);
        std::ranges::generate(randFloat, [](){return random_double();});

        perm_x.resize(pointCount);
        perm_y.resize(pointCount);
        perm_z.resize(pointCount);

        auto iter = std::views::iota(0, pointCount);
        perm_x.assign(std::begin(iter), std::end(iter));
        perm_y.assign(std::begin(iter), std::end(iter));
        perm_z.assign(std::begin(iter), std::end(iter));

        std::ranges::shuffle(perm_x, RandEngine::instance().generator);
        std::ranges::shuffle(perm_y, RandEngine::instance().generator);
        std::ranges::shuffle(perm_z, RandEngine::instance().generator);

    }
    template<typename T>
    inline double noise(const T& P) const {
        auto i = static_cast<int>(4*P.x()) & 255;
        auto j = static_cast<int>(4*P.y()) & 255;
        auto k = static_cast<int>(4*P.z()) & 255;

        return randFloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }
private:
    std::vector<double> randFloat;
    std::vector<int> perm_x;
    std::vector<int> perm_y;
    std::vector<int> perm_z;
};



#endif //ONE_WEEKEND_PERLIN_H
