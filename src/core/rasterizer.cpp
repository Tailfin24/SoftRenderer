#include "./rasterizer.h"

/**
 * Bresenham 画线算法
 * 参考 https://zh.wikipedia.org/wiki/%E5%B8%83%E9%9B%B7%E6%A3%AE%E6%BC%A2%E5%A7%86%E7%9B%B4%E7%B7%9A%E6%BC%94%E7%AE%97%E6%B3%95
 */
void line(TGAImage& framebuffer,int x0, int y0, int x1, int y1,TGAColor color) {
	bool steep = std::abs(x1 - x0) < std::abs(y1 - y0);
	if (steep) {
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	
	int error = 0; // error = err*dx*2;
	int derror = abs(dy*2);
	int y = y0;
	int ystep = (y1 > y0 ? 1 : -1);

	for (int x = x0; x <= x1; x++) {
		if (steep) {
			framebuffer.set(y, x, color);
		} else {
			framebuffer.set(x, y, color);
		}

		error += derror;
		if (error > dx) {
			y += ystep;
			error -= dx*2;
		}
	}
}

/**
 * 计算某点的重心坐标
 */
static inline Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
	Vec3f s[2];
	for (int i = 1; i >= 0; i--) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}

    Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2)
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
	return Vec3f(-1,1,1);
}


void triangle(TGAImage& framebuffer, std::vector<float>& zbuffer, Shader* shader, const std::array<Vec4f, 3>& t) {

    // 创建包围盒
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::min(bboxmin[j], t[i][j]/t[i][3]);
            bboxmax[j] = std::max(bboxmax[j], t[i][j]/t[i][3]);
        }
    }

    // 渲染
    for (int x = bboxmin.x; x <= bboxmax.x; x++) {
        for (int y = bboxmin.y; y <= bboxmax.y; y++) {
            if (x <= 0 || y <= 0 || x > framebuffer.get_width() || y > framebuffer.get_height()) continue;
            Vec3f bary = barycentric(proj<2>(t[0]/t[0][3]), proj<2>(t[1]/t[1][3]), proj<2>(t[2]/t[2][3]), Vec2f(x,y));
            // 使用重心坐标判断是否在三角形内
            if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

            // 使用重心坐标计算 zbuffer
            float z = t[0][2]*bary.x + t[1][2]*bary.y + t[2][2]*bary.z;
            float w = t[0][3]*bary.x + t[1][3]*bary.y + t[2][3]*bary.z;
            float depth = std::max(0.f, z/w);

            if (zbuffer[y*framebuffer.get_width()+x] > depth) continue;

            TGAColor color = shader->fragment(bary);
            zbuffer[y*framebuffer.get_width()+x] = depth;
            framebuffer.set(x,framebuffer.get_height()-y-1,color);
        }
    }
}

void triangle(TGAImage& framebuffer, std::vector<float>& zbuffer, Shader* shader, int nface) {
    std::array<Vec4f, 3> t;
    for (int i = 0; i < 3; i++) {
        t[i] = shader->vertex(nface, i);
    }

    triangle(framebuffer, zbuffer, shader, t);
}
