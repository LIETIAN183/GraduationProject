#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{

}

void MyScene::SetScene(QPixmap pix, vector<Point> points, int width, int height)
{
    result = points;
    this->pix_item = new MyPixItem(pix);
    this->addItem(pix_item);
    this->width = width;
    this->height = height;
    this->circles = vector<MyCircleItem>(points.size());
    int m = 0;
    vector<MyCircleItem>::iterator x = circles.begin();
    for(vector<Point>::iterator i = points.begin(); i != points.end(); i++, x++, m++)
    {
        x->setBrush(QBrush(Qt::red));
        x->setRect(-3, -3, 6, 6);
        x->setPos(i->x * 1.0 / (width - 1)*pix.width() - pix.width() / 2, i->y * 1.0 / (height - 1)*pix.height() - pix.height() / 2);
        x->setId(m);
        x->SetParent(this);
        this->addItem(&*x);
    }
    this->pix_width = pix.width();
    this->pix_height = pix.height();
}
void MyScene::ChangePosition(int index, double x, double y)
{
    this->clearSelection();
    x += pix_width / 2;
    x /= pix_width;
    x *= (width - 1);
    y += pix_height / 2;
    y /= pix_height;
    y *= (height - 1);
    this->result.at(static_cast<unsigned long>(index)).x = static_cast<int>(x);
    this->result.at(static_cast<unsigned long>(index)).y = static_cast<int>(y);
    emit Modified();
}

vector<Point> MyScene::returnResult()
{
    return result;
}

void MyScene::ChangePic(QPixmap pix)
{
    this->pix_item->ChangePix(pix);
}

