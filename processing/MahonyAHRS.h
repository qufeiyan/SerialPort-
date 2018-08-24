#ifndef MAHONYAHRS_H
#define MAHONYAHRS_H
#include <processing/AbstractAHRS.h>

class MahonyAHRS : public AbstractAHRS
{
public:
    MahonyAHRS();
    MahonyAHRS(float samplePeriod);
    MahonyAHRS(float samplePeriod, float kp);
    MahonyAHRS(float samplePeriod, float kp, float ki);

    void setSamplePeriod(float samplePeriod);
    float getSamplePeriod();

    void setKi(float ki);
    void setKp(float kp);
    void seteInt(float eInt[]);
    float* geteInt();
    float* getQuaternion() override;
    void update(float gx,float gy, float gz, float ax, float ay, float az) override;
    void update(float gx,float gy, float gz, float ax, float ay, float az,
                float mx,float my, float mz) override;
private:
    float samplePeriod;

    float quaternion[4];

    float kp ; // these are the free parameters in the Mahony filter and fusion scheme, kp for proportional feedback, ki for integral private float ki ;
    float ki;
    float eInt[3];

};

#endif // MAHONYAHRS_H
