//
// Created by star on 4/28/2022.
//

#ifndef ONE_WEEKEND_GLM_OPERATORS_H
#define ONE_WEEKEND_GLM_OPERATORS_H
#include <concepts>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
concept arithmetic_real = std::is_arithmetic_v<T>;

template<typename T>
concept exclude_float = std::is_arithmetic_v<T> && !std::is_same_v<T,float>;

template<typename T>
struct glm_type_size;

template<>
struct glm_type_size<glm::vec2>{
    static constexpr int value = 2;
};
template<>
struct glm_type_size<glm::vec3>{
    static constexpr int value = 3;
};
template<>
struct glm_type_size<glm::vec4>{
    static constexpr int value = 4;
};

template<typename T>
constexpr int glm_type_size_v = glm_type_size<T>::value;

template<typename  T>
concept expect_glm_vec = std::is_same_v<T, glm::vec3 > || std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec4>;


// vec * double
inline decltype(auto) operator * (expect_glm_vec auto vec, exclude_float auto  real ){
    using type = std::remove_cvref_t<decltype(vec)>; type ret{};
    for(int i=0; i < glm_type_size<type>::value ; i++)
        ret[i] = vec[i] * real;
    return ret;
}

// float * vec
inline auto operator * (exclude_float auto  real ,expect_glm_vec auto var2){
    using type = std::remove_cvref_t<decltype(var2)>; type ret{};
    for(int i=0; i < glm_type_size<type>::value ; i++)
        ret[i] = var2[i] * real;
    return ret;
}

// float / vec
inline decltype(auto) operator / (std::floating_point auto real , expect_glm_vec auto var2){
    using type = std::remove_cvref_t<decltype(var2)>; type ret{};
    for(int i=0; i < glm_type_size<type>::value ; i++)
        ret[i] = real/ var2[i];
    return ret;
}

// vec / float
inline decltype(auto) operator / ( expect_glm_vec auto var, std::floating_point auto real){
    using type = std::remove_cvref_t<decltype(var)>; type ret{};
    for(int i=0; i < glm_type_size_v<type> ; i++)
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

inline auto isZeroReal(arithmetic_real auto var)  {
    const auto s = 1e-8;
    return std::fabs(var) < s ;
}

inline auto isZeroVector(expect_glm_vec auto vec){
    bool ret{false};
    using type = std::remove_cvref_t<decltype(vec)>;
    for(int i=0; i < glm_type_size_v<type>; i++){
        ret = ret && isZeroReal(vec[i]);
    }
    return ret;
}



#endif //ONE_WEEKEND_GLM_OPERATORS_H
