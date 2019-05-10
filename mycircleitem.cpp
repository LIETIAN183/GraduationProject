#include "mycircleitem.h"
#include <iostream>
#include <QKeyEvent>
#include <QtDebug>
#include "myscene.h"
#include <QInputDialog>
#include <QDir>
#include "point3fdialog.h"
using namespace std;
MyCircleItem::MyCircleItem(): QGraphicsEllipseItem ()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(ItemIsSelectable | ItemIsMovable);
    this->IsSelected = false;
    this->isEdit = false;
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
    this->isEdit = c.isEdit;
    this->parent = c.parent;
    connect(this, SIGNAL(Released(int, double, double)), this->parent, SLOT(ChangePosition(int, double, double)));
    //connect(this, SIGNAL(SetPoint(int, Point3f)), this->parent, SLOT(ChangeControlPoint(int, Point3f )));

}

void MyCircleItem::setId(int id)
{
    this->id = id;
}

//----可能消耗大量性能
void MyCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->isEdit)
    {
        this->setPos(event->scenePos().x(), event->scenePos().y());
    }
}
void MyCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->isEdit)
    {
        Q_UNUSED(event);
        emit Released(id, this->scenePos().x(), this->scenePos().y());
        qDebug() << "CircleItem:" << id << "Changed";
        this->IsSelected = false;
    }
}

void MyCircleItem::SetParent(QGraphicsScene *parent)
{
    this->parent = parent;
    if(isEdit)
    {
        connect(this, SIGNAL(SetPoint(int, Point3f)), this->parent, SLOT(ChangeControlPoint(int, Point3f)));
    }
    connect(this, SIGNAL(Released(int, double, double)), this->parent, SLOT(ChangePosition(int, double, double)));

}

void MyCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->isEdit)
    {
        Q_UNUSED(event);
        this->IsSelected = true;
        qDebug() << "Clicked" << id;
    }
    else
    {
        Point3fDialog dialog;
        dialog.SetNumber(1, static_cast<int>(point.x));
        dialog.SetNumber(2, static_cast<int>(point.y));
        dialog.SetNumber(3, static_cast<int>(point.z));
        if(dialog.exec() == QDialog::Accepted)
        {
            point.x = dialog.getNumber(1);
            point.y = dialog.getNumber(2);
            point.z = dialog.getNumber(3);
            emit SetPoint(id, point);
        }
    }
}

int MyCircleItem::getId()
{
    return this->id;
}

void MyCircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED(event);
    if(this->isEdit)
    {
        qDebug() << id << ":[" << point.x << "," << point.y << "," << point.z << "]";
    }
    else if(!this->isEdit)
    {
        qDebug() << id;
    }

}

