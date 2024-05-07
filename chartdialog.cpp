#include "chartdialog.h"
#include "ui_chartdialog.h"
#include "QString"
#include "stdio.h"

chartDialog::chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);
    this->showMaximized();

    _chart = new QChart();
    //ui->verticalLayoutRange->setAlignment(ui->pushButtonAutoAdjustment,Qt::AlignCenter);
    //ui->pushButtonAutoAdjustment->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

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

    QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));
    xAxisMin = xAxis->min();
    xAxisMax = xAxis->max();
    yAxisMin = yAxis->min();
    yAxisMax = yAxis->max();

    ui->doubleSpinBoxXAxisMin->setValue(xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(yAxisMax);

    ui->doubleSpinBoxXAxisMin->setMinimum(xAxisMin);
    ui->doubleSpinBoxXAxisMax->setMaximum(xAxisMax);
    ui->doubleSpinBoxYAxisMin->setMinimum(yAxisMin);
    ui->doubleSpinBoxYAxisMax->setMaximum(yAxisMax);

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


void chartDialog::on_doubleSpinBoxXAxisMin_valueChanged(double arg1)
{
    xAxisMin = arg1;
    _chart->axisX()->setMin(xAxisMin);
}


void chartDialog::on_doubleSpinBoxXAxisMax_valueChanged(double arg1)
{
    xAxisMax = arg1;
    _chart->axisX()->setMax(xAxisMax);
}


void chartDialog::on_doubleSpinBoxYAxisMin_valueChanged(double arg1)
{
    yAxisMin = arg1;
    _chart->axisY()->setMin(yAxisMin);
}


void chartDialog::on_doubleSpinBoxYAxisMax_valueChanged(double arg1)
{
    yAxisMax = arg1;
    _chart->axisY()->setMax(yAxisMax);
}


void chartDialog::on_pushButtonAutoAdjustment_clicked()
{
    double minXValue = 0;
    double maxXValue = 0;
    double minYValue = 0;
    double maxYValue = 0;
    for (int i = 0; i < 6; ++i)
    {
        QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
        if(series && series->isVisible())
        {
            foreach (const QPointF &point, series->points())
            {
                if (point.x() < minXValue)
                    minXValue = point.x();
                if (point.y() < minYValue)
                    minYValue = point.y();
                if (point.x() > maxXValue)
                    maxXValue = point.x();
                if (point.y() > maxYValue)
                    maxYValue = point.y();
            }
        }
    }

    xAxisMin = minXValue;
    xAxisMax = maxXValue;
    yAxisMin = minYValue;
    yAxisMax = maxYValue;

    ui->doubleSpinBoxXAxisMin->setValue(xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(yAxisMax);

}

