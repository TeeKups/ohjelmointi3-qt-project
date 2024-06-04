#include "gamevieweventfilter.hh"
#include <memory>
#include <QGraphicsView>

using namespace Game;

#define block return true
#define pass return false

GameViewEventFilter::GameViewEventFilter()
{

}

bool GameViewEventFilter::eventFilter(QObject* ob, QEvent* e)
{
    switch(e->type())
    {
        case QEvent::Wheel:
        {
            block;
        }
        case QEvent::KeyPress:
        {
            QKeyEvent* ke = (QKeyEvent*)e;
            switch (ke->key())
            {
                case Qt::Key_Up: { block; }
                case Qt::Key_Down: { block; }
                case Qt::Key_Left: { block; }
                case Qt::Key_Right: { block; }
                default: { pass; }
            }

        }
        case QEvent::MouseButtonPress:
        {
            QMouseEvent* me = (QMouseEvent*)e;
            switch (me->button())
            {
                case Qt::LeftButton: {
                    emit onClick(me->pos());
                    block;
                }
            default: pass;

            }
        }
        case QFocusEvent::FocusOut:
        {
            emit outOfFocus(ob, (QFocusEvent*)e);
            pass;
        }
        case QFocusEvent::FocusIn:
        {
            emit inFocus(ob, (QFocusEvent*)e);
            pass;
        }
        default:
        {
            return QObject::eventFilter(ob, e);
        }
    }
}
