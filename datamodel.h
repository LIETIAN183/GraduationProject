#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QOpenGLFunctions>
using namespace std;
using namespace cv;

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    void SaveBoundary(vector<Point3f> boundary);
    vector<Point3f> ReturnBoundary();
    void SaveTex(vector<GLfloat> tex);
    vector<GLfloat> ReturnTex();
signals:

public slots:
private:
    vector<Point3f> boundary;
    vector<GLfloat> texCoord;

};

#endif // DATAMODEL_H
