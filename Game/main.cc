#include <memory>
#include <QApplication>
#include <QImage>

#include "creategame.hh"
#include "core/city.hh"
#include "core/logic.hh"
#include "core/engine.hh"
#include "errors/initerror.hh"

#include "gui/mainwindow.hh"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);

    Game::Engine engine;
    try {
        engine.start();
    }  catch (...) {
        qDebug() << "Could not start engine!";
    }

    QObject::connect(&engine, &Game::Engine::exitSig, &a, &QCoreApplication::quit);

   return a.exec();
}

