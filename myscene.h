#ifndef MYSCENE_H
#define MYSCENE_H

#include "mypixitem.h"
#include <QGraphicsScene>
#include <QPixmap>
#include "mycircleitem.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <list>
#include <QObject>

using namespace std;
using namespace cv;
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);
    void SetScene(QPixmap pix, vector<Point> points, int width, int height);//配置树叶图片和控制点，存储树叶图片大小
    vector<Point> returnResult();//返回控制点数据
    void ChangePic(QPixmap pix);//传递修改控制点后新的picture
    void keyPressEvent(QKeyEvent *keyEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);


public slots:
    void ChangePosition(int index, double x, double y);//对控制点进行修改
signals:
    void Modified();//scene内item发生改变的信号
private:
    MyPixItem *pix_item;
    list<MyCircleItem> circles;
    vector<Point> result;
    int width, height;//读入图片的宽高
    int pix_width, pix_height;//view中pix显示的宽高
};

#endif // MYSCENE_H
