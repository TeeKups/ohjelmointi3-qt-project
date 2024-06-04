#ifndef OFFLINEREADER_HH
#define OFFLINEREADER_HH

#include "actors/stop.hh"
#include "core/location.hh"
#include "actors/nysse.hh"

#include <list>
#include <QString>
#include <map>
#include <QJsonObject>


namespace CourseSide
{

struct Place {
    bool stop;
    std::shared_ptr<Stop> stopPtr;
    std::shared_ptr<Interface::Location> locationPtr;
};

struct BusData {
    unsigned int routeNumber; // means bus number, e.g. line 25
    unsigned int routeId; // unique identifier, running number
    std::string routeName; // name of the line, in the form of "Pyynikintori - Hervanta"

    std::map< QTime, std::shared_ptr<Stop> > stops;
    std::list<QTime> schedule;
    std::vector< Interface::Location > route;
    std::map<QTime, Interface::Location > timeRoute;
    std::map< QTime, std::pair<Interface::Location, std::shared_ptr<Stop>> > timeRoute2;
};

struct OfflineData {
    std::vector< std::shared_ptr<Stop> > stops;
    std::list< std::shared_ptr<BusData> > buses;
};

class OfflineReader
{
public:
    OfflineReader();

    std::shared_ptr<OfflineData> readFiles(const QString& busfile, const QString& stopfile);

private:
    std::shared_ptr<OfflineData> offlinedata_;

    void readBusFile(const QString& busfile);
    void readStopFile(const QString& stopfile);
    void readDepartureTimes(const QJsonArray& timearray, BusData* bus);
    void readRoute(std::shared_ptr<BusData> bus, QJsonObject& o);
    std::shared_ptr<Stop> findStops(int id);
    QTime calculateQTime(int time);
};

}

#endif // OFFLINEREADER_HH
