#include "shader.h"

Vec4f PhongShader::vertex(int iface, int nthvert) {
    payload.varying_uv[nthvert] = payload.model->uv(iface, nthvert);
    payload.varying_worldcoord[nthvert] = payload.model->vert(iface, nthvert);
    payload.varying_clipcoord[nthvert] = payload.uniform_viewport * payload.uniform_mvp * embed<4>(payload.varying_worldcoord[nthvert]);
    payload.varying_normal[nthvert] = payload.model->normal(iface, nthvert);
    return payload.varying_clipcoord[nthvert];
}

TGAColor PhongShader::fragment(Vec3f bary) {
    std::array<Vec4f, 3>& clipcoord = payload.varying_clipcoord;
    Vec3f zs(clipcoord[0][3], clipcoord[1][3],clipcoord[2][3]);
    float zt = 1.f/(bary[0]/zs[0] + bary[1]/zs[1] + bary[2]/zs[2]);
    Vec2f uv = perspective_interpolation(payload.varying_uv, bary, zs, zt);

    Vec3f world_pos = perspective_interpolation(payload.varying_worldcoord, bary, zs, zt);

    Vec3f normal;
    if (payload.model->has_normalmap()) {
        normal = payload.model->normal(uv);
    } else {
        // normal = (payload.varying_normal[0] + payload.varying_normal[1] + payload.varying_normal[2]) / 3;
        normal = perspective_interpolation(payload.varying_normal, bary, zs, zt);
    }

    float alpha = 50;
    if (payload.model->has_specularmap()) {
        alpha = payload.model->specular(uv);
    }

    Vec3f& light_dir = payload.uniform_light.light_dir;

    // Blinn-Phong Model
    TGAColor color(WHITE);
    if (payload.model->has_diffusemap()) {
        color = payload.model->diffuse(uv);
    }

    Vec3f view_dir = (payload.uniform_eye - world_pos).normalize();
    Vec3f half_vector = (light_dir + view_dir).normalize();
    Vec3f& ks = payload.uniform_light.specular;
    Vec3f& kd = payload.uniform_light.diffuse;
    Vec3f& ka = payload.uniform_light.ambient;

    Vec3f spec, diff, ambient;

    for (int i = 0; i < 3; i++) {
        spec[i] = ks[i] * std::pow(std::max(dot(normal, half_vector), 0.0f), alpha);
        diff[i] = kd[i] * std::max(0.f, dot(normal, light_dir));
        ambient[i] = ka[i];
    }

    Vec3f intensity = spec + diff + ambient;

    for (int i = 0; i < 3; i++) {
        color[i] = std::min(255, (int) (color[i] * intensity[i]));
    }

    return color;
}
