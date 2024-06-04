#ifndef IPASSENGER_HH
#define IPASSENGER_HH


#include "iactor.hh"
#include "ivehicle.hh"


/**
  * @file
  * @brief Defines interface that represents the passengers operations.
  */

namespace Interface
{

class IStop;

/**
 * @brief PassengerIF is an interface which every passenger in game implements.
 *
 * PassengerIF is inherited from ActorIF interface.
 * If class method doesn't have exception guarantee of nothrow, method can leak out exception `std::bad_alloc` (out of memory)
 */
class IPassenger : public virtual IActor
{
public:
    /**
      * @brief Default constructor for the Interface.(For documentation).
      * @post Passenger is not in any vehicle by default. Passengers destination is set.
      */

    IPassenger() = default;

    /**
      * @brief Interface has default virtual destructor (base class needs to have a virtual destructor).
      */
    virtual ~IPassenger() = default;

    /**
     * @brief isInVehicle tells if passenger is in any vehicle currently.
     * @pre -
     * @return Boolean, tells wether passenger is in any vehicle.
     * @post Exception guarantee: nothrow
     */
    virtual bool isInVehicle() const = 0;

    /**
     * @brief getVehicle returns the vehicle passenger is in.
     * @pre -
     * @return Vechicle where passenger is in. Empty pointer if passenger is not in any vehicle.
     * @post Exception guarantee: nothrow.
     */
    virtual std::shared_ptr<IVehicle> getVehicle() const = 0;

};

}

#endif // IPASSENGER_HH
