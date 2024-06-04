#include "gameerror.hh"


namespace Interface
{

GameError::GameError() :
    std::exception(), message_()
{
}


GameError::GameError(const QString& message) :
    std::exception(), message_(message)
{
}


GameError::~GameError()
{
}


const char* GameError::what() const noexcept
{
    return "Interface::GameError";
}


QString GameError::giveMessage() const
{
    return message_;
}

}
