//
// Created by Jerry Ye on 2022/3/30.
//

#ifndef INC_580_FINAL_PROJ_RAYCAST_H
#define INC_580_FINAL_PROJ_RAYCAST_H

#include "../linear/Vec.h"

namespace RayCast{

    class Tri{
    public:
        virtual Vec3 operator[](int i ) const = 0;
        virtual Vec3& operator[](int i ) = 0;
    };

    class Ray{
    public:
        [[nodiscard]] virtual Vec3 getStartPoint() const = 0;
        [[nodiscard]] virtual Vec3 getDir() const = 0;
    };

    class RayCast
    {
    public:
        virtual const Tri & detectCollision(const Ray&) = 0;
    };
}



#endif //INC_580_FINAL_PROJ_RAYCAST_H
