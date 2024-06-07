#pragma once
#include "../core/model.h"

const TGAColor WHITE(255,255,255);

struct Payload {
    Model* model;

    // Transform Matrices
    Mat4f uniform_mvp;
    Mat4f uniform_mit;
    Mat4f uniform_viewport;
    

    Vec3f uniform_light_dir;

    // Vertex
    std::array<Vec2f, 3> varying_uv;
};

class Shader {
public:
    Payload payload;
    virtual ~Shader() {}
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual TGAColor fragment(Vec3f bary) = 0;
    
};

class GouraudShader : public Shader {
public:
    virtual Vec4f vertex(int iface, int nthvert) override;
    virtual TGAColor fragment(Vec3f bary) override;
};

class PhongShader : public Shader {
public:
    virtual Vec4f vertex(int iface, int nthvert) override;
    virtual TGAColor fragment(Vec3f bary) override;
};
