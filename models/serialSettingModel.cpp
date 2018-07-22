#include "serialSettingModel.h"
#include <mainwindow.h>
#include <network/SerialConnection.h>

SerialSettingModel::SerialSettingModel()
{

}

SerialSettingModel::SerialSettingModel(SerialSetting_t _serialSetting)
{
    this->_serialSetting = _serialSetting;
    serialConnection = new SerialConnection;
}

SerialConnection *SerialSettingModel::setSerialConnection()
{
    serialConnection->setModel(this);
    return serialConnection;
}

