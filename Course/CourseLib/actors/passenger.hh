#ifndef PASSENGER_HH
#define PASSENGER_HH

#include "interfaces/ipassenger.hh"
#include "interfaces/icity.hh"
#include "core/location.hh"

#include <memory>


namespace CourseSide
{

class Nysse;

class Passenger : public Interface::IPassenger
{
public:
    Passenger(std::weak_ptr< Interface::IStop> destination);

    Interface::Location giveLocation() const;
    void move(Interface::Location loc);
    void remove();
    bool isRemoved() const;

    bool isInVehicle() const;
    std::shared_ptr<Interface::IVehicle> getVehicle() const;
    std::shared_ptr<Interface::IStop> getStop() const;

    // Tells passenger about the arrival of the bus and asks if the passenger wants in.
    virtual bool wantToEnterNysse(std::weak_ptr< Nysse > nysse ) const;
    // Moves the passenger from stop into the bus.
    void enterNysse(std::weak_ptr< Nysse > nysse );

    // WantToEnterNysse but make it accept all vehicles
    virtual bool wantToEnterVehicle(std::weak_ptr< Interface::IVehicle > vehicle ) const;
    // enterNysse but make it accept all vehicles
    void enterVehicle(std::weak_ptr< Interface::IVehicle > vehicle);

    // Tells passenger that the bus has arrived to the stop and asks if passenger wants to get out.
    virtual bool wantToEnterStop(std::weak_ptr<Interface::IStop> stop) const;

    // Moves the passenger from but to the stop.
    void enterStop(std::weak_ptr<Interface::IStop> stop);

protected:
    bool removed_;
    std::weak_ptr< Interface::IStop > destination_;


private:

    // Current bus
    std::weak_ptr< Interface::IVehicle > nyssep_;
    // Current stop
    std::weak_ptr< Interface::IStop > stopp_;


    };

}

#endif // PASSENGER_HH
