//
// Created by star on 3/14/2022.
//

#ifndef ONE_WEEKEND_BVH_HPP
#define ONE_WEEKEND_BVH_HPP
#include "Hittable.hpp"


class BVH : public Hittable{
public:
    BVH( HittableList & hittableList, double time0, double time1);
    BVH(std::vector<HittablePtr> &objects, size_t start, size_t end, double time0 , double time1);

    [[nodiscard]] bool hit(const Ray &ray , double t_min, double t_max, HitRecord & rec) const override;
    [[nodiscard]] AABB bbox(double time0, double time1) const override;

private:
    HittablePtr left;   //  left is std::make_shared<BVH>
    HittablePtr right;  //  right is std::make_shared<BVH>
    AABB box;

};




// constructor
BVH::BVH( HittableList &hittableList, double time0, double time1): BVH (hittableList.objects, 0, hittableList.objects.size(), time0, time1) {
}




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

bool BVH::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const {
    if(!box.hit(ray,t_min,t_max) ) return false;
    bool hitL = left->hit(ray,t_min, t_max, rec);
    bool hitR = right-> hit(ray, t_min, hitL? rec.t : t_max, rec);
    return hitL || hitR;
}

AABB BVH::bbox(double time0, double time1) const {
    return box;
}


#endif //ONE_WEEKEND_BVH_HPP
