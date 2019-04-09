#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processpicture.h"
#include <QMainWindow>
#include "myscene.h"
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    ~MainWindow();
    // static
public slots:
    void readFrontImage();

private:
    Ui::MainWindow *ui;
    ProcessPicture *pp;
    bool read_picture;
    MyScene *scene;
};

#endif // MAINWINDOW_H
