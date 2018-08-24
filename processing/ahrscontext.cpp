#include "AHRSContext.h"
#include "MadgwickAHRS.h"
#include "MahonyAHRS.h"
#include <QDebug>

AHRSContext::AHRSContext()
{

}

void AHRSContext::setAHRS(AbstractAHRS* ahrs)
{
    this->ahrs = ahrs;
}

AbstractAHRS *AHRSContext::getAHRS()
{
    return this->ahrs;
}

float *AHRSContext::updateAHRS(IMUFrame *imu)
{
//    this->getAHRS()->update(
//            (imu->gyrData[0]/16.4 - imuBias.gyrBias[0]) *PI/180 ,
//            (imu->gyrData[1]/16.4 - imuBias.gyrBias[1]) *PI/180 ,
//            (imu->gyrData[2]/16.4 - imuBias.gyrBias[2]) *PI/180 ,
//            (imu->accData[0]/8192.0 - imuBias.accBias[0] ),
//            (imu->accData[1]/8192.0 - imuBias.accBias[1] ),
//            (imu->accData[2]/8192.0 - imuBias.accBias[2] ),
//             imu->magData[0]*15 ,
//             imu->magData[1]*15 ,
//             imu->magData[2]*15  );
    this->getAHRS()->update((imu->gyrData[0]/16.4 - imuBias.gyrBias[0]) *PI/180 ,
                            (imu->gyrData[1]/16.4 - imuBias.gyrBias[1]) *PI/180 ,
                            (imu->gyrData[2]/16.4 - imuBias.gyrBias[2]) *PI/180 ,
                            (imu->accData[0]/8192.0 - imuBias.accBias[0]),
                            (imu->accData[1]/8192.0 - imuBias.accBias[1]),
                            (imu->accData[2]/8192.0 - imuBias.accBias[2]));
    this->quaternion[0] = this->getAHRS()->getQuaternion()[0];
    this->quaternion[1] = this->getAHRS()->getQuaternion()[1];
    this->quaternion[2] = this->getAHRS()->getQuaternion()[2];
    this->quaternion[3] = this->getAHRS()->getQuaternion()[3];
    return this->quaternion;
}
