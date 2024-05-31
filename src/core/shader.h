#pragma once
#include "../core/model.h"
#include <array>

const TGAColor WHITE(255,255,255);

struct Payload {
    Model* model;

    // Transform Matrices
    Mat4f uniform_mvp;
    Mat4f uniform_mit;
    Mat4f uniform_viewport;
    

    Vec3f uniform_light_dir;

    // Vertex
    std::array<Vec2f, 2> varying_uv;
};

class Shader {
public:
    Payload payload;
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(const Vec3f& bary, TGAColor &color) = 0;
    
};

class GouraudShader : public Shader {
public:
    virtual Vec4f vertex(int iface, int nthvert) {
        payload.varying_uv.set_col(nthvert, payload.model->uv(iface, nthvert));
        Vec4f gl_vertex = embed<4>(payload.model->vert(iface, nthvert));
        gl_vertex = payload.uniform_viewport * payload.uniform_mvp * gl_vertex;
        return gl_vertex;
    }

    virtual bool fragment(const Vec3f& bary, TGAColor& color) {
        Vec2f uv(0,0);
        for (int i = 0; i < 3; i++) {
            uv += payload.varying_uv[i] * bary[i];
        }


        Vec3f normal = proj<3>(payload.uniform_mit*embed<4>(payload.model->normal(uv))).normalize();
        Vec3f light = proj<3>(payload.uniform_mvp * embed<4>(payload.uniform_light_dir)).normalize();
        
        // Phong Model
        Vec3f reflect = (normal*(normal*light*2.f)-light).normalize();
        float spec = pow(std::max(reflect.z,0.0f), payload.model->specular(uv));
        float diff = std::max(0.f, normal*light);
        float ambient = 0.1;
        float intensity = .6*spec + diff + ambient;

        color = payload.model->diffuse(uv);
        for (int i = 0; i < 3; i++) {
            color[i] = std::min<float>(255, color[i] * intensity);
        }

        return false;
    }
};
