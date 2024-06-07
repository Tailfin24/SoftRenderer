#include "camera.h"

Camera::Camera() : aspect(0.75) {

}

Camera::Camera(const Vec3f &e, const Vec3f &c, const Vec3f &up, float aspect)
    : aspect(aspect) {
    update_view(e, c, up);
}

Camera::Camera(const Vec3f& center, float radius, float theta, float phi, float aspect)
    : center(center), aspect(aspect), radius(radius), theta(theta), phi(phi) {
    update_ecu();
}

void Camera::update_view(const Vec3f &e, const Vec3f &c, const Vec3f &u) {
    eye = e;
    center = c;
    up = u;

    Vec3f r = eye-center;
    radius = r.norm();

    Vec2f proj_xy = Vec2f(r.x, r.y);
    Vec2f proj_yz = Vec2f(r.y, r.z);

    theta = proj_xy * Vec2f(1,0) / proj_xy.norm();
    phi = proj_yz * Vec2f(1,0) / proj_yz.norm();
}

void Camera::update_view(const Vec3f &c, float r, float th, float ph) {
    center = c;
    radius = r;
    theta = th;
    phi = ph;
    update_ecu();
}

void Camera::update_ecu() {
    eye = Vec3f(radius * cos(phi) * cos(theta), radius * sin(phi), radius * cos(phi) * sin(theta)) + center;
    up = Vec3f(sin(phi)*cos(theta), cos(phi), sin(phi)*sin(theta));
}

void Camera::scale(float scale) {
    radius *= scale;
    eye = (eye-center)*scale+center;
}

void Camera::rotate_horizontal(float angle) {
    theta += angle;
    update_ecu();
}

void Camera::rotate_vertical(float angle) {
    phi += angle;
    update_ecu();
}

void Camera::translate_xyz(int dx, int dy, int dz) {
    center = center + Vec3f(dx, dy, dz);
    update_ecu();
}

void Camera::translate_xyz(const Vec3f &xyz) {
    center = center + xyz;
    update_ecu();
}

Vec3f Camera::get_eye() {
    return eye;
}

Vec3f Camera::get_center() {
    return center;
}

Vec3f Camera::get_up() {
    return up;
}
