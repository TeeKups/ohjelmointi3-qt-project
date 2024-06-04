#ifndef STATISTICS_HH
#define STATISTICS_HH

#include <memory>
#include <interfaces/istatistics.hh>
#include <QObject>

#include "core/city.hh"

/**
 * @file
 * @brief Defines a Statistics-class
 */

namespace Game {

/**
 * @brief The Statistics class keeps track of certain actions.
 *
 * The class could be extended to keep track of even more actions etc., hence the virtual methods.
 */
class Statistics : public QObject, public Interface::IStatistics
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Statistics object
     */
    Statistics();

    // IStatistics

    /**
     * @brief Increments number of total passengers by 'num'
     * @param num: Number of passengers
     * @pre -
     * @post passengertotal is incremented by 'num'. Exception guarantee: basic.
     */
    virtual void morePassengers(int num) override;

    /**
     * @brief Increments score by NYSSESCORE
     * @pre -
     * @post score is incremented by NYSSESCORE. Exception guarantee: basic.
     */
    virtual void nysseRemoved() override;

    /**
     * @brief Increments number of total nysses by one.
     * @pre -
     * @post nyssetotal is incremented by one. Exception guarantee: basic.
     */
    virtual void newNysse() override;

    /**
     * @brief Increments number of leaved nysses by one.
     * @pre -
     * @post nyssesleaved is incremented by one. Exception guarantee: basic.
     */
    virtual void nysseLeft() override;
    //

    /**
     * @brief Increments score by PASSENCGERSCORE
     * @pre -
     * @post score is incremented by PASSENGERSCORE. Exception guarantee: basic.
     */
    virtual void passengerRemoved();

    /**
     * @brief Returns score
     * @return score
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getScore() const noexcept;

    /**
     * @brief Returns bulletshit
     * @return bulletshit
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getBulletshit() const noexcept;

    /**
     * @brief Returns bulletsshot
     * @return bulletsshot
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getBulletsshot() const noexcept;

    /**
     * @brief Returns passengertotal
     * @return passengertotal
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getPassengertotal() const noexcept;

    /**
     * @brief Returns NYSSESCORE
     * @return NYSSESCORE
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getNYSSESCORE() const noexcept;

    /**
     * @brief Returns nyssesleaved
     * @return nyssesleaved
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getNyssesleaved() const noexcept;

    /**
     * @brief Returns nyssetotal
     * @return nyssetotal
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getNyssetotal() const noexcept;

    /**
     * @brief Returns PASSENGERSCORE
     * @return PASSENGERSCORE
     * @pre -
     * @post Exception guarantee: no-throw.
     */
    int getPASSENGERSCORE() const noexcept;

public slots:
    /**
     * @brief Increments bulletshit by one
     * @param which: the bullet that hit something
     * @pre -
     * @post bulletshit is incremented by one. Exception guarantee: basic.
     */
    virtual void bulletHitSlot(std::shared_ptr<Bullet> which);

    /**
     * @brief Increments bulletshot by one
     * @pre -
     * @post bulletshot is incremented by one. Exception guarantee: basic.
     */
    virtual void bulletShotSlot();

    /**
     * @brief Increases correct counter based on who died
     * @pre actor must point to a valid Interface::IActor
     * @post Correct counter is increased. Exception guarantee: basic.
     *
     * If Nysse dies, calls nysseRemoved. If passenger dies, calls passengerRemoved.
     */
    virtual void actorDiedSlot(std::shared_ptr<Interface::IActor> actor);

protected:
    int score;
    int nyssetotal;
    int passengertotal;
    int nyssesleaved;
    int bulletshit;
    int bulletsshot;
    const int NYSSESCORE = 5;
    const int PASSENGERSCORE = 1;
};

}

#endif // STATISTICS_HH
