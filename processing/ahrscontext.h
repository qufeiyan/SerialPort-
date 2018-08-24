#ifndef AHRSCONTEXT_H
#define AHRSCONTEXT_H
#include "AbstractAHRS.h"
#include "models/serialFrameModel.h"
class AHRSContext
{
public:
    AHRSContext();
    void setAHRS(AbstractAHRS* ahrs);
    AbstractAHRS* getAHRS();
    float* updateAHRS(IMUFrame*);

private:
    AbstractAHRS* ahrs;
    float quaternion[4];
};

#endif // AHRSCONTEXT_H
