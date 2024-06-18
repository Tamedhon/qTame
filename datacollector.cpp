#include "datacollector.h"


void DataCollector::run()
{
    while(!cancel)
    {
        if(timer->hasExpired())
        {
            emit sendDataCollection(buffer);
            buffer.clear();
        }
        msleep(10);
    }
}

void DataCollector::dataChunk(QString chunk)
{
    buffer += chunk;
    timer->setDeadline(250);
}
