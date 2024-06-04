#include "mainwindow.hh"
#include "core/engine.hh"

#include <QDebug>

using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(800, 600);
    this->move((2560-1920)/2, (1440-1080)/2);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);



//    connect(timer, &QTimer::timeout, gameui, &GameWidget::advance);
//    timer->start(tick_);

    connect(this, SIGNAL(focusSig(QPointF)), this, SLOT(focus(QPointF)));

    connect(this->gameui, SIGNAL(onClick(QPoint)), this, SLOT(onViewClick(QPoint)));
    connect(this->gameui, SIGNAL(outOfFocus(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));
    connect(this->gameui, SIGNAL(inFocus(QFocusEvent*)), this, SLOT(focusInEvent(QFocusEvent*)));

    this->showStartMenu();

    // End screen
    this->endlayout->setMargin(this->width()/2-100);
    this->endscreen->setLayout(this->endlayout);

}

MainWindow::~MainWindow()
{
    delete closeBtn;
    delete gameui;
    delete timer;
}

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

void MainWindow::endGame()
{
    setParents(this->endscreen);

    this->endlayout->addWidget(this->bulletsHit.get());
    this->endlayout->addWidget(this->bulletsShot.get());
    this->endlayout->addWidget(this->accuracy.get());
    this->endlayout->addWidget(this->score.get());
    this->endlayout->addWidget(this->closeBtn);

    this->setCentralWidget(this->endscreen);

    disconnect(this, SLOT(close()));
    connect(this->closeBtn, &QPushButton::clicked, this, &MainWindow::close);

}

void MainWindow::updateCoords(ID id, int nX, int nY)
{
    this->gameui->updateCoords(id, nX, nY);
}

void MainWindow::updateScore(int score)
{
    this->score->setText(QString("Current score: %1").arg(score));
}

void MainWindow::updateBulletshit(int bulletshit)
{
    this->bulletsHit->setText(QString("Bullets hit: %1").arg(bulletshit));
}

void MainWindow::updateBulletsshot(int bulletsshot)
{
    this->bulletsShot->setText(QString("Bullets shot: %1").arg(bulletsshot));
}

void MainWindow::updateAccuracy(int accuracy)
{
    this->accuracy->setText(QString("Current accuracy: %1%").arg(accuracy));
}

void MainWindow::updateGameTime(int time)
{
    this->gameTimeLeftLabel->setText(QString("Time left: %1").arg(time));
}

void MainWindow::setPicture(const QPixmap& img)
{
    this->gameui->setBg(img);
}

void MainWindow::setPicture(const QString imgPath, const char type[])
{
    this->gameui->setBg(QPixmap(imgPath, type));
}

void MainWindow::setPicture(const QImage& img)
{
    this->gameui->setBg(QPixmap::fromImage(img));
}

std::unordered_set<int> MainWindow::getKeysPressed() const
{
    return this->keysPressed;
}

QRectF MainWindow::getPlayArea() const
{
    return this->gameui->getPlayArea();
}

int MainWindow::getGameTime() const
{
    if (this->startmenu != nullptr)
        return this->smGameTime->value();
    else
        return 0;
}

int MainWindow::getPlayerSpeed() const
{
    if (this->startmenu != nullptr)
        return this->smPlayerSpeed->value();
    else
        return this->smDefaultPlayerSpeed;
}


/* * * * * * * * * *
 *  Public  slots  *
 * * * * * * * * * */

ID MainWindow::addItem(int locX, int locY, ItemType type, ID id)
{
    QPoint loc = Engine::gamecoord2uicoord(QPoint(locX, locY));
    std::shared_ptr<Game::BetterActorItem> nActor = std::make_shared<Game::BetterActorItem>(loc.x(), loc.y(), type);
    this->gameui->addItem(nActor, id);

    return id;
}

ID MainWindow::addItem(int locX, int locY, ItemType type)
{
    return this->addItem(locX, locY, type, getNextID());
}

bool MainWindow::removeItem(ID id)
{
    return this->gameui->removeItem(id);
}

void MainWindow::focus(QPoint xy)
{
    this->gameui->centerOn(xy);
}

void MainWindow::focus(QPointF xy)
{
    this->gameui->centerOn(QPoint(xy.x(), xy.y()));
}

/* * * * * * * * *
 * Private slots *
 * * * * * * * * */

void MainWindow::on_smStartButton_clicked()
{
    this->showGame();
    emit startBtnClicked();
}

void MainWindow::onViewClick(QPoint xy)
{
    emit viewClicked(xy);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ( this->actuallyHasFocus()
         && !event->isAutoRepeat()
         && this->acceptedKeys.find(event->key()) != this->acceptedKeys.end()
       )
        keysPressed.insert(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if ( this->actuallyHasFocus()
         && !event->isAutoRepeat()
         && this->keysPressed.find(event->key()) != this->keysPressed.end()
       )
        this->keysPressed.erase(event->key());

}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    this->haveFocus = false;
    keysPressed.clear();
}

void MainWindow::focusInEvent(QFocusEvent *event)
{
    this->haveFocus = true;
}

void MainWindow::close() noexcept
{
    emit closedSig();
}

void MainWindow::showGame()
{
    this->setParents(this->gameui);
    this->setCentralWidget(this->gameui);
    this->gameTimeLeftLabel->setText(QString("Time left: %1").arg(this->smGameTime->value()));
    this->gameTimeLeft = this->smGameTime->value();
    this->gameTimeLeftLabel->setParent(this->gameui);

    disconnect(this, SLOT(close));
    connect(this->gameui, &GameWidget::close, this, &QMainWindow::close);

    QPoint btnPos = this->gameui->getCloseBtn()->pos();
    this->gameTimeLeftLabel->move(btnPos.x(), closeBtn->pos().y() + 230);
    this->bulletsHit->move(btnPos.x(), closeBtn->pos().y() + 250);
    this->bulletsShot->move(btnPos.x(), closeBtn->pos().y() + 270);
    this->accuracy->move(btnPos.x(), closeBtn->pos().y() + 290);
    this->score->move(btnPos.x(), closeBtn->pos().y() + 310);
}

void MainWindow::showStartMenu()
{

    if (this->startmenu == nullptr)
        this->initStartMenu();

    disconnect(this, SLOT(close()));
    this->setCentralWidget(this->startmenu);
}


/* * * * * * * * * *
 * Private methods *
 * * * * * * * * * */

bool MainWindow::actuallyHasFocus() const noexcept
{
    return ( this->haveFocus || this->gameui->hasFocus() ) ;
}

void MainWindow::initStartMenu()
{
    if (this->startmenu != nullptr)
        delete this->startmenu;

    this->startmenu = new QWidget(this);

    this->smGameTime->setRange(1, 60);
    this->smGameTime->setSingleStep(10);
    this->smGameTime->setValue(60);

    this->smPlayerSpeed->setRange(20, 100);
    this->smPlayerSpeed->setSingleStep(20);
    this->smPlayerSpeed->setValue(this->smDefaultPlayerSpeed);

    QPushButton* smStartBtn = new QPushButton("Start game", this->startmenu);
    QLabel* smPlayerSpeedLabel = new QLabel("Player movespeed", this->startmenu);
    QLabel* smGameTimeLabel = new QLabel("Game length (s)", this->startmenu);

    QVBoxLayout* lo = new QVBoxLayout(this->startmenu);
    lo->setMargin(this->width()/2-100);
    lo->addWidget(smGameTimeLabel);
    lo->addWidget(smGameTime);
    lo->addSpacing(20);
    lo->addWidget(smPlayerSpeedLabel);
    lo->addWidget(smPlayerSpeed);
    lo->addSpacing(20);
    lo->addWidget(smStartBtn);

    this->startmenu->setLayout(lo);
    connect(smStartBtn, &QPushButton::clicked, this, &MainWindow::on_smStartButton_clicked);
}

void MainWindow::setParents(QWidget* parent)
{
    this->bulletsHit->setParent(parent);
    this->bulletsShot->setParent(parent);
    this->accuracy->setParent(parent);
    this->score->setParent(parent);
}




