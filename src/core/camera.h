#ifndef CAMERA_H
#define CAMERA_H

#include "maths.h"

class Camera {
public:
    Camera();
    Camera(const Vec3f& eye, const Vec3f& center, const Vec3f& up, float aspect);
    Camera(const Vec3f& center, float radius, float theta, float phi, float aspect);

    void scale(float scale);
    void rotate_horizontal(float angle);
    void rotate_vertical(float angle);
    void translate_xyz(int dx, int dy, int dz);
    void translate_xyz(const Vec3f& xyz);
    void update_view(const Vec3f& eye, const Vec3f& center, const Vec3f& up);
    void update_view(const Vec3f& center, float radius, float theta, float phi);
    Vec3f get_eye();
    Vec3f get_center();
    Vec3f get_up();

protected:
    void update_ecu();
    Vec3f eye;
    Vec3f center;
    Vec3f up;
    float aspect;
    float z_near;
    float z_far;
    float radius;
    float theta;
    float phi;
};

#endif // CAMERA_H
