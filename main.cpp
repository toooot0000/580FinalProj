#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"

void renderTeapot(Renderer &renderer){
    Mesh teapot = ObjLoader::load("obj/teapot.obj");
    teapot.setRotateY(90);
//    teapot.setRotateX(30);
//    teapot.setRotateZ(90);
    teapot.setX(-5);
    renderer.render(teapot);
    renderer.flushToImg("teapot.tga");
}

void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-2);
    mesh.setRotateX(45);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);
    renderer.render(mesh);
    renderer.flushToImg("wooden stool.tga");
}

int main()
{
    Renderer renderer(1024, 1024, {
            {-0.52, 0.38, 0.064}, {0.41, 0.56, 0.119}, {0.27, 0.08, 0.147},
            {-0.17, -0.29, 0.249}, {0.58, -0.55, 0.104}, {-0.31, -0.71, 0.106},
            {0.52, -0.38, 0.064}, {-0.27, -0.08, 0.147}
    });

//    Renderer renderer(1024, 1024);

    renderer.addLight(
            {Vec3{-0.7071, 0.7071, 0}, Util::Color({0.5, 0.5, 0.9}) }
            );
    renderer.addLight(
            {Vec3{0, -0.7071, -0.7071}, Util::Color({0.9, 0.2, 0.3}) }
            );
    renderer.addLight(
            {Vec3{0.7071, 0.0, -0.7071}, Util::Color({0.2, 0.7, 0.3})}
            );
    renderer.setCamera({
                               Vec3{0, -25, 0},
                               Vec3{0, 0, 1},
                               Vec3{0, 1, 0}}
                               );
//    renderTeapot(renderer);
    renderWoodenStool(renderer);
    return 0;
}