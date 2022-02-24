#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFile>




using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerCallBack()));
    timer->start(100);
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


    for(int i=-1;i<5;i++)
    {
        ellipsePainter.drawEllipse(QRect(100+50*i,200+50*i,800-100*i,800-100*i));
    }

//    ellipsePainter.drawEllipse(QRect(100+20,200+20,800-40,800-40));

    QPainter small_circle;
    small_circle.begin(this);
    int test_amplified=50;
    double x=500+400*sin(test_amplified*angle/180*PI);
    double y=600-400*cos(test_amplified*angle/180*PI);


    int radius=20;
    small_circle.setBrush(Qt::SolidPattern);
    small_circle.setBrush(Qt::red);
    small_circle.drawEllipse(QRect(x-radius,y-radius,2*radius,2*radius));
    small_circle.setPen(Qt::red);
    small_circle.drawText(QRect(x-45,y-40,100,40),"sound source");

//    small_circle.end();



//    QPen pen_small;

//    pen_small.setWidth(3);
//    pen_small.setColor(Qt::red);
//    pen_small.setBrush(Qt::SolidPattern);
//    small_circle.setPen(pen);


}

void MainWindow::timerCallBack()
{

    QString s;
    QFile f("/home/chevy/Desktop/FYP_desk/visualize_qt/angles.txt");
    f.open(QIODevice::ReadOnly|QIODevice::Text);
    s=f.readLine();
    angle=s.toDouble();
    s="angle: "+s;
    ui->textEdit->setText(s);
    update();






}
