#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    void SaveBoundary(vector<Point> boundary);
signals:

public slots:
private:
    vector<Point> boundary;
};

#endif // DATAMODEL_H