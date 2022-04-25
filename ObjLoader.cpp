//
// Created by Jerry Ye on 2022/3/24.
//

#include "./renderer/Mesh.h"
#include "ObjLoader.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include "lodepng-master/lodepng.h"

using namespace std;

//Mesh<> ObjLoader::load(const char *filename)
//{
//    ifstream obj(filename);
//    string line, op;
//    vector<Vec3> vs, uvs, norms;
//    vector<TriInd> tris;
//
//    while(getline(obj, line)){
//        if(line[0] == '#'){
//            continue;
//        }
//        stringstream ss(line);
//        ss >> op;
//        switch (op[0])
//        {
//            case 'v':{
//                if(op.length() == 1){
//                    double x, y, z;
//                    ss >> x >> y >> z;
//                    vs.emplace_back(x, y, z);
//                } else switch(op[1]){
//                        case 't':   // vt
//                            double u, v;
//                            ss >> u >> v;
//                            uvs.emplace_back(u, v, 1.f);
//                            break;
//                        case 'n':   // vn
//                            double x, y, z;
//                            ss >> x >> y >> z;
//                            norms.emplace_back(x, y, z);
//                            break;
//                        default:
//                            break;
//                    }
//                break;
//            }
//            case 'f':{
//                string face, token;
//                vector<vector<int>> nums;
//                int groupsNum = 0;
//                while(ss>>face){
//                    stringstream faceSs(face);
//                    nums.emplace_back(vector<int>());
//                    while(getline(faceSs, token, '/')){
//                        nums[groupsNum].emplace_back(stoi(token));
//                    }
//                    groupsNum++;
//                }
//                if(nums.size() == 3){
//                    TriInd tri{
//                            {nums[0][0], nums[1][0], nums[2][0]},
//                            {nums[0][1], nums[1][1], nums[2][1]},
//                            {nums[0][2], nums[1][2], nums[2][2]}
//                    };
//                    tris.emplace_back(tri);
//                } else if(nums.size() == 4) {
//                    TriInd tri1{
//                            {nums[0][0], nums[1][0], nums[2][0]},
//                            {nums[0][1], nums[1][1], nums[2][1]},
//                            {nums[0][2], nums[1][2], nums[2][2]}
//                    }, tri2{
//                            {nums[0][0], nums[2][0], nums[3][0]},
//                            {nums[0][1], nums[2][1], nums[3][1]},
//                            {nums[0][2], nums[2][2], nums[3][2]}
//                    };
//                    tris.emplace_back(tri1);
//                    tris.emplace_back(tri2);
//                }
//                break;
//            }
//            default: { break; }
//        }
//    }
//    return {vs, uvs, norms, tris};
//}

Texture ObjLoader::loadTexture(const char *filename)
{
    std::vector<unsigned char> image;
    unsigned width, height;

    auto error = lodepng::decode(image, width, height, filename);
    if(error){
        std::cerr << "Sth wrong!" << lodepng_error_text(error) << std::endl;
        return {};
    }
    return Texture(width, height, image);
}
