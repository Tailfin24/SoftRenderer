#ifndef FRAMEBUFFERWIDGET_H
#define FRAMEBUFFERWIDGET_H

#include "../core/scene.h"
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <iostream>

namespace Ui {
class FrameBufferWidget;
}

class FrameBufferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameBufferWidget(int width, int height, const std::string& scene, QWidget *parent = nullptr);
    ~FrameBufferWidget();

public slots:
    void updateFrameBuffer();
    void rotateHorizontal();

protected slots:
    void calcFPS();
protected:
    void paintEvent(QPaintEvent *event) override;
    Scene* m_scene;
    QTimer* m_timer;
    TGAImage m_fb;
    int m_last_calc_time;
    int m_frame_count;
    int m_rotate_count;
    float m_fps;
};

#endif // FRAMEBUFFERWIDGET_H
