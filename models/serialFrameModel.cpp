#include "serialFrameModel.h"
#include <QDebug>
#include <QtEndian>

SerialFrameModel::SerialFrameModel()
{
    init();
}

SerialFrameModel::~SerialFrameModel()
{

}

void SerialFrameModel::init()
{
    this->imu = new IMUFrame;
}

void SerialFrameModel::unpackFrame(QByteArray bytes)
{
   int length = bytes.length();
   int offset = 0;
   while(length >= IMU_FRAME_LENGTH )
   {  
       while(length >= IMU_FRAME_LENGTH )
       {
           QByteArray header = bytes.mid(offset, 2);
           if(header.contains("KS")) //loop here until we reach header ("ma")
           {
               break;
           }
           offset += 2;
           length -= 2;
       }
       if(length < IMU_FRAME_LENGTH)
       {
           return;
       }

       QByteArray imuReport = bytes.mid(offset, IMU_FRAME_LENGTH);

       memcpy(&imu->gyrData[0],imuReport.mid(4,2).constData(),2);
       memcpy(&imu->gyrData[1],imuReport.mid(6,2).constData(),2);
       memcpy(&imu->gyrData[2],imuReport.mid(8,2).constData(),2);

       memcpy(&imu->accData[0],imuReport.mid(10,2).constData(),2);
       memcpy(&imu->accData[1],imuReport.mid(12,2).constData(),2);
       memcpy(&imu->accData[2],imuReport.mid(14,2).constData(),2);

       memcpy(&imu->magData[0],imuReport.mid(16,2).constData(),2);
       memcpy(&imu->magData[1],imuReport.mid(18,2).constData(),2);
       memcpy(&imu->magData[2],imuReport.mid(20,2).constData(),2);

       memcpy(&imu->frmSeq,imuReport.mid(22,2).constData(),2);

       qDebug()<<"this is"<<imu->frmSeq<<imu->gyrData[2]<<imu->magData[0];

       offset += IMU_FRAME_LENGTH;
       length -= IMU_FRAME_LENGTH;

       qDebug()<<this->thread();
       emit sendIMUModel(imu);
   }
}

