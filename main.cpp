#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"

#include "lodepng-master/lodepng.h"
#include <vector>

void renderTeapot(Renderer &renderer){
    Mesh teapot = ObjLoader::load<Mesh<>>("obj/teapot.obj");
    teapot.setRotateY(90);
    teapot.setRotateX(30);
    teapot.setX(-5);

    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
    teapot.setTexture(std::move(t));

    renderer.render(teapot);
    renderer.flushToImg("teapot.tga");
}

void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);

    auto t = ObjLoader::loadTexture("obj/wooden stool texture.png");
    mesh.setTexture(std::move(t));

    renderer.render(mesh);
    renderer.flushToImg("wooden stool.tga");
}

void renderTest(Renderer& renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);
    renderer.rayCastRender(mesh);
    renderer.flushToImg("test.tga");
}


int main()
{

    Renderer renderer(128, 128);

    renderer.addLight(
            {Vec3{-0.7071, 0.7071, 0}, Util::Color({0, 0, 1}) }
            );
    renderer.addLight(
            {Vec3{0, -0.7071, -0.7071}, Util::Color({1, 0, 0}) }
            );
    renderer.addLight(
            {Vec3{0.7071, 0.0, -0.7071}, Util::Color({0, 1, 0})}
            );

    renderer.setCamera({
                               Vec3{0, -25, 0},
                               Vec3{0, 0, 1},
                               Vec3{0, 1, 0}}
                               );
//    renderTeapot(renderer);
//    renderWoodenStool(renderer);
    renderTest(renderer);
    return 0;
}