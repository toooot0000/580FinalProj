#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"
#include <vector>


void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);

//    auto t = ObjLoader::loadTexture("obj/wooden stool texture.png");
//    mesh.setTexture(std::move(t));

    renderer.render(mesh);
    renderer.flushToImg("wooden stool.tga");
}

void renderRayCastWoodenStool(Renderer& renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);
    std::cout << "Total number of triangles: " << mesh.getTris().size() << "\n";

    renderer.rayCastRender(mesh);

    std::cout << "KdTree layer: " << mesh.getRepresent()->layer << "\n";
    std::cout << "KdTree max Tri num: " << mesh.getRepresent()->maxTri << "\n";
    std::cout << "KdTree min Tri num: " << mesh.getRepresent()->minTri << "\n";

    renderer.flushToImg("wooden stool-ray trace.tga");
}

void renderRayCastPlainWoodenStool(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/wooden stool.obj");
    mesh.setScale(6);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);

//    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
//    teapot.setTexture(std::move(t));

    renderer.rayCastRender(mesh);
    renderer.flushToImg("wooden stool-ray trace-plain.tga");
}

void renderTeapot(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(90);
    mesh.setRotateZ(60);
//    mesh.setX(-5);
    renderer.render(mesh);
    renderer.flushToImg("teapot.tga");
}

void renderRayCastTeapot(Renderer& renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(90);
    mesh.setRotateZ(60);
    std::cout << "Total number of triangles: " << mesh.getTris().size() << "\n";

    renderer.rayCastRender(mesh);

    std::cout << "KdTree layer: " << mesh.getRepresent()->layer << "\n";
    std::cout << "KdTree max Tri num: " << mesh.getRepresent()->maxTri << "\n";
    std::cout << "KdTree min Tri num: " << mesh.getRepresent()->minTri << "\n";

    renderer.flushToImg("teapot-ray trace.tga");
}

void renderRayCastPlainTeapot(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(90);
    mesh.setRotateZ(60);

//    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
//    teapot.setTexture(std::move(t));

    renderer.rayCastRender(mesh);
    renderer.flushToImg("teapot-ray trace-plain.tga");
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
    renderer.setCamera({Vec3{0, -25, 0},Vec3{0, 0, 1},Vec3{0, 1, 0}});

    renderTeapot(renderer);
    renderer.clearBuffer();
    renderRayCastTeapot(renderer);
//    renderer.clearBuffer();
//    renderRayCastPlainTeapot(renderer);

//    renderWoodenStool(renderer);
//    renderer.clearBuffer();
//    renderRayCastWoodenStool(renderer);
//    renderer.clearBuffer();
//    renderRayCastPlainWoodenStool(renderer);
    return 0;
}