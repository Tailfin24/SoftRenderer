#ifndef LIGHTCONTROLWIDGET_H
#define LIGHTCONTROLWIDGET_H

#include <QDial>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class LightControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LightControlWidget(QWidget *parent = nullptr);
    QDial* m_lightYawDial;
    QSlider* m_lightPitchSlider;
signals:

};

#endif // LIGHTCONTROLWIDGET_H
