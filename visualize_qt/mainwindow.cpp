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
    timer->start(10);
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

    for (int i = -1; i < 5; i++)
    {
        ellipsePainter.drawEllipse(QRect(100 + 50 * i, 200 + 50 * i, 800 - 100 * i, 800 - 100 * i));
    }

    //    ellipsePainter.drawEllipse(QRect(100+20,200+20,800-40,800-40));

    QPainter small_circle;
    small_circle.begin(this);
    int test_amplified = 1;
    double x = 500 + 400 * sin(test_amplified * angle / 180 * PI);
    double y = 600 - 400 * cos(test_amplified * angle / 180 * PI);
    double x_filtered = 500 + 400 * sin(test_amplified * angle_filtered / 180 * PI);
    double y_filtered = 600 - 400 * cos(test_amplified * angle_filtered / 180 * PI);

    double opacity = 0.2;
    double opacity_filtered = 1;
    if ((angle == -1000) && (angle_filtered == -1000))
    {
        opacity = 0;
        opacity_filtered = 0;
    }

    int radius = 20;
    small_circle.setBrush(Qt::SolidPattern);
    small_circle.setBrush(Qt::red);
    small_circle.setOpacity(opacity_filtered);
    small_circle.drawEllipse(QRect(x_filtered - radius, y_filtered - radius, 2 * radius, 2 * radius));
    small_circle.setPen(Qt::red);
    small_circle.drawText(QRect(x_filtered - 45, y_filtered - 40, 100, 40), "sound source");

    small_circle.setBrush(Qt::SolidPattern);
    small_circle.setBrush(Qt::blue);
    small_circle.setOpacity(opacity);
    small_circle.drawEllipse(QRect(x - radius, y - radius, 2 * radius, 2 * radius));
    small_circle.setPen(Qt::red);
    small_circle.setOpacity(1);
    // small_circle.drawText(QRect(x-45,y+40,100,40),"NOLPF source");

    //    small_circle.end();

    //    QPen pen_small;

    //    pen_small.setWidth(3);
    //    pen_small.setColor(Qt::red);
    //    pen_small.setBrush(Qt::SolidPattern);
    //    small_circle.setPen(pen);
}

void MainWindow::timerCallBack()
{

    system("adb pull CHEVY_FYP/sound_data/angles.txt /home/chevy/Desktop/FYP_desk/RK_DOA/sound_data/");
    std::cout << "pulling" << std::endl;
    QString s;
    QString s_filtered;

    QFile f("/home/chevy/Desktop/FYP_desk/RK_DOA/sound_data/angles.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    s = f.readLine(0);
    angle = s.toDouble();

    s_filtered = f.readLine();
    angle_filtered = s_filtered.toDouble();
    s = "angle: " + s + "\n" + "filterd: " + s_filtered;

    ui->textEdit->setText(s);
    update();
}
