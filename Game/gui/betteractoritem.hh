#ifndef BETTERACTORITEM_HH
#define BETTERACTORITEM_HH

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "misc/types.hh"

/**
 * @file
 * @brief Defines a BetterActorItem class
 */
namespace Game {

/**
 * @brief The BetterActorItem class has methods for moving and drawing shapes representing actors based on their types.
 *
 * The shape and color are defined by the ItemType of the actror, the BetterActorItem is representing.
 */
class BetterActorItem : public QGraphicsItem
{
public:
    /**
     * @brief Constructs a BetterActorItem
     * @param x: X-coordinate
     * @param y: Y-coordinate
     * @param type: ItemType
     * @pre -
     * @post A BetterActorItem is created. Exception guarantee: strong.
     */
    BetterActorItem(int x, int y, ItemType type);

    /**
     * @brief Destructs BetterActorItem
     */
    virtual ~BetterActorItem();

    /**
     * @brief Returns a QRectF defining the bounding rectangle of the item, where coordinates of BetterActorItem are the center point.
     * @return QRectF defining the bounding rectangle of the item.
     * @pre -
     * @post Bounding rectangle is returned
     */
    QRectF boundingRect() const override;

    /**
     * @brief Paints the contents of an item in local coordinates
     * @param painter: used to paint the item
     * @param option: provides style options for the item, such as its state, exposed area and its level-of-detail hints.
     * @param widget: Optional. If provided, it points to the widget that is being painted on; otherwise, it is 0.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief Sets the coordinates of the BetterActorItem
     * @param x: X-coordinate
     * @param y: Y-coordinate
     * @pre: -
     * @post Coordinates are set. Exception guarantee: no-throw
     */
    void setCoord(int x, int y) noexcept;

    /**
     * @brief Returns the ItemType of the item
     * @return ItemType of the item
     * @pre -
     * @post Exception guarantee: no-throw
     */
    ItemType getType() const noexcept;

private:
    const int WIDTH = 15;
    const int HEIGHT = 15;

    int x_;
    int y_;
    ItemType type_;
};

}
#endif // BETTERACTORITEM_HH
