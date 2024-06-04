#include "simpleactoritem.hh"

namespace CourseSide {

SimpleActorItem::SimpleActorItem(int x, int y, int type):x_(x), y_(y), type_(type)
{
    setPos(mapToParent(x_, y_ ));
}

SimpleActorItem::~SimpleActorItem()
{

}

QRectF SimpleActorItem::boundingRect() const
{
    return QRectF(0, 0, WIDTH, HEIGHT);
}

void SimpleActorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF bounds = boundingRect();
    QColor color(type_%256, type_%256, type_%256);
    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawEllipse(bounds);
}

void SimpleActorItem::setCoord(int x, int y)
{
    setX( x );
    setY( y );
}

}
