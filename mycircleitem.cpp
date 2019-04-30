#include "mycircleitem.h"
#include <iostream>
#include <QKeyEvent>
#include <QtDebug>
#include "myscene.h"
using namespace std;
MyCircleItem::MyCircleItem(): QGraphicsEllipseItem ()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(ItemIsSelectable | ItemIsMovable);
    this->IsSelected = false;
}

MyCircleItem::MyCircleItem(const MyCircleItem &c)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(ItemIsSelectable | ItemIsMovable);
    this->IsSelected = false;
    this->setBrush(QBrush(Qt::red));
    this->setRect(-3, -3, 6, 6);
    this->setPos(c.scenePos().x(), c.scenePos().y());
    this->id = c.id;
    this->parent = c.parent;
    connect(this, SIGNAL(Released(int, double, double)), this->parent, SLOT(ChangePosition(int, double, double)));
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
    qDebug() << "CircleItem:" << id << "Changed";
    this->IsSelected = false;
}

void MyCircleItem::SetParent(QGraphicsScene *parent)
{
    this->parent = parent;
    connect(this, SIGNAL(Released(int, double, double)), this->parent, SLOT(ChangePosition(int, double, double)));
}

void MyCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    this->IsSelected = true;
    qDebug() << "Clicked" << id;
}

int MyCircleItem::getId()
{
    return this->id;
}

void MyCircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug() << id;
}
