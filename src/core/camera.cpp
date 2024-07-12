#include "camera.h"

Camera::Camera() : eye(1,0,0), center(0,0,0), up(0,1,0), aspect(1.0), fov(M_PI/6), znear(-0.1), zfar(-100) {

}

void Camera::scale(int ratio) {
        Vec3f form_target = eye-center;
        float radius = form_target.norm();
        float yaw = (float)std::atan2(form_target.x, form_target.z);
        float pitch = (float)std::asin(form_target.y / radius);
        Vec3f offset;
        radius *= (float)std::pow(0.95, ratio);
        offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
        offset.y = (radius * (float)std::sin(pitch));
        offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));
        eye = center + offset;
}


void Camera::rotate_around_target(Vec2f uv) {
    Vec3f form_target = eye-center;
    float radius = form_target.norm();
    float yaw = (float)std::atan2(form_target.x, form_target.z);
    float pitch = (float)std::asin(form_target.y / radius);
    float factor = (float)M_PI * 2.f;
    Vec3f offset;
    yaw -= uv.x * factor;
    pitch += uv.y * factor;

    if(pitch + M_PI_2 < FLT_EPSILON) pitch = - 1.569051f;
    if(pitch - M_PI_2 > FLT_EPSILON) pitch = 1.569051f;

    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));

    eye = center + offset;
}

void Camera::move_target(Vec2f uv) {
    Vec3f from_position = center-eye;
    Vec3f forward = (from_position).normalize();
    Vec3f left = (cross(Vec3f(0,1,0),forward)).normalize();
    Vec3f _up = (cross(forward,left)).normalize();

    float dist = from_position.norm();
    float factor = dist * (float)tan(fov / 2) * 2;
    Vec3f dx = left * factor * aspect * uv.x;
    Vec3f dy = _up * factor * uv.y;
    center = center + (dx+dy);
    eye = eye + (dx+dy);
}

