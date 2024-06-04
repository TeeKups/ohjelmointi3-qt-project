#ifndef ISTOP_HH
#define ISTOP_HH

#include "ipassenger.hh"

#include <memory>


/**
  * @file
  * @brief Defines an interface that describes stops operations.
  */

#include <QString>

namespace Interface
{

/**
 * @brief StopIF is an interface that stops fulfill.
 *
 * If class method doesn't have exception guarantee of nothrow, method can leak out exception `std::bad_alloc` (out of memory)
 */
class IStop
{
public:
    /**
      * @brief Default constructor for the interface. (For documentation)
      * @post Stop has no passengers by default.
      */
    IStop() = default;

    /**
      * @brief Interface has default virtual destructor (base class needs to have a virtual destructor).
      */
    virtual ~IStop() = default;

    /**
     * @brief getLocation returns the location of the stop.
     * @pre -
     * @return Stops location
     * @post Exception guarantee: nothrow
     */
    virtual Location getLocation() const = 0;

    /**
     * @brief getName returns the name of the stop.
     * @pre -
     * @return Stops name
     * @post Exception guarantee: nothrow
     */
    virtual QString getName() const = 0;

    /**
     * @brief getId returns the id of the stop (the stop number).
     * @pre -
     * @return stop number
     * @post Exception guarantee: nothrow
     */
    virtual unsigned int getId() const = 0;

    /**
     * @brief getPassengers returns all passangers in the stop.
     * @pre -
     * @return Vector that constains all passangers in the stop.
     * @post Exception guarantee: strong
     */
    virtual std::vector<std::shared_ptr<Interface::IPassenger>> getPassengers() const = 0;
};

}

#endif // ISTOP_HH
