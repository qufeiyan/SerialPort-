#ifndef MADGWICKAHRS_H
#define MADGWICKAHRS_H
#include "processing/AbstractAHRS.h"

class MadgwickAHRS : public AbstractAHRS
{
public:
    MadgwickAHRS();
    MadgwickAHRS(float samplePeriod);
    MadgwickAHRS(float samplePeriod, float beta);

    void setSamplePeriod(float samplePeriod);
    float getSamplePeriod();
    void setBeta(float beta);
    float getBeta();
    float* getQuaternion() override;

    void update(float gx,float gy, float gz, float ax, float ay, float az) override;

    void update(float gx,float gy, float gz, float ax, float ay, float az,
                       float mx,float my, float mz) override;
private:
    float samplePeriod;

    float quaternion[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    float beta;
};

#endif // MADGWICKAHRS_H
