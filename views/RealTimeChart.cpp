#include "RealTimeChart.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>

RealTimeChart::RealTimeChart(QWidget *parent)
    : QWidget(parent)
{

}

RealTimeChart::RealTimeChart(DispCurve_t type)
    : maxX(500),
      axisx(new QValueAxis)
{
    this->type = type;
    this->timerID = timerID;
    if(type == Gyr){
        maxY = 1000;
    }else if(type == Acc){
        maxY = 4;
    }else if(type == Mag){
        maxY = 400;
    }
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

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
    this->setVisible(false);
}

RealTimeChart::~RealTimeChart()
{
    if(timerID != 0)
        killTimer(timerID);
}



void RealTimeChart::init()
{
    if(!isVisible()){
        switch(type)
        {
            case Gyr:
                splineSeriesX->setName("Gyr_X");
                splineSeriesY->setName("Gyr_Y");
                splineSeriesZ->setName("Gyr_Z");
                chart->setTitle("陀螺仪实时动态曲线");
                break;
            case Acc:
                splineSeriesX->setName("ACC_X");
                splineSeriesY->setName("ACC_Y");
                splineSeriesZ->setName("ACC_Z");
                chart->setTitle("加速度计实时动态曲线");
                break;
            case Mag:
                splineSeriesX->setName("Mag_X");
                splineSeriesY->setName("Mag_Y");
                splineSeriesZ->setName("Mag_Z");
                chart->setTitle("磁力计实时动态曲线");
                break;
            default:
                break;
        }
        for(int i=0;i<maxX;++i){
            splineSeriesX->append(i,0);
            splineSeriesY->append(i,0);
            splineSeriesZ->append(i,0);
        }
        this->setVisible(true);
    }
    timerID = startTimer(20);
}

void RealTimeChart::dataReceived(IMUFrame *imu)
{
    data<<*imu;
    temp<<*imu; //保存用
//    qDebug()<<"imu-->"<<imu->accData[0];
//    qDebug()<<"data-->imu"<<data.last().accData[0];
}

void RealTimeChart::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerID) {
        int size = data.length();//数据个数
        qDebug()<<"size-->"<<size;
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
                if(type == Acc){
                    points_X.append(QPointF(k+sizePoints,(double)data.at(k).accData[0]/8192));
                    points_Y.append(QPointF(k+sizePoints,(double)data.at(k).accData[1]/8192));
                    points_Z.append(QPointF(k+sizePoints,(double)data.at(k).accData[2]/8192));
                }else if(type == Mag){
                    points_X.append(QPointF(k+sizePoints,(double)data.at(k).magData[0]*0.6));
                    points_Y.append(QPointF(k+sizePoints,(double)data.at(k).magData[1]*0.6));
                    points_Z.append(QPointF(k+sizePoints,(double)data.at(k).magData[2]*0.6));
                }else if(type == Gyr){
                    points_X.append(QPointF(k+sizePoints,(double)data.at(k).gyrData[0]/32.8));
                    points_Y.append(QPointF(k+sizePoints,(double)data.at(k).gyrData[1]/32.8));
                    points_Z.append(QPointF(k+sizePoints,(double)data.at(k).gyrData[2]/32.8));
                }
            }
            splineSeriesX->replace(points_X);
            splineSeriesY->replace(points_Y);
            splineSeriesZ->replace(points_Z);
            data.clear();
       }
    }
}

void RealTimeChart::creatCSV()
{
    QString fileName = QCoreApplication::applicationDirPath()+"/"
            +QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss")
            +tr(".csv");
    qDebug()<<fileName;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    //add header
    QString s = tr("帧序号,")+"Gyr_X,Gyr_Y,Gyr_Z,"
                +"Acc_X,Acc_Y,Acc_Z,"
                +"Mag_X,Mag_Y,Mag_Z"+"\n";
    out<<s;
    s = "";
    foreach (IMUFrame imu, temp) {
        s += QString::number(imu.frmSeq) + ","
                + QString::number(imu.gyrData[0]/32.8) + "," + QString::number(imu.gyrData[1]/32.8) + "," + QString::number(imu.gyrData[2]/32.8) + ","
                + QString::number(imu.accData[0]/8192.0) + "," + QString::number(imu.accData[1]/8192.0) + "," + QString::number(imu.accData[2]/8192.0) + ","
                + QString::number(imu.magData[0]*0.6) + "," + QString::number(imu.magData[1]*0.6) + "," + QString::number(imu.magData[2]*0.6) + "\n";
    }
    out<<s;
    QMessageBox::information(this,tr("导出数据成功"),tr("信息已保存在%1！").arg(fileName),tr("确定"));
    file.flush();
    file.close();
}

void RealTimeChart::stopTimer()
{
    killTimer(timerID);
}


