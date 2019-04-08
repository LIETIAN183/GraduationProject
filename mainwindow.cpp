#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pp = new ProcessPicture();
    //点击按钮读取图片
    connect(ui->ac_Front_View,SIGNAL(triggered()),this,SLOT(readFrontImage()));
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
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),desktop_path,tr("Images ( *.jpg)"));
    if(filename.isEmpty())
    {
        //QMessageBox::warning(this,"Warning","READ PICTURE FAILING");
        return;
    }
    pp->ReadPicture(filename);
    pp->FindBoundary();

    QImage img = pp->ReturnImage();
    ui->label->setPixmap(QPixmap::fromImage(img.scaled(ui->label->size(),Qt::KeepAspectRatio)));
}
