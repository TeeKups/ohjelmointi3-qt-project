#include "player.hh"
#include "errors/gameerror.hh"
#include "core/engine.hh"

using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

Player::Player(const Interface::Location& location) :
    location_(location), lastLocation_(location), id(getNextID())
{
    this->preciseLocation.x = (float)location.giveX();
    this->preciseLocation.y = (float)location.giveY();

    this->lastPreciseLocation = this->preciseLocation;
    emit playerMovedSig(QPoint(this->location_.giveX(),this->location_.giveY()));
}

Player::~Player(){};

/* * * * * * * * * * *
 * Inherited methods *
 * * * * * * * * * * */

Interface::Location Player::giveLocation() const
{
    return location_;
}


void Player::move(Interface::Location loc)
{
    location_ = loc;
    QPoint xy(loc.giveX(), loc.giveY());
    emit playerMovedSig(Engine::gamecoord2uicoord(xy));
}

bool Player::isRemoved() const noexcept
{
    return removed_;
}

void Player::remove()
{
    removed_ = true;
}

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

float Player::getBulletSpeed() const
{
    return this->bulletSpeed;
}

void Player::moveBack()
{
    this->location_ = this->lastLocation_;
    this->preciseLocation = this->lastPreciseLocation;
    this->move(this->location_);
}

void Player::addBullet(std::shared_ptr<Bullet> b, ID id)
{
    this->bullets[b] = id;
}

ID Player::getBulletID(std::shared_ptr<Bullet> b)
{
    if (this->bullets.find(b) != this->bullets.end())
        return this->bullets[b];
    else
        throw Interface::GameError("Tried to look for unknown bullet");
}

ID Player::getID() const
{
    return id;
}

std::unordered_map<std::shared_ptr<Bullet>, ID> Player::getBullets() const
{
    return this->bullets;
}

void Player::setMoveSpeed(float speed)
{
    this->moveSpeed = speed;
}

/* * * * * * * * * *
 *  Public  slots  *
 * * * * * * * * * */

void Player::shoot(QPoint target)
{
    if ( !(target.x() == this->location_.giveX() && target.y() == this->location_.giveY()) )
    {
        target = Engine::uicoord2gamecoord(target);

        float x = (float)target.x() - (float)this->giveLocation().giveX();
        float y = (float)target.y() - (float)this->giveLocation().giveY();

        vec2DF dir = Math::unitVector2d(x, y);
        std::shared_ptr<Bullet> b = std::make_shared<Bullet>(this, dir * this->bulletSpeed);
        this->bullets[b] = getNextID();
        emit shot(b->giveLocation().giveX(), b->giveLocation().giveY(), BULLET, this->bullets[b]);
    }
}

void Player::advance()
{
    for ( auto iter : this->bullets )
    {
        auto b = std::static_pointer_cast<Bullet>(iter.first);
        b.get()->move();
        emit bulletMovedSig(b);
    }
}

void Player::bulletDied(std::shared_ptr<Bullet> which)
{
    if ( this->bullets.find(which) != this->bullets.end() )
    {
        which->remove();
        this->bullets.erase(which);
    }
}

void Player::moveToDir(vec2DF dir)
{
    this->lastLocation_ = this->location_;
    this->lastPreciseLocation = this->preciseLocation;

    this->preciseLocation.x += dir.x * this->moveSpeed;
    this->preciseLocation.y += dir.y * this->moveSpeed;

    int xcoord = (int)std::round(this->preciseLocation.x);
    int ycoord = (int)std::round(this->preciseLocation.y);

    Interface::Location newloc = Interface::Location();
    newloc.setXY(xcoord, ycoord);
    this->move(newloc);
}

/* * * * * * * * * *
 * Private methods *
 * * * * * * * * * */
    /// nothing here

/* * * * * * * * *
 * Private slots *
 * * * * * * * * */
    /// nothing here
