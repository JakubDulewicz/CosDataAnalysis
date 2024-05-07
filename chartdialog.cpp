#include "chartdialog.h"
#include "ui_chartdialog.h"

chartDialog::chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);
    this->showMaximized();

    _chart = new QChart();
}

chartDialog::~chartDialog()
{
    delete ui;
}

void chartDialog::prepareChart()
{
    int i = 1;
    foreach(QLineSeries *series, _lineSeries) {
        _chart->addSeries(series);
        series->setName("Seria " /*+ i*/);
        i++;
    }
    _chart->setTitle("Wykres");
    _chart->createDefaultAxes();

    ui->chartView->setChart(_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

QVector<QLineSeries *> chartDialog::lineSeries() const
{
    return _lineSeries;
}

void chartDialog::setLineSeries(QVector<QLineSeries *> &newLineSeries)
{
    qDebug() << newLineSeries.at(0)->count();
    _lineSeries = newLineSeries;
}
