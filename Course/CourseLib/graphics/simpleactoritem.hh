#ifndef SIMPLEACTORITEM_HH
#define SIMPLEACTORITEM_HH

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

namespace CourseSide {

const int WIDTH = 15;
const int HEIGHT = 15;

class SimpleActorItem : public QGraphicsItem
{
public:
    SimpleActorItem(int x, int y, int type = 0);
    virtual ~SimpleActorItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setCoord(int x, int y);

private:
    int x_;
    int y_;
    int type_;
};

}
#endif // SIMPLEACTORITEM_HH
