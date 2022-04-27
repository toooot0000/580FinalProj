#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"
#include <vector>
#include <chrono>


void timingRender(auto&& lambda){
    auto start = std::chrono::system_clock::now();
    lambda();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Rendering time: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms\n";
}


void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/wooden stool.obj");
    mesh.setScale(6.5);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);

//    auto t = ObjLoader::loadTexture("obj/wooden stool texture.png");
//    mesh.setTexture(std::move(t));

    timingRender([&renderer, &mesh](){renderer.render(mesh);});
    renderer.flushToImg("wooden stool.tga");
}

void renderRayCastWoodenStool(Renderer& renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/wooden stool.obj");
    mesh.setScale(6.5);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);
    std::cout << "Total number of triangles: " << mesh.getTris().size() << "\n";

    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});

    std::cout << "KdTree layer: " << mesh.getRepresent()->layer << "\n";
    std::cout << "KdTree max Tri num: " << mesh.getRepresent()->maxTri << "\n";
    std::cout << "KdTree min Tri num: " << mesh.getRepresent()->minTri << "\n";

    renderer.flushToImg("wooden stool-ray trace.tga");
}

void renderRayCastPlainWoodenStool(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/wooden stool.obj");
    mesh.setScale(6.5);
    mesh.setZ(-1);
    mesh.setRotateX(25);
    mesh.setRotateZ(20);
    mesh.setRotateY(5);

//    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
//    teapot.setTexture(std::move(t));

    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});
    renderer.flushToImg("wooden stool-ray trace-plain.tga");
}

void renderTeapot(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(80);
    mesh.setRotateZ(60);
    timingRender([&renderer, &mesh](){renderer.render(mesh);});
    renderer.flushToImg("teapot.tga");
}

void renderRayCastTeapot(Renderer& renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(80);
    mesh.setRotateZ(60);
    std::cout << "Total number of triangles: " << mesh.getTris().size() << "\n";

    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});

    std::cout << "KdTree layer: " << mesh.getRepresent()->layer << "\n";
    std::cout << "KdTree max Tri num: " << mesh.getRepresent()->maxTri << "\n";
    std::cout << "KdTree min Tri num: " << mesh.getRepresent()->minTri << "\n";

    renderer.flushToImg("teapot-ray trace.tga");
}

void renderRayCastPlainTeapot(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(80);
    mesh.setRotateZ(60);

    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});

    renderer.flushToImg("teapot-ray trace-plain.tga");
}


void renderTeapotPatric(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(90);
    mesh.setRotateZ(60);


    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
    mesh.setTexture(std::move(t));

    timingRender([&renderer, &mesh](){renderer.render(mesh);});
    renderer.flushToImg("teapot-patric.tga");
}

void renderTeapotPatricRayCast(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/utah teapot.obj");
    mesh.setRotateY(60);
    mesh.setRotateX(90);
    mesh.setRotateZ(60);


    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
    mesh.setTexture(std::move(t));

    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});
    renderer.flushToImg("teapot-patric-ray trace.tga");
}


void renderLargeTeapot(Renderer &renderer){
    Mesh mesh = ObjLoader::load<Mesh<>>("obj/teapot.obj");
    mesh.setRotateY(90);
    mesh.setRotateX(30);
    mesh.setX(-3);
    mesh.setScale(1.2);
    timingRender([&renderer, &mesh](){renderer.render(mesh);});
    renderer.flushToImg("large teapot.tga");
}

void renderLargeTeapotRayCast(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::Mesh>("obj/teapot.obj");
    mesh.setRotateY(90);
    mesh.setRotateX(30);
    mesh.setX(-3);
    mesh.setScale(1.2);


    std::cout << "Total number of triangles: " << mesh.getTris().size() << "\n";
    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});
    std::cout << "KdTree layer: " << mesh.getRepresent()->layer << "\n";
    std::cout << "KdTree max Tri num: " << mesh.getRepresent()->maxTri << "\n";
    std::cout << "KdTree min Tri num: " << mesh.getRepresent()->minTri << "\n";

    renderer.flushToImg("large teapot-ray trace.tga");
}

void renderLargeTeapotRayCastPlain(Renderer &renderer){
    auto mesh = ObjLoader::load<RayCast::PlainMesh>("obj/teapot.obj");
    mesh.setRotateY(90);
    mesh.setRotateX(30);
    mesh.setX(-3);
    mesh.setScale(1.2);
    timingRender([&renderer, &mesh](){renderer.rayCastRender(mesh);});
    renderer.flushToImg("large teapot-ray trace-plain.tga");
}

int main()
{

//    Renderer renderer(256, 256, {
//            {-0.52, 0.38, 0.064}, {0.41, 0.56, 0.119}, {0.27, 0.08, 0.147},
//            {-0.17, -0.29, 0.249}, {0.58, -0.55, 0.104}, {-0.31, -0.71, 0.106},
//            {0.52, -0.38, 0.064}, {-0.27, -0.08, 0.147}
//    });

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
    renderer.clearBuffer();
    renderRayCastPlainTeapot(renderer);

//    renderTeapotPatric(renderer);
//    renderer.clearBuffer();
//    renderTeapotPatricRayCast(renderer);

//    renderLargeTeapot(renderer);
//    renderer.clearBuffer();
//    renderLargeTeapotRayCast(renderer);
//    renderer.clearBuffer();
//    renderLargeTeapotRayCastPlain(renderer);

//    renderWoodenStool(renderer);
//    renderer.clearBuffer();
//    renderRayCastWoodenStool(renderer);
//    renderer.clearBuffer();
//    renderRayCastPlainWoodenStool(renderer);
    return 0;
}