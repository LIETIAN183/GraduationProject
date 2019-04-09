#include "mycircleitem.h"
#include <iostream>
#include "myscene.h"
using namespace std;
MyCircleItem::MyCircleItem(): QGraphicsEllipseItem (0, 0, 5, 5)
{
    setAcceptHoverEvents(true);
    //setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(ItemIsSelectable | ItemIsMovable);
    //modify = false;
}

void MyCircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    cout << "cursor坐标" << event->scenePos().x() << "," << event->scenePos().y() << endl;
    cout << "item坐标" << this->scenePos().x() << "," << this->scenePos().y() << endl;
    cout << "id" << this->id << endl;
}

void MyCircleItem::setId(int id)
{
    this->id = id;
}

//----可能消耗大量性能
void MyCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(event->scenePos().x(), event->scenePos().y());
}

bool MyCircleItem::isModified(int x, int y)
{
    if((x - this->scenePos().x()) * (x - this->scenePos().x()) + (y - this->scenePos().y()) * (y - this->scenePos().y()) <= 2.5 * 2.5)
    {
        return true;
    }
    return false;
}

int MyCircleItem::returnId()
{
    return id;
}
