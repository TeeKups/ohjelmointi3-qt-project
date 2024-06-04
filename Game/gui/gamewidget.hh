#ifndef GAMEWIDGET_HH
#define GAMEWIDGET_HH

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>
#include <unordered_map>
#include <QPixmap>
#include <QPushButton>

#include "gamevieweventfilter.hh"
#include "betteractoritem.hh"
#include "misc/types.hh"

/**
 * @file
 * @brief Defines a GameWidget class
 */
namespace Game {

/**
 * @brief The GameWidget class contains functions for easily managing a composition of certain UI components
 *
 * GameWidget is a composition of a QWidget, QGraphicsView, QGraphicsScene and a QPushbutton. The class implements
 * methods for adding/removing BetterActorItems to/from the QGraphicScene, moving the BetterActorItems, setting the
 * background of the QGraphicScene, and observing the components.
 *
 * The class emits signals when the QGraphicsView loses or gains focus, is clicked or when closeBtn is clicked.
 *
 * The class could be used to create e.g. multiple GameWidgets which could represent entirely different cities,
 * wievs of different players (split screen), or something like that. The composition of the aforementioned
 * classes is justified by the fact that handling all the components in a singl MainWindow would be very tedious
 * in comparison to passing the internal logic of the Widget to its own class.
 */
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a GameWidget
     * @param parent: parent of the widget
     * @post A GameWidget with an empty background and close button is created. Exception guarantee: strong
     */
    GameWidget(QWidget* parent = nullptr);

    /**
      * @brief Destructs a GameWidget
      */
    ~GameWidget();

    /**
     * @brief Adds a BetterActorItem into the widget's scene if it isnt there yet.
     * @param nActor: pointer to actor being added
     * @param id: the actor's ID
     * @pre The same actor should not have been added before.
     * @post An actor is added to the scene. Exception guarantee: strong.
     */
    void addItem(std::shared_ptr<Game::BetterActorItem> nActor, ID id);

    /**
     * @brief Removes a BetterActorItem with ID'id' from the widget's scene.
     * @param itemID: ID of the item that should be removed
     * @return 'true' on success, 'false' otherwise
     * @pre -
     * @post Item is removed from the scene if found. Exception guarantee: strong.
     */
    bool removeItem(ID itemID);

    /**
     * @brief Sets the background of the widget's scene
     * @param img: the new background
     * @pre img should be a valid QPixmap
     * @post Background is set. Exception guarantee: basic.
     */
    void setBg(const QPixmap img);

    /**
     * @brief Centers the widget's view on 'xy'
     * @param xy: target of centering
     * @pre -
     * @post View is centered on 'xy'. Exception guarantee: strong.
     */
    void centerOn(QPoint xy);

    /**
     * @brief Updates the coordinates of an item with ID 'id'
     * @param id: id of the item
     * @param nX: X-coordinate
     * @param nY: Y-coordinate
     * @pre A BetterActorItem with ID 'id' should have been added before
     * @post Betteractoritem with ID 'id' is removed. Exception guarantee: strong
     */
    void updateCoords(ID id, int nX, int nY);

    /**
     * @brief Returns the sceneRect of the wiget's scene
     * @return scenerect of widget's scene
     * @pre -
     * @post Exception guarantee: strong.
     */
    QRectF getPlayArea() const;

    /**
     * @brief Returns 'true' if the widget's view has focus
     * @return 'true' if the widget's view has focus, 'false' otherwise
     * @pre -
     * @post Exceptin guarantee: no-throw;
     */
    bool hasFocus() const noexcept;

    /**
     * @brief Returns the geometry of the widget's viewport.
     * @return Geometry of the widget's viewport.
     * @pre -
     * @post Exception guarantee: strong
     */
    QRect getViewArea() const;

    /**
     * @brief Returns a const pointer to the closeBtn of this widget
     * @return Const pointer to the closeBtn
     * @pre -
     * @post Exception guarantee: strong
     */
    const QPushButton* getCloseBtn();

signals:
    void onClick(QPoint xy);
    void outOfFocus(QFocusEvent*);
    void inFocus(QFocusEvent*);
    void close();

private slots:
    /**
     * @brief emits onClick(Qpoint xy) when view is clicked, where xy is in scene coordinates
     * @param xyf: coordinates of view
     * @pre -
     * @post emits onClick. Exception guarantee: basic
     */
    void onClickSlot(QPointF xyf);

    /**
     * @brief Emits outOfFocus(QFocusEvent*) when view loses focus
     * @param ob: watched object
     * @param e: QFocusEvent
     * @pre -
     * @post Exception guarantee: strong
     */
    void outOfFocusSlot(QObject* ob, QFocusEvent* e);

    /**
     * @brief Emits outOfFocus(QFocusEvent*) when view gains focus
     * @param ob: watched object
     * @param e: QFocusEvent
     * @pre -
     * @post Exception guarantee: strong
     */
    void inFocusSlot(QObject* ob, QFocusEvent* e);

private:
    const int PADDING = 5;
    const int GAME_SCENE_WIDTH = 500; //pxls
    const int GAME_SCENE_HEIGHT = 500;

    QGraphicsView* view = new QGraphicsView(this);
    QGraphicsScene* scene = new QGraphicsScene(0, 0, GAME_SCENE_WIDTH, GAME_SCENE_HEIGHT, view);
    QObject* filter  = new GameViewEventFilter();
    QPushButton* closeBtn = new QPushButton("Quit", this);

    std::unordered_map<ID, std::shared_ptr<Game::BetterActorItem>> items;
    bool viewFocus = false;
    QPixmap mapImg;

};
}

#endif // GAMEWIDGET_HH
