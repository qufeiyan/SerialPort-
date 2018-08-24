#ifndef PROCESSING_H
#define PROCESSING_H
#include <models/serialFrameModel.h>
#include <QObject>
#include <QList>

class AbstractProcessing
{
public:
    AbstractProcessing();

public:
    virtual bool detectState(IMUFrame*) = 0;

    virtual bool reportState(IMUFrame*) = 0;

};

#endif // PROCESSING_H
