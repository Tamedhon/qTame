#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QThread>
#include <QDeadlineTimer>

class DataCollector : public QThread
{
    Q_OBJECT
public:
    DataCollector(){timer = new QDeadlineTimer(250);}
    ~DataCollector(){delete timer;}
    void run() override;
    void Cancel(){cancel = true;}

private:
    bool cancel = false;
    QString buffer;
    QDeadlineTimer *timer;

signals:
    void sendDataCollection(QString data);

public slots:
    void dataChunk(QString chunk);
};

#endif // DATACOLLECTOR_H
