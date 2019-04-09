#include "mypixitem.h"

MyPixItem::MyPixItem()
{

}

MyPixItem::MyPixItem(QPixmap pix)
{
    this->setPixmap(pix);
    this->setPos(-pix.width() / 2, -pix.height() / 2);
}
