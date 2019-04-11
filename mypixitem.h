#ifndef MYPIXITEM_H
#define MYPIXITEM_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
class MyPixItem : public QGraphicsPixmapItem
{
public:
    MyPixItem();
    MyPixItem(QPixmap pix);
    void ChangePix(QPixmap pix);//修改控制点位置后重绘图
};

#endif // MYPIXITEM_H
