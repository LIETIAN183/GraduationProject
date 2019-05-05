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
    this->data = new DataModel();
    this->scene = new MyScene();
    this->ui->graphicsView->setScene(scene);
    //点击按钮读取图片
    connect(ui->ac_Front_View, SIGNAL(triggered()), this, SLOT(readFrontImage()));
    connect(this->scene, SIGNAL(Modified()), this, SLOT(freshPic()));
    connect(this->ui->ac_3D_Model, SIGNAL(triggered()), this, SLOT(generateModel()));
    connect(this->ui->ac_Edit_Mode, SIGNAL(triggered()), this, SLOT(EditMode()));
    connect(this->ui->ac_Def1, SIGNAL(triggered()), this, SLOT(SetDef()));
    connect(this->ui->ac_Def2, SIGNAL(triggered()), this, SLOT(SetDef()));
    connect(this->ui->ac_Def3, SIGNAL(triggered()), this, SLOT(SetDef()));
    this->ui->ac_3D_Model->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    this->ui->ac_3D_Model->setEnabled(false);
    this->ui->ac_Edit_Mode->setEnabled(false);

    this->ui->ac_Def1->setEnabled(false);
    this->ui->ac_Def2->setEnabled(false);
    this->ui->ac_Def3->setEnabled(false);
    this->index = 0;
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
    this->ui->ac_Def1->setEnabled(true);
    this->ui->ac_Def2->setEnabled(true);
    this->ui->ac_Def3->setEnabled(true);

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
    if(this->scene->Edit)
    {
        vector<Point3f> tmp = this->data->ReturnBoundary();
        vector<GLfloat> t = this->pp->ProcessPoint(tmp, this->index);
        vector<GLfloat> tex = this->data->ReturnTex();
        QImage texture = this->pp->getOriginPic();
        this->ui->openGLWidget->Draw(t, tex, texture);
        this->ui->openGLWidget->setFocus();
    }
    else
    {
        vector<Point> temp = this->scene->returnResult();
        vector<Point3f> tmp = this->pp->PointList(temp);
        vector<GLfloat> t = this->pp->ProcessPoint(tmp, this->index);
        vector<GLfloat> tex = this->pp->ReturnTexCoord();
        QImage texture = this->pp->getOriginPic();
        this->ui->openGLWidget->Draw(t, tex, texture);
        this->ui->openGLWidget->setFocus();
    }
}

void MainWindow::EditMode()
{
    if(!this->scene->Edit)
    {
        QMessageBox msgBox;
        msgBox.setText("开启Edit模式后不能再调整叶片轮廓.");
        msgBox.setInformativeText("确定开启吗?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
            this->scene->Edit = true;
            break;
        case QMessageBox::No:
            return;
        default:
            break;
        }

        vector<Point> temp = this->scene->returnResult();
        vector<Point3f> tmp = this->pp->PointList(temp);
        this->data->SaveBoundary(tmp);
        vector<GLfloat> tex = this->pp->ReturnTexCoord();
        this->data->SaveTex(tex);

        this->scene->setEditPoint(tmp);
        this->scene->EditMode();
        this->scene->data = this->data;
    }
    else if(this->scene->Edit)
    {
        //关闭Edit模式
        //复原Qgraphicsview
        this->scene->Edit = false;
        qDebug() << "Edit Closed";
        vector<Point> temp = this->scene->returnResult();
        QImage tmp = this->pp->drawBoundary(temp);
        QPixmap pix = QPixmap::fromImage(tmp.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
        this->scene->SetScene(pix, temp, pp->width(), pp->height());
    }
}

void MainWindow::SetDef()
{
    QString get = this->sender()->objectName();
    if(get == "ac_Def1")
    {
        this->index = 1;
    }
    else if(get == "ac_Def2")
    {
        this->index = 2;
    }
    else if (get == "ac_Def3")
    {
        this->index = 3;
    }
    this->generateModel();
}
