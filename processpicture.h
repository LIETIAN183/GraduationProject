#ifndef PROCESSPICTURE_H
#define PROCESSPICTURE_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
using namespace std;
using namespace cv;

class ProcessPicture : public QObject
{
    Q_OBJECT
public:
    explicit ProcessPicture(QObject *parent = nullptr);
    void ReadPicture(QString path);
    QImage ReturnImage();
    void FindBoundary();
    bool ReturnFlag();
    vector<Point> ReturnBoundary();
    QImage drawBoundary(vector<Point> points);
    vector<float> PointList(vector<Point> points);
    int width();
    int height();
signals:

public slots:
private:
    Mat image;
    Mat back;
    Mat gray;
    //Mat edge;
    Mat result;
    vector<Point> boundary;
    bool find_flag;
};

#endif // PROCESSPICTURE_H
