#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <processing/Processing.h>

#ifndef QT_NO_OPENGL
#include "sensorModel/mainwidget.h"
#else
#include <QLabel>
#endif

class ElmProcessing;
class RealTimeChart;
class ConnectionWidget;
class TextDispView;
class CustomDispView;
class AHRSRealTimeChart;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_action_WriteFile_triggered();

    void on_action_CloseSerial_triggered();

    void on_action_OpenSerial_triggered();

    void on_action_ReadFile_triggered();

    void receiveState(bool);

    void on_action_showModel_triggered();

private:

    MainWidget *sensorWidget;

    ElmProcessing  *elm;

    Ui::MainWindow *ui;

    ConnectionWidget *connectionWidget;
    TextDispView *textDispView;
    CustomDispView *customDispView;
    RealTimeChart* gyrChart;
    RealTimeChart* accChart;
    RealTimeChart* magChart;

    QDockWidget* gyrDock;
    QDockWidget* accDock;
    QDockWidget* magDock;
    QDockWidget* ahrsDock;

    AHRSRealTimeChart* ahrsChart;

    void startDisp();

    QThread processThread;
};

#endif // MAINWINDOW_H
