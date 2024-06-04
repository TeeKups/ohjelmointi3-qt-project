#ifndef NYSSE_HH
#define NYSSE_HH

#include "interfaces/ivehicle.hh"
#include "interfaces/icity.hh"
#include "interfaces/ipassenger.hh"
#include "core/location.hh"
#include "actors/stop.hh"

#include <QTime>
#include <memory>
#include <set>
#include <map>
#include <vector>


namespace CourseSide
{

class Nysse : public Interface::IVehicle
{
public:
    Nysse(unsigned int line);

    // ActorIF interface
    std::string getName() const;

    Interface::Location giveLocation() const;
    void move(Interface::Location loc);
    void remove();
    bool isRemoved() const;

    // VehicleIF
    std::vector<std::shared_ptr<Interface::IPassenger>> getPassengers() const;
    void addPassenger(std::shared_ptr<Interface::IPassenger> passenger);
    void removePassenger(std::shared_ptr<Interface::IPassenger> passenger);

    std::weak_ptr<Stop> getFinalStop() const;


    std::map<QTime, std::pair<Interface::Location, std::weak_ptr<Stop> > > getTimeRoute() const;

    void setRoute(const std::map<QTime, std::pair<Interface::Location, std::shared_ptr<Stop> > > &timeroute,
                     QTime &departuretime);

    unsigned int getLine();
    std::weak_ptr<Stop> getStop();

    Interface::Location moveToNextPosition(QTime time);

    // Is called before start of the game. Calculates the next stop.
    void calcStartingPos(QTime time);

    void setCity(std::shared_ptr<Interface::ICity> city);



    int getSID() const;
    void setSID(int sid);
private:
    unsigned int line_;
    std::string name_;
    std::shared_ptr<Interface::ICity> city_;
    Interface::Location location_;
    std::vector<std::shared_ptr<Interface::IPassenger>> passengers_;
    std::map< QTime, std::pair< Interface::Location, std::weak_ptr<Stop> > > timeroute2_;
    std::map< QTime, std::pair< Interface::Location, std::weak_ptr<Stop> > >::iterator ar2Iterator_;


    std::weak_ptr<Stop> stop_;
    bool removed_;
    int SID_;
};

}

#endif // NYSSE_HH
