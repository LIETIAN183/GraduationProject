#ifndef MYPIXITEM_H
#define MYPIXITEM_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
class MyPixItem : public QGraphicsPixmapItem
{
public:
    MyPixItem();
    MyPixItem(QPixmap pix);
};

#endif // MYPIXITEM_H
