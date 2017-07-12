#include "esitimer.h"
#include <QtCore>
#include <qDebug>
#include <QString>

EsiTimer::EsiTimer(QObject *parent) : QObject(parent)
{
    tpTimer = new QTimer(this);
    this->setTemperature(0);
    connect(tpTimer, SIGNAL(timeout()), this, SLOT(temperatureTimer()));
    tpTimer->start(1000);
}

void EsiTimer::temperatureTimer(){
    this->setTemperature(this->getTemperature() + 1);
    qDebug() << "temperature Timer executed";
    emit temperatureReading(getTemperature());
}

void EsiTimer::setTemperature(float val){
    this->temperature = val;
}

float EsiTimer::getTemperature(){
    return this->temperature;
}
