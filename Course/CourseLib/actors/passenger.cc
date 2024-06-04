#include "actors/passenger.hh"
#include "actors/nysse.hh"
#include "errors/gameerror.hh"


namespace CourseSide
{

Passenger::Passenger(std::weak_ptr< Interface::IStop > destination) :
    removed_(false),
    destination_(destination)
{
}


Interface::Location Passenger::giveLocation() const
{
    if (isInVehicle() ) {

        return nyssep_.lock()->giveLocation();
       } else {

        return stopp_.lock()->getLocation();
    }
}

void Passenger::move(Interface::Location loc)
{
    if (!(loc == giveLocation())) {
        throw Interface::GameError("Passenger cannot be moved from stop or nysse!");
    }
}

void Passenger::remove()
{
    removed_ = true;
}

bool Passenger::isRemoved() const
{
    return removed_;
}

bool Passenger::isInVehicle() const
{
    if (nyssep_.expired()) {
        return false;
    }
    return true;
}

std::shared_ptr<Interface::IVehicle> Passenger::getVehicle() const
{
    return nyssep_.lock();
}

std::shared_ptr<Interface::IStop> Passenger::getStop() const
{
    return stopp_.lock();
}



bool Passenger::wantToEnterNysse(std::weak_ptr< Nysse > /*bus*/) const
{
    // Board the bus with 1/2 chance
    if (rand() % 2 == 0) {
        return false;
    }

    return true;
}

bool Passenger::wantToEnterVehicle(std::weak_ptr<Interface::IVehicle> /*vehicle*/) const
{
    if (rand() % 2 == 0) {
        return false;
    }

    return true;
}

void Passenger::enterVehicle(std::weak_ptr<Interface::IVehicle> vehicle)
{
    nyssep_ = vehicle;
    stopp_.reset();
}

void Passenger::enterNysse(std::weak_ptr< Nysse > bus)
{
    // Set nyssep_ to point to given Nysse and forget stopp_
    nyssep_ = bus;
    stopp_.reset();
}

bool Passenger::wantToEnterStop(std::weak_ptr<Interface::IStop> /*stop*/) const
{
    // Leave to stop with 1/2 chance
    if (rand() % 2 == 0) {
        return false;
    }

    return true;
}

void Passenger::enterStop(std::weak_ptr< Interface::IStop > stop)
{
    // Go to stop from bus
    nyssep_.reset();
    stopp_ = stop;
}

}
