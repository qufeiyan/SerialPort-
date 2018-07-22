#ifndef CUSTOMDISPVIEW_H
#define CUSTOMDISPVIEW_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include "models/serialFrameModel.h"

class CustomDispView : public QObject
{
    Q_OBJECT
public:
    CustomDispView(QTableView *table);

    void addHeader();

public slots:
    void dispFrame(IMUFrame *imu);
private:
    QTableView *tableView;
    QStandardItemModel  *model;
};

#endif // CUSTOMDISPVIEW_H
