//
// Created by star on 4/5/2022.
//

#ifndef ONE_WEEKEND_NOISETEXTURE_HPP
#define ONE_WEEKEND_NOISETEXTURE_HPP

#include "Core/Texture.hpp"
#include "Perlin.hpp"
class NoiseTexture: public Texture{
public:
    NoiseTexture()= default;
    explicit NoiseTexture(float df):amp(df) {}
    Color value(double u, double v, const Vec3 &P) override {
        return Color{1,1,1}  * noise.noise(P) * amp;
    }
    Perlin noise;
    float amp{1};
};



#endif //ONE_WEEKEND_NOISETEXTURE_HPP
