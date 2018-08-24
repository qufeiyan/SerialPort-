// -------------------------------------------------------------------------------------------------------------------
//
//  File: SerialConnection.cpp
//
//  Copyright 2016 (c) Decawave Ltd, Dublin, Ireland.
//
//  All rights reserved.
//
//  Author:
//
// -------------------------------------------------------------------------------------------------------------------

#include "SerialConnection.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QMessageBox>

#define DEVICE_STR ("STMicroelectronics Virtual COM Port")

#define CH430 ("USB-SERIAL CH340")
#define CP210 ("Silicon Labs CP210x USB to UART Bridge")

SerialConnection::SerialConnection(QObject *parent) :
    QObject(parent)
{
//    init();
}

//SerialConnection::SerialConnection()
//{

//}

SerialConnection::~SerialConnection()
{
    if(_serial->isOpen())
        _serial->close();

    delete _serial;
}


void SerialConnection::init()
{
    _serial = new QSerialPort(this);

    connect(_serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(_serial, SIGNAL(readyRead()), this, SLOT(readData()));

    _processingData = true;
}


QStringList SerialConnection::portsList()
{
    return _ports;
}

void SerialConnection::setModel(SerialSettingModel *serialSetting )
{
    baudRate = serialSetting->_serialSetting.baudRate;
    dataBits = serialSetting->_serialSetting.dataBits;
    parity = serialSetting->_serialSetting.parity;
    stopBits = serialSetting->_serialSetting.stopBits;
}

void SerialConnection::findSerialDevices()
{
    _portInfo.clear();
    _ports.clear();

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    //for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        //Their is some sorting to do for just list the port I want, with vendor Id & product Id
        qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier()
                 << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy()
                 << port.manufacturer() << port.description();


        if(port.description() == CH430 || port.description() == CP210)
        {
            _portInfo += port;
            _ports += port.portName();
        }
    }
}

int SerialConnection::openSerialPort(QSerialPortInfo x)
{
    int error = 0;
    _serial->setPort(x);

    if(!_serial->isOpen())
    {
        if (_serial->open(QIODevice::ReadWrite))
        {
            _serial->setBaudRate(baudRate/*p.baudRate*/);
            _serial->setDataBits(dataBits/*p.dataBits*/);
            _serial->setParity(parity/*p.parity*/);
            _serial->setStopBits(stopBits/*p.stopBits*/);
            _serial->setFlowControl(QSerialPort::NoFlowControl /*p.flowControl*/);


            emit statusBarMessage(tr("Connected to %1").arg(x.portName()));

           // qDebug() << "send \"decA$\"" ;
          //  writeData("deca$q");
            //writeData("deca?");
            emit connectionStateChanged(Connected);
            //emit serialOpened(); - wait until we get reply from the unit

        }
        else
        {
            //QMessageBox::critical(NULL, tr("Error"), _serial->errorString());

            emit statusBarMessage(tr("Open error"));

            qDebug() << "Serial error: " << _serial->error();

            _serial->close();

            emit serialError();

            error = 1;
        }
    }
    else
    {
        qDebug() << "port already open!";

        error = 0;
        emit connectionStateChanged(Connected);
    }

    return error;
}

int SerialConnection::openConnection(int index)
{
    QSerialPortInfo x;
    int foundit = -1;
    int open = false;

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(port.description() == CH430 || port.description() == CP210)
        {
            foundit++;
            if(foundit==index)
            {
                x = port;
                open = true;
                break;
            }
        }
    }

    qDebug() << "is busy? " << x.isBusy() << "index " << index << " = found " << foundit;

    if(!open) return -1;

    qDebug() << "open serial port " << index << x.portName();

    //open serial port
    return openSerialPort(x);
}

void SerialConnection::closeConnection()
{
    _serial->close();
    emit statusBarMessage(tr("COM port Disconnected"));
    emit connectionStateChanged(Disconnected);

    _processingData = true;
}

void SerialConnection::writeData(const QByteArray &data)
{
    if(_serial->isOpen())
    {
        _serial->write(data);
        //waitForData = true;
    }
    else
    {
        qDebug() << "not open - can't write?";
    }

    emit connectionStateChanged(Connected);
}


void SerialConnection::cancelConnection()
{
    emit connectionStateChanged(ConnectionFailed);
}


void SerialConnection::readData(void)
{
    if(_processingData)
    {
        QByteArray data = _serial->readAll();
        if(!data.isEmpty())
        {

            QByteArray header = data.mid(0,2);

            //NOTE - only TREK rev 2.0+ is supported by this GUI
            //e.g. nVersion X.Y TREKZ
            _header = QString::fromLocal8Bit(header, 2);

            emit serialOpened(data);
        }
    }
}

void SerialConnection::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        _serial->close();

        _processingData = true;
    }
}


