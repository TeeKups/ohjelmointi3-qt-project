#ifndef GAMEVIEWEVENTFILTER_HH
#define GAMEVIEWEVENTFILTER_HH

#include <QObject>
#include <QEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFocusEvent>

/**
 * @file
 * @brief Defines a GameViewEventFilter class for custom event filtering
 */

namespace Game {
/**
 * @brief The GameViewEventFilter class defines a custom event filter.
 *
 * The filter can detect when the watched object loses ot gains focus and when the watched object is clicked.
 * The filter also disables mouse-wheel and arrow keys.
 */
class GameViewEventFilter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a custom event filter designed for the QGraphicsView used in Game::MainWindow.
     */
    GameViewEventFilter();

    /**
     * @brief Filters events
     * @param ob: watched ibject
     * @param e: event
     * @return 'true' if blocked, otherwise 'false'
     * @pre this object has been installed as an event filter for the watched object 'ob'
     * @post Events are handled and appropriate signals emitted. Exception guarantee: strong.
     */
    bool eventFilter(QObject* ob, QEvent* e) override;

signals:
    void onClick(QPointF xy);
    void outOfFocus(QObject* ob, QFocusEvent* e);
    void inFocus(QObject* ob, QFocusEvent* e);
};

}

#endif // GAMEVIEWEVENTFILTER_HH
