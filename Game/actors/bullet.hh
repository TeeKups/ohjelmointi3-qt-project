#ifndef BULLET_HH
#define BULLET_HH

#include <QObject>
#include <memory>

#include "interfaces/iactor.hh"
#include "core/location.hh"
#include "math.h"
#include "misc/types.hh"

/**
 * @file
 * @brief Defines a Bullet class
 */

namespace Game {

/**
 * @brief The Bullet class has methods for moving the bullets and obserbing who its parent is.
 *
 * Unlike normal Interface::IActors, the bullets know where they are going. The bullet-like movement
 * is achieved via move-slot. The bullets also know their sizes i.e. hitboxes.
 */

class Bullet : public QObject, public Interface::IActor
{
    Q_OBJECT
public:
    /**
     * @brief Bullet constructs a Bullet with given parent and velocity
     * @param parent
     * @param velocity
     * @pre Parent must have a valid location
     * @post Exception guarantee: strong
     * @exception GameError - No valid location from parent
     */
    Bullet(Interface::IActor* parent, vec2DF velocity);

    /**
      * @brief Destroys the bullet
      */
    ~Bullet();

    // IActor

    /**
     * @brief returns the location of the actor.
     * @pre -
     * @return Actors location.
     * @post Exception guarantee: strong.
     */
    Interface::Location giveLocation() const override;

    /**
     * @brief Moves the actor to given location.
     * @param loc new location of the actor
     * @pre -
     * @post Actor's location is loc. Excaption guarantee: strong.
     *
     * "Teleports" the bullet to a location, preserving velocity.
     */
    void move(Interface::Location loc) override;

    /**
     * @brief Tells if the actor is removed ingame.
     * @pre -
     * @return `true`, if actor is removed ingame, otherwise `false`.
     * @post Exception guarantee: no-throw.
     */
    bool isRemoved() const noexcept override;

    /**
     * @brief Marks the actor as removed.
     * @pre Actor is not removed already.
     * @post Actor is removed, after this isRemoved() returns `true`. Exception guarantee: basic.
     */
    void remove() override;

    // Own

    /**
     * @brief Returns the radius of the bullet i.e. its hitbox
     * @pre -
     * @post Exception guarantee: no-throw
     * @return Radius of the bullet. Exception guarantee: strong.
     *
     * The radius is the bulelt's hitbox
     */
    int getRadius() const;

    /**
     * @brief Returns a pointer to the parent of the bullet
     * @return Pointer to the parent of the bullet
     * @pre -
     * @post Exception guarantee: strong.
     *
     * The parent can be used to identify who shot the bullet, in case there
     * are multiple actors who can shoot.
     */
    Interface::IActor* getParent() const;

public slots:
    /**
     * @brief Moves the bullet by this->velocity
     * @pre -
     * @post Exception-guarantee: basic
     *
     * Bullet-like movement.
     */
    void move();

private:
    Interface::IActor* parent;  // constructor parameter
    Interface::Location location;  // set in constructor
    vec2DF preciseLocation;  // set in constructor
    vec2DF velocity;  // constructor parameter
    bool removed = false;
    int radius = 10;

};

}

#endif // BULLET_HH
