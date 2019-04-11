#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    this->pp = new ProcessPicture();
    this->scene = new MyScene();
    this->model = new DataModel();
    //点击按钮读取图片
    connect(ui->ac_Front_View, SIGNAL(triggered()), this, SLOT(readFrontImage()));
    connect(this->scene, SIGNAL(Modified()), this, SLOT(freshPic()));
    connect(this->ui->ac_Flat_Model, SIGNAL(triggered()), this, SLOT(flatModel()));
    this->ui->openGLWidget->TestFunction(1);
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), desktop_path, tr("Images (*.png *.jpg)"));
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
    this->ui->graphicsView->setScene(scene);
}

void MainWindow::freshPic()
{
    vector<Point> temp = this->scene->returnResult();
    QImage tmp = this->pp->drawBoundary(temp);
    QPixmap pix = QPixmap::fromImage(tmp.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    this->scene->ChangePic(pix);
    model->SaveBoundary(temp);
}

void MainWindow::flatModel()
{

}
