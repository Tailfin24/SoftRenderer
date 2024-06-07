#include "shader.h"

Vec4f GouraudShader::vertex(int iface, int nthvert) {
    payload.varying_uv[nthvert] = payload.model->uv(iface, nthvert);
    payload.varying_worldcoord[nthvert] = payload.model->vert(iface, nthvert);
    payload.varying_clipcoord[nthvert] = payload.uniform_viewport * payload.uniform_mvp * embed<4>(payload.varying_worldcoord[nthvert]);
    return payload.varying_clipcoord[nthvert];
}

TGAColor GouraudShader::fragment(Vec3f bary) {
    std::array<Vec4f, 3>& clipcoord = payload.varying_clipcoord;
    Vec3f zs(clipcoord[0][3], clipcoord[1][3],clipcoord[2][3]);
    float zt = 1.f/(bary[0]/zs[0] + bary[1]/zs[1] + bary[2]/zs[2]);
    Vec2f uv = perspective_interpolation(payload.varying_uv, bary, zs, zt);

    Vec3f normal = proj<3>(payload.uniform_mit*embed<4>(payload.model->normal(uv))).normalize();
    Vec3f light = proj<3>(payload.uniform_mvp * embed<4>(payload.uniform_light_dir)).normalize();
    // Phong Model
    float alpha = payload.model->specular(uv);
    Vec3f reflect = (normal*(normal*light*2.f)-light).normalize();
    float spec = std::pow(std::max(reflect.z,0.0f), alpha);
    float diff = std::max(0.f, normal*light);
    float ambient = 0.1;
    float intensity = .6*spec + diff + ambient;
    TGAColor color = payload.model->diffuse(uv);
    for (int i = 0; i < 3; i++) {
        color[i] = std::min<float>(255, color[i] * intensity);
    }

    return color;
}
