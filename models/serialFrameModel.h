#ifndef SERIALFRAMEMODEL_H
#define SERIALFRAMEMODEL_H

#include <QObject>

#define IMU_FRAME_LENGTH 26
#define IMU_FRAME_HEADER 2
#define IMU_FRAMW_SEQUENCE 2

typedef struct{
    float yaw;
    float pitch;
    float roll;
} Euler;

typedef struct{
    int16_t gyrData[3];
    int16_t accData[3];
    int16_t magData[3];

    uint16_t frmSeq;
}IMUFrame;

typedef struct
{
   double gyrBias[3];
   double accBias[3];
}IMUBias ;
const static double PI = 3.1415926;
const static IMUBias imuBias = {
     -0.705667943 ,
     -0.775148642 ,
     -0.510346951 ,

     -0.006670705 ,
     0.004940403 ,
     0.980300195  - 0.97966
};

class SerialFrameModel : public QObject
{
    Q_OBJECT
public:
    SerialFrameModel();
    ~SerialFrameModel();
signals:
    void sendIMUModel(IMUFrame*);
public slots:

    void init();

    void unpackFrame(QByteArray);  //解析IMU帧
private:
    IMUFrame *imu;
};

#endif // SERIALFRAME_H
