#ifndef ABSTRACTAHRS_H
#define ABSTRACTAHRS_H


class AbstractAHRS
{
public:
    AbstractAHRS();

    virtual void update(float gx,float gy, float gz, float ax, float ay, float az) = 0;
    virtual void update(float gx,float gy, float gz, float ax, float ay, float az,
                   float mx,float my, float mz) = 0;
    virtual float* getQuaternion() = 0;
};

#endif // ABSTRACTAHRS_H
