//
// Created by star on 4/10/2022.
//

#ifndef ONE_WEEKEND_IMAGETEXTURE_H
#define ONE_WEEKEND_IMAGETEXTURE_H
#define STB_IMAGE_IMPLEMENTATION
#include "Core/Texture.hpp"
#include "Perlin.hpp"
#include <stb_image.h>

class ImageTexture: public Texture{
public:
    inline static constexpr int bytes_per_pixel = 3;
    explicit ImageTexture(std::string_view fileName){
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(fileName.data(),&width,&height, &components_per_pixel, bytes_per_pixel );
        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << fileName << "'.\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }
    ~ImageTexture(){
        stbi_image_free(data);
    }


    Color value(double u, double v, const Vec3 &P) override {
        if (data == nullptr)
            return Color(0,1,1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = std::clamp(u, 0.0, 1.0);
        v = 1.0 - std::clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width)  i = width-1;
        if (j >= height) j = height-1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

        return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }
private:
    unsigned char * data;
    int width{};
    int height{};
    int bytes_per_scanline;
};





#endif //ONE_WEEKEND_IMAGETEXTURE_H
