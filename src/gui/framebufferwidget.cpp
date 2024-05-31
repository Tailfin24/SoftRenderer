#include "framebufferwidget.h"

FrameBufferWidget::FrameBufferWidget(int width, int height, const std::string& scene, QWidget *parent)
    : QWidget(parent) {
    // window and scene setting
    setFixedSize(width, height);
    m_scene = new Scene(width, height, scene);

    // rasterization
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, FrameBufferWidget::updateFrameBuffer);
    m_timer->start(16);

    // fps initialization
    m_last_calc_time = QTime::currentTime().msecsSinceStartOfDay();
    connect(m_timer, &QTimer::timeout, this, FrameBufferWidget::calcFPS);

    // rotate count
    m_rotate_count = 0;
    connect(m_timer, &QTimer::timeout, this, FrameBufferWidget::rotateHorizontal);
}

FrameBufferWidget::~FrameBufferWidget() {
    delete m_scene;
    delete m_timer;
}

void FrameBufferWidget::updateFrameBuffer() {
    m_scene->rasterize();
    m_fb = m_scene->get_framebuffer();
    update();
    calcFPS();
}

void FrameBufferWidget::rotateHorizontal() {
    if (m_rotate_count >= 5) {
        m_scene->camera_rotate_horizontal(0.05);
        m_rotate_count = 0;
    } else {
        ++m_rotate_count;
    }
}


void FrameBufferWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    if (m_fb.get_width() == 0 || m_fb.get_height() == 0) {
        return;
    }

    QImage image(m_fb.buffer(), m_fb.get_width(), m_fb.get_height(), QImage::Format_ARGB32);
    QPainter painter(this);

    // painter.setPen(Qt::NoPen);
    // painter.setBrush(Qt::black);
    // painter.drawRect(rect());
    painter.drawImage(0,0,image);
    m_frame_count++;
}

void FrameBufferWidget::calcFPS() {
    int current_time = QTime::currentTime().msecsSinceStartOfDay();
    if (current_time-m_last_calc_time >= 1000) {
        m_fps = 1000.f * m_frame_count / (current_time - m_last_calc_time);
        m_frame_count = 0;
        m_last_calc_time = current_time;
        std::cerr << m_fps << '\n';
    }
}
