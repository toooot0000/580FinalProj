//
// Created by Jerry Ye on 2022/3/24.
//

#include "Renderer.h"

#include <utility>
#include "Mesh.h"

Renderer::Renderer(int xRes, int yRes):
    xRes(xRes), yRes(yRes),
    aaSetting({{0, 0, 1}}),
    pixelBuffer(new Pixel[xRes*yRes]),
    tempBuffer(new Pixel[xRes*yRes])
{
}



Renderer::Renderer(int xRes, int yRes, std::vector<std::tuple<double, double, double>> aaSetting)
        :xRes(xRes), yRes(yRes), aaSetting(std::move(aaSetting)),
        pixelBuffer(new Pixel[xRes*yRes]),
        tempBuffer(new Pixel[xRes*yRes])
{
}


void Renderer::render(const Mesh<> &mesh)
{
//  DONE: Compute transformation matrix;
    Mat4 mVp = makeViewportTrans(xRes, yRes ),
        mPer = makePerspectiveProjectTrans(),
        mCmr = makeCameraTrans(camera.eye, camera.gaze, camera.viewUp),
        trans = mVp * mPer * mCmr
                * makeScaleTrans(mesh.getScale())
                * makeTranslateTrans(mesh.getX(), mesh.getY(), mesh.getZ())
                * makeXRotationTrans(mesh.getRotateX())
                * makeYRotationTrans(mesh.getRotateY())
                * makeZRotationTrans(mesh.getRotateZ());
     Mat4 nTrans = makeNormTrans(mCmr)
                   * makeXRotationTrans(mesh.getRotateX())
                   * makeYRotationTrans(mesh.getRotateY())
                   * makeZRotationTrans(mesh.getRotateZ());

    for(const auto& aaTp : aaSetting)
    {
        auto xOff = get<0>(aaTp);
        auto yOff = get<1>(aaTp);
        auto weight = get<2>(aaTp);
        clearBuffer(tempBuffer);

//  Loop triangles; For each triangle, apply transformation, interpolate color with Phong shading
        for (const Tri &tri: mesh.getTris())
        {
            std::array<Vec4, 3> v, norm;
            std::array<Vec3, 3> uv;

            v[0] = Vec4(tri[0].position, 1);
            v[1] = Vec4(tri[1].position, 1);
            v[2] = Vec4(tri[2].position, 1);

            uv[0] = tri[0].uvw;
            uv[1] = tri[1].uvw;
            uv[2] = tri[2].uvw;

            norm[0] = Vec4(tri[0].normal, 1);
            norm[1] = Vec4(tri[1].normal, 1);
            norm[2] = Vec4(tri[2].normal, 1);

//  Apply the transformation
            v[0] = std::move(trans * v[0]);
            v[1] = std::move(trans * v[1]);
            v[2] = std::move(trans * v[2]);

            v[0].homogenize();
            v[1].homogenize();
            v[2].homogenize();

            v[0][3] = std::abs(1 / v[0][3]);
            v[1][3] = std::abs(1 / v[1][3]);
            v[2][3] = std::abs(1 / v[2][3]);

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

            norm[0] = (nTrans * norm[0]);
            norm[1] = (nTrans * norm[1]);
            norm[2] = (nTrans * norm[2]);



//  Rasterization
            rasterize(mesh, v, norm, uv, tempBuffer, xOff, yOff);

        }

//        Write pixelBuffer from tempBuffer
        for (int i = 0; i < xRes * yRes; i++)
        {
            tempBuffer[i].color.scale(weight);
            pixelBuffer[i].color.translate(tempBuffer[i].color);
        }
    }
}


void Renderer::rasterize(const Mesh<> &mesh, std::array<Vec4, 3> &v, std::array<Vec4, 3> &norm, std::array<Vec3, 3> &uvs,
                         PixelBuffer buffer, double xOff, double yOff) // Phong shading!
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
    xMax = Util::clamp(xMax, xMin, xRes - 1);
    yMin = Util::clamp(yMin, 0, yRes - 1);
    yMax = Util::clamp(yMax, yMin, yRes - 1);

    auto l01 = Util::D2Line(v[0], v[1]);
    auto l12 = Util::D2Line(v[1], v[2]);
    auto l20 = Util::D2Line(v[2], v[0]);
    double k01 = l01(v[2]);
    double k12 = l12(v[0]);
    double k20 = l20(v[1]);
    double p01 = l01(-1, -1)*k01;
    double p12 = l12(-1, -1)*k12;
    double p20 = l20(-1, -1)*k20;


    Vec3
            nxs = { norm[0].getX(), norm[1].getX(), norm[2].getX() },
            nys = { norm[0].getY(), norm[1].getY(), norm[2].getY() },
            nzs = { norm[0].getZ(), norm[1].getZ(), norm[2].getZ() },
            ws = {v[0].getW(), v[1].getW(), v[2].getW()},
            zs = { v[0].getZ(), v[1].getZ(), v[2].getZ() },
            us{uvs[0][0], uvs[1][0], uvs[2][0]},
            vs{uvs[0][1], uvs[1][1], uvs[2][1]},
            curNorm;


//        Write tempBuffer
    for (int x0 = xMin; x0 <= xMax; x0++) {
        auto x = static_cast<double>(x0) + xOff;
        auto t01 = l01(x, static_cast<double>(yMin) + yOff);
        auto t12 = l12(x, static_cast<double>(yMin) + yOff);
        auto t20 = l20(x, static_cast<double>(yMin) + yOff);
        for (int y0 = yMin; y0 <= yMax; y0++) {
            auto theta = t01 / k01,
                    alpha = t12 / k12,
                    beta = t20 / k20;
            if(
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
//                Backface culling
//                Remove all pixel that face back to the camera
//                if(curNorm.getZ() < 0){
//                    continue;
//                }
                putPixel(buffer, y0, x0, {computeColor(mesh, curNorm, curU, curV), z});
            }
            t01 += l01.B;
            t12 += l12.B;
            t20 += l20.B;
        }
    }
}


void Renderer::sortVertices(std::array<Vec4, 3> &v, std::array<Vec4, 3> &n, std::array<Vec3, 3> &uvs)
{
    if (v[1].getY() < v[0].getY()) {
        std::swap(v[1], v[0]);
        std::swap(n[1], n[0]);
        std::swap(uvs[1], uvs[0]);
    }
    if (v[2].getY() < v[0].getY()) {
        std::swap(v[2], v[0]);
        std::swap(n[2], n[0]);
        std::swap(uvs[2], uvs[0]);
    }


    if (Util::D2Line(v[0], v[1])(v[2]) * (v[0].getX() - v[1].getX()) > 0) {
        std::swap(v[2], v[1]);
        std::swap(n[2], n[1]);
        std::swap(uvs[2], uvs[1]);
    }
}


Util::Color Renderer::computeColor(const Mesh<> &mesh, const Vec3& norm, double u, double v)
{
//  Color = kd*[light.intensity*max(0, norm * (-light.dir))]
//          + ks*[light.intensity*max(0, (norm * h)^s )]
//          + ka*light.intensity
//  h = (-camera.gaze in image space which is (0, 0, -1) - light.dir).normalized();
    Vec3 sumD(0, 0, 0), sumS(0, 0, 0), cmr(0, 0, -1);

    for(const auto& light : lights){
//        If light shot at the back of the pixel, skip this light;
        if(light.direction.dot(norm)*cmr.dot(norm) < 0){
            continue;
        }
//        Compute sumS;
        Vec3 h = (cmr + light.direction).normalized();
        double temp = std::pow(std::abs(norm.dot(h)), mesh.getS());
        sumS += light.color.toVec3().scaled(temp);
        sumS.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});
//        Compute sumD
        temp = std::abs(norm.dot(light.direction));
        sumD += light.color.toVec3().scaled(temp);
        sumD.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});
    }

    Vec3 ret;
    if(mesh.getTexture().isSet){
        auto tex = mesh.getTexture().lookup(u, v).toVec3();
        ret = tex.scaled(ambientLight.color.toVec3())
                + tex.scaled(sumD)
                + tex.scaled(sumS);
    } else {
        ret = mesh.getKa().scaled(ambientLight.color.toVec3())
              + mesh.getKd().scaled(sumD)
              + mesh.getKs().scaled(sumS);
    }
    ret.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});
    return Util::Color(ret);
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
    delete[] tempBuffer;
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


void Renderer::putPixel(PixelBuffer buffer, int row, int col, Pixel &&p) const
{
    int ind = row*yRes + col;
    if(buffer[ind].z > p.z){
        return;
    }
    buffer[ind] = std::move(p);
}

void Renderer::clearBuffer(PixelBuffer buffer) const
{
    for(int i = 0; i<xRes * yRes; i++){
        buffer[i].color.r = 0;
        buffer[i].color.g = 0;
        buffer[i].color.b = 0;
        buffer[i].z = -2;
    }
}

void Renderer::rayCastRender(RayCast::MeshInterface &mesh)
{
    Mat4 mVp = makeViewportTrans(xRes, yRes ),
        mPer = makePerspectiveProjectTrans(),
        mCmr = makeCameraTrans(camera.eye, camera.gaze, camera.viewUp),
        trans = mCmr
                * makeScaleTrans(mesh.getScale())
                * makeTranslateTrans(mesh.getX(), mesh.getY(), mesh.getZ())
                * makeXRotationTrans(mesh.getRotateX())
                * makeYRotationTrans(mesh.getRotateY())
                * makeZRotationTrans(mesh.getRotateZ());
    Mat4 nTrans = makeNormTrans(mCmr)
                  * makeXRotationTrans(mesh.getRotateX())
                  * makeYRotationTrans(mesh.getRotateY())
                  * makeZRotationTrans(mesh.getRotateZ());


    mesh.applyTransformation(trans, nTrans);

    double left = -1;
    double b = -1;
    double n = -1.5;
    double r = 1;
    double top = 1;

    double xStep = (r - left) / xRes, yStep = (top - b) / yRes;

    double x = left - xStep, y;

    for(int i = 0; i < xRes; i++){
        x += xStep;
        y = b - yStep;
        for(int j = 0; j < yRes; j++){
//            Make ray
            y += yStep;
            RayCast::Ray ray({0, 0, 0}, {x, y, n});
            auto collRes = mesh.detectCollision(ray);
            if(collRes){
                auto bct = ray.triangleIntersect(collRes);
                auto beta = bct[0], gamma = bct[1], alpha = 1 - beta - gamma, t = bct[2];
                auto hitPoint = t * ray.getDir();
                auto curNorm = alpha * (*collRes)[0].normal + beta *(*collRes)[1].normal + gamma * (*collRes)[2].normal;
                curNorm.normalize();
                auto l = (*collRes).getWs().dot({alpha, beta, gamma});
                auto curU = ((*collRes)[0].uvw[0] * alpha + (*collRes)[1].uvw[0] * beta +(*collRes)[2].uvw[0] * gamma )/l;
                auto curV = ((*collRes)[0].uvw[1] * alpha + (*collRes)[1].uvw[1] * beta +(*collRes)[2].uvw[1] * gamma )/l;
                auto color = computeColor(mesh, *collRes, hitPoint, curNorm, curU, curV);
                putPixel(pixelBuffer, j, i, {color, 0});
            }
        }
    }
}

Util::Color Renderer::computeColor(const RayCast::MeshInterface &mesh, const RayCast::Tri &hitTri, const Vec3 &hitPoint,
                                   const Vec3 &norm, double u, double v)
{

//  Color = kd*[light.intensity*max(0, norm * (-light.dir))]
//          + ks*[light.intensity*max(0, (norm * h)^s )]
//          + ka*light.intensity
//  h = (-camera.gaze in image space which is (0, 0, -1) - light.dir).normalized();
    Vec3 sumD(0, 0, 0), sumS(0, 0, 0);
    const Vec3& cmr = hitPoint;

    for(const auto& light : lights){
//        If light shot at the back of the pixel, skip this light;
        if(light.direction.dot(norm)*cmr.dot(norm) < 0){
            continue;
        }
//        If this light can't hit this point
        RayCast::Ray lightRay{hitPoint + 0.01*light.direction.negatived(), light.direction.negatived()};
        if(mesh.detectCollision(lightRay)){
            continue;
        }
//        Compute sumS;
        Vec3 h = (cmr + light.direction).normalized();
//        Vec3 h = (Vec3{ 0, 0, -1 } + light.direction).normalized();
        double temp = std::pow(std::abs(norm.dot(h)), mesh.getS());
        sumS += light.color.toVec3().scaled(temp);
        sumS.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});

//        Compute sumD
        temp = std::abs(norm.dot(light.direction));
        sumD += light.color.toVec3().scaled(temp);
        sumD.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});
    }

    Vec3 ret;
    if(mesh.getTexture().isSet){
        auto tex = mesh.getTexture().lookup(u, v).toVec3();
        ret = tex.scaled(ambientLight.color.toVec3())
              + tex.scaled(sumD)
              + tex.scaled(sumS);
    } else {
        ret = mesh.getKa().scaled(ambientLight.color.toVec3())
              + mesh.getKd().scaled(sumD)
              + mesh.getKs().scaled(sumS);
    }
    ret.clamp(Vec3{0, 0, 0}, Vec3{1, 1, 1});
    return Util::Color(ret);
}

void Renderer::clearBuffer()
{
    clearBuffer(pixelBuffer);
    clearBuffer(tempBuffer);
}

Light::Light(const Vec3& direction, Util::Color color) : direction(std::move(direction.normalized())), color(std::move(color)) {}
