#ifndef STOP_HH
#define STOP_HH

#include "core/location.hh"
#include "interfaces/ipassenger.hh"
#include "actors/passenger.hh"
#include "interfaces/istop.hh"

#include <QString>
#include <set>


namespace CourseSide
{

class Stop : public Interface::IStop
{
public:
    Stop(const Interface::Location& location, const QString& name, unsigned int id);
    ~Stop();

    //StopIF
    Interface::Location getLocation() const;
    QString getName() const;
    unsigned int getId() const;
    std::vector<std::shared_ptr<Interface::IPassenger>> getPassengers() const;

    void setLocation(const Interface::Location &location);
    void setName(const QString &name);
    void setId(unsigned int id);
    void addPassenger(const std::weak_ptr<Interface::IPassenger> passenger);
    void removePassenger(const std::weak_ptr<Interface::IPassenger> passenger);

private:
    Interface::Location location_;
    QString name_;
    unsigned int id_;
    std::vector<std::shared_ptr<Interface::IPassenger>> passengers_;
};

}

#endif // STOP_HH
