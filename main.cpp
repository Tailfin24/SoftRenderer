#include "./src/gui/mainwindow.h"
#include <QApplication>

const int WIDTH = 600;
const int HEIGHT = 600;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile qss("./qss/MaterialDark.qss");
    qss.open(QFile::ReadOnly);
    app.setStyleSheet(qss.readAll());
    qss.close();

    MainWindow w(WIDTH, HEIGHT);

    w.findChild<FrameBufferWidget*>()->updateFrameBuffer();
    w.show();

    return app.exec();
}
