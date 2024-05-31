#pragma once

#include "../core/model.h"
#include "../core/camera.h"
#include "../core/rasterizer.h"
#include "../shader/shader.h"
#include <exception>

class Scene {
protected:
    Shader* shader;
    std::vector<Model*> models;
    TGAImage framebuffer;
    TGAImage zbuffer;
    Camera camera;
    Vec3f light_dir;
public:
    Scene(int width, int height, const std::string& scene_name = "african_head");
    ~Scene();
    void load_scene(const std::string& scene_name);
    void rasterize();
    TGAImage get_framebuffer();
    TGAImage get_zbuffer();

    void camera_translate(const Vec3f& xyz);
    void camera_translate(float x, float y, float z);
    void camera_scale(float scale);
    void camera_rotate_vertical(float phi);
    void camera_rotate_horizontal(float theta);

    void update_light_dir();

    void clear_framebuffer();
protected:
    void update_mvp();
};

