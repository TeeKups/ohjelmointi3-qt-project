#include "stop.hh"


namespace CourseSide
{

Stop::Stop(const Interface::Location& location, const QString& name, unsigned int id) :
    location_(location),
    name_(name),
    id_(id)
{

}

Stop::~Stop()
{

}

Interface::Location Stop::getLocation() const
{
    return location_;
}

QString Stop::getName() const
{
    return name_;
}

unsigned int Stop::getId() const
{
    return id_;
}

std::vector<std::shared_ptr<Interface::IPassenger> > Stop::getPassengers() const
{
    return passengers_;
}

void Stop::setLocation(const Interface::Location &location)
{
    location_ = location;
}

void Stop::setName(const QString &name)
{
    name_ = name;
}

void Stop::setId(unsigned int id)
{
    id_ = id;
}

void Stop::addPassenger(const std::weak_ptr<Interface::IPassenger> passenger)
{
    passengers_.push_back(passenger.lock());

}

void Stop::removePassenger(const std::weak_ptr<Interface::IPassenger> passenger)
{
    for (auto it = passengers_.begin(); it!= passengers_.end(); ++it){
        if (*it == passenger.lock()){
            passengers_.erase(it);
            break;
        }
    }
}

}
