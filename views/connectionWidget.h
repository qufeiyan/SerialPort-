#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include "models/serialSettingModel.h"
#include "network/SerialConnection.h"
#include "models/serialFrameModel.h"
#include <QThread>

class QComboBox;
class QAction;

class ConnectionWidget : public QObject
{
    Q_OBJECT
public:
    ConnectionWidget();
    ConnectionWidget(QList<QComboBox*> _serialCbx,QList<QAction*> _serialAct);
    ~ConnectionWidget();
    void setViewModel();

    SerialConnection *serialConnection;
    SerialFrameModel *serialFrameModel;
public slots:
    void connectionStateChanged(SerialConnection::ConnectionState state);
    void serialError();
    int updateDeviceList();
    void clearSerial();

protected slots:
    void onReady();
    void connectButtonClicked();
    //void loadSettings();
    //void saveSettings();

private:
    SerialConnection::ConnectionState _state;
    int _selected;

    QComboBox *cbx_Serial;
    QComboBox *cbx_BaudRate;
    QComboBox *cbx_DataBits;
    QComboBox *cbx_Parity;
    QComboBox *cbx_StopBits;

    QAction   *action_OpenSerial;
    QAction   *action_CloseSerial;
    QAction   *action_ClearSerial;

    SerialSettingModel *serialSettingModel;
    SerialSetting_t _serialSetting;

    QThread serialThread;
};

#endif // CONNECTIONWIDGET_H
