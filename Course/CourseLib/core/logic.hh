#ifndef LOGIC_HH
#define LOGIC_HH

#include "actors/passenger.hh"
#include "actors/nysse.hh"
#include "offlinereader.hh"
#include "interfaces/icity.hh"

#include <list>
#include <QTime>
#include <QTimer>

/**
 * @file
 * @brief Defines a class that handles the courseside gamelogic
 */


namespace CourseSide
{

// default files
const QString DEFAULT_STOPS_FILE = ":/offlinedata/offlinedata/full_stations_kkj3.json";
const QString DEFAULT_BUSES_FILE = ":/offlinedata/offlinedata/final_bus_liteN.json";


/**
 * @brief The Logic class
 */
class Logic : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Default constructor
     * @param parent is a QObject pointer that defaults to 0
     */
    Logic(QObject *parent = 0);

    /**
     * @brief readOfflineData uses OfflineReader class to read given offlinedata-files
     * @param buses filepath for busfile
     * @param stops filepath for stopfile
     * @return returns true if data files are read correctly, else false
     */
    bool readOfflineData(const QString& buses, const QString& stops);

    /**
     * @brief finalizeGameStart calls to add buses, stops and passengers,
     * calls cityif_ to start the game and starts timer to update buses movement
     * @pre takeCity and fileConfig must be called
     */
    void finalizeGameStart();

    /**
     * @brief fileConfig calls to read offlinedata
     * @param stops datafile for stops, defaults to constant
     * @param buses datafile for buses, defaults to constant
     */
    void fileConfig(QString stops = DEFAULT_STOPS_FILE, QString buses = DEFAULT_BUSES_FILE);

    /**
     * @brief setTime sets time_ to given time
     * @param hr time in hours
     * @param min time in minutes
     */
    void setTime(unsigned short hr, unsigned short min);

    /**
     * @brief takeCity sets given parameter as cityif_
     * @param city pointer of a class that is derived from ICity interface in StudentSide
     * @return true
     */
    bool takeCity(std::shared_ptr<Interface::ICity> city);

public slots:

    /**
     * @brief advance handles the movement and removal of buses and passengers.
     * Gets called every timeout by increaseTime
     */
    void advance();

    /**
     * @brief configChanged handles possible config parameters and calls fileconfig
     * @param time given time in QTime
     * @param debug if debugmode is true
     */
    void configChanged(QTime time, bool debug);

    /**
     * @brief increaseTime gets called when timer_ timeouts and increases time when
     * game is not over, calls advance to move buses to next position.
     */
    void increaseTime();

    /**
     * @brief addNewBuses adds new buses to traffic from offlinedata
     */
    void addNewBuses();

    /**
     * @brief addNewPassengers adds passengers to given stop
     * @param stop pointer to stop that passengers are added
     * @param no number of added passengers
     */
    void addNewPassengers(std::shared_ptr<Stop> stop, unsigned int no);

private:
    // ratio between game time and real time
    static const int TIME_SPEED;
    // time between updates in milliseconds
    static const int UPDATE_INTERVAL_MS;

    std::shared_ptr<Interface::ICity> cityif_;
    std::list< std::shared_ptr<Passenger> > passengers_;
    std::list< std::shared_ptr<Nysse> > buses_;
    std::vector< std::shared_ptr<Stop> > stops_;
    std::shared_ptr<OfflineData> offlinedata_;
    QString busfile_;
    QString stopfile_;
    bool debugstate_;
    bool gamestarted_;

    // Current time
    QTime time_;

    // Timer that increments time_ every second
    QTimer timer_;

    // TImer that moves buses in even intervals
    QTimer animationtimer_;

    // Timer that checks departing buses every minute
    QTimer departuretimer_;

    // Returns true if new location was succesfully calculated
    // False if bus arrived to the final stop or it shouldn't be in traffic
    bool calculateNewLocationForBus(std::shared_ptr<Nysse> bussi);

    // Adds buses to the traffic depending on the time_, called by finalizeGameStart
    void addBuses();

    // Adds stops to the city and passengers to the stops, called by finalizeGameStart
    void addStopsAndPassengers();

    // Calculates stop times based on departure times and time spent at stop
    std::map<QTime, std::weak_ptr<Stop>> calculateStopTimes(std::map<QTime, std::shared_ptr<Stop>>& stops, QTime& departure_time);

    // Creates bus and adds it to traffic, gives the bus to cityif_, called by methods
    // to create a bus
    void createBus(std::shared_ptr<BusData> bus, QTime departure_time);

    int busSID_;
};

}

#endif // LOGIC_HH
