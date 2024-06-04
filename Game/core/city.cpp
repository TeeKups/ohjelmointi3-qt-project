#include "city.hh"
#include "engine.hh"
#include "errors/initerror.hh"
#include "errors/gameerror.hh"

using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

City::City(QRectF bounds) :
    gameRunning(false), cityBounds(bounds)
{

}

City::City() :
    gameRunning(false)
{
    QPointF tl(0, 0);
    QPointF br(this->mapImg.width(), -1.0f*this->mapImg.height());
    this->cityBounds = QRectF(tl, br);
}

City::~City()
{
    qDebug() << "~City() called";
}

/* * * * * * * * * * *
 * Inherited methods *
 * * * * * * * * * * */

void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
    try {
        this->mapImg = QPixmap::fromImage(bigbackground);
    }  catch (...) {
        throw Interface::InitError("Setting the picture was unsuccesful or the picture was invalid.");
    }

    QPointF tl(0, 0);
    QPointF br(this->mapImg.width(), this->mapImg.height());
    this->cityBounds = QRectF(Engine::uicoord2gamecoord(tl), Engine::uicoord2gamecoord(br));
}

void City::setClock(QTime clock)
{
    if (clock.isValid())
        this->time = clock;
}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    // stops are never removed so a globally unique id is not needed. Easy to add if needed.
    this->stops[stop->getId()] = stop;
}

void City::addActor(IActorPtr newactor)
{
    if (findActor(newactor))
    {
       qDebug() << "Actor already in city";
    }
    else
    {
        ItemType type = Engine::getActorType(newactor);
        switch (type) {
            case NYSSE: {
                std::shared_ptr<CourseSide::Nysse> nysse = std::dynamic_pointer_cast<CourseSide::Nysse>(newactor);
                ID id = getNextID();
                emit actorAdded(nysse->giveLocation().giveX(), nysse->giveLocation().giveY(), NYSSE, id);
                this->actors[newactor] = id;
                break;
            }
            case PASSENGER: {
                std::shared_ptr<CourseSide::Passenger> passenger = std::dynamic_pointer_cast<CourseSide::Passenger>(newactor);
                ID id = getNextID();
                emit actorAdded(passenger->giveLocation().giveX(), passenger->giveLocation().giveY(), PASSENGER, id);
                this->actors[newactor] = id;
                break;
            }
            case PLAYER: {
                std::shared_ptr<Game::Player> player = std::dynamic_pointer_cast<Game::Player>(newactor);
                ID id = player->getID();
                emit actorAdded(player->giveLocation().giveX(), player->giveLocation().giveY(), PLAYER, id);
                this->actors[newactor] = id;
                break;
            }
            /*
            // bullets are not considered to be in the city for now, because there is no need for it
            // easy to implement
            case BULLET: {
                std::shared_ptr<Game::Bullet> bullet = std::dynamic_pointer_cast<Game::Bullet>(newactor);
                this->actors[newactor] = id of bullet;
                // emit actorAdded... (?)
                break;
            }
            */
            default: {
                return;
            }
        }
    }
}


void City::removeActor(IActorPtr actor)
{
    if (this->actors.find(actor) != this->actors.end())
    {
        actor->remove();
        emit this->actorDeleted(this->actors[actor]);
        this->actors.erase(actor);
    }
    else
        throw Interface::GameError("Actor not found in the city");

}

void City::actorRemoved(IActorPtr actor)
{
    try {
        this->removeActor(actor);
    }  catch (Interface::GameError) {
        // Same method, but doesn't need to throw error
    }
}

bool City::findActor(IActorPtr actor) const noexcept
{
    return (this->actors.find(actor) != this->actors.end());
}

std::vector<IActorPtr> City::getNearbyActors(Interface::Location loc) const
{
    std::vector<IActorPtr> nearbyActors;
    for (const auto &it : actors) {
        if (it.first->giveLocation().isClose(loc))
            nearbyActors.push_back(it.first);
    }

    return nearbyActors;
}

bool City::isGameOver() const noexcept
{
    return !this->gameRunning;
}

/* * * * * * * * * * * * *
 *  Own public functions *
 * * * * * * * * * * * * */

void City::addPlayer(std::shared_ptr<Player> player)
{
    if ( this->player == nullptr )
    {
        this->addActor(player);
        this->player = player;
    }
    else
        throw Interface::GameError("Only one player is allowed.");

}

std::vector<IActorPtr> City::getNearbyActors(Interface::Location loc, int range) const
{
    std::vector<IActorPtr> nearbyActors;
    for (const auto &it : actors)
    {
        if (it.first->giveLocation().isClose(loc, range))
            nearbyActors.push_back(it.first);
    }

    return nearbyActors;
}

bool City::outOfBounds(Interface::Location loc) const
{
    return (! this->cityBounds.contains(( QPoint(loc.giveX(), loc.giveY()) )));
}

void City::setCityBounds(QRectF size)
{
    this->cityBounds = size;
}

QPixmap City::getBackground() const
{
    return this->mapImg;
}


/* * * * * * * * * *
 *  Public  slots  *
 * * * * * * * * * */

void City::startGame() noexcept
{
    gameRunning = true;
}

void City::gameOver() noexcept
{
    this->gameRunning = false;
}

void City::advance()
{

    std::unordered_set<IActorPtr> markAsRemoved = {};
    std::unordered_set<std::shared_ptr<Bullet>> bulletshit;
    std::unordered_set<std::shared_ptr<Bullet>> bulletsmissed;
    if (this->player != nullptr)
    {
        for ( auto bullet : this->player->getBullets() )
        {
            std::shared_ptr<Bullet> b = bullet.first;
            std::vector<IActorPtr> actorsHit = getNearbyActors(b->giveLocation(), b->getRadius());
            if (actorsHit.size() > 0)
            {
                bool hitSomething = false;
                for ( auto actor : actorsHit )
                {
                    ItemType t = Engine::getActorType(actor);
                    //if ( t != PLAYER && t != BULLET && t != NONE )
                    if ( t == NYSSE || t == PASSENGER )
                    {
                        hitSomething = true;
                        // tehdään juttuja

                        switch (t)
                        {
                            case NYSSE:
                            {
                                std::shared_ptr<CourseSide::Nysse> nysse = std::dynamic_pointer_cast<CourseSide::Nysse>(actor);
                                std::vector<std::shared_ptr<Interface::IPassenger>> passengers = nysse->getPassengers();

                                for ( auto p : passengers )
                                {
                                    markAsRemoved.insert(actor);
                                    emit actorDied(p);
                                }
                                markAsRemoved.insert(actor);
                                emit actorDied(nysse);
                                break;
                            }
                            case PASSENGER:
                            {
                                markAsRemoved.insert(actor);
                                emit actorDied(actor);
                                break;
                            }
                            default: break;
                        }

                    }
                }

                if (hitSomething && !b->isRemoved())
                {
                    bulletshit.insert(b);
                    b->remove();
                    emit this->actorDeleted(bullet.second);
                }
            }

            if ( this->outOfBounds(b->giveLocation()) && !b->isRemoved() )
            {
                b->remove();
                emit this->actorDeleted(bullet.second);
                bulletsmissed.insert(b);
            }
        }
    }

    for ( auto actor : markAsRemoved )
        this->actorRemoved(actor);

    for ( auto b : bulletshit )
        emit bulletHit(b);

    for ( auto b : bulletsmissed )
        emit bulletMissed(b);

}

void City::actorMoved(IActorPtr actor)
{
    try {
        switch (Engine::getActorType(actor))
        {
            case BULLET: {
                // Needs to be extended if more actors that can shoot bullets are added
                std::shared_ptr<Bullet> b = std::static_pointer_cast<Bullet>(actor);
                emit actorMovedSgn(this->player->getBulletID(b), b.get()->giveLocation());
                break;
            }
            case PLAYER: {
                emit actorMovedSgn(this->player->getID(), this->player->giveLocation());
                break;
            }
            default: {
                emit actorMovedSgn(this->actors[actor], actor->giveLocation());
                break;
            }
        }
    } catch (Interface::GameError e)
    {
        throw (e);
    }
}

/* * * * * * * * * *
 * Private methods *
 * * * * * * * * * */

 /// nothing here

/* * * * * * * * *
 * Private slots *
 * * * * * * * * */

    /// nothing here
