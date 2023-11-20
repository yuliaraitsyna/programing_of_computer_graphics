#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QElapsedTimer>
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),algorithms(this)
{
    ui->setupUi(this);

    InitializaAlgorithms();
    ui->spinBox1->setRange(-30, 30);
    ui->spinBox2->setRange(-30, 30);
    ui->spinBox3->setRange(-30, 30);
    ui->spinBox4->setRange(-30, 30);

    ui->algorithms->addAction(ui->actionStepByStep);
    ui->algorithms->addAction(ui->actionBresenham);
    ui->algorithms->addAction(ui->actionDDA);
    ui->algorithms->addAction(ui->actionCircle);

}

void MainWindow::InitializaAlgorithms()
{
    algorithms.addAction(ui->actionStepByStep);
    algorithms.addAction(ui->actionBresenham);
    algorithms.addAction(ui->actionDDA);
    algorithms.addAction(ui->actionCircle);


    algorithms.setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    for(auto action: algorithms.actions())
    {
        action->setCheckable(true);
    }

    ui->actionStepByStep->setChecked(true);
    algorithm = STEP_BY_STEP;

    connect(ui->algorithms,SIGNAL(currentIndexChanged(int index)),this,SLOT(on_comboBox_currentIndexChanged(int index)));
}




void MainWindow::on_comboBox_currentIndexChanged(int index)
{

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::swap(double&a,double&b)
{
    double tmp = a;
    a = b;
    b= tmp;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    double x1 = ui->spinBox1->value(),x2 = ui->spinBox3->value(),
        y1 = ui->spinBox2->value(), y2=ui->spinBox4->value();

    QPixmap pix(650,650);
    pix.fill(Qt::white);
    QPainter p(&pix);
    p.setBrush(Qt::black);

    int max = qMax(abs(x1),qMax(abs(x2),qMax(abs(y1),abs(y2))));
    int range = 10;

    while(range <= max)
    {
        range*=2;
    }


    double step = 0.9/20;
    double stp = 0.9/(2*range);

    p.drawLine(pix.width()*0.05,pix.height()/2,pix.width()*0.95,pix.height()/2);
    p.drawLine(pix.width()/2,pix.height()*0.95,pix.width()/2,pix.height()*0.05);

    for(int i = 0; i < 20;i++)
    {
        p.setPen(QPen(Qt::black,1));
        p.drawLine(pix.width()*(0.05+step*i),pix.height()*0.05,pix.width()*(0.05+step*i),pix.height()*0.95);
        p.drawLine(pix.width()*0.05,pix.height()*(0.05+step*i),pix.width()*0.95,pix.height()*(0.05+step*i));
        p.drawText(pix.width()*(0.04+step*i),pix.height()/1.90,QString::number(range/10*(i-10)));
        if(i!=10)
            p.drawText(pix.width()/2.10,pix.height()*(0.955-step*i),QString::number(range/10*(i-10)));

    }

    QVector<QPoint> line;
    QVector<QVector<QPoint>> circle;

    switch(algorithm)
    {
    case STEP_BY_STEP:
        line = StepByStepLine(x1,y1,x2,y2);
        break;
    case BRESENHAM:
        line = BresenhamLine(x1,y1,x2,y2);
        break;
    case DDA:
        line = DDALine(x1,y1,x2,y2);
        break;
    case BRESENHAM_CIRCLE:
        circle = BresenhamCircle(x1,y1,x2);
        break;
    }

    p.setPen(QPen(Qt::black,3));
    if(algorithm !=BRESENHAM_CIRCLE)
    {
        p.drawLine(pix.width()*(0.5 + stp*line[0].x()),pix.height()*(0.5-stp*line[0].y()),pix.width()*(0.5+stp*line[line.size() - 1].x()),pix.height()*(0.5-stp*line[line.size()-1].y()));
        p.setPen(QPen(Qt::red,2));

        for(int i = 1; i<line.size();i++)
        {
            p.drawLine(pix.width()*(0.5+stp*line[i-1].x()),pix.height()*(0.5-stp*line[i-1].y()),pix.width()*(0.5+stp*line[i].x()),pix.height()*(0.5-stp*line[i].y()));
        }

    }
    else{
        p.setPen(QPen(Qt::black,3));
        p.setBrush(Qt::NoBrush);
        int r = height()*stp*x2;
        p.drawEllipse(pix.width()*(0.5 + stp*x1) - r, pix.height()*(0.5 - stp*y1) - r,2*r, 2*r);
        p.setPen(QPen(Qt::red,2));
        for(int i = 0; i<4;i++)
        {
            for(int j = 1; j<circle[i].size();j++)
            {
                p.drawLine(pix.width()*(0.5+stp*circle[i][j-1].x()),pix.height()*(0.5-stp*circle[i][j-1].y()),pix.width()*(0.5+stp*circle[i][j].x()),pix.height()*(0.5-stp*circle[i][j].y()));
            }
        }
    }
    ui->label->setPixmap(pix);

}

QVector<QPoint> MainWindow::StepByStepLine(double x0, double y0, double x1, double y1){

    bool steep = abs(y1-y0)>abs(x1-x0);

    if(steep)
    {
        swap(x0,y0);
        swap(x1,y1);
    }

    if(x0>x1)
    {
        swap(x0,x1);
        swap(y0,y1);
    }

    double k = (y1 - y0)/(x1 - x0);
    double b = (y0-k*x0);

    QVector<QPoint> line;

    for(int x = x0; x<= x1;x++)
    {
        int y = qRound(k*x+b);
        line.push_back(steep?QPoint(y,x):QPoint(x,y));
    }
    return line;
}


QVector<QPoint> MainWindow::BresenhamLine(double x0, double y0, double x1, double y1)
{
    bool steep = abs(y1-y0)>abs(x1-x0);
    if(steep)
    {
        swap(x0,y0);
        swap(x1,y1);
    }

    if(x0>x1)
    {
        swap(x0,x1);
        swap(y0,y1);
    }

    int dx = x1-x0;
    int dy = abs(y1-y0);
    int error = dx/2;
    int ystep = (y0<y1) ? 1:-1;
    int y = y0;

    QVector<QPoint> line;
    for(int x = x0; x<= x1;x++)
    {
        line.push_back(steep?QPoint(y,x):QPoint(x,y));
        error -= dy;
        if(error<0)
        {
            y+=ystep;
            error+=dx;
        }
    }
    return line;
}

QVector<QPoint> MainWindow::DDALine(double x0, double y0, double x1, double y1){
    bool steep = abs(y1-y0)>abs(x1-x0);
    if(steep)
    {
        swap(x0,y0);
        swap(x1,y1);
    }
    if(x0>x1)
    {
        swap(x0,x1);
        swap(y0,y1);
    }

    double dx = x1-x0;
    double dy = y1-y0;
    double y = y0;

    QVector<QPoint> line;
    line.push_back(steep?QPoint(y0,x0):QPoint(x0,y0));

    for(int x = x0+1; x<= x1;x++)
    {
        y += dy/dx;
        line.push_back(steep?QPoint(qRound(y),x):QPoint(x,qRound(y)));
    }

    return line;
}

QVector<QVector<QPoint>> MainWindow::BresenhamCircle(double x0, double y0, double R){

    QVector<QVector<QPoint>> circle(4);
    int x = 0;
    int y = R;
    int delta = 1 - 2 * R;
    int error = 0;
    while(y >= 0)
    {
        circle[0].push_back(QPoint(x0 + x, y0 + y));
        circle[1].push_back(QPoint(x0 + x, y0 - y));
        circle[2].push_back(QPoint(x0 - x, y0 + y));
        circle[3].push_back(QPoint(x0 - x, y0 - y));
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;

        if(delta > 0 && error > 0)
        {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
    return circle;
}

void MainWindow::on_spinBox1_editingFinished()
{
    update();
}


void MainWindow::on_spinBox2_editingFinished()
{
    update();
}


void MainWindow::on_spinBox3_editingFinished()
{
    update();
}


void MainWindow::on_spinBox4_editingFinished()
{
    update();
}


void MainWindow::on_algorithms_currentIndexChanged(int index)
{
    ui->spinBox4->setVisible(true);
    ui->y2->setVisible(true);
    ui->x2->setText("x2");

    if(ui->algorithms->currentIndex()==0)
        algorithm = STEP_BY_STEP;
    else if(ui->algorithms->currentIndex()==2)
        algorithm = BRESENHAM;

    else if(ui->algorithms->currentIndex()==1)
        algorithm = DDA;

    else
    {
        algorithm = BRESENHAM_CIRCLE;
        ui->spinBox4->setVisible(false);
        ui->y2->setVisible(false);
        ui->x2->setText("R");
    }


    if(ui->algorithms->currentIndex()==3)
    {
        algorithm = BRESENHAM_CIRCLE;
        ui->spinBox4->setVisible(false);
        ui->y2->setVisible(false);
        ui->x2->setText("R");
    }
}

