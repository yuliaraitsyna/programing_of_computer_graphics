#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPoint>
#include <QActionGroup>
#include <QElapsedTimer>

enum Algorithm{
    STEP_BY_STEP,
    BRESENHAM,
    DDA,
    BRESENHAM_CIRCLE
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void on_spinBox1_editingFinished();

    void on_spinBox2_editingFinished();

    void on_spinBox3_editingFinished();

    void on_spinBox4_editingFinished();

    void on_algorithms_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    void swap(double &a,double&b);
    void recalc();
    void drawGraph();
    void checkSpeed();

    QActionGroup algorithms;
    Algorithm algorithm;

    void drawPixel(const QPoint& point);
    QVector<QPoint> StepByStepLine(double x0, double y0, double x1, double y1);
    QVector<QPoint> BresenhamLine(double x0, double y0, double x1, double y1);
    QVector<QPoint> DDALine(double x0, double y0, double x1, double y1);
    QVector<QVector<QPoint>> BresenhamCircle(double x0, double y0,double R);
    void InitializaAlgorithms();
};
#endif // MAINWINDOW_H
