#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/ConnectionWidget.h"
#include "views/TextDispView.h"
#include "views/CustomDispView.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QComboBox*> cbxList;
    QList<QAction*> actList;
    QList<QRadioButton*> rbtList;
    cbxList<<ui->cbx_Serial<<ui->cbx_BaudRate<<ui->cbx_DataBit
          <<ui->cbx_CheckBit<<ui->cbx_StopBit;
    actList<<ui->action_OpenSerial<<ui->action_CloseSerial<<ui->action_ClearSerial;
    rbtList<<ui->rbt_TextDisp<<ui->rbt_HexDisp;
    connectionWidget = new ConnectionWidget(cbxList,actList);

    textDispView = new TextDispView(ui->txtbrs_Disp,ui->lcd_Receive,rbtList);    
    customDispView = new CustomDispView(ui->tbv_Disp);

    connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
           textDispView,&TextDispView::DispData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 0){
        disconnect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
                   connectionWidget->serialFrameModel,&SerialFrameModel::unpackFrame);
        disconnect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
                   customDispView,&CustomDispView::dispFrame);
        connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
               textDispView,&TextDispView::DispData);
    }else if(index == 1){
        disconnect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
                   textDispView,&TextDispView::DispData);
        connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
                connectionWidget->serialFrameModel,&SerialFrameModel::unpackFrame);
        connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
                customDispView,&CustomDispView::dispFrame);
    }
}
