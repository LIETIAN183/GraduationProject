#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
}

MyScene::MyScene(QPixmap pix, vector<Point> points, int width, int height)
{
    result = points;
    this->pix_item = new MyPixItem(pix);
    this->addItem(pix_item);
    this->width = width;
    this->height = height;
    this->circles = vector<MyCircleItem>(points.size());
    //int m = 0;
    this->item = nullptr;
    //QPen pen;
    vector<MyCircleItem>::iterator x = circles.begin();
    for(vector<Point>::iterator i = points.begin(); i != points.end(); i++, x++)
    {
        x->setBrush(QBrush(Qt::red));
        x->setPos(i->x * 1.0 / (width - 1)*pix.width() - pix.width() / 2, i->y * 1.0 / (height - 1)*pix.height() - pix.height() / 2);
        this->addItem(&*x);
        /*
        circles[m].setBrush(QBrush(Qt::red));
        circles[m].setPos(i->x * 1.0 / (width - 1)*pix.width() - pix.width() / 2, i->y * 1.0 / (height - 1)*pix.height() - pix.height() / 2);
        circles[m].setId(m);
        this->addItem(&circles[m]);
        */
    }
}

void MyScene::ChangePosition(int index, double x, double y)
{
    cout << "id:" << index << "," << x << y << "调用了该函数" << endl;
    /*
    x += pix_width / 2;
    x /= pix_width;
    x *= (width - 1);
    y += pix_height / 2;
    y /= pix_height;
    y *= (height - 1);
    //后期修改-----
    this->result.at(index).x = x;
    this->result.at(index).y = y;
    */
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    cout << "鼠标释放：" << event->scenePos().x() << "," << event->scenePos().y() << endl;
    this->clearSelection();
    for(vector<MyCircleItem>::iterator i = circles.begin(); i != circles.end(); i++)
    {
        if(i->isModified(static_cast<int>(event->scenePos().x()), static_cast<int>(event->scenePos().y())))
        {
            ChangePosition(i->returnId(), i->scenePos().x(), i->scenePos().y());
        }
    }
    cout << "yyyyy" << endl;
}

