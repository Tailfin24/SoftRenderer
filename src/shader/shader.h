#pragma once
#include "../core/model.h"

const TGAColor WHITE(255,255,255);
const TGAColor BLACK(0,0,0);

struct Light {
    Vec3f light_dir;
    Vec3f specular;
    Vec3f diffuse;
    Vec3f ambient;
};

struct Payload {
    Model* model;

    // Transform Matrices
    Mat4f uniform_mvp;
    Mat4f uniform_mit;
    Mat4f uniform_viewport;
    

    // Vec3f uniform_light_dir;
    Light uniform_light;
    Vec3f uniform_eye;
    // Vertex
    std::array<Vec3f, 3> varying_worldcoord;
    std::array<Vec4f, 3> varying_clipcoord;
    std::array<Vec3f, 3> varying_normal;
    std::array<Vec2f, 3> varying_uv;
};

class Shader {
public:
    Payload payload;
    virtual ~Shader() {}
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual TGAColor fragment(Vec3f bary) = 0;
    
};

class PhongShader : public Shader {
public:
    virtual Vec4f vertex(int iface, int nthvert) override;
    virtual TGAColor fragment(Vec3f bary) override;
};
