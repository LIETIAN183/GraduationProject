#include "myscene.h"
#include <QKeyEvent>
#include <QtDebug>
MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    this->Edit = false;
}

void MyScene::SetScene(QPixmap pix, vector<Point> points, int width, int height)
{
    this->RemoveAllItems();
    result = points;
    this->pix_item = new MyPixItem(pix);
    this->addItem(pix_item);
    this->width = width;
    this->height = height;
    this->circles = list<MyCircleItem>(points.size());
    int m = 0;
    list<MyCircleItem>::iterator x = circles.begin();
    for(vector<Point>::iterator i = points.begin(); i != points.end(); i++, x++, m++)
    {
        x->setBrush(QBrush(Qt::red));
        x->setRect(-3, -3, 6, 6);
        x->setPos(i->x * 1.0 / (width - 1)*pix.width() - pix.width() / 2, i->y * 1.0 / (height - 1)*pix.height() - pix.height() / 2);
        x->setId(m);
        x->isEdit = false;
        x->SetParent(this);
        this->addItem(&*x);
    }
    this->pix_width = pix.width();
    this->pix_height = pix.height();
}
void MyScene::ChangePosition(int index, double x, double y)
{
    qDebug() << "Scene:" << index;
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

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R && !this->Edit)
    {
        for(list<MyCircleItem>::iterator i = circles.begin(); i != circles.end();)
        {
            if(i->IsSelected == true)
            {
                this->removeItem(&*i);
                result.erase(result.begin() + i->getId());
                //vector自定义类使用erase需要写拷贝构造函数，list则不需要
                i = circles.erase(i);
                emit Modified();
            }
            else
            {
                //删除元素后重排Circles Item元素的Id
                //如果不更新Id，删除一个元素后的后续删除会出现问题
                i->setId(static_cast<int>(distance(circles.begin(), i)));
                i++;
            }
        }
    }
}

double calDis(MyCircleItem &a, MyCircleItem &b)
{
    return sqrt(pow(a.scenePos().x() - b.scenePos().x(), 2) + pow(a.scenePos().y() - b.scenePos().y(), 2));
}
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    //非Edit编辑模式下的操作
    if(!this->Edit)
    {
        bool temp = false;
        for(list<MyCircleItem>::iterator i = circles.begin(); i != circles.end(); i++)
        {
            if(i->IsSelected == true)
            {
                temp = true;
                break;
            }
        }
        //没有控制点被选中时，添加控制点
        if(!temp)
        {
            //x插入和x距离之和最近的两个元素之间
            MyCircleItem *x = new MyCircleItem();
            x->setBrush(QBrush(Qt::red));
            x->setRect(-3, -3, 6, 6);
            x->setPos(event->scenePos().x(), event->scenePos().y());
            x->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
            x->SetParent(this);
            x->isEdit = false;
            double dis = calDis(*circles.begin(), *x);
            double distan = calDis(circles.back(), *x) + dis;
            int insert_id = 0;
            auto i = circles.begin();

            //从第二个元素开始计算
            //因为list迭代器不能+1-1，所以只能每次算出每个元素和x的距离dis，后一次在加上
            for(i++; i != circles.end(); i++)
            {
                double t = dis + calDis(*i, *x);
                if(t < distan)
                {
                    distan = t;
                    insert_id = i->getId();
                }
                dis = calDis(*i, *x);
            }
            //插入元素
            x->setId(insert_id);
            int number = 0;
            for(list<MyCircleItem>::iterator i = circles.begin(); i != circles.end(); number++)
            {
                if(number == insert_id)
                {
                    i = circles.insert(i, *x); //这里很可能是深拷贝，所以如果把x加入scene中会出错
                }
                else
                {
                    i++;
                }
            }
            //更新Id
            for(list<MyCircleItem>::iterator i = circles.begin(); i != circles.end(); i++)
            {
                i->setId(static_cast<int>(distance(circles.begin(), i)));
            }

            double xx = x->scenePos().x();
            double y = x->scenePos().y();
            xx += pix_width / 2;
            xx /= pix_width;
            xx *= (width - 1);
            y += pix_height / 2;
            y /= pix_height;
            y *= (height - 1);
            result.insert(result.begin() + insert_id, Point(static_cast<int>(xx), static_cast<int>(y)));

            //加入新增元素到scene中
            for(list<MyCircleItem>::iterator i = circles.begin(); i != circles.end(); i++)
            {
                if(i->getId() == insert_id)
                {
                    this->addItem(&*i);
                }
            }
            emit Modified();
        }
    }
    else
    {

    }
}

void MyScene::RemoveAllItems()
{
    foreach(QGraphicsItem *item, items())
    {
        this->removeItem(item);
    }
}

void MyScene::setEditPoint(vector<Point3f> control_points)
{
    this->EditPoint = control_points;
}

void MyScene::EditMode()
{
    if(this->Edit)
    {
        this->RemoveAllItems();
        this->addItem(pix_item);

        this->circles = list<MyCircleItem>(this->EditPoint.size());
        int m = 0;
        list<MyCircleItem>::iterator x = circles.begin();
        for(vector<Point3f>::iterator i = EditPoint.begin(); i != EditPoint.end(); i++, x++, m++)
        {
            x->setBrush(QBrush(Qt::red));
            x->setRect(-3, -3, 6, 6);
            x->setPos(static_cast<int>(i->x) * 1.0 / (width - 1)*pix_width - pix_width / 2, static_cast<int>(i->y) * 1.0 / (height - 1)*pix_height - pix_height / 2);
            x->setId(m);
            x->isEdit = true;
            x->point = *i;
            x->SetParent(this);
            this->addItem(&*x);
        }
    }
}

void MyScene::ChangeControlPoint(int index, Point3f point)
{
    this->EditPoint.at(static_cast<unsigned long>(index)) = point;
    this->data->SaveBoundary(this->EditPoint);
}
