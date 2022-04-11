## C++ 20 Generic Method : Raytracing the next week
* follow the author
* may be with some extensions 

## CP_01 motion blur:

![image](CP_01_transform_motion_blur/image.jpg)

key: 
1. 关键点就是模拟一个camera shutter_open shutter_close
2. 提供一个Ray class 提供一个RayTime 
3. camera get ray方法， 会生成一个RayTime = [shutter_open, shutter_close] 
4. 所有的Geometry遵循RayTime Position（也就是至少2 frame geometry 位置描述)，current is linear position motion blur，not deformation blur 

## CP_02 BVH
![image](CP_02_BVH/image.jpg)
Key:
1. BVH Node是在构造函数时候就递归划分了，所以是在递归构造函数
2. hit超级简单，但是由于hit中访问BVH的left, right,left和right是递归构造的的BVH子Node,如下：
```c
else{
        std::sort(objects.begin() + start, objects.begin() + end, func);
        auto mid =  start + object_span/2;
        left = std::make_shared<BVH>(objects, start, mid, time0, time1);
        right = std::make_shared<BVH>(objects, mid, end, time0, time1);
    }
```
3. 不过我觉得有一定的bug，他的bvh sort函数，有问题: bbox访问的是time=[0,0]
```c
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;
    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    return box_a.min().e[axis] < box_b.min().e[axis];
}
bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) { return box_compare(a, b, 0); }
bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) { return box_compare(a, b, 1); }
bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) { return box_compare(a, b, 2); }

```
4. 现代C++提供了更加好的解决方案，代码量减少:
```c++
BVH::BVH(std::vector<HittablePtr> &objects, size_t start, size_t end, double time0, double time1) {
    int axis = random_int(0,2);
    auto func = [axis, time0, time1](const HittablePtr &a, const HittablePtr &b){
        auto a_bbox = a->bbox(time0, time1);
        auto b_bbox = b->bbox(time0, time1);
        return a_bbox.min()[axis] < b_bbox.min()[axis];
    };
    
    size_t object_span = end - start;
    if(object_span == 1){
        left = right = objects[start];
    }
    else if(object_span == 2){
        if (func(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    }
    else{
        std::sort(objects.begin() + start, objects.begin() + end, func);
        auto mid =  start + object_span/2;
        left = std::make_shared<BVH>(objects, start, mid, time0, time1);
        right = std::make_shared<BVH>(objects, mid, end, time0, time1);
    }
    AABB box_left = left->bbox(time0, time1);
    AABB box_right = right->bbox(time0, time1);
    box = surroundingBox(box_left, box_right);
}
```
5. performance：

* BVH : 83s
* Brute force : 450s

## CP_03 Texture : a checker ground

![image](CP_03_Texture/image.jpg)


## CP_04 PerlinBlock
![image](CP_04_PerlinBlockNoise/image.jpg)

More elegant implementation PerlinBlock of C++ 20 

```C++
#include <vector>
#include <ranges>
#include "Core/Utils.hpp"
class Perlin {
public:
    static inline constexpr int pointCount = 256;
    Perlin(){
        randFloat.resize(pointCount);
        std::ranges::generate(randFloat, [](){return random_double();});

        permX.resize(pointCount);
        permY.resize(pointCount);
        permZ.resize(pointCount);

        auto iter = std::views::iota(0, pointCount);
        permX.assign(std::begin(iter), std::end(iter));
        permY.assign(std::begin(iter), std::end(iter));
        permZ.assign(std::begin(iter), std::end(iter));

        std::ranges::shuffle(permX, RandEngine::instance().generator);
        std::ranges::shuffle(permY, RandEngine::instance().generator);
        std::ranges::shuffle(permZ, RandEngine::instance().generator);
    }
    template<typename T>
    inline double noise(const T& P) const {
        auto i = static_cast<int>(4*P.x()) & 255;
        auto j = static_cast<int>(4*P.y()) & 255;
        auto k = static_cast<int>(4*P.z()) & 255;
        return randFloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }
private:
    std::vector<double> randFloat;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};


```
Compared to the previous:

```c
#ifndef PERLIN_H
#define PERLIN_H
#include "rtweekend.h"
class perlin {
    public:
        perlin() {
            ranfloat = new double[point_count];
            for (int i = 0; i < point_count; ++i) {
                ranfloat[i] = random_double();
            }
            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }
        ~perlin() {
            delete[] ranfloat;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }
        double noise(const point3& p) const {
            auto i = static_cast<int>(4*p.x()) & 255;
            auto j = static_cast<int>(4*p.y()) & 255;
            auto k = static_cast<int>(4*p.z()) & 255;
            return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        }
    private:
        static const int point_count = 256;
        double* ranfloat;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm() {
            auto p = new int[point_count];
            for (int i = 0; i < perlin::point_count; i++)
                p[i] = i;
            permute(p, point_count);
            return p;
        }

        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
};
#endif
```

## CP_05 TextureMapping
![image](CP_05_TextureMapping/image.jpg)

## CP_06 Constant Material
![image](CP_06_ConstantMat/image.jpg)
