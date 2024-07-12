#pragma once

#include "maths.h"
#include "tgaimage.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
    bool diffusemap_loaded;
    bool normalmap_loaded;
    bool specularmap_loaded;

    bool load_texture(const std::string& filename, TGAImage &img);
public:
    Model(const char *filename);
    ~Model();

    int nverts();
    int nfaces();

    Vec3f normal(int iface, int nthvert);
    Vec3f normal(Vec2f uv);
    Vec3f vert(int iface, int nthvert);
    Vec3f vert(int i);
    Vec2f uv(int iface, int nthvert);
    TGAColor diffuse(Vec2f uv);
    float specular(Vec2f uv);
    std::vector<int> face(int idx);

    bool load_diffusemap(const std::string& filename);
    bool load_normalmap(const std::string& filename);
    bool load_specularmap(const std::string& filename);
    bool has_diffusemap();
    bool has_normalmap();
    bool has_specularmap();
};

