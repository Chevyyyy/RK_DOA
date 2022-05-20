#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFile>
#include "iostream"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerCallBack()));
    timer->start(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter ellipsePainter(this);
    QPen pen;

    pen.setWidth(3);
    pen.setColor(Qt::black);
    pen.setBrush(Qt::SolidPattern);
    ellipsePainter.setPen(pen);

    for (int i = -1; i < 2; i++)
    {
        ellipsePainter.drawEllipse(QRect(100 + 50 * i, -400 + 50 * i, 800 - 100 * i, 800 - 100 * i));
    }



    QPainter small_circle;
    small_circle.begin(this);

    double x = 500 + 400 * sin(  angle / 180 * PI);
    double y = 400 * cos( angle / 180 * PI);
    double x_filtered = 500 + 400 * sin( angle_filtered / 180 * PI);
    double y_filtered =  400 * cos( angle_filtered / 180 * PI);

    double opacity = 0.2;
    double opacity_filtered = 1;


    if ((angle < -800) )
    {
            radius=0.9*radius;
    }
    else{

radius=40;
    }


    small_circle.setBrush(Qt::SolidPattern);
    small_circle.setBrush(Qt::red);
    small_circle.setOpacity(opacity_filtered);
    small_circle.drawEllipse(QRect(x_filtered - radius, y_filtered - radius, 2 * radius, 2 * radius));
    small_circle.setPen(Qt::red);
//    small_circle.drawText(QRect(x_filtered - 55, y_filtered - 70, 200, 80), "sound source");

    small_circle.setBrush(Qt::SolidPattern);
    small_circle.setBrush(Qt::blue);
    small_circle.setOpacity(opacity);
    small_circle.drawEllipse(QRect(x - radius, y - radius, 2 * radius, 2 * radius));
    small_circle.setPen(Qt::red);
    small_circle.setOpacity(1);

}

void MainWindow::timerCallBack()
{

    system("adb pull CHEVY_FYP/angles.txt .");
    std::cout << "pulling" << std::endl;
    QString s;
    QString s_filtered;

    QFile f("./angles.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    s = f.readLine(0);
    angle = s.toDouble();

    s_filtered = f.readLine();


    angle_filtered = s_filtered.toDouble();

    s = "angle: " + s + "\n" + "filterd: " + s_filtered;



    ui->textEdit->setText(s);
    update();
}
