#include "framebufferwidget.h"

FrameBufferWidget::FrameBufferWidget(int width, int height, QWidget *parent)
    : QWidget(parent) {
    // window and scene setting
    setFixedSize(width, height);
    m_scene = new Scene(width, height);

    // rasterization
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &FrameBufferWidget::updateFrameBuffer);
    m_timer->start(1);

    // fps initialization
    m_last_calc_time = QTime::currentTime().msecsSinceStartOfDay();
    connect(m_timer, &QTimer::timeout, this, &FrameBufferWidget::calcFPS);
    connect(this, SIGNAL(signal_calcFPSFinished(float)), parent, SLOT(slot_updateFPS(float)));
    // Camera operation
    m_wheel_ratio = 0;
}

FrameBufferWidget::~FrameBufferWidget() {
    delete m_scene;
    delete m_timer;
}

void FrameBufferWidget::updateFrameBuffer() {
    update();
}

void FrameBufferWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    processInput();
    m_scene->rasterize();
    TGAImage& fb = m_scene->framebuffer;
    QImage image(fb.buffer(), fb.get_width(), fb.get_height(), QImage::Format_ARGB32);
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
        emit signal_calcFPSFinished(m_fps);
        //std::cerr << m_fps << '\n';
    }
}

void FrameBufferWidget::mousePressEvent(QMouseEvent *event)
{
    m_cur_button = event->buttons();
    m_cur_pos = event->pos();
    m_last_pos = {0,0};
}

void FrameBufferWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_cur_button = event->buttons();
}

void FrameBufferWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_cur_pos = event->pos();
}

void FrameBufferWidget::wheelEvent(QWheelEvent *event) {
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    QPoint res;
    if (!numPixels.isNull()) {
        res = numPixels;
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        res = numSteps;
    }
    m_wheel_ratio += res.y();
}

void FrameBufferWidget::processInput() {
    if ((m_cur_button & Qt::LeftButton) || (m_cur_button & Qt::RightButton)) {
        if(!m_last_pos.isNull())
        {
            Vec2f motion((float)(m_cur_pos - m_last_pos).x(),(float)(m_cur_pos - m_last_pos).y());
            motion.x = (motion.x / m_scene->width());
            motion.y = (motion.y / m_scene->height());
            if(m_cur_button & Qt::LeftButton)
            {
                m_scene->camera_rotate_around_target(motion);
            }
            if(m_cur_button & Qt::RightButton)
            {
                m_scene->camera_move_target(motion);
            }
        }
        m_last_pos = m_cur_pos;
    }
    if(m_wheel_ratio != 0)
    {
        m_scene->camera_scale(m_wheel_ratio);
        m_wheel_ratio = 0;
    }
}

void FrameBufferWidget::loadScene(const QString &path) {
    QFileInfo fileInfo(path);
    QString folderPath = fileInfo.absolutePath();
    QFile file(path);


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), tr("该文件无法打开！"));
        return;
    }

    QTextStream iss(&file);
    QString str;
    QString errorMessage;
    int errorCount = 0;
    iss.setEncoding(QStringConverter::Utf8);

    while(!iss.atEnd()) {
        iss >> str;
        // Load Model
        if (str == "DEFMODEL") {
            // Load Model Obj
            iss >> str;
            if (!m_scene->add_model((folderPath + str).toStdString())) {
                errorMessage.append(QString("\n 无法加载模型: %1").arg(str));
                errorCount++;
                continue;
            }

            while (str != "ENDDEF") {
                iss >> str;
                // load diffuse map
                if (str == "DIFFUSE") {
                    iss >> str;
                    if (!m_scene->add_diffusemap((folderPath + str).toStdString())) {
                        errorMessage.append(QString("\n 无法加载材质贴图: %1").arg(str));
                        errorCount++;
                    }
                } else if (str == "NORMAL") {
                    iss >> str;
                    if (!m_scene->add_normalmap((folderPath + str).toStdString())) {
                        errorMessage.append(QString("\n 无法加载法线贴图: %1").arg(str));
                        errorCount++;
                    }
                } else if (str == "SPECULAR") {
                    iss >> str;
                    if (!m_scene->add_specularmap((folderPath + str).toStdString())) {
                        errorMessage.append(QString("\n 无法加载高光贴图: %1").arg(str));
                        errorCount++;
                    }
                }
            }
        }
    }

    // Output Error Message
    if (errorCount == 0) {
        QMessageBox::information(this, tr("场景加载完成"), tr("场景加载成功！"));
    } else {
        QMessageBox::warning(this, tr("场景加载完成"), QString("场景加载完成, 出现 %1 个问题").arg(errorCount) + errorMessage);
    }
}

void FrameBufferWidget::clearScene() {
    m_scene->clear();
}
