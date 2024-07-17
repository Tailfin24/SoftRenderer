#ifndef FRAMEBUFFERWIDGET_H
#define FRAMEBUFFERWIDGET_H

#include "../core/scene.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class FrameBufferWidget;
}
QT_END_NAMESPACE

class FrameBufferWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameBufferWidget(int width, int height, QWidget *parent = nullptr);
    ~FrameBufferWidget();
signals:
    void signal_calcFPSFinished(float fps);

public slots:
    void updateFrameBuffer();
    void loadScene(const QString& filename);
    void clearScene();

protected slots:
    void calcFPS();
    void processInput();
    void setLightYaw(int yaw);
    void setLightPitch(int pitch);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    Scene* m_scene;
    QTimer* m_timer;
    QPoint m_last_pos;
    QPoint m_cur_pos;
    Qt::MouseButtons m_cur_button;
    int m_wheel_ratio;
    int m_last_calc_time;
    int m_frame_count;
    float m_fps;
};

#endif // FRAMEBUFFERWIDGET_H
