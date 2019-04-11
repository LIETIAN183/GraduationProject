#ifndef MYCIRCLEITEM_H
#define MYCIRCLEITEM_H
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneHoverEvent>
#include <QObject>
class MyCircleItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    MyCircleItem();
    void setId(int id);//储存item对应vector控制点的id
    void SetParent(QGraphicsScene *parent);//设置父Scene
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);//用于随鼠标移动
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//鼠标释放，发出信号修改scene中vector数组
signals:
    void Released(int, double, double);
private:
    int id;
    QGraphicsScene *parent;
};

#endif // MYCIRCLEITEM_H
