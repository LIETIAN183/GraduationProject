#include "datamodel.h"
DataModel::DataModel(QObject *parent) : QObject(parent)
{

}

void DataModel::SaveBoundary(vector<Point3f> boundary)
{
    this->boundary.assign(boundary.begin(), boundary.end());
}

vector<Point3f> DataModel::ReturnBoundary()
{
    return boundary;
}

void DataModel::SaveTex(vector<GLfloat> tex)
{
    this->texCoord = tex;
}

vector<GLfloat> DataModel::ReturnTex()
{
    return texCoord;
}
