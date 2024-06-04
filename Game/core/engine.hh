#ifndef ENGINE_HH
#define ENGINE_HH

#include <memory>
#include <QApplication>
#include <QImage>
#include <QPoint>
#include <QTimer>

#include "creategame.hh"

#include "core/logic.hh"
#include "gui/mainwindow.hh"
#include "stats/statistics.hh"

#include "city.hh"

/**
 * @file
 * @brief Defines an Engine class, i.e. a game engine
 */

namespace Game {

/**
 * @brief The Engine class has methods for communicating between different elements of the game. One could think of it as a "breadboard".
 *
 * Engine has static methods that are commonly used by many other classes that are dependent on Engine. The static methods aid in
 * communicating between different kinds of coordinate systems and observing the types of the actors.
 *
 * The Engine handles making connections between different classes, starting and stopping the game logic, and kayboard events.
 */
class Engine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the engine pointer. Logic doesn't start until Engine::start is called.
     */
    Engine();

    /**
      * @brief Destructs the engine
      */
    ~Engine();

    /**
     * @brief Starts the game logic
     * @pre -
     * @post Game starts. Exception guarantee strong
     * @exception Interface::InitError - Something went wrong in the initialization
     */
    void start();

    /**
     * @brief Tells the type of the actor
     * @param actor: std::shared_ptr<Interface::IActor> whose type is wanted
     * @return ItemType of the actor. Exception guarantee: strong.
     */
    static ItemType getActorType(IActorPtr actor);

    /**
     * @brief Translates the game's coordinate system to Qt coordinate system
     * @param xy: coordinates in game's system
     * @return Equivalent coordinates in Qt system. Exception guarantee: strong.
     */
    static QPoint gamecoord2uicoord(QPoint xy);

    /**
     * @brief Translates the Qt coordinate system to game's coordinate system
     * @param xy: coordinates in Qt system
     * @return Equivalent coordinates in game's system. Exception guarantee: strong.
     */
    static QPoint uicoord2gamecoord(QPoint xy);

    /**
     * @brief Translates the game's coordinate system to Qt coordinate system
     * @param xy: coordinates in game's system
     * @return Equivalent coordinates in Qt system. Exception guarantee: strong.
     */
    static QPointF gamecoord2uicoord(QPointF xy);

    /**
     * @brief Translates the Qt coordinate system to game's coordinate system
     * @param xy: coordinates in Qt system
     * @return Equivalent coordinates in game's system. Exception guarantee: strong.
     */
    static QPointF uicoord2gamecoord(QPointF xy);

signals:
    void advance();
    void actorMoved(ID who);
    void exitSig();

private:

    const int framerateTgt = static_cast<int>(std::round(1000.0f/144.0f));
    MainWindow* ui = new MainWindow();
    QTimer timer;  // gets initialized in its constructor or smthing idk

    std::unique_ptr<Statistics> stats = std::make_unique<Statistics>();
    std::unique_ptr<QTimer> statsTimer = std::make_unique<QTimer>();
    std::shared_ptr<QTimer> gameEndTimer = std::make_shared<QTimer>();
    std::shared_ptr<Game::City> city = std::make_shared<Game::City>();
    std::unique_ptr<CourseSide::Logic> logic = std::make_unique<CourseSide::Logic>();
    std::shared_ptr<Player> player = std::make_shared<Player>(Interface::Location(6825700, 3327645));
    int gameTime;
    bool logicIsRunning = false;

    /**
     * @brief Reads pressed keys from this->ui and performs actions based on them
     * @pre -
     * @post Actions are performed. Exception guarantee: basic.
     */
    void kboardHandler();

    /**
     * @brief Stops the game logic.
     * @pre -
     * @post Game is stopped. Exception guarantee: strong.
     */
    void stopGame();

private slots:
    /**
     * @brief Starts the game logic
     * @pre Engine must be running.
     * @post Game starts. Exception guarantee: basic.
     */
    void startGameLogic();

    /**
     * @brief Updates statistics
     * @pre this->statistics must be initialized.
     * @post Statistics are updated. Exception guarantee: basic.
     */
    void updateStats();

    /**
     * @brief Updates graphic objects' coordinates
     * @param who: ID of moved actor
     * @param where: location of moved actor
     * @pre -
     * @post Actors move on screen. Exception guarantee: basic
     */
    void onActorMoved(ID who, Interface::Location where);

    /**
     * @brief Updates remaining time in UI and decrements remaining time counter
     * @pre this->logicIsRunning == true
     * @post Remaining time is updated and this->gameTime decreased. Exception guarantee: basic.
     */
    void gameTimerStep();

    /**
     * @brief Emits exit signal. Can be used in i.e. main() to tell when the engine exits.
     * @pre -
     * @post Exit signal is emitted. Exception guarantee: no-throw
     */
    void exit() noexcept;

};

}

#endif // ENGINE_HH
