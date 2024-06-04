#ifndef CITY_HH
#define CITY_HH

#include <QTime>
#include <QImage>
#include <unordered_map>
#include <memory>
#include <QDebug>
#include <unordered_set>
#include <QKeyEvent>
#include <QSharedPointer>
#include <QRect>

#include "gui/mainwindow.hh"
#include "interfaces/icity.hh"
#include "core/logic.hh"
#include "gui/betteractoritem.hh"
#include "actors/stop.hh"
#include "misc/types.hh"
#include "actors/player.hh"
#include "actors/bullet.hh"

/**
 * @file
 * @brief Defines a City class
 */

namespace Game {


using IActorPtr = std::shared_ptr<Interface::IActor>;

/**
 * @brief The City class fulfills the Interface::ICity interface and some extensions.
 *
 * Player-actors can be added to the city, and city has knowledge of a "play-area",
 * which Players and Bullets cennot leave. Also some signals and slots are defined
 * for communication with Engine.
 */
class City : public QObject, public Interface::ICity
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a city
     * @param bounds: QRectF that defines the bounds of the city
     * @pre -
     * @post A City is created with given bounds. Exception guarantee: no-throw
     */
    City(QRectF bounds);

    /**
     * @brief Constructs a city
     * @pre -
     * @post A City is created with default bounds. Exception guarantee: no-throw
     */
    City();

    /**
      * @brief Destructs the City
      */
    ~City();

    // ICity

    /**
     * @brief setBackground sets the bitmap picture of the game area.
     * @param basicbackground  Normal sized picture used as the game area.
     * @param bigbackground  Background of the game that is bigger than normal. Can be used for e.g. scrolling-map expansion
     * @pre City is in init state.
     * @post Picture for the game area is set. Exception guarantee: basic.
     * @exception InitError Setting the picture was unsuccesful or the picture was invalid.
     */
    void setBackground(QImage& basicbackground, QImage& bigbackground) override;

    /**
     * @brief Sets city clock to a value
     * @param clock: current time
     * @pre clock.isValid() == true
     * @post Time is set. Exception guarantee: strong
     */
    void setClock(QTime clock) override;

    /**
     * @brief Adds a stop to the city.
     * @param stop: pointer to a stop object
     * @pre Stop has not been added yet.
     * @post Stop is added to the city. Exception guarantee: basic
     */
    void addStop(std::shared_ptr<Interface::IStop> stop) override;

    /**
     * @brief Adds a new actor to the city
     * @param newactor: Actor to be added to the city
     * @pre Actor should not have been added to the city before. Actor must be of type NYSSE, PASSENGER or PLAYER.
     * @post Actor is added to the city. Emits actorAdded. Exception guarantee: strong.
     */
    void addActor(IActorPtr newactor) override;

    /**
     * @brief Removes actor from the city and sets its internal state to removed.
     * @param actor: actor to be removed
     * @pre Actor must be in the city
     * @post Actor is removed from the city. Emits actorDeleted. Exception guarantee: strong.
     * @exception Intergface::GameError - Actor not found in the city
     */
    void removeActor(IActorPtr actor) override;

    /**
     * @brief Removes actor from the city and sets its internal state to removed.
     * @param actor: actor to be removed
     * @pre Actor must be in the city
     * @post Actor is removed from the city. Emits actorDeleted. Exception guarantee: strong.
     */
    void actorRemoved(IActorPtr actor) override;

    /**
     * @brief Tells if the actor is in the city
     * @param actor: actor that is looked for
     * @return 'true' if found, otherwise 'false'
     * @pre .
     * @post Exception guarantee: no-throw.
     */
    bool findActor(IActorPtr actor) const noexcept override;

    /**
     * @brief Returns actors that are on range Interface::Location::close(loc) to given position
     * @param loc: Location where to measure
     * @return std::vector containing actors close to the location.
     * @pre -
     * @post Exception guarantee: strong
     */
    std::vector<IActorPtr> getNearbyActors(Interface::Location loc) const override;

    /**
     * @brief Tells if the city is running or not.
     * @return 'true' if yes, otherwise 'false'
     * @pre Game has been started with startGame()
     * @post Exception guarantee: no-throw.
     */
    bool isGameOver() const noexcept override;

    // own
    /**
     * @brief Adds a player-actor to the city
     * @param player: pointer to a player
     * @pre There must not be a player yet
     * @post Player is added. Emits actorAdded. Exception guarantee: strong
     * @exception Interface::GameError - Only one player is allowed.
     */
    void addPlayer(std::shared_ptr<Player> player);

    /**
     * @brief Returns actors that are on range 'range' to given position
     * @param loc: Location where to measure
     * @param range: Maximum range from loc
     * @return std::vector containing actors close to the location.
     * @pre -
     * @post Exception guarantee: strong
     */
    std::vector<IActorPtr> getNearbyActors(Interface::Location loc, int range) const;

    /**
     * @brief Tells if a location is inside the playable city limits
     * @param loc: location to be tested
     * @return 'true' if loc is insice playable area
     * @pre City limits must be set to correct values
     * @post Exception guarantee: strong
     */
    bool outOfBounds(Interface::Location loc) const;

    /**
     * @brief Sets the playable area in the city
     * @param size: QRectF defining the limits of the playable area
     * @pre -
     * @post Limits of the playable area are set. Exception guarantee: basic.
     */
    void setCityBounds(QRectF size);

    /**
     * @brief Returns the current background
     * @return Current background
     * @pre -
     * @post Exception guarantee: strong
     */
    QPixmap getBackground() const;

signals:
    //void startClicked();
    void actorAdded(int, int, ItemType, ID);
    void actorDied(std::shared_ptr<Interface::IActor>);
    void actorDeleted(ID);
    void bulletHit(std::shared_ptr<Bullet>);
    void bulletMissed(std::shared_ptr<Bullet>);
    void actorMovedSgn(ID, Interface::Location);

public slots:
    /**
     * @brief Tells the city that game is running
     * @pre -
     * @post this->gameRunning = true. Exception guarantee: no-throw;
     */
    void startGame() noexcept override;

    /**
     * @brief Tells the city when the game ends
     * @pre City must have been started with startGame()
     * @post this->isGameOver returns false. Exception guarantee: no-throw
     */
    void gameOver() noexcept;

    /**
     * @brief Advances the state of the city and all its actors. Emits actorDied, actorDeleted, bulletHit and bulletMissed when appropriate.
     * @pre -
     * @post State of the city and all its actors is advanced. Exception guarantee: basic.
     */
    void advance();

    /**
     * @brief Emits actorMovedSgn(ID, Interface::Location) when called
     * @param actor: Actor who moved
     * @pre Actor must be found from the city, or it must be a known bullet
     * @post emits actorMovedSgn(ID, Interface::Location). Exception guarantee: strong
     * @exception Interface::GameError
     */
    void actorMoved(std::shared_ptr<Interface::IActor> actor) override;

private:
    QTime time = QTime();
    std::unordered_map<IActorPtr, ID> actors;
    std::unordered_map<int, std::shared_ptr<Interface::IStop>> stops;
    ItemType getActorType(IActorPtr actor);
    bool gameRunning;
    std::shared_ptr<Player> player = nullptr;
    QRectF cityBounds;
    QPixmap mapImg = QPixmap(":/offlinedata/offlinedata/kartta_pieni_500x500.png", "PNG");
};

}

#endif // CITY_HH
