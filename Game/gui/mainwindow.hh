#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <iostream>
#include <memory>
#include <QVector>
#include <map>
#include <QPushButton>
#include <unordered_map>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <unordered_set>

#include "interfaces/iactor.hh"
#include "gui/betteractoritem.hh"
#include "misc/types.hh"
#include "gamevieweventfilter.hh"
#include "gamewidget.hh"

/**
 * @file
 * @brief Defines a MainWindow class
 */

namespace Game {

/**
 * @brief The MainWindow class has methods for updating the displayed GUI.
 *
 * The class can change the displayed scene on certain conditions, focus the view of a GameWidget to a point, set
 * a GameWidget's background, and handle adding, removing and moving BetterActorItems in a GameWidget, and gathering
 * keyboard-input.
 *
 * The class also has methods for updating and the shown statistics.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow
     * @param parent: parent of the MainWindow
     */
    MainWindow(QWidget *parent = nullptr);

    /**
      * @brief Destructs the MainWindow
      */
    ~MainWindow();

    /**
     * @brief Sets the endGame -widget visible.
     * @pre The game should end first.
     * @post The endGame -widget is set visible. Exception guarantee: basic
     */
    void endGame();

    /**
     * @brief Updates the coordinates of item with ID 'id' from this->gameui
     * @param id: ID of the item
     * @param nX: X-coordinate
     * @param nY: Y-coordinate
     * @pre item with id ID should exist in this->gameui
     * @post If item with ID id is found, its coordinates are updated. Exception guarantee: strong
     */
    void updateCoords(ID id, int nX, int nY);

    /**
     * @brief Updates the displayed score
     * @param score: new score
     * @pre -
     * @post Exception guarantee: basic
     */
    void updateScore(int score);

    /**
     * @brief Updates displayed bullets hit counter
     * @param bulletshit: new amount
     * @pre -
     * @post Exception guarantee: baisc
     */
    void updateBulletshit(int bulletshit);

    /**
     * @brief Updates displayed bullets shot counter
     * @param bulletsshot: new amount
     * @pre -
     * @post Exception guarantee: basic
     */
    void updateBulletsshot(int bulletsshot);

    /**
     * @brief Updates the displayed accuracy
     * @param accuracy: new accuracy
     * @pre -
     * @post Exception guarantee: basic
     */
    void updateAccuracy(int accuracy);

    /**
     * @brief Updates the displayed game time
     * @param time: new time
     * @pre -
     * @post Exception guarantee: basic
     */
    void updateGameTime(int time);

    /**
     * @brief Sets background picture of this->gameui
     * @param imgPath: path to picture
     * @param type: type of picture
     * @pre Must be a valid image
     * @post Background is set. Exception guarantee: basic
     */
    void setPicture(const QString imgPath, const char type[]);

    /**
     * @brief Sets background picture of this->gameui
     * @param img: QImage object containing the background
     * @pre Must be a valid QImage
     * @post Background is set. Exception guarantee: basic
     */
    void setPicture(const QImage& img);

    /**
     * @brief Sets background picture of this->gameui
     * @param img: QPixmap object containing the background
     * @pre Must be a valid QPixmap
     * @post Background is set. Exception guarantee: basic
     */
    void setPicture(const QPixmap& img);

    /**
     * @brief Returns the pressed keys
     * @return Set of pressed keys
     * @pre -
     * @post Exception guarantee: strong
     */
    std::unordered_set<int> getKeysPressed() const;

    /**
     * @brief Returns QRectF that defines the scene of this->gameui
     * @return QRectF that defines the scene of this->gameui
     * @pre -
     * @post Exception guarantee: strong
     */
    QRectF getPlayArea() const;

    /**
     * @brief Returns the time that is set in start menu
     * @return The time set in start menu
     * @pre The start menu should be as central widget i.e. showStartMenu called and showGame not called
     * @post Exception guarantee: strong
     */
    int getGameTime() const;

    /**
     * @brief Returns the player speed set in start menu
     * @return The player speed set in start menu
     * @pre The start menu should be as central widget i.e. showStartMenu called and showGame not called
     * @post  Exception guarantee: strong
     */
    int getPlayerSpeed() const;

signals:
    void startBtnClicked();
    void debugBtnClicked();
    void focusSig(QPointF xy);
    void viewClicked(QPoint xy);
    void closedSig();

public slots:
    /**
     * @brief adds a BetterActorItem to this->gameui
     * @param locX: X-coordinate
     * @param locY: Y-coordinate
     * @param type: ItemType of the actor represented by this item
     * @param id: ID of the item
     * @return ID of the item
     * @pre The item should not have been added before
     * @post Item is added in this->gameui. Exception guarantee: strong
     */
    ID addItem(int locX, int locY, ItemType type, ID id);

    /**
     * @brief adds a BetterActorItem to this->gameui and generates an ID for it
     * @param locX: X-coordinate
     * @param locY: Y-coordinate
     * @param type: ItemType of the actor represented by this item
     * @return generated ID of the item
     * @pre The item should not have been added before
     * @post Item is added in this->gameui and ID is generated. Exception guarantee: strong
     */
    ID addItem(int locX, int locY, ItemType type);

    /**
     * @brief Removes a BetterActorItem that matches the ID from this->gameui
     * @param id: ID of the BetterActorItem
     * @return 'true' on succcess, 'false' otherwise
     * @pre a BetterActorItem with ID 'id' should have been added earlier
     * @post BetterActorItem is removed. Exception guarantee: strong
     */
    bool removeItem(ID id);

    /**
     * @brief Focuses the view in this->gameui on point xy
     * @param xy: target of focus
     * @pre this->gameui should be as the central widget i.e. showGame() called
     * @post Exception guarantee: basic
     */
    void focus(QPoint xy);

    /**
     * @brief Focuses the view in this->gameui on point xy
     * @param xy: target of focus
     * @pre this->gameui should be as the central widget i.e. showGame() called
     * @post Exception guarantee: basic
     */
    void focus(QPointF xy);

private slots:

    /**
     * @brief Calls showGame();
     * @pre Should be connected to a signal
     * @post showGame is called. Exception guarantee: strong.
     */
    void on_smStartButton_clicked();

    /**
     * @brief Emits viewClicked(xy)
     * @param xy: coordinates of the click
     * @pre Should be connected to a signal that passes clicks from a view
     * @post Emits viewClicked. Exception guarantee: basic.
     */
    void onViewClick(QPoint xy);

    /**
     * @brief Reads keypresses and adds to keysPressed
     * @param event: Key-press-event
     * @pre -
     * @post keysPressed contains the pressed key. Exception guarantee: strong
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief Removes released key from keysPressed
     * @param event: Key-release-event
     * @pre -
     * @post keysPressed does not contain the pressed key Exception guarantee: strong
     */
    void keyReleaseEvent(QKeyEvent* event);

    /**
     * @brief Window loses focus
     * @param event: QFocusEvent*
     * @pre -
     * @post Window no longet has focus and pressed keys are cleared. Exception guarantee: strong.
     */
    void focusOutEvent(QFocusEvent* event);

    /**
     * @brief Window gains focus
     * @param event: QFocusEvent*
     * @pre -
     * @post Window has focus. Exception guarantee: strong
     */
    void focusInEvent(QFocusEvent* event);

    /**
     * @brief Closes the window and emits closedSig
     * @pre -
     * @post Exception guarantee: no-throw
     */
    void close() noexcept;

    /**
     * @brief Shows this->gameui and stats
     * @pre this->gameui is not nullptr.
     * @post this->gameui is shown and old connections to MainWindow::close are disconnected. Exception guarantee: basic
     */
    void showGame();

    /**
     * @brief Shows the start menu
     * @pre -
     * @post Start menu is shown. Exception guarantee: strong.
     */
    void showStartMenu();

private:

    /// Methods

    /**
     * @brief Tells wether the MainWindow has focus or not
     * @return 'true' if has focus, 'false' otherwise
     * @pre -
     * @post Exception guarantee: no-throw
     */
    bool actuallyHasFocus() const noexcept;

    /**
     * @brief Initializes the start menu.
     * @pre -
     * @post Start menu is initialized. Exception guarantee: strong.
     */
    void initStartMenu();

    /**
     * @brief Hands ceratain UI elements to a new parent
     * @param parent: new parent of the elements
     * @pre -
     * @post Exception guarantee: strong
     */
    void setParents(QWidget* parent);

    /// Some variables
    const std::unordered_set<int> acceptedKeys = {int(Qt::Key_W), int(Qt::Key_A), int(Qt::Key_S), int(Qt::Key_D)};
    const int PADDING = 5;
    const int smDefaultPlayerSpeed = 40;

    int gameTimeLeft;

    /// UI elements

    // Parents should be defined first.
    GameWidget* gameui = new GameWidget();
    QWidget* startmenu = nullptr;
    QWidget* endscreen = new QWidget();

    QPixmap mapImg;
    std::unordered_map<ID, std::shared_ptr<Game::BetterActorItem>> items;

    bool haveFocus = false;

    std::unique_ptr<QLabel> bulletsHit = std::make_unique<QLabel>("Bullets hit: 0      ");
    std::unique_ptr<QLabel> bulletsShot = std::make_unique<QLabel>("Bullets shot: 0      ");
    std::unique_ptr<QLabel> accuracy = std::make_unique<QLabel>("Current accuracy:             ");
    std::unique_ptr<QLabel> score = std::make_unique<QLabel>("Current score:             ");
    std::unique_ptr<QLabel> gameTimeLeftLabel = std::make_unique<QLabel>("Time left:         ");
    QPushButton* closeBtn = new QPushButton("close");

    std::unordered_set<int> keysPressed;
    QSpinBox* smGameTime = new QSpinBox(this->startmenu);
    QSpinBox* smPlayerSpeed = new QSpinBox(this->startmenu);

    QVBoxLayout* layout = new QVBoxLayout();

    QVBoxLayout* endlayout = new QVBoxLayout(this->endscreen);
    QTimer* timer = new QTimer();


};

} // namespace Game
#endif // MAINWINDOW_HH
