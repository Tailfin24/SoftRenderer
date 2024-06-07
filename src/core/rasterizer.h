#pragma once
#include "tgaimage.h"
#include "model.h"
#include "maths.h"
#include "../shader/shader.h"
#include <vector>
#include <array>
#include <limits>
#include <iostream>



void line(TGAImage& framebuffer,int x0, int y0, int x1, int y1,TGAColor color);
void triangle(TGAImage& framebuffer, std::vector<float>& zbuffer, Shader* shader, const std::array<Vec4f, 3>& t);
void triangle(TGAImage& frambuffer, std::vector<float>& zbuffer, Shader* shader, int nface);
