#include <iostream>
#include "ObjLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "util/util.h"

#include "lodepng-master/lodepng.h"
#include <vector>

void renderTeapot(Renderer &renderer){
    Mesh teapot = ObjLoader::load("obj/teapot.obj");
    teapot.setRotateY(90);
    teapot.setRotateX(30);
//    teapot.setRotateZ(90);
    teapot.setX(-5);

//    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
//    teapot.setTexture(std::move(t));

    renderer.render(teapot);
    renderer.flushToImg("teapot.tga");
}

void renderWoodenStool(Renderer &renderer){
    Mesh mesh = ObjLoader::load("obj/wooden stool.obj");
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

void testTexture(){

    auto t = ObjLoader::loadTexture("obj/patrick-star.png");
    std::vector<unsigned char> image;
    for(int j = 0; j<t.height; j++){
        for(int i = 0; i<t.width; i++){
            double u = (double)i / static_cast<double>(t.width), v = (double)j / static_cast<double>(t.height);
            auto c = t.lookup(u, v);
            image.emplace_back(c.r);
            image.emplace_back(c.g);
            image.emplace_back(c.b);
            image.emplace_back(c.a);
        }
    }
    auto error = lodepng::encode("test.png", image,  t.width, t.height);
//    auto error = lodepng::encode("test.png", image,  1, 1);
    if(error)
        std::cout << lodepng_error_text(error) << std::endl;
}


int main()
{
//    testTexture();
//    Renderer renderer(1024, 1024, {
//            {-0.52, 0.38, 0.064}, {0.41, 0.56, 0.119}, {0.27, 0.08, 0.147},
//            {-0.17, -0.29, 0.249}, {0.58, -0.55, 0.104}, {-0.31, -0.71, 0.106},
//            {0.52, -0.38, 0.064}, {-0.27, -0.08, 0.147}
//    });

    Renderer renderer(1024, 1024);

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
    renderTeapot(renderer);
//    renderWoodenStool(renderer);
    return 0;
}