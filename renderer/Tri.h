//
// Created by Jerry Ye on 2022/3/31.
//

#ifndef INC_580_FINAL_PROJ_TRI_H
#define INC_580_FINAL_PROJ_TRI_H

#include <array>
#include "../linear/Vec.h"

struct Vertex{
    Vec3 position;
    Vec3 normal;
    Vec3 uvw;
};

class Tri{
private:
    std::array<Vertex, 3> vertices;
public:
    Tri() = default;
    ~Tri() = default;
    Tri(const Tri& other) = delete;
    Tri& operator=(const Tri& other) = delete;
    Tri(Tri&& other) = default;
    Tri& operator=(Tri&& other) = default;
    Tri(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    Tri(Vertex&& v1, Vertex&& v2, Vertex&& v3);
    explicit Tri(std::array<Vertex, 3>  vs);
    Vertex& operator[](size_t i);
    Vertex operator[](size_t i) const;
};


#endif //INC_580_FINAL_PROJ_TRI_H
