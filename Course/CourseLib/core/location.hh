#ifndef LOCATION_HH
#define LOCATION_HH

/**
  * @file
  * @brief Defines a class that contains methods for handling location. (coordinates)
  *
  */

namespace Interface
{

/**
 * @brief Location is a class, which has methods dealing with the location of the objects.
 *
 * The class provides transformation from map coordinates to pixel coordinates and back, calculation
 * of the distnce and the possibility of generating points between two coordinates.
 *
 */
class Location
{
public:
    /**
     * @brief Default constructor
     * @post Location is set to north=6700000, east=3500000.
     */
    Location();

    /**
     * @brief Constructor that defines a location in certain map coordinate.
     * @param northcoord northern coordinate of the location
     * @param eastcoord eastern coordinate of the location
     * @pre -
     * @post Location is given.
     */
    Location(int northcoord, int eastcoord);

    /**
     * @brief giveX returns the x-coordinate of the location in the pixel grid of the game ui.
     * @pre -
     * @return X-pixel coordinate.
     * @post Exception guarantee: nothrow.
     */
    int giveX() const;

    /**
     * @brief giveY returns the y-coordinate of the location in the pixel grid of the game ui.
     * @pre -
     * @return Y-pixel coordinate.
     * @post Exception guarantee: nothrow.
     */
    int giveY() const;

    /**
     * @brief setXY moves the location to a new point.(In pixel grid)
     * @param x x-pixel coordinate of the new location
     * @param y y-pixel coordinate of the new location
     * @pre -
     * @post Location is updated. Exception guarantee: strong.
     */
    void setXY(int x, int y);

    /**
     * @brief giveNorthernCoord returns the location of the northern coordinate from map grid.
     * @pre -
     * @return Northern coordinate
     * @post Exception guarantee: nothrow.
     */
    double giveNorthernCoord() const;

    /**
     * @brief giveEasternCoord returns the location of the eastern coordinate from map grid.
     * @pre -
     * @return Eastern coordinate
     * @post Exception guarantee: nothrow.
     */
    double giveEasternCoord() const;

    /**
     * @brief setNorthEast moves the location to a new coordinate in map grid.
     * @param northcoord northern coordinate of the new location
     * @param eastcoord eastern coordinate of the new location
     * @pre -
     * @post Location is updated. Exception guarantee: strong.
     */
    void setNorthEast(int northcoord, int eastcoord);

    /**
     * @brief isClose tells if given location is close to this location.
     *
     * Returns true if in function call `s1.isClose(s2)` positions `s1` and `s2` are close enough
     * to affect each other in game.
     *
     * @param loc location which closeness ic checked.
     * @pre -
     * @return Boolean which tells wether loc is close to this location.
     * @post Exception guarantee: nothrow.
     */
    bool isClose(Location const& loc, int limit = 10) const;

    /**
     * @brief calcDistance calculates the distance between two locations in map grid.
     * @param a first location
     * @param b second location
     * @pre -
     * @return distance between two locations
     * @post Exception guarantee: nothrow.
     */
    static double calcDistance(Location a, Location b);

    /**
     * @brief calcBetween calculates wanted position between two locations.
     * @param a first location
     * @param b second location
     * @param distance ratio where point between a and b is. 0.0=at a, 1.0=at b, 0.5=in the middle.
     * @pre -
     * @return Location of the ration between a and b
     * @post Exception guarantee: nothrow.
     */
    static Location calcBetween(Location a, Location b, double distance);

    /**
     * @brief printBoth prints (for debugging purposes) both map and pixel coordinates of the location.
     * @pre -
     * @post Prints coordinates. Exception guarantee: strong.
     */
    void printBoth();
    bool operator==(const Location& location);

private:

    static int xFromEast(int eastcoord);
    static int yFromNorth(int northcoord);
    static int EastFromX(int x);
    static int NorthFromY(int y);

    int northcoord_;
    int eastcoord_;
    int x_;
    int y_;
};

}

#endif // LOCATION_HH
