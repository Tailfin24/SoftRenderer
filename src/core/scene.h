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
    int w, h;
    std::vector<float> zbuffer;
    Camera camera;
    // Vec3f light_dir;
    Light light;
public:
    TGAImage framebuffer;
    Scene(int width, int height);
    ~Scene();
    bool add_model(const std::string& obj_path);
    bool add_diffusemap(const std::string& path);
    bool add_normalmap(const std::string& path);
    bool add_specularmap(const std::string& path);
    void clear();

    void rasterize();
    int width() { return w; }
    int height() { return h; }

    void camera_rotate_around_target(Vec2f uv);
    void camera_move_target(Vec2f uv);
    void camera_scale(int ratio);
    void set_light_yaw(int ratio);
    void set_light_pitch(int ratio);

    void clear_framebuffer();
protected:
    void update_mvp();
};

