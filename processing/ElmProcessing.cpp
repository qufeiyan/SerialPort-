#include "ElmProcessing.h"
#include <Eigen/SVD>
#include <QtCore/qmath.h>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

ElmProcessing::ElmProcessing()
    : layers(500),
      classes(5),
      inum(9)
{
    //initialiaze weight and bias
    weight = MatrixXd(inum,layers);
    bias = RowVectorXd(layers);

    beta = MatrixXd(layers,classes);



}


void ElmProcessing::trainData(QList<IMUFrame> *data)
{
    int length = data->length();
    MatrixXd feature(length,inum);

    VectorXd label(length);

    foreach (IMUFrame imu, *data) {
        feature << imu.gyrData[0],imu.gyrData[1],imu.gyrData[2],imu.accData[0],imu.accData[1],imu.accData[2];
    }
}

void ElmProcessing::getWeight()
{
    QString filePath = ":/weight.csv";
    readCSV(filePath,WEIGHT);
    qDebug()<<"weight"<<weight(5,499);
}

void ElmProcessing::getBias()
{
    QString filePath = ":/bias.csv";
    readCSV(filePath,BIAS);
    qDebug()<<"bias"<<bias(498);
}

void ElmProcessing::getBeta()
{
    QString filePath = ":/beta.csv";
    readCSV(filePath,BETA);
    qDebug()<<"Beta"<<beta(499,1);
}

void ElmProcessing::readCSV(QString filePath,Parameter_t para)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    int rows,cols;
    if(para == WEIGHT){
        rows = inum;
        cols = layers;
        QStringList stringList;
        QString str;
        for(int i = 0; i<rows; i++ )
        {
            str =  in.readLine();
            stringList = str.split(",", QString::SkipEmptyParts);//按逗号分割
            QListIterator<QString> strIte(stringList);
            for(int j = 0; j<cols; j++ ){
                 QString str = static_cast<QString>(stringList.at(j));
                 this->weight(i,j) = str.toDouble();
            }
        }

    }else if(para == BIAS){
        rows = 1;
        cols = layers;
        QStringList stringList;
        QString str;
        int j = 0;
        for(int i = 0; i<rows; i++ )
        {
            str = in.readLine();
            stringList = str.split(",", QString::SkipEmptyParts);
            QListIterator<QString> strIte(stringList);
            for(;strIte.hasNext();){
                 this->bias(i,j++) = strIte.next().toDouble();
            }
        }

    }else if(para == BETA){
        rows = layers;
        cols = classes;
        QStringList stringList;
        QString str;
        for(int i = 0; i<rows; i++ )
        {
            str = in.readLine();
            stringList = str.split(",", QString::SkipEmptyParts);
            for(int j = 0; j<cols; j++){
                QString str = static_cast<QString>(stringList.at(j));
                this->beta(i,j) = str.toDouble();
            }
        }

    }
    file.flush();
    file.close();
}

bool ElmProcessing::predict(IMUFrame *imu)
{
//    qDebug()<< " execute pedict \n" ;

    RowVectorXd h(layers);
    int preLabel;
    RowVectorXd preFeature(inum);
    RowVectorXd preExpect(classes);

    /* load data */
    preFeature << imu->gyrData[0]/16.4f,imu->gyrData[1]/16.4f,imu->gyrData[2]/16.4f,
                  imu->accData[0]/8192.0f,imu->accData[1]/8192.0f,imu->accData[2]/8192.0f,
                  imu->magData[0]*15,imu->magData[1]*15,imu->magData[2]*15;


    h = (-(preFeature * weight)+ bias);

    for (int i=0; i< h.cols();i++)
    {
        h(i) = 1.0 / ( 1 + qExp(h(i)) );
    }

    preExpect = h * beta ;

    //
    if(preExpect(0)>0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 1;
    else if(preExpect(1)>0 && preExpect(0)<0 && preExpect(2)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 2;
    else if(preExpect(2)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 3;
    else if(preExpect(3)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(4)<0)
        preLabel = 4;
    else if(preExpect(4)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(3)<0)
        preLabel = 5;
    this->stateList << preLabel;
    qDebug()<< "preLabel"<<preLabel ;

    /* estimate state */
    bool state = estimateState();
    return state;
}

bool ElmProcessing::predict(AHRS *ahrs)
{
    RowVectorXd h(layers);
    int preLabel;
    RowVectorXd preFeature(inum);
    RowVectorXd preExpect(classes);

    /* load data */
    preFeature <<ahrs->imu.gyrData[0]/16.4f,ahrs->imu.gyrData[1]/16.4f,ahrs->imu.gyrData[2]/16.4f,
                 ahrs->imu.accData[0]/8192.0f,ahrs->imu.accData[1]/8192.0f,ahrs->imu.accData[2]/8192.0f,
                 ahrs->imu.magData[0]*15,ahrs->imu.magData[1]*15,ahrs->imu.magData[2]*15;


    h = (-(preFeature * weight)+ bias);

    for (int i=0; i< h.cols();i++)
    {
        h(i) = 1.0 / ( 1 + qExp(h(i)) );
    }

    preExpect = h * beta ;

    //
    if(preExpect(0)>0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 1;
    else if(preExpect(1)>0 && preExpect(0)<0 && preExpect(2)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 2;
    else if(preExpect(2)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(3)<0 && preExpect(4)<0)
        preLabel = 3;
    else if(preExpect(3)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(4)<0)
        preLabel = 4;
    else if(preExpect(4)>0 && preExpect(0)<0 && preExpect(1)<0 && preExpect(2)<0 && preExpect(3)<0)
        preLabel = 5;
    this->stateList << preLabel;
    qDebug()<< "preLabel"<<preLabel ;

    /* estimate state */
    bool state = estimateState();
    return state;
}

bool ElmProcessing::estimateState()
{
    int length = this->stateList.length();
    if(length > 250)
    {
        int stand = 0,fall = 0;
        foreach (int s, stateList) {
            if( s == 4 || s == 5 )
                fall ++;
            else if(s == 1 || s == 2 || s == 3 )
                stand ++;
        }

        if(fall > stand){
            stateList.clear();
            return true;
        }
        stateList = stateList.mid(10);
    }
    return false;
}

bool ElmProcessing::reportState(IMUFrame *imu)
{
    if(predict(imu)){
        emit report(true);
        return true;
    }else{
        return false;
    }
}

bool ElmProcessing::reportState(AHRS *ahrs)
{
    if(predict(ahrs)){
        emit report(true);
        return true;
    }else{
        return false;
    }
}

bool ElmProcessing::detectState(IMUFrame *)
{
    return 0;
}


