cmake_minimum_required(VERSION 3.21)
project(one_weekend)
include_directories(D:/plugin_dev/libs/stb)
include_directories(.)
set(CMAKE_CXX_STANDARD 23)
# openmp
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


add_executable(one_weekend
        main.cpp
        Core/Vec.hpp
        Core/Ray.hpp
        Core/Utils.hpp
        Core/Camera.hpp
        Core/Hittable.hpp
        Core/Kernel.hpp
        Core/AABB.hpp
        primitives/PrimSphere.hpp
        Core/Sampler.hpp
        Core/Material.hpp Materials/Lambertian.hpp
        Core/parallel.h Materials/PerfectReflection.hpp
        Materials/Refract.hpp Core/BVH.hpp Core/Texture.hpp)
