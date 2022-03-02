//
// Created by star on 2/13/2022.
//

#ifndef ONE_WEEKEND_VEC_HPP
#define ONE_WEEKEND_VEC_HPP
#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <concepts>


template<typename T, int DIM>
struct Vec_t{
    T data[DIM]{};
    Vec_t() = default;
    // constructor, Remove warning convert:warning C4838:conversion from 'double' to 'T' requires a narrowing conversion
    template<typename ... o_t>
    constexpr explicit Vec_t(const o_t & ... elems){
        auto v= { static_cast<T>(elems)...};
        for(auto i=0;i<DIM;i++){
            this->data[i] = *(v.begin() + i);
        }
    }

    template<typename o_t>
    constexpr auto operator / (const o_t & v) const{
        auto ret = *this;
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                ret.data[i]/=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                ret.data[i]/=v.data[i];
        }
        return ret;
    }

    template<typename o_t>
    constexpr auto operator /= (const o_t & v) {
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                this->data[i]/=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                this->data[i]/=v.data[i];
        }
        return *this;
    }

    template<typename o_t>
    constexpr auto operator * (const o_t & v) const{
        auto ret = *this;
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                ret.data[i]*=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                ret.data[i]*=v.data[i];
        }return ret;
    }
    template<typename o_t>
    constexpr auto& operator *= (const o_t & v) {
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                this->data[i]*=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                this->data[i]*=v.data[i];
        }
        return *this;
    }


    template<typename o_t>
    constexpr auto operator + (const o_t & v) const{
        auto ret = *this;
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                ret.data[i]+=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                ret.data[i]+=v.data[i];
        }return ret;
    }

    template<typename o_t>
    constexpr auto & operator += (const o_t & v) {
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                this->data[i]+=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                this->data[i]+=v.data[i];
        }
        return *this;
    }

    template<typename o_t>
    constexpr auto operator - (const o_t & v) const{
        auto ret = *this;
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                ret.data[i]-=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                ret.data[i]-=v.data[i];
        }return ret;
    }

    template<typename o_t>
    constexpr auto & operator -= (const o_t & v) {
        if constexpr( std::is_arithmetic_v<o_t>){
            for(auto i=0;i<DIM;i++)
                this->data[i]-=v;
        }
        else{
            for(auto i=0;i<DIM;i++)
                this->data[i]-=v.data[i];
        }
        return *this;
    }

    auto operator-() const {
        return Vec_t(-data[0], -data[1], -data[2]);
    }

    [[nodiscard]] inline constexpr auto x() const{
        static_assert(DIM >= 1);
        return data[0];
    }
    [[nodiscard]] inline constexpr auto y() const{
        static_assert(DIM >= 2);
        return data[1];
    }
    [[nodiscard]] inline constexpr auto z() const{
        static_assert(DIM >=3);
        return data[2];
    }
    [[nodiscard]] inline constexpr auto w() const{
        static_assert(DIM >=4);
        return data[3];
    }
    T operator[](int i) const { return data[i]; }
    T& operator[](int i) { return data[i]; }



};


// var + vector
template<typename T, int DIM, typename op_t> requires std::is_arithmetic_v<op_t>
constexpr inline Vec_t<T,DIM> operator+(op_t t, const Vec_t<T,DIM> &v) {
    auto ret = v;
    for(auto i = 0; i< DIM; i++)
        ret[i] += t;
    return ret;
}

// var * vector
template<typename T, int DIM, typename op_t> requires std::is_arithmetic_v<op_t>
constexpr inline Vec_t<T,DIM> operator*(op_t t, const Vec_t<T,DIM> &v) {
    auto ret = v;
    for(auto i = 0; i< DIM; i++)
        ret[i] *= t;
    return ret;
}


template<typename T, int DIM>
inline std::ostream & operator << (std::ostream & os , const Vec_t<T,DIM> & v)
{
    os << "{ ";
    for(int i=0;i<DIM;i++)
        if (i == DIM-1)
            os << v.data[i] ;
        else
            os << v.data[i] << ", ";
    os << "}";
    return os;
}

template<typename T, int DIM>
inline auto length_squared(const Vec_t<T,DIM> &r){
    /*
    auto ret = std::accumulate(r.data , r.data + DIM , T{}, [](auto previous_count, auto &element){
        return previous_count  + element * element;
    });
     */
    T ret{0.0};
    for(int i=0;i<DIM;i++){
        ret += (r.data[i] * r.data[i]);
    }
    return ret;
}

template<typename T, int DIM>
inline auto length(const Vec_t<T,DIM> &r){
    T ret = length_squared(r);
    ret = sqrt(ret);
    return ret;
}
template<typename T, int DIM>
inline auto normalize(const Vec_t<T,DIM> &r){
    return r / length(r);
}

template<typename T, int DIM>
inline auto dot(const Vec_t<T,DIM> &r1, const Vec_t<T, DIM> &r2){
    T ret{};
    for(auto i=0;i<DIM;i++)
        ret += (r1.data[i] * r2.data[i]);
    return ret;
}

template<typename T>
inline auto cross(const Vec_t<T,3> & u, const Vec_t<T, 3> & v ){
    auto x = u.y() * v.z() - u.z() * v.y();
    auto y = u.z() * v.x() - u.x() * v.z();
    auto z = u.x() * v.y() - u.y() * v.x();
    return Vec_t<T,3>{x, y, z};
}

template<typename T>
inline bool isZeroVector(const Vec_t<T,3> &e){
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}

// v is incoming ray, N is incoming ray hit point N
template<typename T>
constexpr inline auto reflect( const Vec_t<T,3> & v, const Vec_t<T,3> & N ){
    return v - 2*dot(v,N) * N;
}

// get to refract ray
template<typename T>
constexpr inline auto refract(const Vec_t<T,3> &v, const Vec_t<T,3> &N , double eta){
    auto cos_theta = std::fmin(dot(-v, N), 1.0);
    auto r_out_perp =  eta * (v + cos_theta*N);
    auto r_out_parallel = -sqrt(std::fabs(1.0 - length_squared(r_out_perp))  ) * N;
    return r_out_perp + r_out_parallel;
}

#endif //ONE_WEEKEND_VEC_HPP
