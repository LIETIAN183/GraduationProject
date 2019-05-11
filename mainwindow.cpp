#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <vector>
#include <QInputDialog>
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
    connect(this->ui->ac_Save, SIGNAL(triggered()), this, SLOT(Save()));
    connect(this->ui->ac_Load, SIGNAL(triggered()), this, SLOT(Load()));
    connect(this->ui->ac_Cancel, SIGNAL(triggered()), this, SLOT(SetDef()));
    connect(this->ui->ac_Close_Project, SIGNAL(triggered()), this, SLOT(CloseProject()));
    this->ui->ac_3D_Model->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    this->ui->ac_Save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    this->ui->ac_3D_Model->setEnabled(false);
    this->ui->ac_Edit_Mode->setEnabled(false);

    this->ui->ac_Def1->setEnabled(false);
    this->ui->ac_Def2->setEnabled(false);
    this->ui->ac_Def3->setEnabled(false);
    this->ui->ac_Cancel->setEnabled(false);
    this->index = 0;
    this->ui->ac_Save->setEnabled(false);
    QDir tempDir;
    tempDir.mkdir("Data");
    tempDir.mkdir("Image");
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
    this->ui->graphicsView->show();
    this->ui->openGLWidget->Clear();
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
    this->ui->ac_Save->setEnabled(false);

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
    this->ui->ac_Save->setEnabled(true);
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
        this->ui->ac_Front_View->setEnabled(false);
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
        this->ui->ac_Front_View->setEnabled(true);
    }
}

void MainWindow::SetDef()
{
    QString get = this->sender()->objectName();
    if(get == "ac_Def1")
    {
        this->index = 1;
        this->ui->ac_Cancel->setEnabled(true);
    }
    else if(get == "ac_Def2")
    {
        this->index = 2;
        this->ui->ac_Cancel->setEnabled(true);
    }
    else if (get == "ac_Def3")
    {
        this->index = 3;
        this->ui->ac_Cancel->setEnabled(true);
    }
    else if (get == "ac_Cancel")
    {
        this->index = 0;
        this->ui->ac_Cancel->setEnabled(false);
    }
    this->generateModel();
}

void MainWindow::Save()
{
    //读取模型名称
    QInputDialog dia(nullptr);
    QString name;
    dia.setWindowTitle(tr("存储模型"));
    dia.setLabelText(tr("名字:"));
    dia.setInputMode(QInputDialog::TextInput);
    if(dia.exec() == QInputDialog::Accepted)
    {
        name = dia.textValue();
    }
    else
    {
        qDebug() << 100;
        return;
    }
    //存储数据
    QString path = QCoreApplication::applicationDirPath() + "/Data/";
    path.append(name);
    path.append(".obj");

    QFileInfo fileInfo(path);
    //检查名字是否重复
    if(fileInfo.isFile())
    {
        QMessageBox msgBox;
        msgBox.setText("已存在相同名字的模型");
        msgBox.setInformativeText("是否继续保存?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Save:
            break;
        case QMessageBox::Cancel:
            qDebug() << 101;
            return;
        }
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << 102;
        return;
    }
    QTextStream out(&file);
    out << "# " << name << ".obj" << " made By ChuyiFan\n";
    for(auto i = this->ui->openGLWidget->control_points.begin(); i != this->ui->openGLWidget->control_points.end(); i += 3)
    {
        out << "v " << *i << " " << *(i + 1) << " " << *(i + 2) << "\n";
    }

    for(auto i = this->ui->openGLWidget->texCoord.begin(); i != this->ui->openGLWidget->texCoord.end(); i += 2)
    {
        out << "vt " << *i << " " << *(i + 1)  << "\n";
    }
    out << "usemtl " << name << ".png\n";

    //存储图片
    QString pic_path = QCoreApplication::applicationDirPath() + "/Image/";
    pic_path.append(name);
    pic_path.append(".png");
    this->ui->openGLWidget->texture.save(pic_path, "PNG");

    QMessageBox msgBox;
    msgBox.setText("保存成功。");
    msgBox.exec();

}

void MainWindow::Load()
{
    //获取所有文件名字
    QString path = QCoreApplication::applicationDirPath() + "/Data/";
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.obj";
    QStringList items;
    items << "";
    items.append(dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name));
    items.replaceInStrings(".obj", "");

    QInputDialog dialog;
    QString name;
    QString item = dialog.getItem(this, "加载模型", "请选择一个模型", items, 0, false);
    if(item == "")
    {
        return;
    }
    else
    {
        name = item;
    }

    vector<GLfloat> control_points;
    vector<GLfloat> tex;
    QImage texture;

    this->scene->RemoveAllItems();
    this->ui->graphicsView->hide();


    path.append(name);
    path.append(".obj");

    QString pic_path = QCoreApplication::applicationDirPath() + "/Image/";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split(" ");
        if(list.at(0) == "#")
        {
            continue;
        }
        else if (list.at(0) == "v")
        {
            for(int i = 1; i < list.size(); i++)
            {
                control_points.push_back(list.at(i).toFloat());
            }
        }
        else if (list.at(0) == "vt")
        {
            for(int i = 1; i < list.size(); i++)
            {
                tex.push_back(list.at(i).toFloat());
            }
        }
        else if(list.at(0) == "usemtl")
        {
            pic_path.append(list.at(1));
            texture.load(pic_path);
        }
    }
    this->ui->openGLWidget->Draw(control_points, tex, texture);
    this->ui->openGLWidget->setFocus();

    this->ui->ac_Edit_Mode->setEnabled(false);
    this->ui->ac_Def1->setEnabled(false);
    this->ui->ac_Def2->setEnabled(false);
    this->ui->ac_Def3->setEnabled(false);
    this->ui->ac_3D_Model->setEnabled(false);

}

void MainWindow::CloseProject()
{
    this->scene->RemoveAllItems();
    this->ui->graphicsView->show();
    this->ui->openGLWidget->Clear();

    this->ui->ac_3D_Model->setEnabled(false);
    this->ui->ac_Edit_Mode->setEnabled(false);

    this->ui->ac_Def1->setEnabled(false);
    this->ui->ac_Def2->setEnabled(false);
    this->ui->ac_Def3->setEnabled(false);
    this->ui->ac_Cancel->setEnabled(false);
    this->index = 0;
    this->ui->ac_Save->setEnabled(false);
}
