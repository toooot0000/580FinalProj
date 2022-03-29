//
// Created by Jerry Ye on 2022/3/24.
//

#ifndef INC_580_FINAL_PROJ_OBJLOADER_H
#define INC_580_FINAL_PROJ_OBJLOADER_H

class Mesh;
class Texture;

class ObjLoader
{
public:
    static Mesh load(const char* filename);
    static Texture loadTexture(const char* filename);
};

#endif //INC_580_FINAL_PROJ_OBJLOADER_H
