#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processpicture.h"
#include <QMainWindow>

namespace Ui {
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
private:
    Ui::MainWindow *ui;
    ProcessPicture *pp;
};

#endif // MAINWINDOW_H
