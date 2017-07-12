#ifndef ESITIMER_H
#define ESITIMER_H
#include <QtCore>
#include <QObject>

class EsiTimer : public QObject
{
    Q_OBJECT
public:
    explicit EsiTimer(QObject *parent = 0);
    void setTemperature(float val);
    float getTemperature();

signals:
    void temperatureReading(float);

public slots:
    void temperatureTimer();

private:
    QTimer *tpTimer;
    float temperature;
};

#endif // ESITIMER_H
