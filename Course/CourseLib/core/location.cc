#include "location.hh"

#include <math.h>
#include <QDebug>


namespace Interface
{

Location::Location() : northcoord_(6700000), eastcoord_(3500000), x_(0), y_(0)
{

}

Location::Location(int northcoord, int eastcoord) : Location()
{

    // LATITUDE = Y-AXIS
    // LONGITUDE = X-AXIS
    setNorthEast(northcoord, eastcoord);
}

int Location::giveX() const
{
    return x_;
}

int Location::giveY() const
{
    return y_;
}

void Location::setXY(int x, int y)
{
    x_ = x;
    y_ = y;
    eastcoord_ = EastFromX(x);
    northcoord_ = NorthFromY(y);
}

double Location::giveNorthernCoord() const
{
    return northcoord_;
}

double Location::giveEasternCoord() const
{
    return eastcoord_;
}

void Location::setNorthEast(int northcoord, int eastcoord)
{
    northcoord_ = northcoord;
    eastcoord_ = eastcoord;
    x_ = xFromEast(eastcoord);
    y_ = yFromNorth(northcoord);
}

bool Location::isClose(const Location &loc, int limit) const
{
    int dx = loc.giveX() - giveX();
    int dy = loc.giveY() - giveY();

    return dx*dx + dy*dy <= limit*limit;
}

int Location::xFromEast(int eastcoord)
{
    // 3327034 is kkj3-joint coordinate system point
    // at bottom left corner of small map
    // scaling is 500:1500
    return static_cast<int>((eastcoord - 3327034) * 0.333333);
}

int Location::yFromNorth(int northcoord)
{
    // 6824642 is kkj3-joint coordinate system point
    // at nottom left corner of small map
    // scaling is 500:1500
    return static_cast<int>((northcoord - 6824642) * 0.333333);
}


int Location::EastFromX(int x)
{
    return static_cast<int>(x/0.333333 + 3327034);
}

int Location::NorthFromY(int y)
{
    return static_cast<int>(y/0.333333 + 6824642);
}

double Location::calcDistance(Location a, Location b)
{


    // Approximation because distances are vesy short
    double distance = sqrt(pow(a.giveEasternCoord() - b.giveEasternCoord(), 2) + pow(a.giveNorthernCoord() - b.giveNorthernCoord(), 2));
    return distance;
}

Location Location::calcBetween(Location a, Location b, double distance)
{
    // Creating a vector
    double widthdiff = b.giveNorthernCoord() - a.giveNorthernCoord();
    double heightdiff = b.giveEasternCoord() - a.giveEasternCoord();

    // New location is found by moving along the vector from point a for the wanted portion.
    return Location(a.giveNorthernCoord() + widthdiff*distance, a.giveEasternCoord() + heightdiff*distance);
}

void Location::printBoth() {
    qDebug() << northcoord_ << " " << eastcoord_;
    qDebug() << x_ << " " << y_;
}

bool Location::operator==(const Location &location)
{
    // comparison operator
    // needed, for determining wether a bus stop and location are equal
    return (this->x_==location.giveX() && this->y_ == location.giveY());
}

}
