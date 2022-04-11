//
// Created by star on 4/10/2022.
//

#ifndef ONE_WEEKEND_CONSTANT_H
#define ONE_WEEKEND_CONSTANT_H


#include "Core/Material.hpp"
#include "Core/Texture.hpp"

class Constant: public Material{
public:
    explicit Constant(Color color): emit{std::make_shared<SolidColorTex>(color)}{}
    explicit Constant(TexturePtr tex): emit{tex}{}

    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        return false;
    }

    Color emitted(double u, double v, const Vec3 &P) const override {
        return emit->value(u,v,P);
    }

    TexturePtr emit;

};


#endif //ONE_WEEKEND_CONSTANT_H
