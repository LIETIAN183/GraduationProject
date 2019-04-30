#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processpicture.h"
#include <QMainWindow>
#include "myscene.h"
#include "datamodel.h"
#include "myopenglwidget.h"
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void readFrontImage();
    void freshPic();
    void generateModel();
private:
    Ui::MainWindow *ui;
    ProcessPicture *pp;
    //bool read_picture;
    MyScene *scene;
    DataModel *model;
};

#endif // MAINWINDOW_H
