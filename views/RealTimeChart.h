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

#define TIMER_INTERVAL 40
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
    virtual void creatCSV();
    virtual void stopTimer();
public slots:
    virtual void init();
    virtual void dataReceived(IMUFrame *imu);

protected:

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

    virtual void timerEvent(QTimerEvent*);

    void movingAverage();

private:
//    void dataReceived(int[3]);
    DispCurve_t type;
};

#endif // REALTIMECURVEQCHARTWIDGET_H
