#ifndef MYCIRCLEITEM_H
#define MYCIRCLEITEM_H
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneHoverEvent>
class MyCircleItem : public QGraphicsEllipseItem
{
public:
    MyCircleItem();
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void setId(int id);
    int returnId();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    bool isModified(int x, int y);
private:
    int id;
};

#endif // MYCIRCLEITEM_H
