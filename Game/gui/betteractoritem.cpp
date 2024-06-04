#include "betteractoritem.hh"

using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

BetterActorItem::BetterActorItem(int x, int y, ItemType type):x_(x), y_(y), type_(type)
{
    setPos(mapToParent(x_, y_ ));
}

BetterActorItem::~BetterActorItem()
{

}

/* * * * * * * * * * *
 * Inherited methods *
 * * * * * * * * * * */

QRectF BetterActorItem::boundingRect() const
{
    return QRectF(-WIDTH/2, -HEIGHT/2, WIDTH, HEIGHT);
}

void BetterActorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF bounds = boundingRect();
    // NYSSE
    if (type_ == NYSSE){
        QColor color(0, 0, 255, 100);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawRect(bounds);
    }
    // PASSENGER
    else if(type_ == PASSENGER){
        QColor color(0, 255, 0, 150);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawPie(bounds,60*16,60*16);
    }
    // STOP
    else if(type_ == STOP){
        QColor color(255, 0, 0, 50);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawRect(QRectF(-WIDTH/2, -HEIGHT/2, WIDTH/3, HEIGHT/3));
    }
    // PLAYER
    else if(type_ == PLAYER){
        QColor color(255, 0, 255, 255);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawEllipse(bounds);
    }
    else if (type_ == BULLET) {
        QColor color(0, 0, 0, 255);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawEllipse(bounds);
    }
    else {
        qDebug() << "tuntematonta actoria koitetaan piirtää";
    }
}

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

void BetterActorItem::setCoord(int x, int y) noexcept
{
    setX( x );
    setY( y );
}

ItemType BetterActorItem::getType() const noexcept
{
    return this->type_;
}

