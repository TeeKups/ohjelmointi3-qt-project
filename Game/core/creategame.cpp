#include <memory>

#include "creategame.hh"
#include "city.hh"

std::shared_ptr<Interface::ICity> Interface::createGame()
{
    return std::make_shared<Game::City>();
}
