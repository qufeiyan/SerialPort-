#ifndef AHRSREALTIMECHART_H
#define AHRSREALTIMECHART_H
#include "RealTimeChart.h"

#include "processing/AHRSContext.h"


typedef struct{
    IMUFrame imu;
    Euler ypr;
} AHRS;

class AHRSRealTimeChart : public RealTimeChart
{
    Q_OBJECT
public:
    AHRSRealTimeChart();
    ~AHRSRealTimeChart();

    void creatCSV() override;
    void stopTimer() override;
signals:
    void sendAHRS(AHRS*);
    void snedQuaternion(float*);
    void sendEulerAngles(Euler*);

public slots:
    void dataReceived(IMUFrame*) override;
    void init() override;
private:

    int timerID;
    int maxX;
    int maxY;
    QList<AHRS> data;
    QList<AHRS> temp;
    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisx;
    QSplineSeries *splineSeriesX;
    QSplineSeries *splineSeriesY;
    QSplineSeries *splineSeriesZ;

    AHRSContext* myAHRS ;
    IMUFrame* imu;
    // yaw/pitch/roll
    Euler ypr;

    Callout *tip;

    void parseAngle();
    void timerEvent(QTimerEvent*) override;
private slots:
    void tipSlot(QPointF position, bool isHovering) override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // AHRSREALTIMECHART_H
