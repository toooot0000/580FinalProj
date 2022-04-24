//
// Created by Jerry Ye on 2022/3/31.
//

#include "Tri.h"

#include <utility>


Tri::Tri(const Vertex& v1, const Vertex& v2, const Vertex& v3)
:vertices({v1, v2, v3})
{

}

Tri::Tri(std::array<Vertex, 3> vs)
: vertices(std::move(vs))
{

}

Vertex &Tri::operator[](size_t i)
{
    return vertices[i];
}

const Vertex& Tri::operator[](size_t i) const
{
    return vertices[i];
}

Tri::Tri(Vertex &&v1, Vertex &&v2, Vertex &&v3)
: vertices({std::move(v1), std::move(v2), std::move(v3)})
{

}
