#include "CustomDispView.h"
#include <QDebug>
CustomDispView::CustomDispView(QTableView *table)
{
    this->tableView = table;
    this->model = new QStandardItemModel;
    addHeader();
    this->tableView->setModel(model);
}

void CustomDispView::addHeader()
{
    model->setColumnCount(10);

    model->setHeaderData(0,Qt::Horizontal,tr("帧序号"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Gyr_X"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("Gyr_Y"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("Gyr_Z"));

    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("Acc_X"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("Acc_Y"));
    model->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("Acc_Z"));

    model->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("Mag_X"));
    model->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("Mag_Y"));
    model->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("Mag_Z"));
}

void CustomDispView::dispFrame(IMUFrame *imu)
{
    QList<QStandardItem*> item;
    QStandardItem* item0 = new QStandardItem(QString::number(imu->frmSeq));

    QStandardItem* item1 = new QStandardItem(QString::number(imu->gyrData[0]));
    QStandardItem* item2 = new QStandardItem(QString::number(imu->gyrData[1]));
    QStandardItem* item3 = new QStandardItem(QString::number(imu->gyrData[2]));

    QStandardItem* item4 = new QStandardItem(QString::number(imu->accData[0]));
    QStandardItem* item5 = new QStandardItem(QString::number(imu->accData[1]));
    QStandardItem* item6 = new QStandardItem(QString::number(imu->accData[2]));

    QStandardItem* item7 = new QStandardItem(QString::number(imu->magData[0]));
    QStandardItem* item8 = new QStandardItem(QString::number(imu->magData[1]));
    QStandardItem* item9 = new QStandardItem(QString::number(imu->magData[2]));

    qDebug()<<item0;
    item <<item0<<item1<<item2<<item3<<item4<<item5<<item6
        <<item7<<item8<<item9;
    model->appendRow(item);
    tableView->scrollToBottom();
}

