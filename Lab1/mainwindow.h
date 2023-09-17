#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cmyk.h"
#include "hsl.h"
#include "rgb.h"
#include <QMainWindow>
#include <QColor>
#include <QSlider>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QColorDialog* colorDialog;
    CMYK cmyk;
    RGB rgb;
    HSL hsl;
    bool isRGB;
    bool isCMYK;
    bool isHSL;
    QColor currentColor;
    void SetColor();
    void RGBtoCMYK();
    void CMYKtoRGB();
    void RGBtoHSL();
    void HSLtoRGB();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void CMYKparam();
    void RGBparam();
    void HSLparam();
    void RGBSliderChanger();
    void RGBBoxChanger();
    void CMYKBoxChanger();
    void HSLBoxChanger();
private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
