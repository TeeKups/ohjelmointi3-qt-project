#ifndef IVEHICLE_HH
#define IVEHICLE_HH

#include "iactor.hh"

#include <string>
#include <vector>
#include <memory>


/**
  * @file
  * @brief Defines an interface that desribes operations of the vericle.
  */


namespace Interface
{

class IPassenger;

/**
 * @brief VehicleIF is an interface that describes vehicles (nysse) in game.
 *
 * VehicleIF is inherited from ActorIF-interface.
 * If class method doesn't have exception guarantee of nothrow, method can leak out exception `std::bad_alloc` (out of memory)
 */
class IVehicle : public virtual IActor
{
public:
    /**
      * @brief Default constructor (For documentation).
      * @post vehicle has no passengers by default
      */
    IVehicle() = default;

    /**
      * @brief Interface has default virtual destructor (base class needs to have a virtual destructor).
      */
    virtual ~IVehicle() = default;

    /**
     * @brief getName returns the name of the vehicle(might not be unique).
     * @pre -
     * @return name of the vehicle
     * @post Exception guarantee: strong
     */
    virtual std::string getName() const = 0;

    /**
     * @brief getPassengers returns all passengers in the vehicle.
     * @pre -
     * @return Vector containing all passengers in the vehicle.
     * @post Exception guarantee: strong.
     */
    virtual std::vector<std::shared_ptr<IPassenger> > getPassengers() const = 0;

    /**
     * @brief addPassenger adds a new passenger to the vehicle.
     * @param passenger an passenger object to be added to the Vehicle.
     * @pre `matkustaja.onkoKulkuneuvossa() == false`.
     * @post Passenger is added into the vehicle. Exception guarantee: basic.
     */
    virtual void addPassenger(std::shared_ptr<IPassenger> passenger) = 0;

    /**
     * @brief removePassenger removes the passenger from the vehicle.
     * @param passenger Passenger to be removed from the vehicle.
     * @pre -
     * @post Passenger is removed from the vehicle. Exception guarantee: basic.
     * @exception GameError Passenger is not in the vehicle.
     */
    virtual void removePassenger(std::shared_ptr<IPassenger> passenger) = 0;

};

}

#endif // IVEHICLE_HH
