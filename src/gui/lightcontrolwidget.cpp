#include "lightcontrolwidget.h"

LightControlWidget::LightControlWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(180, 180);
    QVBoxLayout* vbox = new QVBoxLayout();


    QHBoxLayout* hboxLightDir = new QHBoxLayout();

    m_lightYawDial = new QDial(this);
    m_lightYawDial->setWrapping(true);
    m_lightYawDial->setMaximum(360);
    m_lightYawDial->setMinimum(0);

    m_lightPitchSlider = new QSlider(this);
    m_lightPitchSlider->setMaximum(89.9);
    m_lightPitchSlider->setMinimum(-89.9);

    hboxLightDir->addWidget(m_lightPitchSlider);
    hboxLightDir->addWidget(m_lightYawDial);

    vbox->addLayout(hboxLightDir);

    this->setLayout(vbox);

}
