#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/ConnectionWidget.h"
#include "views/TextDispView.h"
#include "views/CustomDispView.h"
#include "views/RealTimeChart.h"


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

//    textDispView = new TextDispView(ui->txtbrs_Disp,ui->lcd_Receive,rbtList);
//    customDispView = new CustomDispView(ui->tbv_Disp);

    gyrChart = new RealTimeChart(Gyr);
    accChart = new RealTimeChart(Acc);
    magChart = new RealTimeChart(Mag);

    ui->vlay_tab2->addWidget(gyrChart);
    ui->vlay_tab2->addWidget(accChart);
    ui->vlay_tab2->addWidget(magChart);
    ui->vlay_tab2->setStretchFactor(gyrChart,1);
    ui->vlay_tab2->setStretchFactor(accChart,1);
    ui->vlay_tab2->setStretchFactor(magChart,1);

//    connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
//           textDispView,&TextDispView::DispData);
    connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
            connectionWidget->serialFrameModel,&SerialFrameModel::unpackFrame);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            gyrChart,&RealTimeChart::dataReceived);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            accChart,&RealTimeChart::dataReceived);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            magChart,&RealTimeChart::dataReceived);
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
                   gyrChart,&RealTimeChart::dataReceived);
        connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
               textDispView,&TextDispView::DispData);
    }else if(index == 1){
        disconnect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
                   textDispView,&TextDispView::DispData);
        connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
                connectionWidget->serialFrameModel,&SerialFrameModel::unpackFrame);
        connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
                gyrChart,&RealTimeChart::dataReceived);
    }
}

void MainWindow::startDisp()
{

}



void MainWindow::on_action_WriteFile_triggered()
{
    magChart->creatCSV();
}

void MainWindow::on_action_CloseSerial_triggered()
{
    if(ui->tab_2->isEnabled()){
        gyrChart->stopTimer();
        accChart->stopTimer();
        magChart->stopTimer();
    }
}

void MainWindow::on_action_OpenSerial_triggered()
{
    if(ui->tab_2->isEnabled()){
        gyrChart->init();
        accChart->init();
        magChart->init();
    }
}
