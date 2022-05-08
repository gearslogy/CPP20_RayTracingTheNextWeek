//
// Created by star on 4/28/2022.
//

#ifndef ONE_WEEKEND_GLM_OPERATORS_H
#define ONE_WEEKEND_GLM_OPERATORS_H
#include <concepts>
#include <glm/glm.hpp>

template<typename T>
concept exclude_float = std::is_arithmetic_v<T> && !std::is_same_v<T,float>;

template<typename T>
struct GLM_TypeSize;

template<>
struct GLM_TypeSize<glm::vec2>{
    static inline int value = 2;
};
template<>
struct GLM_TypeSize<glm::vec3>{
    static inline int value = 3;
};
template<>
struct GLM_TypeSize<glm::vec4>{
    static inline int value = 4;
};

template<typename  T>
concept expect_glm_vec = std::is_same_v<T, glm::vec3 > || std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec4>;


// vec * double
inline decltype(auto) operator * (expect_glm_vec auto vec, exclude_float auto  real ){
    using type = std::remove_cvref_t<decltype(vec)>; type ret{};
    for(int i=0;i < GLM_TypeSize<type>::value ;i++)
        ret[i] = vec[i] * real;
    return ret;
}

// float * vec
inline auto operator * (exclude_float auto  real ,expect_glm_vec auto var2){
    using type = std::remove_cvref_t<decltype(var2)>; type ret{};
    for(int i=0;i < GLM_TypeSize<type>::value ;i++)
        ret[i] = var2[i] * real;
    return ret;
}

// float / vec
inline decltype(auto) operator / (std::floating_point auto real , expect_glm_vec auto var2){
    using type = std::remove_cvref_t<decltype(var2)>; type ret{};
    for(int i=0;i < GLM_TypeSize<type>::value ;i++)
        ret[i] = real/ var2[i];
    return ret;
}

// vec / float
inline decltype(auto) operator / ( expect_glm_vec auto var, std::floating_point auto real){
    using type = std::remove_cvref_t<decltype(var)>; type ret{};
    for(int i=0;i < GLM_TypeSize<type>::value ;i++)
        ret[i] = var[i] / real;
    return ret;
}
// vec / int
inline decltype(auto) operator / ( expect_glm_vec auto var,  int real){
    return var/static_cast<float>(real);
}

inline auto length_squared(expect_glm_vec auto vec){
    return glm::dot(vec,vec);
}


#endif //ONE_WEEKEND_GLM_OPERATORS_H
