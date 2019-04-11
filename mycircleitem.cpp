#include "mycircleitem.h"
#include <iostream>
#include "myscene.h"
using namespace std;
MyCircleItem::MyCircleItem(): QGraphicsEllipseItem ()
{
    setAcceptHoverEvents(true);
    //setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(ItemIsSelectable | ItemIsMovable);
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
void MyCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit Released(id, this->scenePos().x(), this->scenePos().y());
}

void MyCircleItem::SetParent(QGraphicsScene *parent)
{
    this->parent = parent;
    connect(this, SIGNAL(Released(int, double, double)), this->parent, SLOT(ChangePosition(int, double, double)));
}


