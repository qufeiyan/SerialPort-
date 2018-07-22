// -------------------------------------------------------------------------------------------------------------------
//
//  File: SerialConnection.h
//
//  Copyright 2016 (c) Decawave Ltd, Dublin, Ireland.
//
//  All rights reserved.
//
//  Author:
//
// -------------------------------------------------------------------------------------------------------------------

#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QStringList>
#include "models/serialSettingModel.h"
#include "models/serialFrameModel.h"

/**
* @brief SerialConnection
*        Constructor, it initialises the Serial Connection its parts
*        it is used for managing the COM port connection.
*/
class SerialConnection : public QObject
{
    Q_OBJECT
public:
    explicit SerialConnection(QObject *parent = 0);
//    SerialConnection();
    ~SerialConnection();

    enum ConnectionState
    {
        Disconnected = 0,
        Connecting,
        Connected,
        ConnectionFailed
    };

    void findSerialDevices(); //find any tags or anchors that are connected to the PC

    int openSerialPort(QSerialPortInfo x); //open selected serial port

    QStringList portsList(); //return list of available serial ports (list of ports with tag/anchor connected)

    QSerialPort* serialPort() { return _serial; }

    void setModel(SerialSettingModel *serialSetting);

signals:
    //void clearTags();
    void serialError(void);
    //void getCfg(void);
    //void nextCmd(void);

    void statusBarMessage(QString status);
    void connectionStateChanged(SerialConnection::ConnectionState);
    void serialOpened(QByteArray data);

public slots:

    void init();
    void closeConnection();
    void cancelConnection();
    int  openConnection(int index);
    void readData(void);

protected slots:
    void writeData(const QByteArray &data);
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;

    QSerialPort *_serial;

    QList<QSerialPortInfo>	_portInfo ;
    QStringList _ports;

    QList<QByteArray> _cmdList ;

    QString _header;
    bool _processingData;
};

#endif // SERIALCONNECTION_H
