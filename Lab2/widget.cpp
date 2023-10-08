#include "widget.h"
#include "ui_widget.h"
#include <QImageWriter>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QHeaderView>

widget::widget(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::widget)
{
    ui->setupUi(this);
    provider = new QFileSystemModel(this);
    provider->setFilter(QDir::AllEntries);
    provider->setRootPath("");
    ui->listView->setModel(provider);
    ui->textEdit->setText(" ");
    ui->textEdit->setEnabled(false);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(Direction(QModelIndex)));
    info = new QTableWidget;
    info->setStyleSheet("background-color: white");
    ui->horizontalLayout_4->addWidget(info);
    info->setColumnCount(5);
    info->setEditTriggers(QAbstractItemView::NoEditTriggers);
    info->setHorizontalHeaderItem(0, new QTableWidgetItem("File name"));
    info->setHorizontalHeaderItem(1, new QTableWidgetItem("Size"));
    info->setHorizontalHeaderItem(2, new QTableWidgetItem("Resolution"));
    info->setHorizontalHeaderItem(3, new QTableWidgetItem("Color Depth"));
    info->setHorizontalHeaderItem(4, new QTableWidgetItem("Compression"));
    info->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->OpenFileButton, SIGNAL(clicked(bool)), this, SLOT(OpenFile()));
    connect(ui->ProviderButton, SIGNAL(clicked(bool)), this, SLOT(ProviderChoice()));
}
void widget::Direction(const QModelIndex &index)
{
    QListView *listView = qobject_cast<QListView *>(sender());
    QFileInfo fileInfo = provider->fileInfo(index);
    ui->textEdit->setText(provider->filePath(index));

    if (fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        provider->index(dir.absolutePath());
        listView->setRootIndex(provider->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(provider->index(""));
    }
    else if (fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
    else if (!fileInfo.isDir())
    {
        QString fileExt = provider->fileName(index);
        QImageWriter a(provider->filePath(index));
        QString resolution = "";
        QImage img(provider->filePath(index));

        for (int i = fileExt.lastIndexOf('.'); i < fileExt.size(); i++)
        {
            resolution.append(fileExt[i]);
        }
        if (resolution == ".jpg" || resolution == ".JPG" || resolution == ".gif" || resolution == ".tif" || resolution == ".bmp" ||
            resolution == ".png" || resolution == ".pcx" || resolution == ".BMP")
        {
            ui->FileName->setText("File name: " + provider->fileName(index));
            ui->SizeLabel->setText("Size: " + QString("%1x%2").arg(img.size().width()).arg(img.size().height()));
            double compressionRatio = calculateCompressionRatio(img.size(), fileInfo.size());
            ui->Compression->setText("Compression: " + QString::number(compressionRatio));
            ui->ColorDepth->setText("Color depth: " + QString::number(img.bitPlaneCount()) + " bits");
            ui->Resolution->setText("Resolution:" + QString::number(img.physicalDpiX()) + " DPI");
            addToTable(provider->fileName(index), QString::number(fileInfo.size()), QString::number(img.physicalDpiX()) + " DPI", QString::number(img.bitPlaneCount()) + " bits", QString::number(compressionRatio));
        }
    }
}

void widget::addToTable(const QString &fileName, const QString &size, const QString &resolution, const QString &colorDepth, const QString &compression)
{
    int row = info->rowCount();
    info->insertRow(row);
    info->setItem(row, 0, new QTableWidgetItem(fileName));
    info->setItem(row, 1, new QTableWidgetItem(size));
    info->setItem(row, 2, new QTableWidgetItem(resolution));
    info->setItem(row, 3, new QTableWidgetItem(colorDepth));
    info->setItem(row, 4, new QTableWidgetItem(compression));
}

void widget::addImageToTable(const QString &imagePath)
{
    QFileInfo fileInfo(imagePath);
    QImage image(imagePath);

    double compressionRatio = calculateCompressionRatio(image.size(), fileInfo.size());
    QString colorDepth = image.format() == QImage::Format_Indexed8 ? "8 bits" : "32 bits";
    QString resolution = fileInfo.suffix();

    int row = info->rowCount();
    info->insertRow(row);
    info->setItem(row, 0, new QTableWidgetItem(fileInfo.fileName()));
    info->setItem(row, 1, new QTableWidgetItem(QString("%1x%2").arg(image.size().width()).arg(image.size().height()))); //size
    info->setItem(row, 2, new QTableWidgetItem(QString::number(image.physicalDpiX()) + " DPI")); //resolution
    info->setItem(row, 3, new QTableWidgetItem(colorDepth)); //color depth
    info->setItem(row, 4, new QTableWidgetItem(QString::number(compressionRatio))); //compression
}

void widget::ProviderChoice()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку с изображениями");

    if (folderPath.isNull() || folderPath.isEmpty())
    {
        return;
    }

    provider->setRootPath(folderPath);
    ui->listView->setRootIndex(provider->index(folderPath));

    //info->clearContents();
    //info->setRowCount(0);

    QDir directory(folderPath);
    QStringList imageFilters;
    imageFilters << "*.jpg" << "*.jpeg" << "*.png" << "*.gif" << "*.bmp" << "*.tiff";
    QStringList imageFiles = directory.entryList(imageFilters, QDir::Files);

    for (const QString &filename : imageFiles)
    {
        QString imagePath = directory.filePath(filename);
        addImageToTable(imagePath);
    }
}

void widget::OpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть изображение", "", "Images (*.png *.jpg);;All files (*.*)");

    if (!filePath.isEmpty())
    {
        QFileInfo fileInfo(filePath);
        QImageWriter a(filePath);
        QImage img(filePath);

        QString resolution = fileInfo.suffix();
        if (resolution == "jpg" || resolution == "JPG" || resolution == "gif" || resolution == "tif" || resolution == "bmp" ||
            resolution == "png" || resolution == "pcx" || resolution == "BMP")
        {
            ui->FileName->setText("File name: " + fileInfo.fileName());
            ui->SizeLabel->setText("Size: " + QString("%1x%2").arg(img.size().width()).arg(img.size().height()));
            double compressionRatio = calculateCompressionRatio(img.size(), fileInfo.size());
            ui->Compression->setText("Compression: " + QString::number(compressionRatio));
            ui->ColorDepth->setText("Color depth: " + QString::number(img.bitPlaneCount()) + " bits");
            ui->Resolution->setText("Resolution:" + QString::number(img.physicalDpiX()) + " DPI");
            addToTable(fileInfo.fileName(), QString("%1x%2").arg(img.size().width()).arg(img.size().height()), QString::number(img.physicalDpiX()) + " DPI", QString::number(img.bitPlaneCount()) + " bits",QString::number(compressionRatio));
        }
    }
}

double widget::calculateCompressionRatio(const QSize &imageSize, qint64 fileSize)
{
    double resolutionFactor = imageSize.width() * imageSize.height();
    double compressionRatio = resolutionFactor / fileSize;
    return compressionRatio;
}

widget::~widget()
{
    delete ui;
}

