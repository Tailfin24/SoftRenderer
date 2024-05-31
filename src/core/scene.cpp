#include "./scene.h"

Scene::Scene(int width, int height, const std::string& scene_name)
    : camera(Vec3f(0,0,0), std::sqrt(10), 0.7f, 0.6f, 4.f/3.f)
{
    // 加载模型
    load_scene(scene_name);
    // 创建着色器
    shader = new GouraudShader();


    // 初始化着色器
    light_dir = Vec3f(1,1,1).normalize();
    Mat4f proj = Mat::projection(-1.f/(camera.get_eye()-camera.get_center()).norm());
    Mat4f view = Mat::lookat(camera.get_eye(), camera.get_center(), camera.get_up());
    Mat4f viewport = Mat::viewport(width/8, height/8, width*3/4, height*3/4);
    shader->payload.uniform_mvp = proj*view;
    shader->payload.uniform_mit = shader->payload.uniform_mvp.invert_transpose();
    shader->payload.uniform_viewport = viewport;
    shader->payload.uniform_light_dir = light_dir;
    // 创建缓冲
    framebuffer = TGAImage(width, height, TGAImage::RGBA);
    zbuffer = TGAImage(width, height, TGAImage::GRAYSCALE);
}

Scene::~Scene() {
    delete shader;
    for (Model* model : models) {
        delete model;
    }
}

void Scene::load_scene(const std::string& scene_name) {
    if (scene_name == "african_head") {
        models.push_back(new Model(".\\obj\\african_head\\african_head.obj"));
    } else if (scene_name == "diablo3") {
        models.push_back(new Model(".\\obj\\diablo3_pose\\diablo3_pose.obj"));
    } else if (scene_name == "qiyana"){
        models.push_back(new Model(".\\obj\\qiyana\\qiyanabody.obj"));
        models.push_back(new Model(".\\obj\\qiyana\\qiyanaface.obj"));
        models.push_back(new Model(".\\obj\\qiyana\\qiyanahair.obj"));
    }
    else {
        std::cerr << "No such scene!";
    }
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
    zbuffer.clear();
}


TGAImage Scene::get_framebuffer() {
    return framebuffer;
}

void Scene::camera_rotate_horizontal(float theta) {
    camera.rotate_horizontal(theta);
    update_mvp();
}

void Scene::camera_rotate_vertical(float phi) {
    camera.rotate_vertical(phi);
    update_mvp();
}

void Scene::camera_scale(float scale) {
    camera.scale(scale);
    update_mvp();
}

void Scene::camera_translate(const Vec3f &xyz) {
    camera.translate_xyz(xyz);
    update_mvp();
}

void Scene::camera_translate(float x, float y, float z) {
    camera.translate_xyz(x,y,z);
    update_mvp();
}

void Scene::update_mvp() {
    Vec3f eye = camera.get_eye();
    Vec3f center = camera.get_center();
    Vec3f up = camera.get_up();
    Mat4f view = Mat::lookat(eye, center, up);
    Mat4f proj = Mat::projection(-1.f/(camera.get_eye()-camera.get_center()).norm());
    shader->payload.uniform_mvp = proj*view;
    shader->payload.uniform_mit = shader->payload.uniform_mvp.invert_transpose();
}
