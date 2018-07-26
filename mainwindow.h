#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class RealTimeChart;
class ConnectionWidget;
class TextDispView;
class CustomDispView;
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

private:
    Ui::MainWindow *ui;

    ConnectionWidget *connectionWidget;
    TextDispView *textDispView;
    CustomDispView *customDispView;
    RealTimeChart* gyrChart;
    RealTimeChart* accChart;
    RealTimeChart* magChart;
    void startDisp();
};

#endif // MAINWINDOW_H
