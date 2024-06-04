#ifndef IACTOR_HH
#define IACTOR_HH

#include "core/location.hh"


/**
  * @file
  * @brief Defines a single actor (= an object acting in the game), operations describe the interface.
  */


namespace Interface
{

/**
 * @brief ActorIF is an interface, which every single actor moving in the game implements.
 *
 * If class method doesn't have exception guarantee of nothrow, method can leak out exception `std::bad_alloc` (out of memory)
 */
class IActor
{
public:

    /**
      * @brief Default constructor for the Interface (For documentation).
      * @post Actor isn't in removed-state by default. (Toimija ei alussa ole tuhottu-tilassa.)
      */
    IActor() = default;

    /**
      * @brief Interface has default virtual destructor (base class needs to have a virtual destructor).
      */
    virtual ~IActor() = default;

    /**
     * @brief giveLocation returns the location of the actor.
     * @pre -
     * @return Actors location.
     * @post Exception guarantee: strong.
     * @exception GameError - actor wasn't given a location.
     */
    virtual Location giveLocation() const = 0;

    /**
     * @brief move-method moves the actor to given location.
     * @param loc Actors new location.
     * @pre -
     * @post Actors location is sij. Excaption guarantee: strong.
     * @exception GameError Location is not possible.
     */
    virtual void move(Location loc) = 0;

    /**
     * @brief isRemoved tells if the actor is removed ingame.
     * @pre -
     * @return `true`, if actor is removed ingame, otherwise `false`.
     * @post Exception guarantee: nothrow.
     */
    virtual bool isRemoved() const = 0;

    /**
     * @brief remove marks the actor as removed.
     * @pre Actor is not removed already.
     * @post Actor is removed, after this isRemoved() returns `true`. Exception guarantee: basic.
     */
    virtual void remove() = 0;

};

}

#endif // IACTOR_HH
