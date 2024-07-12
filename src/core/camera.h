#ifndef CAMERA_H
#define CAMERA_H

#include "maths.h"

class Camera {
public:
    Camera();

    void scale(int ratio);
    void rotate_around_target(Vec2f uv);
    void move_target(Vec2f uv);

    Vec3f eye;
    Vec3f center;
    Vec3f up;
        float aspect;
    float fov;

    float znear;
    float zfar;

};

#endif // CAMERA_H
