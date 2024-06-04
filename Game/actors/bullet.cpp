#include "bullet.hh"
#include <QDebug>

#include "errors/gameerror.hh"

using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */
Bullet::Bullet(Interface::IActor* parent, vec2DF velocity) :
    parent(parent), velocity(velocity)
{
    try {
        this->location = parent->giveLocation();
    }  catch (...) {
        throw Interface::GameError("");
    }

    this->preciseLocation.x = (float)this->location.giveX();
    this->preciseLocation.y = (float)this->location.giveY();
}

Bullet::~Bullet(){};

/* * * * * * * * * * *
 * Inherited methods *
 * * * * * * * * * * */

Interface::Location Bullet::giveLocation() const
{
    return this->location;
}

void Bullet::move(Interface::Location loc)
{
    this->location = loc;
}

bool Bullet::isRemoved() const noexcept
{
    return this->removed;
}

void Bullet::remove()
{
    this->removed = true;
}

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

int Bullet::getRadius() const
{
    return this->radius;
}

Interface::IActor* Bullet::getParent() const
{
    return this->parent;
}

/* * * * * * * * * *
 *  Public  slots  *
 * * * * * * * * * */

void Bullet::move()
{

    this->preciseLocation += velocity;
    this->location.setXY((int)std::round(this->preciseLocation.x), (int)std::round(this->preciseLocation.y));
}

/* * * * * * * * * *
 * Private methods *
 * * * * * * * * * */
    /// nothing here

/* * * * * * * * *
 * Private slots *
 * * * * * * * * */
    /// nothing here
