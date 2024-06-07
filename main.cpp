#include "src/gui/mainwindow.h"
#include "src/core/scene.h"
#include "src/scene/scene_list.h"
#include <QApplication>

const int WIDTH = 600;
const int HEIGHT = 600;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w(WIDTH, HEIGHT, "african_head");

    w.findChild<FrameBufferWidget*>()->updateFrameBuffer();
    w.show();

    return a.exec();
}
