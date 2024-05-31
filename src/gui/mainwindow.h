#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framebufferwidget.h"
#include <QMainWindow>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width, int height, const std::string& scene, QWidget *parent = nullptr);
    ~MainWindow();

private:
    FrameBufferWidget *m_fb_widget;
};
#endif // MAINWINDOW_H
