//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_OBJLOADER_H
#define INC_580_FINAL_PROJ_OBJLOADER_H

#include "Mesh.h"
#include "RayCast.h"
#include <concepts>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

template<class T>
requires std::derived_from<T, Tri>
class Mesh;

class Texture;

class ObjLoader
{
public:
//    static Mesh<> load(const char* filename);

    template<class T>
    static T load(const char* filename){
        std::ifstream obj(filename);
        std::string line, op;
        std::vector<Vec3> vs, uvs, norms;
        std::vector<TriInd> tris;

        while(getline(obj, line)){
            if(line[0] == '#'){
                continue;
            }
            std::stringstream ss(line);
            ss >> op;
            switch (op[0])
            {
                case 'v':{
                    if(op.length() == 1){
                        double x, y, z;
                        ss >> x >> y >> z;
                        vs.emplace_back(x, y, z);
                    } else switch(op[1]){
                            case 't':   // vt
                                double u, v;
                                ss >> u >> v;
                                uvs.emplace_back(u, v, 1.f);
                                break;
                            case 'n':   // vn
                                double x, y, z;
                                ss >> x >> y >> z;
                                norms.emplace_back(x, y, z);
                                break;
                            default:
                                break;
                        }
                    break;
                }
                case 'f':{
                    std::string face, token;
                    std::vector<std::vector<int>> nums;
                    int groupsNum = 0;
                    while(ss>>face){
                        std::stringstream faceSs(face);
                        nums.emplace_back(std::vector<int>());
                        while(getline(faceSs, token, '/')){
                            nums[groupsNum].emplace_back(stoi(token));
                        }
                        groupsNum++;
                    }
                    if(nums.size() == 3){
                        TriInd tri{
                                {nums[0][0], nums[1][0], nums[2][0]},
                                {nums[0][1], nums[1][1], nums[2][1]},
                                {nums[0][2], nums[1][2], nums[2][2]}
                        };
                        tris.emplace_back(tri);
                    } else if(nums.size() == 4) {
                        TriInd tri1{
                                {nums[0][0], nums[1][0], nums[2][0]},
                                {nums[0][1], nums[1][1], nums[2][1]},
                                {nums[0][2], nums[1][2], nums[2][2]}
                        }, tri2{
                                {nums[0][0], nums[2][0], nums[3][0]},
                                {nums[0][1], nums[2][1], nums[3][1]},
                                {nums[0][2], nums[2][2], nums[3][2]}
                        };
                        tris.emplace_back(tri1);
                        tris.emplace_back(tri2);
                    }
                    break;
                }
                default: { break; }
            }
        }
        return {vs, uvs, norms, tris};
    }

    static Texture loadTexture(const char* filename);
};

#endif //INC_580_FINAL_PROJ_OBJLOADER_H
