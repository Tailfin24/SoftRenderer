#include "shader.h"

Vec4f GouraudShader::vertex(int iface, int nthvert) {
    payload.varying_uv[nthvert] = payload.model->uv(iface, nthvert);
    Vec4f gl_vertex = embed<4>(payload.model->vert(iface, nthvert));
    gl_vertex = payload.uniform_viewport * payload.uniform_mvp * gl_vertex;
    return gl_vertex;
}

TGAColor GouraudShader::fragment(Vec3f bary) {
    Vec2f uv(0.f, 0.f);
    for (int i = 0; i < 3; i++) {
        uv = uv + payload.varying_uv[i] * bary[i];
    }

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
