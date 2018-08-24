#ifndef ELMPROCESSING_H
#define ELMPROCESSING_H
#include "processing/Processing.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <QObject>
#include "views/AHRSRealTimeChart.h"
using namespace Eigen;

typedef enum{
    WEIGHT = 0,
    BIAS,
    BETA
}Parameter_t;

class ElmProcessing : public QObject, public AbstractProcessing
{
    Q_OBJECT
public:
    ElmProcessing();

    bool detectState(IMUFrame*) override;
//private:
    void trainData( QList<IMUFrame> *);

    void getWeight();
    void getBias();
    void getBeta();

    void readCSV(QString filePath,Parameter_t para);
    bool estimateState();

    int layers;
    int classes;
    int inum;

    MatrixXd weight;
    RowVectorXd bias;

    MatrixXd beta;

    bool predict(IMUFrame*);
    bool predict(AHRS*);
signals:
    void report(bool);
public slots:
    bool reportState(IMUFrame*);
    bool reportState(AHRS*);
private:
    QList<int> stateList;
};

#endif // ELMPROCESSING_H
