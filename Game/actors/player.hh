#ifndef PLAYER_HH
#define PLAYER_HH

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QPoint>
#include <algorithm>

#include "interfaces/iactor.hh"
#include "core/location.hh"
#include "actors/bullet.hh"
#include "gui/mainwindow.hh"
#include "misc/math.hh"
#include "misc/types.hh"

/**
 * @file
 * @brief Defines a PLayer class
 */

namespace Game {

/**
 * @brief The Player class defines a Player object, that can shoot and be moved to any direction at a constant rate.
 *
 * Intended to be a manually controlled Actor. Main features are moving by providing a direction and shooting bullets.
 * Apart from functions inherited from interface, contains getters and setters related to movement of the player, the
 * player's ID and bullet speed.
 */
class Player : public QObject, public Interface::IActor
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Player
     * @param location: Location of the player
     * @return Player object
     * @pre There must be room for next ID
     * @post The player is in initialized state. Exception guarantee: Basic
     */
    Player(const Interface::Location& location);

    /**
      * @brief Destructs the Player
      */
    ~Player();

    // IActor

    /**
     * @brief Returns the location of the actor
     * @return Loccation of the actor
     * @pre -
     * @post Exception guarantee: strong.
     */
    Interface::Location giveLocation() const override;

    /**
     * @brief Moves the actor to given location
     * @param loc: new location of the Player
     * @pre -
     * @post Player is at location loc. Emits playerMovedSig. Exception guarantee: basic.
     */
    void move(Interface::Location loc) override;

    /**
     * @brief Tells if the actor is in removed-state
     * @return 'true' if the actor is in removed-state, otherwise 'false'
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    bool isRemoved() const noexcept override;

    /**
     * @brief Puts the actor into removed-state
     * @pre -
     * @post Actor is in removed-state. Exception guarantee: basic.
     */
    void remove() override;

    // own
    /**
     * @brief Returns the speed of the bullets that the player shoots
     * @return The speed of bulellets shot by player
     * @pre -
     * @post Exception guarantee: strong
     */
    float getBulletSpeed() const;

    /**
     * @brief Moves the player to the location prior to current location
     * @pre The player must've been moved at least once for any effect. Calling more than once only moves once (i.e. only undo, no redo)
     * @post The player is moved back into last known location. Emits playerMovedSig. Exception guarantee: basic.
     */
    void moveBack();

    /**
     * @brief Adds a new bullet to this->bullets
     * @param b: pointer to the bullet
     * @param id: id of the bullet
     * @pre The bullet should not have been added before.
     * @post Bullet is added to this->bullets. Exception guarantee: basic.
     */
    void addBullet(std::shared_ptr<Bullet> b, ID id);

    /**
     * @brief Returns the ID of given bullet
     * @param b: pointer to the bullet
     * @return ID of the bullet b
     * @pre The bullet must have been added with addBullet or shoot before.
     * @post Returns the ID of the bullet. Exception guarantee: strong
     * @exception GameError - Tried to look for unknown bullet.
     */
    ID getBulletID(std::shared_ptr<Bullet> b);

    /**
     * @brief Returns the ID of the Player
     * @return ID of the Player
     * @pre -
     * @post Exception guarantee: strong.
     */
    ID getID() const;

    /**
     * @brief Returns bullets that the player knows of
     * @return Bullet-pointers and their respective IDs
     * @pre -
     * @post Exception-guarantee: strong
     */
    std::unordered_map<std::shared_ptr<Bullet>, ID> getBullets() const;

    /**
     * @brief Sets the movement speed of the player
     * @param speed: movement speed of the player
     * @pre -
     * @post this->moveSpeed is set to 'speed'. Exception guarantee: basic
     */
    void setMoveSpeed(float speed);

signals:
    void playerMovedSig(QPoint);
    void bulletMovedSig(std::shared_ptr<Interface::IActor>);
    void shot(int, int, ItemType, ID);

public slots:
    /**
     * @brief Shoots a bullet towards target, adding it to this->bullets. Emits shot -signal.
     * @param target: the point at which the bullet is shot
     * @pre Position of target is not position of player
     * @post A bullet is created with velocity that is the product of this->bulletSpeed and
     * a unit vector pointing from Player to target. Emits shot -signal on success. Exception guarantee: basic
     */
    void shoot(QPoint target);

    /**
     * @brief Advances the state of each bullet in this->bullets.
     * @pre -
     * @post Bullets are moved by their velocities. Emits bulletMovedSig. Exception guarantee: basic
     */
    void advance();

    /**
     * @brief Removes the bullet 'which' from this->bullets
     * @param which: bullet that should be removed
     * @pre Bullet should have been added before by calling addBullet or shoot
     * @post 'which' is removed from this->bullets and its state is set to removed. Exception guarantee: strong
     */
    void bulletDied(std::shared_ptr<Bullet> which);

    /**
     * @brief Moves the player by dir * this->moveSpeed
     * @param dir: direction
     * @pre The movement speed of the player should not be so high that Interface::Location or Game::vec2DF overflows.
     * The direction should be a unit-vector.
     * @post Player is moved to this->location_ + ( this->moveSpeed * dir ). Emits playerMovedSig. Exception-guarantee: basic.
     */
    void moveToDir(vec2DF dir);

private:
    Interface::Location location_;
    vec2DF preciseLocation;

    Interface::Location lastLocation_;
    vec2DF lastPreciseLocation;

    mutable bool removed_ = false;
    float moveSpeed = 2.0f;
    float bulletSpeed = 10.0f;
    ID id;  // gets initialized in constructor
    std::unordered_map<std::shared_ptr<Bullet>, ID> bullets = {};
};

}

#endif // PLAYER_HH
