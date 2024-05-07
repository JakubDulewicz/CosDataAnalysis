#include "chartdialog.h"
#include "ui_chartdialog.h"
#include "QString"

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
        QString seriesName = "Seria " + QString::number(i);
        series->setName(seriesName);
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

void chartDialog::setSeriesVisible(int seriesNumber, bool visible)
{
    QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(seriesNumber));
    if (series)
        series->setVisible(visible);
}


void chartDialog::on_checkBoxSeries1_stateChanged(int arg1)
{
    setSeriesVisible(0, arg1);
}


void chartDialog::on_checkBoxSeries2_stateChanged(int arg1)
{
    setSeriesVisible(1, arg1);
}


void chartDialog::on_checkBoxSeries3_stateChanged(int arg1)
{
    setSeriesVisible(2, arg1);
}


void chartDialog::on_checkBoxSeries4_stateChanged(int arg1)
{
    setSeriesVisible(3, arg1);
}


void chartDialog::on_checkBoxSeries5_stateChanged(int arg1)
{
    setSeriesVisible(4, arg1);
}


void chartDialog::on_checkBoxSeries6_stateChanged(int arg1)
{
    setSeriesVisible(5, arg1);
}

