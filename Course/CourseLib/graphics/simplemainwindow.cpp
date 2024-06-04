#include "simplemainwindow.hh"
#include "ui_simplemainwindow.h"
#include <QDebug>

const int PADDING = 10;

namespace CourseSide {

SimpleMainWindow::SimpleMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleMainWindow)
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_, height_);
    ui->centralwidget->setFixedSize(width_ + ui->startButton->width() + PADDING, height_ + PADDING);

    ui->startButton->move(width_ + PADDING , PADDING);

    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    resize(minimumSizeHint());
    //ui->gameView->fitInView(0,0, MAPWIDTH, MAPHEIGHT, Qt::KeepAspectRatio);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, map, &QGraphicsScene::advance);
    timer->start(tick_);
}

SimpleMainWindow::~SimpleMainWindow()
{
    delete ui;
}

void SimpleMainWindow::setSize(int w, int h)
{
    width_ = w;
    height_ = h;
}

void SimpleMainWindow::setTick(int t)
{
    tick_ = t;
}

void SimpleMainWindow::addActor(int locX, int locY, int type)
{
    SimpleActorItem* nActor = new SimpleActorItem(locX, locY, type);
    actors_.push_back(nActor);
    map->addItem(nActor);
    last_ = nActor;
}

void SimpleMainWindow::updateCoords(int nX, int nY)
{
    last_->setCoord(nX, nY);
}

void SimpleMainWindow::setPicture(QImage &img)
{
    map->setBackgroundBrush(img);
}

}

void CourseSide::SimpleMainWindow::on_startButton_clicked()
{
    qDebug() << "Start clicked";
    emit gameStarted();
}
