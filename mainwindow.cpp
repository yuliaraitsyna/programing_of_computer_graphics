#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentColor.setRgb(0,0,0);
    colorDialog = new QColorDialog;
    cmyk.k = 0;
    ui->spinBoxK->setValue(0);
    SetColor();

    //RGB SLIDERS -> CHANGE RGB SPINS
    connect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    connect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    connect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));

    //RGB SPINS -> CHANGE RGB SLIDERS
    connect(ui->spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(RGBSliderChanger()));
    connect(ui->spinBoxG, SIGNAL(valueChanged(int)), this, SLOT(RGBSliderChanger()));
    connect(ui->spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(RGBSliderChanger()));

    //CMYK SPINS FIXED WHEN ANY RGB IS CHANGED
    connect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));

    connect(ui->spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->spinBoxG, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));

    connect(ui->spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(HSLparam()));
    connect(ui->spinBoxG, SIGNAL(valueChanged(int)), this, SLOT(HSLparam()));
    connect(ui->spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(HSLparam()));

    //CMYK SPINS CHANGED -> RGB FIXED: SPINS AND SLIDERS

}

void MainWindow:: SetColor()
{
    QString rv = QString::number(currentColor.red());
    QString gv = QString::number(currentColor.green());
    QString bv = QString::number(currentColor.blue());
    ui->color->setStyleSheet("QWidget{""background-color:rgb("+rv+","+gv+","+bv+"); border: 2px solid black;""}");
}
void MainWindow:: RGBBoxChanger()
{
    //CHANGE SPINS
    rgb.r = ui->horizontalSliderR->value();
    rgb.g = ui->horizontalSliderG->value();
    rgb.b = ui->horizontalSliderB->value();

    ui->spinBoxR->setValue(rgb.r);
    ui->spinBoxG->setValue(rgb.g);
    ui->spinBoxB->setValue(rgb.b);

    //FIX COLOR
    currentColor.setRgb(rgb.r, rgb.g, rgb.b);
    SetColor();
    //TRANSLATE
    RGBtoCMYK();
}
void MainWindow:: RGBSliderChanger()
{
    //CHANGE SLIDERS
    rgb.r = ui->spinBoxR->value();
    rgb.g = ui->spinBoxG->value();
    rgb.b = ui->spinBoxB->value();

    ui->horizontalSliderR->setValue(rgb.r);
    ui->horizontalSliderG->setValue(rgb.g);
    ui->horizontalSliderB->setValue(rgb.b);
    //FIX COLOR
    currentColor.setRgb(rgb.r, rgb.g, rgb.b);
    SetColor();
    //TRANSLATE
    RGBtoCMYK();
}
void MainWindow::RGBtoCMYK()
{
    cmyk.k = qMin(qMin(255 - rgb.r, 255 - rgb.g), 255 - rgb.b);
    cmyk.c = (int)((255 - rgb.r - cmyk.k)/(1. - (cmyk.k/255.)));
    cmyk.m = (int)((255 - rgb.g - cmyk.k)/(1. - (cmyk.k/255.)));
    cmyk.y = (int)((255 - rgb.b - cmyk.k)/(1. - (cmyk.k/255.)));
}
void MainWindow:: CMYKtoRGB()
{
    rgb.r = (int)((255 - currentColor.cyan())*(1. - (currentColor.black() /255.)));
    rgb.g = (int)((255 - currentColor.magenta())*(1. - (currentColor.black()/255.)));
    rgb.b = (int)((255 - currentColor.yellow())*(1. - (currentColor.black()/255.)));
}
void MainWindow:: CMYKparam()
{
    //CHANGE SPINS ACCORDING TO THE CURRENT COLOR
    ui->spinBoxC->setValue(currentColor.cyan());
    ui->spinBoxM->setValue(currentColor.magenta());
    ui->spinBoxY->setValue(currentColor.yellow());
    ui->spinBoxK->setValue(currentColor.black());
}
void MainWindow:: HSLparam()
{
    ui->spinBoxH->setValue(currentColor.hue());
    ui->spinBoxS->setValue(currentColor.hslSaturation());
    ui->spinBoxL->setValue(currentColor.lightness());
}
void MainWindow:: RGBparam()
{
    ui->horizontalSliderR->setValue(currentColor.red());
    ui->horizontalSliderG->setValue(currentColor.green());
    ui->horizontalSliderB->setValue(currentColor.blue());
    ui->spinBoxR->setValue(currentColor.red());
    ui->spinBoxG->setValue(currentColor.green());
    ui->spinBoxB->setValue(currentColor.blue());
}

void MainWindow:: CMYKBoxChanger()
{
    cmyk.c = ui->spinBoxC->value();
    cmyk.m = ui->spinBoxM->value();
    cmyk.y = ui->spinBoxY->value();
    cmyk.k = ui->spinBoxK->value();

    //FIX COLOR
    currentColor.setCmyk(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
    SetColor();

    //TRANSLATE
    CMYKtoRGB();
    hsl.h = currentColor.hue();
    hsl.s = currentColor.saturation();
    hsl.l = currentColor.lightness();

}












void MainWindow:: HSLBoxChanger()
{
    hsl.h = ui->spinBoxH->value();
    hsl.s = ui->spinBoxS->value();
    hsl.l = ui->spinBoxL->value();
    currentColor.setHsl(hsl.h, hsl.s, hsl.l);
    SetColor();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    currentColor = colorDialog->getColor();
    disconnect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    disconnect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    disconnect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    disconnect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    disconnect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    disconnect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));

    ui->horizontalSliderR->setValue(currentColor.red());
    ui->horizontalSliderG->setValue(currentColor.green());
    ui->horizontalSliderB->setValue(currentColor.blue());
    ui->spinBoxR->setValue(currentColor.red());
    ui->spinBoxG->setValue(currentColor.green());
    ui->spinBoxB->setValue(currentColor.blue());
    ui->spinBoxC->setValue(currentColor.cyan());
    ui->spinBoxM->setValue(currentColor.magenta());
    ui->spinBoxY->setValue(currentColor.yellow());
    ui->spinBoxK->setValue(currentColor.black());
    ui->spinBoxH->setValue(currentColor.hue());
    ui->spinBoxS->setValue(currentColor.hslSaturation());
    ui->spinBoxL->setValue(currentColor.lightness());

    connect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    connect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    connect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(RGBBoxChanger()));
    connect(ui->horizontalSliderR, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->horizontalSliderG, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));
    connect(ui->horizontalSliderB, SIGNAL(valueChanged(int)), this, SLOT(CMYKparam()));

    update();
    SetColor();
}





