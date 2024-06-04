#ifndef GAMEERROR_HH
#define GAMEERROR_HH

#include <exception>
#include <QString>


/**
  * @file
  * @brief Defines an exception class for errors ingame.
  */


namespace Interface
{

/**
 * @brief Exception class that expresses errors ingame.
 */
class GameError : public std::exception
{
public:

    /**
     * @brief Default constructor.
     * @pre -
     * @post Creates GameError without message.
     */
    GameError();

    /**
     * @brief Constructor.
     * @param message- a message that clarifies what the exception is.
     * @pre -
     * @post Creates GameError that contains the message.
     */
    GameError(const QString& message);

    /**
     * @brief Destructor.
     */
    virtual ~GameError();

    /**
     * @brief Implements std::exception interface.
     * @pre -
     * @post Exception guarantee: nothrow.
     * @return Name of the exception class.
     */
    virtual const char* what() const noexcept;

    /**
     * @brief giveMessage gives a message that clarifies the cause of the exception.
     * @pre -
     * @post Exception guarantee: nothrow.
     * @return Message given in the constructor or empty string.
     */
    QString giveMessage() const;


private:
    QString message_;
};

}

#endif // GAMEERROR_HH
