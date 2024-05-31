#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QColor>
#include <QImage>
#include <QString>

class FrameBuffer {
public:
    FrameBuffer(int w, int h);
    void clear(QColor color = QColor(0,0,0,0));
    void set_pixel(int x, int y, QColor color);
    void set_depth(int x, int y, float z);
    void save_image(QString filepath);
    int get_depth(int x, int y);
    QImage get_framebuffer();
    int get_width();
    int get_height();

private:
    int width, height;
    std::vector<float> depth_buffer;
    QImage framebuffer;
};



#endif // FRAMEBUFFER_H
