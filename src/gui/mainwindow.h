#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framebufferwidget.h"
#include "lightcontrolwidget.h"
#include <QMainWindow>
#include <QMenubar>
#include <QToolBox>
#include <QPushButton>
#include <QVBoxLayout>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int width, int height, QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void slot_updateFPS(float fps);
    void slot_openFile();
    void slot_loadFile();
    void slot_clear();
private:
    FrameBufferWidget *m_fb_widget;
};
#endif // MAINWINDOW_H
