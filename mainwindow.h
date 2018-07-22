#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    ConnectionWidget *connectionWidget;
    TextDispView *textDispView;
    CustomDispView *customDispView;
};

#endif // MAINWINDOW_H
