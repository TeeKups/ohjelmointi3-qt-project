#include "core/logic.hh"
#include "offlinereader.hh"
#include <QTimer>
#include <memory>
#include <iostream>
#include <QDebug>
#include <QDir>

namespace CourseSide
{

Logic::Logic(QObject *parent)
    : QObject(parent),
      debugstate_(false),
      gamestarted_(false),
      time_(QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second()),
      busSID_(0)
{
}

bool Logic::readOfflineData(const QString &buses, const QString &stops)
{
    OfflineReader offlinereader;
    if((offlinedata_ = offlinereader.readFiles(buses, stops)) == NULL) {
        return false;
    }
    return true;

}

void Logic::finalizeGameStart()
{
    // Adds buses that are currently operating
    addBuses();

    addStopsAndPassengers();

    cityif_->setClock(time_);
    cityif_->startGame();
    gamestarted_ = true;

    connect(&timer_, SIGNAL(timeout()), this, SLOT(increaseTime()));
    timer_.start(UPDATE_INTERVAL_MS);

}

void Logic::setTime(unsigned short hr, unsigned short min)
{
    time_.setHMS(hr, min, 0);
}

void Logic::advance()
{
    // Tells the city a new time every minute
    if (time_.second() == 0) {
        cityif_->setClock(time_);
    }

    // Goes through current passengers and removes removed (from game) passengers from data structures
    for (std::list<std::shared_ptr<Passenger>>::iterator it = passengers_.begin();
         it != passengers_.end(); it++) {

        // Check if removed
        if (it->get()->isRemoved()) {
            // Remove passenger from buses or stops accounting
            if (it->get()->isInVehicle()) {
                it->get()->getVehicle()->removePassenger(*it);
            } else {
                std::shared_ptr<Stop> pysakki = std::dynamic_pointer_cast<Stop>(it->get()->getStop());
                pysakki->removePassenger(*it);
            }

            // Remove passenger from accounting
            if (cityif_->findActor(*it)) {
                cityif_->removeActor(*it);
            }
            it = passengers_.erase(it);
            continue;
        }
    }

    // Goes through current buses and removes ones that are removed
    for (std::list<std::shared_ptr<Nysse>>::iterator it = buses_.begin(); it != buses_.end();) {
        std::shared_ptr<Nysse> bus = *it;

        // Check if removed
        if (bus->isRemoved()) {

            // If bus is marked removed, go through its passengers and check that they are marked as removed too
            // If not marked --> mark as removed and remove the bus and its passengers from data structures
            for (std::shared_ptr<Interface::IPassenger> passengerif : bus->getPassengers()) {
                std::shared_ptr<Passenger> passenger = std::dynamic_pointer_cast<Passenger> (passengerif);

                if (!passenger->isRemoved()) {
                    passenger->remove();
                }
                // if city keeps track of passengers in removed bus, remove passengers from city
                if (cityif_->findActor(passenger)) {
                    cityif_->removeActor(passenger);
                }
                // remove the pasenger from course side data structure
                passengers_.remove(passenger);
            }
            qDebug() << "Removing nysse: " << QString::fromStdString(bus->getName());

            std::shared_ptr<Interface::IActor> toimijaBussi = std::dynamic_pointer_cast<Interface::IActor> (*it);

            if (cityif_->findActor(toimijaBussi)) {
                cityif_->removeActor(toimijaBussi);
            }
            it = buses_.erase(it);
            continue;
        }

        // move the bus
        bool ret = calculateNewLocationForBus(bus);

        if (!ret) { // Remove bus if at final stop or wrong time

            // Put passengers to the final stop of the bus
            std::vector<std::shared_ptr<Interface::IPassenger>> passengers = bus->getPassengers();
            std::weak_ptr<Stop> finalStop = bus->getFinalStop();

            // every passenger moved out of bus before it is removed
            for (auto passengerIt = passengers.begin(); passengerIt != passengers.end(); passengerIt++) {
                // assuming that passengers are of certain type
                std::shared_ptr<Passenger> passenger = std::dynamic_pointer_cast<Passenger>(*passengerIt);
                Q_ASSERT(passenger != nullptr);

                // Updates passenger to the final stop
                // Location of the passenger is already updated at
                // previous point of time, when bus arrived at final stop
                passenger->enterStop(finalStop);
                bus->removePassenger(passenger);
                finalStop.lock()->addPassenger(passenger);
                cityif_->actorMoved(passenger);
            }

            // check from city if bus is already removed
            if (cityif_->findActor(*it)) {
                cityif_->removeActor(*it);
            }

            it = buses_.erase(it);

        } else {
            ++it;
        }
    }

    // now:
    // 1. removed 'removed' passengers and buses
    // 2. removed buses that were in grey area
    // 3. moved buses to their new locations
    // 4. removed passengers from the buses in final stop

    // add new buses
    if (time_.second() == 0) {
        addNewBuses();
    }

    // go through all stops that have buses
    for (std::shared_ptr <Nysse> bus : buses_) {
        std::shared_ptr <Stop> stop = bus->getStop().lock();

        if (stop != nullptr) {
            // 1. move passengers to stop if they want to

            for (std::shared_ptr <Interface::IPassenger> ipassenger : bus->getPassengers()) {
                std::shared_ptr <Passenger> passenger = std::dynamic_pointer_cast<Passenger> (ipassenger);
                Q_ASSERT(passenger != nullptr);

                if (passenger->wantToEnterStop(stop)) {
                    passenger->enterStop(stop);
                    bus->removePassenger(ipassenger);
                    stop->addPassenger(passenger);
                    cityif_->actorMoved(passenger);
                }
            }
        }
    }

    // 2. let every passenger in this stop about the buses in this stop at this time
    for (std::shared_ptr <Nysse> stopbus : buses_) {
        std::shared_ptr <Stop> stop = stopbus->getStop().lock();

        // stopbus is bus that is currently at the same stop
        if (stop != nullptr) {
            for (std::shared_ptr <Interface::IPassenger> istoppassenger : stop->getPassengers()) {
                std::shared_ptr <Passenger> stoppassenger = std::dynamic_pointer_cast<Passenger> (istoppassenger);
                Q_ASSERT(stoppassenger != nullptr);

                if (stoppassenger->wantToEnterNysse(stopbus)) {
                    stoppassenger->enterNysse(stopbus);
                    stop->removePassenger(stoppassenger);
                    stopbus->addPassenger(stoppassenger);
                    cityif_->actorMoved(stoppassenger);
                }
            }
        }
    }
}

void Logic::fileConfig(QString stops, QString buses) {
            qDebug () << "fileConfig";
            qDebug () << " ----check";
            qDebug () << QDir::currentPath();
    bool ret = readOfflineData(buses, stops);

    if (ret) {
        busfile_ = buses;
        stopfile_ = stops;
    } else {
        qDebug () << "Something went wrong with reading the files.";
    }
}

void Logic::configChanged(QTime time, bool debug)
{
    // handling the command line parameters
    this->debugstate_ = debug;
    this->time_ = time;

    // reading files etc.
    this->fileConfig();
}

void Logic::increaseTime()
{
    if ( cityif_->isGameOver() )
    {
        timer_.stop();
        return;
    }

    time_ = time_.addMSecs(TIME_SPEED * UPDATE_INTERVAL_MS);
    if (time_.second() == 0) {
        qDebug() << "";
        qDebug() << "time is: " << time_.toString();
    }

    // move all old buses
    advance();


}

bool Logic::calculateNewLocationForBus(std::shared_ptr<Nysse> bus)
{
    // Check if bus is at traffic
    // Deals with removing the buses at final stop
    // --> if bus got to final stop during last advance-routine, is ingame
    // time larger than the time of the buses last route point
    // --> return false and bus is deleted
    if (time_ < bus->getTimeRoute().begin()->first || time_ > bus->getTimeRoute().rbegin()->first) {
        // Not in traffic
        return false;
    }

    // Get new location
    Interface::Location newLocation = bus->moveToNextPosition(time_);

    bus->move(newLocation);
    cityif_->actorMoved(bus);

    // passengers are moved
    std::vector<std::shared_ptr<Interface::IPassenger>> passengers = bus->getPassengers();
    for (auto it = passengers.begin(); it != passengers.end(); it++) {
        it->get()->move(newLocation);
        cityif_->actorMoved(*it);
    }

    return true;
}

void Logic::addBuses()
{
    qDebug() << "Current time: " << time_.toString();
    // Find buses that are on route from offlinedata
    for (std::shared_ptr<BusData> bus: offlinedata_->buses) {
        for (QTime starttime : bus->schedule) {

            // relative time from first route point to last route point
            // added to game time just now
            // --> if starttime is smaller and this recieved time is larger than time now, bus is currently driving
            QTime finalstoptime = starttime.addSecs( ((--bus->timeRoute2.end())->first.hour() *60*60) + (--bus->timeRoute2.end())->first.minute() * 60 );
            if (starttime < time_ &&  finalstoptime > time_ ) {
                createBus(bus, starttime);

                // if debug state on, add only one
                if (debugstate_) {
                    qDebug() << "Debug on --> only one bus";
                    return;
                }
            }
        }
    }

    qDebug() << "Buses currently at traffic: " << buses_.size();
}

void Logic::addNewBuses()
{
    if (debugstate_ && buses_.size() != 0) {
        return;
    }

    for (std::shared_ptr<BusData> bussi: offlinedata_->buses) {
        for (QTime starttime : bussi->schedule) {

            if (starttime == time_) {
                createBus(bussi, starttime);

                // if debug state, add only one
                if (debugstate_) {
                    return;
                }

                break;
            }
        }
    }
    qDebug() << "Buses currently at traffic: " << buses_.size();
}


void Logic::createBus(std::shared_ptr<BusData> bus, QTime starttime)
{
    QTime time = (--(bus->timeRoute2.end()))->first;
    int minutes = time.minute() + time.hour() * 60;

    if (time_ == starttime.addSecs( minutes * 60 )) {
        // return if bus is at final stop
        return;
    }

    // Create new bus and add it to city
    std::shared_ptr<Nysse> newBus = std::make_shared<Nysse>(bus->routeNumber);

    // Add data to buses
    newBus->setRoute(bus->timeRoute2, starttime);
    newBus->calcStartingPos(time_);
    buses_.push_back(newBus);
    newBus->setCity(cityif_);
    newBus->setSID(busSID_);

    busSID_ += 1;
    cityif_->addActor(newBus);

    // Buses at transport
    qDebug() << "Bus added! Route nro: " << bus->routeNumber;
}

std::map<QTime, std::weak_ptr<Stop> > Logic::calculateStopTimes(std::map<QTime, std::shared_ptr<Stop> > &stops, QTime &starttime)
{
    // go through stops and calculate minute schedules for them
    std::map<QTime, std::weak_ptr<Stop>> stoptimes;
    QTime time = starttime;
    for (std::pair<QTime, std::shared_ptr<Stop>> stop : stops) {
        stoptimes.insert(std::make_pair(time.addSecs(stop.first.minute() * 60), stop.second));
    }

    return stoptimes;
}

void Logic::addStopsAndPassengers()
{
    // give city all stops before creating passengers
    for ( std::shared_ptr<Stop> stop: offlinedata_->stops) {
        cityif_->addStop(stop);
    }

    for ( std::shared_ptr<Stop> stop: offlinedata_->stops) {
        // add new passengers
        int randi = rand() % 10 + 1;

        for (int i = 0; i < randi; i++) {
            std::weak_ptr<Interface::IStop> destinationStop = offlinedata_->stops.at( rand() % offlinedata_->stops.size() );
            std::shared_ptr<Passenger> newPassenger = nullptr;
            newPassenger = std::make_shared<Passenger>( destinationStop );

            // add passengers for this stop
            newPassenger->enterStop( stop );
            passengers_.push_back(newPassenger);
            stop->addPassenger(newPassenger);
            cityif_->addActor(newPassenger);
        }
    }
}

void Logic::addNewPassengers(std::shared_ptr<Stop> stop, unsigned int no)
{

    for(unsigned int i = 0; i < no; i++) {
        // new passenger
        std::weak_ptr<Interface::IStop> destinationStop = offlinedata_->stops.at( rand() % offlinedata_->stops.size() );
        std::shared_ptr<Passenger> newPassenger = nullptr;
        newPassenger = std::make_shared<Passenger>( destinationStop );

        // add into data structure
        newPassenger->enterStop(stop);
        passengers_.push_back(newPassenger);
        stop->addPassenger(newPassenger);
        cityif_->addActor(newPassenger);
    }
}

bool Logic::takeCity(std::shared_ptr<Interface::ICity> city) {

    cityif_ = city;
    return true;
}

// ratio between real time and game time
const int Logic::TIME_SPEED = 10;
// time between updates in milliseconds
const int Logic::UPDATE_INTERVAL_MS = 100;

}
