//
// Created by Jerry Ye on 2022/3/30.
//

#include "RayCast.h"

#include <utility>

RayCast::Tri * RayCast::Mesh::detectCollision(const RayCast::Ray &)
{
    return {};
}

Vec3 RayCast::Ray::getStartPoint() const
{
    return startPoint;
}

Vec3 RayCast::Ray::getDir() const
{
    return dir;
}

RayCast::Ray::Ray(Vec3 startPoint, Vec3 dir)
: startPoint(std::move(startPoint)), dir(std::move(dir))
{
}
