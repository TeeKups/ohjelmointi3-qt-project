#include "gamewidget.hh"
#include <QGraphicsView>
#include "core/engine.hh"


using namespace Game;

/* * * * * * * * * * * * * * * *
 *  Constructors & Destructor  *
 * * * * * * * * * * * * * * * */

GameWidget::GameWidget(QWidget* parent) :
    QWidget(parent)
{
    //this->view->setParent(this);

    this->view->setScene(scene);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setFixedSize(GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT);

    this->view->viewport()->installEventFilter(this->filter);
    this->view->installEventFilter(this->filter);

    this->closeBtn->move(GAME_SCENE_WIDTH + this->PADDING , this->PADDING);
    connect(closeBtn, &QPushButton::clicked, this, &GameWidget::close);

    connect(this->filter, SIGNAL(outOfFocus(QObject*, QFocusEvent*)), this, SLOT(outOfFocusSlot(QObject*, QFocusEvent*)));
    connect(this->filter, SIGNAL(inFocus(QObject*, QFocusEvent*)), this, SLOT(inFocusSlot(QObject*, QFocusEvent*)));
    connect(this->filter, SIGNAL(onClick(QPointF)), this, SLOT(onClickSlot(QPointF)));
}

GameWidget::~GameWidget()
{
    delete this->filter;
}

/* * * * * * * * * * *
 * Inherited methods *
 * * * * * * * * * * */
    ///nothing here

/* * * * * * * * * * * *
 *  Own public methods *
 * * * * * * * * * * * */

void GameWidget::addItem(std::shared_ptr<BetterActorItem> nActor, ID id)
{
    if (this->items.find(id) == this->items.end())
    {
        this->items[id] = nActor;
        scene->addItem(nActor.get());
    }
}

bool GameWidget::removeItem(ID id)
{
    if (this->items.find(id) != this->items.end())
    {
        this->scene->removeItem(this->items[id].get());
        this->items.erase(id);
        return true;
    }
    else
        return false;
}

void GameWidget::setBg(const QPixmap img)
{
    this->mapImg = img;
    this->scene->setBackgroundBrush(img);
    this->scene->setSceneRect(0, 0, this->mapImg.width(), this->mapImg.height());
}

void GameWidget::centerOn(QPoint xy)
{
    this->view->centerOn(xy);
}

void GameWidget::updateCoords(ID id, int nX, int nY)
{
    if (this->items.find(id) != this->items.end())
    {
        QPoint coord = Engine::gamecoord2uicoord(QPoint(nX, nY));
        this->items[id]->setCoord(coord.x(), coord.y());
    }
}

QRectF GameWidget::getPlayArea() const
{
    return this->scene->sceneRect();
}

bool GameWidget::hasFocus() const noexcept
{
    return this->viewFocus;
}

QRect GameWidget::getViewArea() const
{
    return this->view->viewport()->geometry();
}

const QPushButton *GameWidget::getCloseBtn()
{
    return this->closeBtn;
}


/* * * * * * * * * *
 *  Private slots  *
 * * * * * * * * * */

void GameWidget::onClickSlot(QPointF xyf)
{

    QPoint xy((int)xyf.x(), (int)xyf.y());
    xyf = this->view->mapToScene(xy);
    xy = QPoint((int)xyf.x(), (int)xyf.y());
    // sellaset Qt:t sieltä taas

    qDebug("x: %d, y: %d", xy.x(), xy.y());
    emit onClick(xy);
}

void GameWidget::outOfFocusSlot(QObject *ob, QFocusEvent* e)
{
    if ( ob == this->view  )
    {
        this->viewFocus = false;
        emit(outOfFocus(e));
    }
}

void GameWidget::inFocusSlot(QObject *ob, QFocusEvent* e)
{
    if ( ob == this->view )
    {
        this->viewFocus = true;
        emit(inFocus(e));
    }
}
