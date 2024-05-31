#include "mainwindow.h"

MainWindow::MainWindow(int width, int height, const std::string& scene, QWidget *parent)
    : QMainWindow(parent) {
    m_fb_widget = new FrameBufferWidget(width, height, scene, this);
    setCentralWidget(m_fb_widget);
}

MainWindow::~MainWindow()
{
    delete m_fb_widget;
}

