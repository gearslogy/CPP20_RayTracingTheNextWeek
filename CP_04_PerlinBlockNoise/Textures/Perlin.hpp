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

        permX.resize(pointCount);
        permY.resize(pointCount);
        permZ.resize(pointCount);

        auto iter = std::views::iota(0, pointCount);
        permX.assign(std::begin(iter), std::end(iter));
        permY.assign(std::begin(iter), std::end(iter));
        permZ.assign(std::begin(iter), std::end(iter));

        std::ranges::shuffle(permX, RandEngine::instance().generator);
        std::ranges::shuffle(permY, RandEngine::instance().generator);
        std::ranges::shuffle(permZ, RandEngine::instance().generator);
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
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};



#endif //ONE_WEEKEND_PERLIN_H
