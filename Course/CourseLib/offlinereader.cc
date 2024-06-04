#include "offlinereader.hh"
#include "actors/stop.hh"

#include <string>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QString>


namespace CourseSide
{

OfflineReader::OfflineReader()
{
}

std::shared_ptr<OfflineData> OfflineReader::readFiles(const QString& busfile,
                                                         const QString& stopfile)
{
    offlinedata_ = std::make_shared<OfflineData>();
    readStopFile(stopfile);
    readBusFile(busfile);


    return offlinedata_;
}

void OfflineReader::readBusFile(const QString &busfile)
{
    QString filecontent;
    QFile file;
    file.setFileName(busfile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    filecontent = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument document = QJsonDocument::fromJson( filecontent.toUtf8()  , &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing bus JSON: " << parse_error.errorString() << " at: " << filecontent.mid(parse_error.offset-3, parse_error.offset+3);
        return;
    }
    QJsonObject jsonObject = document.object();
    QJsonArray jsonArray = document.array();

    qDebug() << busfile;

    for (int i = 0; i < jsonArray.size(); i++) {

        QJsonObject o = jsonArray.at(i).toObject();

        std::shared_ptr<BusData> bus = std::make_shared<BusData>();
        bus->routeId = o.value("busId").toString().toInt();
        bus->routeName = o.value("busLineName").toString().toStdString();
        bus->routeNumber = o.value("busNro").toString().toInt();

        // Read final stop departure times
        QJsonArray departuretimes = o.value("startTimes").toArray();
        for (int j = 0; j < departuretimes.size(); j++) {
            QTime departuretime = calculateQTime(departuretimes.at(j).toString().toInt());
            bus->schedule.push_back(departuretime );
        }

        // Read stops and time from final stop
        QJsonArray fullSS = o.value("fullSS").toArray();
        for (int j = 0; j < fullSS.size(); j++) {

            // station / stop = SS obj
            QJsonObject SSobj = fullSS.at(j).toObject();
            std::string tikaikastring = SSobj.value("mns").toString().toStdString();

            int timemm = SSobj.value("mm").toInt();
            int timess = SSobj.value("ss").toInt();
            QTime time = QTime(timemm/60, timemm % 60, timess);

            std::shared_ptr<Stop> stop = nullptr;

            if (SSobj.value("stop") == true) {

                // Finding pointer for stop
                int id = SSobj.value("stationId").toInt();

                stop  = findStops(id);
                // Time, that is spent from final stop to this stop
                QJsonValue timevalue = SSobj.value("mm");
            }

            int width = SSobj.value("y").toDouble();
            int heightt = SSobj.value("x").toDouble();

            std::pair<Interface::Location, std::shared_ptr<Stop> > pair = { Interface::Location(width, heightt), stop  };

            bus->timeRoute2.insert(std::pair<QTime, std::pair<Interface::Location, std::shared_ptr<Stop> > >( time, pair  ));

        }

        offlinedata_->buses.push_back(bus);
    }
}

void OfflineReader::readStopFile(const QString &stopfile)
{
    QString filecontent;
    QFile file;
    file.setFileName(stopfile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    filecontent = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(filecontent.toUtf8());
    QJsonObject jsonObject = document.object();
    QJsonArray jsonArray = document.array();

    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject o = jsonArray.at(i).toObject();

        std::shared_ptr<Stop> pysakki = std::make_shared<Stop>(Interface::Location(o.value("y").toString().toDouble(), o.value("x").toString().toDouble()),
                                                                     o.value("name").toString(),
                                                                     (unsigned int)o.value("stationId").toString().toInt());

        offlinedata_->stops.push_back(pysakki);
    }
}

void OfflineReader::readRoute(std::shared_ptr<BusData> bus, QJsonObject& o)
{
    QJsonArray route = o.value("shape").toArray();
    for (int i = 0; i < route.size(); i++) {

        QJsonObject point = route.at(i).toObject();

        double width = point.value("y").toString().toDouble();
        double height = point.value("x").toString().toDouble();
        bus->route.push_back(Interface::Location(width, height));
    }
}

std::shared_ptr<Stop> OfflineReader::findStops(int id)
{
    std::shared_ptr<Stop> stop = NULL;
    for (unsigned int i = 0; i < offlinedata_->stops.size(); i++) {
        stop = offlinedata_->stops.at(i);
        int current = stop->getId();
        if (current == id) break;
    }
    return stop;
}

QTime OfflineReader::calculateQTime(int time)
{
    return QTime(time/100, time%100);
}

}
