#include "./maths.h"

template <> template <> vec<3,int>  ::vec(const vec<3,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)),z(int(v.z+.5f)) {}
template <> template <> vec<3,float>::vec(const vec<3,int> &v)   : x(v.x),y(v.y),z(v.z) {}
template <> template <> vec<2,int>  ::vec(const vec<2,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)) {}
template <> template <> vec<2,float>::vec(const vec<2,int> &v)   : x(v.x),y(v.y) {}

/*
 * viewport: 创建视口变换矩阵
 * 返回一个矩阵，将 [-1,1]^3 的空间变换为 [x, x+w]*[y,y+h]*[0,depth] 空间
 */
Mat4f Mat::viewport(int x, int y, int w, int h) {
    Mat4f m = Mat4f::identity();
    // 平移
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = 255.f / 2.f;

    // 缩放
    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = 255.f / 2.f;
    return m;
}

/**
 * lookat: 创建 lookat 矩阵
 * 根据相机的 eye, center, up 向量确定视图矩阵
 */
Mat4f Mat::lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();
    Mat4f res = Mat4f::identity();

    for (int i = 0; i < 3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[3][i] = -center[i];
    }
    return res;
}

Mat4f Mat::projection(float fov, float aspect, float znear, float zfar) {
    Mat4f m = Mat4f::identity();
    float cot_fov_div_2 = 1.0f / tan(fov / 2.f);
    m[0][0] = cot_fov_div_2 / aspect;
    m[1][1] = cot_fov_div_2;
    m[2][2] = (znear + zfar) / (znear - zfar);
    m[2][3] = 2 * znear * zfar / (znear - zfar);
    m[3][2] = -1;
    m[3][3] = 0;
    return m;
}

Mat4f Mat::projection(float coef) {
    Mat4f m = Mat4f::identity();
    m[3][2] = coef;
    return m;
}
