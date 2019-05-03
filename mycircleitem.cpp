#include "mycircleitem.h"
#include <iostream>
#include <QKeyEvent>
#include <QtDebug>
#include "myscene.h"
#include <QInputDialog>
#include <QDir>
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
        QInputDialog dia(nullptr);
        dia.setWindowTitle(tr("调整控制点的深度值"));
        dia.setLabelText(tr("z:"));
        dia.setInputMode(QInputDialog::IntInput);
        dia.setIntMaximum(10000);
        dia.setIntMinimum(-10000);
        if(dia.exec() == QInputDialog::Accepted)
        {
            emit SetPoint(id, Point3f(point.x, point.y, dia.intValue()));
        }
    }
}

int MyCircleItem::getId()
{
    return this->id;
}

void MyCircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

