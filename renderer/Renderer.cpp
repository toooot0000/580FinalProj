//
// Created by Jerry Ye on 2022/3/24.
//

#include "Renderer.h"
#include "Mesh.h"

Renderer::Renderer(int xRes, int yRes):
    xRes(xRes), yRes(yRes),
    pixelBuffer(new Pixel[xRes*yRes])
{}

void Renderer::render(const Mesh &mesh)
{
//  DONE: Compute transformation matrix;
    Mat4 mVp = makeViewportTrans(xRes, yRes),
        mPer = makePerspectiveProjectTrans(),
        mCmr = makeCameraTrans(camera.eye, camera.gaze, camera.viewUp),
        trans = mVp * mPer * mCmr
                * makeScaleTrans(mesh.getScale())
                * makeTranslateTrans(mesh.getX(), mesh.getY(), mesh.getZ())
                * makeXRotationTrans(mesh.getRotateX())
                * makeYRotationTrans(mesh.getRotateY())
                * makeZRotationTrans(mesh.getRotateZ());
     Mat4 nTrans = makeNormTrans(makeCameraTrans(camera.eye, camera.gaze, camera.viewUp))
                   * makeXRotationTrans(mesh.getRotateX())
                   * makeYRotationTrans(mesh.getRotateY())
                   * makeZRotationTrans(mesh.getRotateZ());
//  Loop triangles; For each triangle, apply transformation, interpolate color with Phong shading
    for(const Tri& indTri : mesh.getTris()){
        Vec4 v[3], norm[3];
        Vec3 uv[3];

        v[0] = Vec4(mesh.getVertices()[indTri.vers[0] - 1], 1);
        v[1] = Vec4(mesh.getVertices()[indTri.vers[1] - 1], 1);
        v[2] = Vec4(mesh.getVertices()[indTri.vers[2] - 1], 1);

        uv[0] = mesh.getUvs()[indTri.uv[0] - 1];
        uv[1] = mesh.getUvs()[indTri.uv[1] - 1];
        uv[2] = mesh.getUvs()[indTri.uv[2] - 1];

        norm[0] = Vec4(mesh.getNorms()[indTri.norm[0] - 1].normalized(), 1);
        norm[1] = Vec4(mesh.getNorms()[indTri.norm[1] - 1].normalized(), 1);
        norm[2] = Vec4(mesh.getNorms()[indTri.norm[2] - 1].normalized(), 1);

//  Apply the transformation
        v[0] = std::move(trans*v[0]);
        v[1] = std::move(trans*v[1]);
        v[2] = std::move(trans*v[2]);

        v[0].homogenize();
        if(v[0].getZ()>0) continue;
        v[1].homogenize();
        if(v[1].getZ()>0) continue;
        v[2].homogenize();
        if(v[2].getZ()>0) continue;

        v[0][3] = std::abs(1/v[0][3]);
        v[1][3] = std::abs(1/v[1][3]);
        v[2][3] = std::abs(1/v[2][3]);

        Vec3
            ws{v[0][3], v[1][3], v[2][3]},
            us{uv[0][0], uv[1][0], uv[2][0]},
            vs{uv[0][1], uv[1][1], uv[2][1]};

        us = ws * us;
        vs = ws * vs;

        uv[0][0] = us[0];
        uv[1][0] = us[1];
        uv[2][0] = us[2];

        uv[0][1] = vs[0];
        uv[1][1] = vs[1];
        uv[2][1] = vs[2];

        norm[0] = (nTrans * norm[0]).normalized();
        norm[1] = (nTrans * norm[1]).normalized();
        norm[2] = (nTrans * norm[2]).normalized();


//  Rasterization
        rasterize(mesh, v, norm, uv);
    }
}

void Renderer::rasterize(const Mesh& mesh, Vec4 v[3], Vec4 norm[3], Vec3 uvs[3]) // Phong shading!
{
// Sort Vertices
    sortVertices(v, norm, uvs);

    auto xMin = static_cast<int>(
            std::floor(Util::min(v[0].getX(), v[1].getX(), v[2].getX()))
            );
    auto yMin = static_cast<int>(
            std::floor(Util::min(v[0].getY(), v[1].getY(), v[2].getY()))
            );
    auto xMax = static_cast<int>(
            std::ceil(Util::max(v[0].getX(), v[1].getX(), v[2].getX()))
            );
    auto yMax = static_cast<int>(
            std::ceil(Util::max(v[0].getY(), v[1].getY(), v[2].getY()))
            );

    xMin = Util::clamp(xMin, 0, xRes - 1);
    xMax = Util::clamp(xMax, 0, xRes - 1);
    yMin = Util::clamp(yMin, 0, yRes - 1);
    yMax = Util::clamp(yMax, 0, yRes - 1);

    auto l01 = Util::D2Line(v[0], v[1]);
    auto l12 = Util::D2Line(v[1], v[2]);
    auto l20 = Util::D2Line(v[2], v[0]);
    float k01 = l01(v[2]);
    float k12 = l12(v[0]);
    float k20 = l20(v[1]);
    float p01 = l01(-1, -1)*k01;
    float p12 = l12(-1, -1)*k12;
    float p20 = l20(-1, -1)*k20;


    Vec3
            nxs = { norm[0].getX(), norm[1].getX(), norm[2].getX() },
            nys = { norm[0].getY(), norm[1].getY(), norm[2].getY() },
            nzs = { norm[0].getZ(), norm[1].getZ(), norm[2].getZ() },
            ws = {v[0].getW(), v[1].getW(), v[2].getW()},
            zs = { v[0].getZ(), v[1].getZ(), v[2].getZ() },
            us{uvs[0][0], uvs[1][0], uvs[2][0]},
            vs{uvs[0][1], uvs[1][1], uvs[2][1]},
            curNorm;

    for (int x = xMin; x <= xMax; x++) {
        auto t01 = l01(x, yMin);
        auto t12 = l12(x, yMin);
        auto t20 = l20(x, yMin);
        for (int y = yMin; y <= yMax; y++) {
            auto theta = t01 / k01,
                    alpha = t12 / k12,
                    beta = t20 / k20;
            if(
//                    (t01>=0 && t12>=0 && t20>=0)
//                    || (t01<=0 && t12<=0 && t20<=0)
                    (alpha>=0 && beta >= 0 && theta>=0)
                    && (alpha > 0 || p12>0)
                    && (beta > 0 || p20>0)
                    && (theta > 0 || p01>0)
            ){
                Vec3 fs{ alpha, beta, theta };
                auto z = fs.dot(zs);

                auto l = fs.dot(ws);
                auto curU = fs.dot(us)/l;
                auto curV = fs.dot(vs)/l;

                curNorm[0] = fs.dot(nxs);
                curNorm[1] = fs.dot(nys);
                curNorm[2] = fs.dot(nzs);

                curNorm.normalize();
                putPixel(y, x, {computeColor(mesh, curNorm, curU, curV), z});
            }
            t01 += l01.B;
            t12 += l12.B;
            t20 += l20.B;
        }
    }


}

void Renderer::sortVertices(Vec4 *v, Vec4 *n, Vec3 *uvs)
{
    if (v[1].getY() < v[0].getY()) {
        std::swap(v[1], v[0]);
        if (n) {
            std::swap(n[1], n[0]);
        }
        if (uvs) {
            std::swap(uvs[1], uvs[0]);
        }
    }
    if (v[2].getY() < v[0].getY()) {
        std::swap(v[2], v[0]);
        if (n) {
            std::swap(n[2], n[0]);
        }
        if (uvs) {
            std::swap(uvs[2], uvs[0]);
        }
    }


    if (Util::D2Line(v[0], v[1])(v[2]) * (v[0].getX() - v[1].getX()) > 0) {
        std::swap(v[2], v[1]);
        if (n) {
            std::swap(n[2], n[1]);
        }
        if (uvs) {
            std::swap(uvs[2], uvs[1]);
        }
    }
}

Util::Color Renderer::computeColor(const Mesh &mesh, const Vec3& norm, float u, float v)
{
//  Color = kd*[light.intensity*max(0, norm * (-light.dir))]
//          + ks*[light.intensity*max(0, (norm * h)^s )]
//          + ka*light.intensity
//  h = (-camera.gaze in image space which is (0, 0, -1) - light.dir).normalized();
//    assert(norm.length() == 1);
    Vec3 sumD(0, 0, 0), sumS(0, 0, 0);

    for(const auto& light : lights){
//        If light shot at the back of the triangle, skip this light;
        if(light.direction.dot(norm) > 0){
            continue;
        }
//        Compute sumS;
        Vec3 h = (Vec3(0, 0, -1) - light.direction).normalized();
        float temp = std::pow((norm.dot(h) < 0? 0 : norm.dot(h)), mesh.getS());
        sumS += light.color.toVec3().scaled(temp);
        sumS = clamp(sumS, {0, 0, 0}, {1, 1, 1});
//        Compute sumD
        temp = norm.dot(light.direction.negative());
        sumD += light.color.toVec3().scaled(Util::max(temp, 0.f));
        sumD = clamp(sumD, {0, 0, 0}, {1, 1, 1});
    }

    Vec3 ret = mesh.getKa().scaled(ambientLight.color.toVec3())
            + mesh.getKd().scaled(sumD)
            + mesh.getKs().scaled(sumS);
    clamp(ret, {0, 0, 0}, {1, 1, 1});
    return Util::Color(ret);
}

void Renderer::putPixel(int row, int col, Pixel&& p)
{
    int ind = row*yRes + col;
   if(pixelBuffer[ind].z < p.z){
       return;
   }
   pixelBuffer[ind] = std::move(p);
}

void Renderer::flushToImg(const char *name)
{
    TGAImage img(xRes, yRes, TGAImage::RGB);
    for(int i = 0; i< xRes; i++){
        for(int j = 0; j<yRes; j++){
            img.set(i, j, pixelBuffer[i * yRes + j].color);
        }
    }
    img.write_tga_file(name);
}

Renderer::~Renderer()
{
    delete[] pixelBuffer;
}

void Renderer::addLight(const Light &light)
{
    lights.emplace_back(light);
}

void Renderer::addLight(Light &&light)
{
    lights.emplace_back(std::move(light));
}

void Renderer::setCamera(const Camera &camera)
{
    Renderer::camera = camera;
}

void Renderer::setAmbientLight(const Light &ambientLight)
{
    Renderer::ambientLight = ambientLight;
}

