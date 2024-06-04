#include "statistics.hh"
#include "core/engine.hh"

using namespace Game;

Statistics::Statistics() :
    score(0),
    nyssetotal(0),
    passengertotal(0),
    nyssesleaved(0),
    bulletshit(0),
    bulletsshot(0)
{

}

void Statistics::morePassengers(int num)
{
    this->passengertotal += num;
}

void Statistics::nysseRemoved()
{
    this->score += NYSSESCORE;
}

void Statistics::newNysse()
{
    this->nyssetotal++;
}

void Statistics::nysseLeft()
{
    this->nyssesleaved++;
}

void Statistics::passengerRemoved()
{
    this->score += PASSENGERSCORE;
}

int Statistics::getScore() const noexcept
{
    return score;
}

int Statistics::getPASSENGERSCORE() const noexcept
{
    return PASSENGERSCORE;
}

int Statistics::getNyssetotal() const noexcept
{
    return nyssetotal;
}

int Statistics::getNyssesleaved() const noexcept
{
    return nyssesleaved;
}

int Statistics::getNYSSESCORE() const noexcept
{
    return NYSSESCORE;
}

int Statistics::getPassengertotal() const noexcept
{
    return passengertotal;
}

int Statistics::getBulletsshot() const noexcept
{
    return bulletsshot;
}

int Statistics::getBulletshit() const noexcept
{
    return bulletshit;
}

void Statistics::bulletHitSlot(std::shared_ptr<Bullet> which)
{
    this->bulletshit++;
}

void Statistics::bulletShotSlot()
{
    this->bulletsshot++;
}

void Statistics::actorDiedSlot(std::shared_ptr<Interface::IActor> actor)
{
    if (std::dynamic_pointer_cast<CourseSide::Nysse>(actor) != nullptr)
        nysseRemoved();
    else if (std::dynamic_pointer_cast<CourseSide::Passenger>(actor) != nullptr)
        passengerRemoved();
}
