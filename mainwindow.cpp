#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    this->pp = new ProcessPicture();
    read_picture = false;
    //点击按钮读取图片
    connect(ui->ac_Front_View, SIGNAL(triggered()), this, SLOT(readFrontImage()));
    //this->ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
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
    //if(pp->ReturnFlag())
    //{
    QImage img = pp->ReturnImage();
    QPixmap pix = QPixmap::fromImage(img.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
    //ui->label->setPixmap(pix);
    //图片缩放--待探索
    //ui->label->setScaledContents(true);
    //read_picture = true;
    //}
    //int width = pix.width();
    //int height = pix.height();
    vector<Point> temp = this->pp->ReturnBoundary();
    this->scene = new MyScene(pix, temp, pp->width(), pp->height());
    //this->scene->setSceneRect(-width / 2, -height / 2, width, height);
    this->ui->graphicsView->setScene(scene);
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
