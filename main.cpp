#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"

void renderTeapot(Renderer &renderer){
    Mesh teapot = ObjLoader::load("obj/teapot.obj");
    renderer.render(teapot);
    renderer.flushToImg("teapot.tga");
}

void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load("obj/wooden stool.obj");
    mesh.setScale(1.2);
    mesh.setY(1);
    mesh.setRotateX(90);
    renderer.render(mesh);
    renderer.flushToImg("wooden stool.tga");
}

int main()
{
    Renderer renderer(1024, 1024);

    renderer.addLight(
            {Vec3{-0.7071, 0.7071, 0}, Util::Color({0.5, 0.5, 0.9}) }
            );
    renderer.addLight(
            {Vec3{0, -0.7071, -0.7071}, Util::Color({0.9, 0.2, 0.3}) }
            );
    renderer.addLight(
            {Vec3{0.7071, 0.0, -0.7071}, Util::Color({0.2, 0.7, 0.3})}
            );
    renderTeapot(renderer);

    return 0;
}