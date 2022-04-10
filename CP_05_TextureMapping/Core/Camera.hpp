//
// Created by star on 2/14/2022.
//

#ifndef ONE_WEEKEND_CAMERA_HPP
#define ONE_WEEKEND_CAMERA_HPP


#include "Sampler.hpp"

template<typename vec_t, typename ray_t>
struct Camera_T{
    constexpr Camera_T(vec_t lookFrom , vec_t lookAt,
                       double aperture,
                       double verticalFieldOfView,
                       double aspectRatio,
                       double focusDist, double shutterOpenTime, double shutterCloseTime) noexcept:
    lookFromPos{lookFrom},
    lookAtPos{lookAt},
    vFocalLength{focusDist},
    vFov(verticalFieldOfView),
    aspectWHRatio(aspectRatio),
    shutterOpen{shutterOpenTime},
    shutterClose{shutterCloseTime} {

        auto theta = degrees_to_radians(verticalFieldOfView);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspectRatio * viewport_height;

        const vec_t vup {0,1,0};
        w = normalize(lookFrom - lookAt);
        u = normalize(cross(vup, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = focusDist * viewport_width * u;
        vertical = focusDist * viewport_height * v;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDist*w;

        lensRadius = aperture / 2;
    }


    ray_t getRay(double s, double t) const {
        auto rd = lensRadius * DiskScatter<vec_t>::surface();
        auto offset = u * rd.x() + v * rd.y();

        auto rayOrigin = origin + offset;
        auto rayDir = lowerLeftCorner + s*horizontal + t*vertical - rayOrigin;
        auto rt = random_double(shutterOpen,shutterClose);
        return ray_t{rayOrigin,rayDir,rt};
    }

    inline auto getAspectRatio(){
        return aspectWHRatio;
    }

    vec_t origin{0,0,0};
    vec_t lowerLeftCorner;
    vec_t horizontal;
    vec_t vertical;
    vec_t u,v,w;


    vec_t lookFromPos;
    vec_t lookAtPos;
    double vFov{45};  //vertical field-of-view in degrees
    double aspectWHRatio{16.0/9.0};
    double vFocalLength{1.0};
    double lensRadius{0};


    double shutterOpen{0.0};
    double shutterClose{1.0};

};



#endif //ONE_WEEKEND_CAMERA_HPP
