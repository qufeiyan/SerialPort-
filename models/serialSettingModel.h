#ifndef SERIALSETTINGMODEL_H
#define SERIALSETTINGMODEL_H

#include <QtSerialPort/QSerialPort>

class SerialConnection;

typedef struct{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
}SerialSetting_t;

class SerialSettingModel
{
public:
    SerialSetting_t _serialSetting;
    SerialSettingModel();
    SerialSettingModel(SerialSetting_t _serialSetting);

    SerialConnection *setSerialConnection();

private:
    SerialConnection *serialConnection;
};

#endif // SERIALSETTINGMODEL_H
