#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QObject(parent)
{

}

void DataModel::SaveBoundary(vector<Point> boundary)
{
    this->boundary.assign(boundary.begin(), boundary.end());
}
