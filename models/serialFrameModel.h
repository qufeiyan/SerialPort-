#ifndef SERIALFRAMEMODEL_H
#define SERIALFRAMEMODEL_H

#include <QObject>

#define IMU_FRAME_LENGTH 26
#define IMU_FRAME_HEADER 2
#define IMU_FRAMW_SEQUENCE 2

typedef struct{
    int16_t gyrData[3];
    int16_t accData[3];
    int16_t magData[3];

    uint16_t frmSeq;
}IMUFrame;

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
