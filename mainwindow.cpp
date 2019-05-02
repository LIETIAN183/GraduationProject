#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <vector>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    this->pp = new ProcessPicture();
    this->model = new DataModel();
    this->scene = new MyScene();
    this->ui->graphicsView->setScene(scene);
    //点击按钮读取图片
    connect(ui->ac_Front_View, SIGNAL(triggered()), this, SLOT(readFrontImage()));
    connect(this->scene, SIGNAL(Modified()), this, SLOT(freshPic()));
    connect(this->ui->ac_3D_Model, SIGNAL(triggered()), this, SLOT(generateModel()));
    this->ui->ac_3D_Model->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    this->ui->ac_3D_Model->setEnabled(false);
    this->ui->ac_Edit_Mode->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete pp;
    delete ui;
}

//读取主视图
void MainWindow::readFrontImage()
{
    QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), desktop_path, tr("Images (*.png *.jpg *.bmp)"));
    if(filename.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "READ PICTURE FAILING");
        return;
    }
    pp->ReadPicture(filename);
    pp->FindBoundary();

    QImage img = pp->ReturnImage();
    QPixmap pix = QPixmap::fromImage(img.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    vector<Point> temp = this->pp->ReturnBoundary();
    this->scene->SetScene(pix, temp, pp->width(), pp->height());

    freshPic();
    this->ui->ac_3D_Model->setEnabled(true);
    this->ui->ac_Edit_Mode->setEnabled(true);

}

void MainWindow::freshPic()
{
    vector<Point> temp = this->scene->returnResult();
    QImage tmp = this->pp->drawBoundary(temp);
    cout << "QImage width:" << tmp.width() << ";height:" << tmp.height() << endl;
    QPixmap pix = QPixmap::fromImage(tmp.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    this->scene->ChangePic(pix);
}

void MainWindow::generateModel()
{
    //存储进入模型
    vector<Point> temp = this->scene->returnResult();
    vector<GLfloat> tmp = this->pp->PointList(temp);
    vector<GLfloat> tex = this->pp->ReturnTexCoord();
    QImage texture = this->pp->getOriginPic();
    this->ui->openGLWidget->Draw(tmp, tex, texture);
    this->ui->openGLWidget->setFocus();
}
