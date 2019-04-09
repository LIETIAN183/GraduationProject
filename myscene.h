#ifndef MYSCENE_H
#define MYSCENE_H

#include "mypixitem.h"
#include <QGraphicsScene>
#include <QPixmap>
#include "mycircleitem.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <QObject>

using namespace std;
using namespace cv;
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);
    MyScene(QPixmap pix, vector<Point> points, int width, int height);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


public slots:
    void ChangePosition(int index, double x, double y);

signals:

public slots:
private:
    MyPixItem *pix_item;
    vector<MyCircleItem> circles;
    vector<Point> result;
    int width, height;
    int pix_width, pix_height;
    QGraphicsItem  *item;
};

#endif // MYSCENE_H
