#include "engine.hh"
#include <QDebug>

#include "errors/initerror.hh"
#include "errors/gameerror.hh"

using namespace Game;

static bool LOGICDEBUG = false;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

Engine::Engine()
{

}

Engine::~Engine()
{
    qDebug() << "~Engine() called";
}

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

void Engine::start()
{
    this->ui->show();

    try {
        QImage bigBg(":/offlinedata/offlinedata/kartta_iso_1095x592.png", "PNG");
        QImage smallBg(":/offlinedata/offlinedata/kartta_pieni_500x500.png", "PNG");
        this->city->setBackground(smallBg, bigBg);
        this->ui->setPicture(this->city->getBackground());

        this->logic->setTime(13, 30);
        if (this->logic->takeCity(this->city)) {
            if (LOGICDEBUG) {
                QTime time = QTime(16, 30);
                this->logic->configChanged(time, true);
            }
            else
            {
                this->logic->fileConfig();
            }
        }
    }  catch (Interface::InitError e) {
        qDebug() << "Something went wrong in initialization of the game! FATAL";
        emit exitSig();
        throw (e);
    }

    connect(this->ui, &MainWindow::startBtnClicked, this, &Engine::startGameLogic);

    connect(this->ui, SIGNAL(viewClicked(QPoint)), this->player.get(), SLOT(shoot(QPoint)));
    connect(this->ui, &MainWindow::closedSig, this, &Engine::exit);

    connect(this->player.get(), SIGNAL(bulletMovedSig(std::shared_ptr<Interface::IActor>)), this->city.get(), SLOT(actorMoved(std::shared_ptr<Interface::IActor>)));
    connect(this->player.get(), SIGNAL(shot(int, int, ItemType, ID)), this->ui, SLOT(addItem(int, int, ItemType, ID)));
    connect(this->player.get(), SIGNAL(playerMovedSig(QPoint)), this->ui, SLOT(focus(QPoint)));
    connect(this->player.get(), SIGNAL(shot(int, int, ItemType, ID)), this->stats.get(), SLOT(bulletShotSlot()));

    connect(this->city.get(), SIGNAL(bulletHit(std::shared_ptr<Bullet>)), this->player.get(), SLOT(bulletDied(std::shared_ptr<Bullet>)));
    connect(this->city.get(), SIGNAL(bulletMissed(std::shared_ptr<Bullet>)), this->player.get(), SLOT(bulletDied(std::shared_ptr<Bullet>)));
    connect(this->city.get(), SIGNAL(actorMovedSgn(ID, Interface::Location)), this, SLOT(onActorMoved(ID, Interface::Location)));
    connect(this->city.get(), SIGNAL(bulletHit(std::shared_ptr<Bullet>)), this->stats.get(), SLOT(bulletHitSlot(std::shared_ptr<Bullet>)));
    connect(this->city.get(), SIGNAL(actorDied(std::shared_ptr<Interface::IActor>)), this->stats.get(), SLOT(actorDiedSlot(std::shared_ptr<Interface::IActor>)));
    connect(this->city.get(), SIGNAL(actorAdded(int, int, ItemType, ID)), this->ui, SLOT(addItem(int, int, ItemType, ID)));
    connect(this->city.get(), SIGNAL(actorDeleted(ID)), this->ui, SLOT(removeItem(ID)));

    connect(&this->timer, &QTimer::timeout, this->city.get(), &City::advance);
    connect(&this->timer, &QTimer::timeout, this->player.get(), &Player::advance);
    connect(&this->timer, &QTimer::timeout, this, &Engine::kboardHandler);

    connect(this->statsTimer.get(), &QTimer::timeout, this, &Engine::updateStats);
    connect(this->gameEndTimer.get(), &QTimer::timeout, this, &Engine::gameTimerStep);
}

ItemType Engine::getActorType(IActorPtr actor)
{

    if (std::dynamic_pointer_cast<CourseSide::Nysse>(actor) != nullptr)
        return NYSSE;
    else if (std::dynamic_pointer_cast<CourseSide::Passenger>(actor) != nullptr)
        return PASSENGER;
    else if (std::dynamic_pointer_cast<CourseSide::Stop>(actor) != nullptr)
        return STOP;
    else if (std::dynamic_pointer_cast<Game::Player>(actor) != nullptr)
        return PLAYER;
    else if (std::dynamic_pointer_cast<Game::Bullet>(actor) != nullptr)
        return BULLET;
    else
        return NONE;
}

QPoint Engine::gamecoord2uicoord(QPoint gamecrd)
{
    return(QPoint(gamecrd.x()+353, (500+36+20)-gamecrd.y()));
}

QPoint Engine::uicoord2gamecoord(QPoint uicrd)
{
    return(QPoint(uicrd.x()-353, (500+36+20)-uicrd.y()));
}

QPointF Engine::gamecoord2uicoord(QPointF gamecrd)
{
    return(QPointF(gamecrd.x()+353.0f, (500.0f+36.0f+20.0f)-gamecrd.y()));
}

QPointF Engine::uicoord2gamecoord(QPointF uicrd)
{
    return(QPointF(uicrd.x()-353.0f, (500.0f+36.0f+20.0f)-uicrd.y()));
}

/* * * * * * * * * *
 *  Public  slots  *
 * * * * * * * * * */

void Engine::startGameLogic()
{
    this->logicIsRunning = true;
    this->city->addPlayer(this->player);
    this->city->startGame();
    this->logic->finalizeGameStart();
    this->player->setMoveSpeed(static_cast<float>(this->ui->getPlayerSpeed())/20.0f);
    this->gameEndTimer->start(1000);
    this->gameTime = this->ui->getGameTime();
    this->statsTimer->start(100);
    this->timer.start(this->framerateTgt);
}

void Engine::updateStats()
{
    this->ui->updateScore(this->stats->getScore());
    this->ui->updateBulletsshot(this->stats->getBulletsshot());
    this->ui->updateBulletshit(this->stats->getBulletshit());
    if (this->stats->getBulletsshot() != 0){
        int accuracypercent = static_cast<int>(std::round(100 * this->stats->getBulletshit() / this->stats->getBulletsshot()));
        this->ui->updateAccuracy(accuracypercent);
    }
}

void Engine::onActorMoved(ID who, Interface::Location where)
{
    QPoint xy = QPoint(where.giveX(), where.giveY());
    this->ui->updateCoords(who, xy.x(), xy.y());
}

/* * * * * * * * * *
 * Private methods *
 * * * * * * * * * */

void Engine::kboardHandler()
{
    int xdir = 0, ydir = 0;
    std::unordered_set<int> keys = this->ui->getKeysPressed();

    if (keys.find((Qt::Key_W)) != keys.end())
        ydir += 1;

    if (keys.find(int(Qt::Key_A)) != keys.end())
        xdir -= 1;

    if (keys.find(int(Qt::Key_S)) != keys.end())
        ydir -= 1;

    if (keys.find(int(Qt::Key_D)) != keys.end())
        xdir += 1;

    if (this->player != nullptr)
    {
        this->player->moveToDir(Math::unitVector2d(xdir, ydir));

        if ( this->city->outOfBounds( this->player->giveLocation() ))
            this->player->moveBack();
        else {
            this->ui->updateCoords(player->getID(), player->giveLocation().giveX(), player->giveLocation().giveY());
        }
    }

    // more keys and actions can be added
}

void Engine::stopGame()
{
    this->logicIsRunning = false;
    this->statsTimer->stop();
    this->gameEndTimer->stop();
    this->timer.stop();
    this->logic.reset();
    this->ui->endGame();
    this->city->gameOver();
}

/* * * * * * * * *
 * Private slots *
 * * * * * * * * */

void Engine::gameTimerStep()
{
    this->gameTime--;
    if (this->logicIsRunning)
        this->ui->updateGameTime(this->gameTime);
    if (gameTime <= 1)  // <= because the timer ticks 1s "behond"
        this->stopGame();
}

void Engine::exit() noexcept
{
    emit exitSig();
}
