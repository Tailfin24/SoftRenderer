#include "framebuffer.h"

FrameBuffer::FrameBuffer(int w, int h)
    : width(w), height(h), depth_buffer(w*h), framebuffer(w, h, QImage::Format_ARGB32) {
    clear(QColor(0,0,0));
}

void FrameBuffer::clear(QColor color) {
    framebuffer.fill(color);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 0.f);
}

void FrameBuffer::set_pixel(int x, int y, QColor color) {
    framebuffer.setPixelColor(x, y, color);
}

void FrameBuffer::set_depth(int x, int y, float z) {
    depth_buffer[y*width+x]=z;
}

void FrameBuffer::save_image(QString filepath) {
    framebuffer.save(filepath);
}

int FrameBuffer::get_depth(int x, int y) {
    return depth_buffer[y*width+x];
}

QImage FrameBuffer::get_framebuffer() {
    return framebuffer;
}

int FrameBuffer::get_width() {
    return width;
}

int FrameBuffer::get_height() {
    return height;
}
