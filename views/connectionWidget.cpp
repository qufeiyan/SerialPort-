#include "ConnectionWidget.h"
#include <QComboBox>
#include <QAction>
#include <QDebug>
ConnectionWidget::ConnectionWidget()
{
   // QObject::connect(action_OpenSerial, SIGNAL(clicked()), this,SLOT(connectButtonClicked()));  //add a function for the Connect button click
}

ConnectionWidget::ConnectionWidget(QList<QComboBox*> _serialCbx,QList<QAction*> _serialAct)
{
    cbx_Serial = _serialCbx[0];
    cbx_BaudRate = _serialCbx[1];
    cbx_DataBits = _serialCbx[2];
    cbx_Parity = _serialCbx[3];
    cbx_StopBits = _serialCbx[4];
    action_OpenSerial = _serialAct[0];
    action_CloseSerial = _serialAct[1];
    action_ClearSerial = _serialAct[2];
    setViewModel();

    serialSettingModel = new SerialSettingModel(_serialSetting);
    serialFrameModel = new SerialFrameModel();

    QObject::connect(action_OpenSerial,&QAction::triggered, this,&ConnectionWidget::connectButtonClicked);  //add a function for the Connect button click
    QObject::connect(action_CloseSerial,&QAction::triggered, this,&ConnectionWidget::connectButtonClicked);  //add a function for the Connect button click
    QObject::connect(action_ClearSerial,&QAction::triggered,this,&ConnectionWidget::clearSerial);
    onReady();
//    QObject::connect(serialConnection,&SerialConnection::serialOpened,
//                     serialFrameModel,&SerialFrameModel::unpackFrame);

}

void ConnectionWidget::setViewModel()
{
    _serialSetting.baudRate = (QSerialPort::BaudRate)cbx_BaudRate->currentText().toInt();
    _serialSetting.dataBits = (QSerialPort::DataBits)cbx_DataBits->currentText().toInt();

    if( cbx_Parity->currentText() == tr("无校验")){
        _serialSetting.parity = QSerialPort::NoParity;
    }else if(cbx_Parity->currentText() == tr("奇校验")){
        _serialSetting.parity = QSerialPort::OddParity;
    }else if(cbx_Parity->currentText() == tr("偶校验")){
        _serialSetting.parity = QSerialPort::EvenParity;
    }
    _serialSetting.stopBits = (QSerialPort::StopBits)cbx_StopBits->currentText().toInt();
}

void ConnectionWidget::onReady()
{
    serialConnection = serialSettingModel->setSerialConnection();

    serialConnection->moveToThread(&serialThread);

    connect(&serialThread,&QThread::started,serialConnection,&SerialConnection::init);

    connect(serialConnection,&SerialConnection::destroyed,
            &serialThread,&QThread::quit);
    connect(&serialThread,&QThread::finished,
            serialConnection,&SerialConnection::deleteLater);

    QObject::connect( serialConnection, &SerialConnection::connectionStateChanged,
                      this,&ConnectionWidget::connectionStateChanged);

    updateDeviceList();

    connectionStateChanged(SerialConnection::Disconnected);

    serialThread.start();
}

ConnectionWidget::~ConnectionWidget()
{
    delete serialConnection;
}

int ConnectionWidget::updateDeviceList()
{
    int count  = 0;
    qDebug() << "ConnectionWidget::updateDeviceList";

    //remove all items from the combobox
    while(cbx_Serial->count())
    {
        cbx_Serial->removeItem(0);
    }

    serialConnection->findSerialDevices();

    cbx_Serial->addItems(serialConnection->portsList());

    count = cbx_Serial->count();

    //check if we have found any TREK devices in the COM ports list
    if(count == 0)
    {
        action_OpenSerial->setEnabled(false);
        action_CloseSerial->setEnabled(false);
        action_ClearSerial->setEnabled(false);
        cbx_Serial->setEnabled(false);
        cbx_BaudRate->setEnabled(false);
        cbx_DataBits->setEnabled(false);
        cbx_Parity->setEnabled(false);
        cbx_StopBits->setEnabled(false);
        connectionStateChanged(SerialConnection::Disconnected);
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() <<" execute here meaning ready to open serial port!";
#else
        //connect to the first TREK device
        if(serialConnection->openConnection(0) != 0)
        {
            return -1;
        }
#endif
    }

    return count;
}

void ConnectionWidget::clearSerial()
{
    serialConnection->serialPort()->flush();
}

void ConnectionWidget::serialError(void)
{
    cbx_Serial->setEnabled(false);
    cbx_BaudRate->setEnabled(false);
    cbx_DataBits->setEnabled(false);
    cbx_Parity->setEnabled(false);
    cbx_StopBits->setEnabled(false);
    action_CloseSerial->setEnabled(false);
    action_OpenSerial->setEnabled(false);
    action_ClearSerial->setEnabled(false);
}


void ConnectionWidget::connectButtonClicked()
{
    switch (_state)
    {
    case SerialConnection::Disconnected:
    case SerialConnection::ConnectionFailed:
    {
        serialConnection->openConnection(cbx_Serial->currentIndex());
        break;
    }

    case SerialConnection::Connecting:
        serialConnection->cancelConnection();
        break;

    case SerialConnection::Connected:
        serialConnection->closeConnection();
        break;
    }
}

void ConnectionWidget::connectionStateChanged(SerialConnection::ConnectionState state)
{
    this->_state = state;
    switch(state)
    {
    case SerialConnection::Disconnected:
    case SerialConnection::ConnectionFailed:
        //ui->connect_pb->setText("Connect");
        //this->show();
        action_OpenSerial->setVisible(true);
        action_OpenSerial->setStatusTip(tr("点击打开串口"));
        action_CloseSerial->setVisible(false);
        action_ClearSerial->setVisible(false);

        break;

    case SerialConnection::Connecting:
        //ui->connect_pb->setText("Cancel");
        //this->show();
        action_OpenSerial->setVisible(true);
        action_CloseSerial->setVisible(true);
        action_CloseSerial->setStatusTip(tr("点击关闭串口"));
        break;

    case SerialConnection::Connected:
        //ui->connect_pb->setText("Disconnect");
        //this->hide();
        action_OpenSerial->setVisible(false);
        action_CloseSerial->setVisible(true);
        action_ClearSerial->setVisible(true);
        action_CloseSerial->setStatusTip(tr("点击关闭串口"));
        action_ClearSerial->setStatusTip(tr("点击清除串口数据"));
        break;
    }

    bool enabled = (state == SerialConnection::Disconnected || state == SerialConnection::ConnectionFailed) ? true : false;
    //can change the COM port once not opened
    cbx_Serial->setEnabled(enabled);
}
