#include "mainwindow.h"

MainWindow::MainWindow(int width, int height, QWidget *parent)
    : QMainWindow(parent) {

    // 标题设置
    setFixedSize(width+210, height);
    this->setWindowTitle(QString("%1 FPS: %2").arg("SoftRenderer").arg("--"));

    // 帧缓冲设置
    m_fb_widget = new FrameBufferWidget(width, height, this);


    // 菜单设置
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = new QMenu("文件", menuBar);
    menuBar->addMenu(fileMenu);

    QAction *openAction = new QAction("打开", fileMenu);
    QAction *addingAction = new QAction("添加", fileMenu);
    QAction *clearAction = new QAction("清除", fileMenu);
    QAction *exitAction = new QAction("退出", fileMenu);

    fileMenu->addAction(openAction);
    fileMenu->addAction(addingAction);
    fileMenu->addAction(clearAction);
    fileMenu->addAction(exitAction);

    connect(openAction, &QAction::triggered, this, &MainWindow::slot_openFile);
    connect(addingAction, &QAction::triggered, this, &MainWindow::slot_loadFile);
    connect(clearAction, &QAction::triggered, this, &MainWindow::slot_clear);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    // 工具栏设置
    QToolBox* toolBox = new QToolBox(this);

    //光源设置
    LightControlWidget* lightControl = new LightControlWidget(this);
    connect(lightControl->m_lightYawDial, SIGNAL(valueChanged(int)), m_fb_widget, SLOT(setLightYaw(int)));
    connect(lightControl->m_lightPitchSlider, SIGNAL(valueChanged(int)), m_fb_widget, SLOT(setLightPitch(int)));
    toolBox->addItem(lightControl, "光源控制");


    // HBOX
    QWidget* cWidget = new QWidget(this);
    QHBoxLayout* hBox = new QHBoxLayout(cWidget);
    hBox->addWidget(m_fb_widget);
    hBox->addWidget(toolBox);
    cWidget->setLayout(hBox);
    setCentralWidget(cWidget);
}

MainWindow::~MainWindow()
{
    delete m_fb_widget;
}

void MainWindow::slot_updateFPS(float fps) {
    this->setWindowTitle(QString("%1 FPS: %2").arg("SoftRenderer").arg(fps));
}

void MainWindow::slot_openFile() {
    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
    QString path = QFileDialog::getOpenFileName(this,QStringLiteral("选择场景文件"),runPath,"Text Files(*.txt)",nullptr,QFileDialog::DontResolveSymlinks);
    if (!path.isEmpty()) {
        slot_clear();
        m_fb_widget->loadScene(path);
    }
}

void MainWindow::slot_loadFile() {
    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
    QString path = QFileDialog::getOpenFileName(this,QStringLiteral("选择场景文件"),runPath,"Text Files(*.txt)",nullptr,QFileDialog::DontResolveSymlinks);
    if (!path.isEmpty()) {
        m_fb_widget->loadScene(path);
    }
}

void MainWindow::slot_clear() {
    m_fb_widget->clearScene();
}

