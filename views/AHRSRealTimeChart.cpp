#include "AHRSRealTimeChart.h"
#include "processing/MadgwickAHRS.h"
#include "processing/MahonyAHRS.h"
#include "qmath.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QVector4D>


AHRSRealTimeChart::AHRSRealTimeChart()
    : maxX(500),
      maxY(180),
      axisx(new QValueAxis)
{
    myAHRS = new AHRSContext();
    myAHRS->setAHRS(new MadgwickAHRS( 1 / 100.0f , 0.4f ));

    splineSeriesX = new QSplineSeries();
    splineSeriesY = new QSplineSeries();
    splineSeriesZ = new QSplineSeries();

    QPen red(Qt::red);
    red.setWidth(2);
    splineSeriesX->setPen(red);

    QPen green(Qt::green);
    green.setWidth(2);
    splineSeriesY->setPen(green);

    QPen blue(Qt::blue);
    blue.setWidth(2);
    splineSeriesZ->setPen(blue);

    chart = new QChart();
    chart->addSeries(splineSeriesX);
    chart->addSeries(splineSeriesY);
    chart->addSeries(splineSeriesZ);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chart->legend()->hide();
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->createDefaultAxes();

    axisx->setTickCount(11);
    axisx->setMinorTickCount(4);
    chart->setAxisX(axisx,splineSeriesX);
    chart->setAxisX(axisx,splineSeriesY);
    chart->setAxisX(axisx,splineSeriesZ);

    axisx->setLabelFormat("%u");
    axisx->setRange(0,maxX);
//    chart->axisX()->setRange(0, maxX);
    chart->axisY()->setRange(-maxY, maxY);

    for(int i=0;i<maxX;++i){
        splineSeriesX->append(i,0);
        splineSeriesY->append(i,0);
        splineSeriesZ->append(i,0);
    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
    this->setVisible(false);

}

AHRSRealTimeChart::~AHRSRealTimeChart()
{
    if(timerID != 0)
        killTimer(timerID);
}

void AHRSRealTimeChart::stopTimer()
{
    killTimer(this->timerID);
}

void AHRSRealTimeChart::dataReceived(IMUFrame *imu)
{
    AHRS ahrs;
    this->imu = imu;
    this->parseAngle();

    ahrs.imu = *imu;
    ahrs.ypr = this->ypr;

//    qDebug() <<" imuBias is :/r/n " <<imuBias.gyrBias[0] << imuBias.gyrBias[1] <<imuBias.gyrBias[2]
//            <<imuBias.accBias[0] << imuBias.accBias[1] <<imuBias.accBias[2];

    qDebug()<<"ypr:"<<ahrs.ypr.yaw<<ahrs.ypr.pitch<<ahrs.ypr.roll;
    data << ahrs;
    temp << ahrs;
    emit sendAHRS(&ahrs);
}

void AHRSRealTimeChart::init()
{
    if(!isVisible()){
        chart->setTitle("姿态角实时动态曲线");
        this->setVisible(true);
    }
    timerID = startTimer(TIMER_INTERVAL);
}



void AHRSRealTimeChart::parseAngle()
{
//    float gravity[3];
    float q[4];

    float* quaternion = myAHRS->updateAHRS(this->imu);

    q[0] = quaternion[0];
    q[1] = quaternion[1];
    q[2] = quaternion[2];
    q[3] = quaternion[3];

    emit snedQuaternion(q);

//    qDebug()<<"q:"<<q[0]<<q[1]<<q[2]<<q[3];

    // calculate gravity vector
//    gravity[0] = 2 * (q[1]*q[3] - q[0]*q[2]);
//    gravity[1] = 2 * (q[0]*q[1] + q[2]*q[3]);
//    gravity[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];

    // calculate Euler angles
    this->ypr.yaw  = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1) * 180/M_PI; // yaw
    this->ypr.pitch = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch
    this->ypr.roll =  atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll

//      this->ypr.yaw = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3],2 * q[0]*q[0] + 2 * q[1] * q[1] - 1 ) * 180/M_PI;
//      this->ypr.pitch = -atan(2 * q[1] * q[3] + 2 * q[0] * q[2] / sqrt(1-(2 * q[1] * q[3] + 2 * q[0] * q[2]) * (2 * q[1] * q[3] + 2 * q[0] * q[2]))) * 180/M_PI;
//      this->ypr.roll = atan2( 2 * q[2] * q[3] - 2 * q[0] * q[1],2 * q[0]*q[0] + 2 * q[3] * q[3] - 1 ) * 180/M_PI;

//    emit sendEulerAngles(&ypr);

    // calculate yaw/pitch/roll angles
//    this->ypr.yaw = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1) * 180.0f / PI;
//    this->ypr.pitch = atan(gravity[0] / sqrt(gravity[1]*gravity[1] + gravity[2]*gravity[2])) * 180.0f / PI;
//    this->ypr.roll = atan(gravity[1] / sqrt(gravity[0]*gravity[0] + gravity[2]*gravity[2])) * 180.0f / PI;
}

void AHRSRealTimeChart::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerID) {
        int size = data.length();//数据个数
//        qDebug()<<"size-->"<<size;
        if(isVisible()){
            QVector<QPointF> oldPoints_X = splineSeriesX->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points_X;

            QVector<QPointF> oldPoints_Y = splineSeriesY->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points_Y;

            QVector<QPointF> oldPoints_Z = splineSeriesZ->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points_Z;

            for(int i=size;i<oldPoints_X.count();++i){
                points_X.append(QPointF(i-size ,oldPoints_X.at(i).y()));//替换数据用
                points_Y.append(QPointF(i-size ,oldPoints_Y.at(i).y()));
                points_Z.append(QPointF(i-size ,oldPoints_Z.at(i).y()));
            }
            qint64 sizePoints = points_X.count();
            for(int k=0;k<size;++k){

                points_X.append(QPointF(k+sizePoints,(double)data.at(k).ypr.yaw));
                points_Y.append(QPointF(k+sizePoints,(double)data.at(k).ypr.pitch));
                points_Z.append(QPointF(k+sizePoints,(double)data.at(k).ypr.roll));

            }
            splineSeriesX->replace(points_X);
            splineSeriesY->replace(points_Y);
            splineSeriesZ->replace(points_Z);
            data.clear();
       }
    }
}

void AHRSRealTimeChart::creatCSV()
{
    QString fileName = QCoreApplication::applicationDirPath()+"/"
            +QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss")
            +tr("ahrs.csv");
    qDebug()<<fileName;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    //add header
    QString s = tr("帧序号,")+"Yaw,Pitch,Roll,Gyr_X,Gyr_Y,Gyr_Z,"
                +"Acc_X,Acc_Y,Acc_Z,"
                +"Mag_X,Mag_Y,Mag_Z"+"\n";
    out<<s;
    s = "";
    foreach (AHRS ahrs, temp) {
        s += QString::number(ahrs.imu.frmSeq) + ","
                + QString::number(ahrs.ypr.yaw) + "," +  QString::number(ahrs.ypr.pitch) + "," +  QString::number(ahrs.ypr.roll) + ","
                + QString::number(ahrs.imu.gyrData[0]/16.4 - imuBias.gyrBias[0]) + "," + QString::number(ahrs.imu.gyrData[1]/16.4 - imuBias.gyrBias[1]) + "," + QString::number(ahrs.imu.gyrData[2]/16.4 - imuBias.gyrBias[2]) + ","
                + QString::number(ahrs.imu.accData[0]/8192.0 - imuBias.accBias[0]) + "," + QString::number(ahrs.imu.accData[1]/8192.0 - imuBias.accBias[1]) + "," + QString::number(ahrs.imu.accData[2]/8192.0 - imuBias.accBias[2]) + ","
                + QString::number(ahrs.imu.magData[0]*15) + "," + QString::number(ahrs.imu.magData[1]*15) + "," + QString::number(ahrs.imu.magData[2]*15) + "\n";
    }
    out<<s;
    QMessageBox::information(this,tr("导出数据成功"),tr("信息已保存在%1！").arg(fileName),tr("确定"));
    file.flush();
    file.close();
}
