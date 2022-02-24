#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <cmath>
#define PI 3.1415926

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *event);
public slots:
    void timerCallBack();

private:
    Ui::MainWindow *ui;
    double angle;

};

#endif // MAINWINDOW_H
