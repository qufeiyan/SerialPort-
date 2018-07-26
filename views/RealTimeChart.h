#ifndef REALTIMECHART_H
#define REALTIMECHART_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include "models/serialFrameModel.h"
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

typedef enum{
    Gyr = 0,
    Acc,
    Mag
}DispCurve_t;

class RealTimeChart : public QWidget
{
    Q_OBJECT

public:
    RealTimeChart(QWidget *parent = 0);
    RealTimeChart(DispCurve_t);
    ~RealTimeChart();
    void creatCSV();
    void stopTimer();
public slots:
    void init();
    void dataReceived(IMUFrame *imu);
protected:
    void timerEvent(QTimerEvent*);

private:
//    void dataReceived(int[3]);
    DispCurve_t type;
    int timerID;
    int maxX;
    int maxY;
    QList<IMUFrame> data;
    QList<IMUFrame> temp;
    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisx;
    QSplineSeries *splineSeriesX;
    QSplineSeries *splineSeriesY;
    QSplineSeries *splineSeriesZ;
};

#endif // REALTIMECURVEQCHARTWIDGET_H
