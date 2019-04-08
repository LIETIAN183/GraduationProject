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
    read_picture = false;
    //点击按钮读取图片
    connect(ui->ac_Front_View, SIGNAL(triggered()), this, SLOT(readFrontImage()));

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
        //QMessageBox::warning(this,"Warning","READ PICTURE FAILING");
        return;
    }
    pp->ReadPicture(filename);
    pp->FindBoundary();
    if(pp->ReturnFlag())
    {
        QImage img = pp->ReturnImage();
        ui->label->setPixmap(QPixmap::fromImage(img.scaled(ui->label->size(), Qt::KeepAspectRatio)));
        //图片缩放--待探索
        //ui->label->setScaledContents(true);
        //read_picture = true;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //问题：图片无法缩小，待完善
    /*
    if(read_picture)
    {
        ui->label->clear();
        QImage img = pp->ReturnImage();
        ui->label->setPixmap(QPixmap::fromImage(img.scaled(ui->label->size(), Qt::KeepAspectRatio)));
    }
    */
}
