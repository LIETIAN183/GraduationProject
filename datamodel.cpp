#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QObject(parent)
{

}

void DataModel::SaveBoundary(vector<Point> boundary)
{
    this->boundary.assign(boundary.begin(), boundary.end());
}

vector<GLfloat> DataModel::ReturnBoundary()
{
    vector<GLfloat> temp;
    //temp.resize(boundary.size() * 3);
    //cout << boundary.size() << endl;
    //vector<GLfloat>::iterator x = temp.begin();
    for(vector<Point>::iterator i = boundary.begin(); i != boundary.end(); i++)
    {
        temp.push_back(i->x / 5000.0f);
        temp.push_back(-i->y / 5000.0f);
        temp.push_back(0.0f);
    }
    return temp;
}
