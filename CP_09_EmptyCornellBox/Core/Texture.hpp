//
// Created by star on 4/3/2022.
//

#ifndef ONE_WEEKEND_TEXTURE_H
#define ONE_WEEKEND_TEXTURE_H

#include "Kernel.hpp"
#include <memory>
class Texture{
public:
    virtual Color value(double u, double v, const Vec3 &P) = 0;
};

using TexturePtr = std::shared_ptr<Texture>;

// constant color texture
class SolidColorTex : public Texture{
public:
    SolidColorTex()= default;
    explicit SolidColorTex (const Color &color): t{color}{}

    Color value(double u, double v , const Vec3 &P)  override{
        return t;
    }
private:
    Color t{0};
};


// Checker Texture
class CheckerTex : public Texture{
public:
    CheckerTex(TexturePtr tex1, TexturePtr tex2): odd(tex1), even(tex2){}
    CheckerTex(Color c1, Color c2): odd(std::make_shared<SolidColorTex>(c1)), even(std::make_shared<SolidColorTex>(c2)){}
    Color value(double u, double v, const Vec3 &P) override{
        auto sines = std::sin(10* P.x) * std::sin(10*P.y)* std::sin(10*P.z);
        if (sines < 0)
            return odd->value(u, v, P);
        else
            return even->value(u, v, P);
    }
private:
    TexturePtr odd;
    TexturePtr even;
};


#endif //ONE_WEEKEND_TEXTURE_H
