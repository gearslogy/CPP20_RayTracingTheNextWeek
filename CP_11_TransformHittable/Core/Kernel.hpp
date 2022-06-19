//
// Created by star on 2/14/2022.
//

#ifndef ONE_WEEKEND_KERNEL_HPP
#define ONE_WEEKEND_KERNEL_HPP

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Camera.hpp"
#include "AABB.hpp"
#include "GLM_Operators.hpp"


using Vec3   = glm::vec3;
using Color  = Vec3;
using Ray    = Ray_t<Vec3>;
using Camera = Camera_T<Vec3, Ray>;
using AABB   = AABB_T<Vec3, Ray>;



constexpr Vec3 zero_vector{0,0,0};
constexpr Vec3 color_black{0,0,0};

#endif //ONE_WEEKEND_KERNEL_HPP
