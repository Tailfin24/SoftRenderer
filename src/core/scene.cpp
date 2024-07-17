#include "./scene.h"

Scene::Scene(int width, int height)
    :  w(width), h(height), zbuffer(width*height)
{

    // 设置初始相机位置
    camera.eye = Vec3f(0.f, 0.f, 3.5f);
    camera.center = Vec3f(0.0f, 0.0f, 0.0f);
    camera.up = Vec3f(0.0f, 1.0f, 0.0f);

    // 创建着色器
    shader = new PhongShader();

    // 初始化光照
    light.light_dir = Vec3f(1,0,0).normalize();
    light.ambient = Vec3f(0.1, 0.1, 0.1);
    light.diffuse = Vec3f(1.0, 1.0, 1.0);
    light.specular = Vec3f(1.0, 1.0, 1.0);
    shader->payload.uniform_light = light;

    // 初始化着色器

    Mat4f proj = Mat::projection(camera.fov, camera.aspect, camera.znear, camera.zfar);
    Mat4f view = Mat::lookat(camera.eye, camera.center, camera.up);
    Mat4f viewport = Mat::viewport(width/8, height/8, width*3/4, height*3/4);
    shader->payload.uniform_eye = camera.eye;
    shader->payload.uniform_mvp = proj*view;
    shader->payload.uniform_viewport = viewport;
    // shader->payload.uniform_light_dir = light_dir;

    // 创建帧缓冲
    framebuffer = TGAImage(width, height, TGAImage::RGBA);
}

Scene::~Scene() {
    delete shader;
    for (Model* model : models) {
        delete model;
    }
}

bool Scene::add_model(const std::string &obj_path) {
    models.push_back(new Model(obj_path.c_str()));
    return true;

}

bool Scene::add_diffusemap(const std::string& path) {
    return models.back()->load_diffusemap(path);
}

bool Scene::add_normalmap(const std::string& path) {
    return models.back()->load_normalmap(path);
}

bool Scene::add_specularmap(const std::string& path) {
    return models.back()->load_specularmap(path);
}

void Scene::clear() {
    for (auto model : models) {
        delete model;
    }
    models.clear();
}

void Scene::rasterize() {
    if (!shader) {
        throw std::runtime_error("Shader is null.");
    }

    clear_framebuffer();

    for (Model* model : models) {
        shader->payload.model = model;
        for (int i = 0; i < model->nfaces(); i++) {
            triangle(framebuffer, zbuffer, shader, i);
        }
    }
}

void Scene::clear_framebuffer() {
    framebuffer.clear();
    std::fill(zbuffer.begin(), zbuffer.end(), 0.f);
}


void Scene::camera_rotate_around_target(Vec2f uv) {
    camera.rotate_around_target(uv);
    update_mvp();
}

void Scene::camera_move_target(Vec2f uv) {
    camera.move_target(uv);
    update_mvp();
}

void Scene::camera_scale(int ratio) {
    camera.scale(ratio);
    update_mvp();
}

void Scene::update_mvp() {

    Mat4f view = Mat::lookat(camera.eye, camera.center, camera.up);
    Mat4f proj = Mat::projection(camera.fov, camera.aspect, camera.znear, camera.zfar);
    shader->payload.uniform_eye = camera.eye;
    shader->payload.uniform_mvp = proj*view;
}

void Scene::set_light_yaw(int ratio) {
    float yaw = (float)ratio/180*M_PI;
    float pitch = (float)std::asin(light.light_dir.y);
    light.light_dir.x = (float)std::cos(yaw)*std::cos(pitch);
    light.light_dir.z = (float)std::sin(yaw)*std::cos(pitch);
    shader->payload.uniform_light = light;
}

void Scene::set_light_pitch(int ratio) {
    float pitch = (float)ratio/180*M_PI;
    float yaw = (float)std::atan2(light.light_dir.z, light.light_dir.x);
    light.light_dir.x = (float)std::cos(yaw)*std::cos(pitch);
    light.light_dir.z = (float)std::sin(yaw)*std::cos(pitch);
    light.light_dir.y = (float)std::sin(pitch);
    shader->payload.uniform_light = light;
}
