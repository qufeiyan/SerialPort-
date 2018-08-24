#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/ConnectionWidget.h"
#include "views/TextDispView.h"
#include "views/CustomDispView.h"
#include "views/RealTimeChart.h"
#include "QMessageBox"
#include "processing/ElmProcessing.h"
#include "views/AHRSRealTimeChart.h"

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



    elm = new ElmProcessing();
    elm->moveToThread(&processThread);
    connect(&processThread,&QThread::finished,elm,&ElmProcessing::deleteLater);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            elm, static_cast< bool(ElmProcessing::*)(IMUFrame*) > (&ElmProcessing::reportState));
    connect(connectionWidget->serialConnection,&SerialConnection::destroyed,
            &processThread,&QThread::quit);
    processThread.start();

//    textDispView = new TextDispView(ui->txtbrs_Disp,ui->lcd_Receive,rbtList);
//    customDispView = new CustomDispView(ui->tbv_Disp);

    gyrChart = new RealTimeChart(Gyr);
    accChart = new RealTimeChart(Acc);
//    magChart = new RealTimeChart(Mag);

    ahrsChart = new AHRSRealTimeChart();

    ui->vlay_tab2->addWidget(gyrChart);
    ui->vlay_tab2->addWidget(accChart);
//    ui->vlay_tab2->addWidget(magChart);
    ui->vlay_tab2->addWidget(ahrsChart);
    ui->vlay_tab2->setStretchFactor(gyrChart,1);
    ui->vlay_tab2->setStretchFactor(accChart,1);
//    ui->vlay_tab2->setStretchFactor(magChart,1);
    ui->vlay_tab2->setStretchFactor(ahrsChart,1);

//    connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
//           textDispView,&TextDispView::DispData);
    connect(connectionWidget->serialConnection,&SerialConnection::serialOpened,
            connectionWidget->serialFrameModel,&SerialFrameModel::unpackFrame);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            gyrChart,&RealTimeChart::dataReceived);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            accChart,&RealTimeChart::dataReceived);
//    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
//            magChart,&RealTimeChart::dataReceived);
    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
            ahrsChart,&AHRSRealTimeChart::dataReceived);
    connect(elm,&ElmProcessing::report,this,&MainWindow::receiveState);

#ifndef QT_NO_OPENGL
    sensorWidget = new MainWidget();
    sensorWidget->resize(400,300);
    sensorWidget->setWindowTitle(tr("传感器模型"));
    sensorWidget->show();
    sensorWidget->setVisible(false);
    connect(ahrsChart,&AHRSRealTimeChart::snedQuaternion,sensorWidget,&MainWidget::receivedQuaternion);
//    connect(ahrsChart,&AHRSRealTimeChart::sendEulerAngles,sensorWidget,&MainWidget::receivedEulerAngles);
#else
    QLabel note("OpenGL Support required");
#endif



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
    ahrsChart->creatCSV();
}

void MainWindow::on_action_CloseSerial_triggered()
{
    if(ui->tab_2->isEnabled()){
        gyrChart->stopTimer();
        accChart->stopTimer();
//        magChart->stopTimer();
        ahrsChart->stopTimer();
    }
}

void MainWindow::on_action_OpenSerial_triggered()
{
    if(ui->tab_2->isEnabled()){
        gyrChart->init();
        accChart->init();
//        magChart->init();

        ahrsChart->init();
    }
//    connect(connectionWidget->serialFrameModel,&SerialFrameModel::sendIMUModel,
//                elm,&ElmProcessing::reportState);
}

void MainWindow::on_action_ReadFile_triggered()
{
    elm->getBeta();
    elm->getWeight();
    elm->getBias();

    QMessageBox::information(this,tr("参数设置成功"),tr("请开始采集数据"),tr("确定"));
}

void MainWindow::receiveState(bool)
{
    if(true)
        QMessageBox::information(this,tr("Alert!!!"),tr("探测到跌倒，请及时处理！"),tr("确定"));
}

void MainWindow::on_action_showModel_triggered()
{
#ifndef QT_NO_OPENGL
//    sensorWidget->show();
    sensorWidget->setVisible(true);
#else
    note.show();
#endif
}
