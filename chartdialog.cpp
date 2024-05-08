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
    setVisibleDimensionlessTime(false);

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
        if(series)
        {
            _chart->addSeries(series);
            QString seriesName = "Seria " + QString::number(i);
            series->setName(seriesName);
        }
        i++;
    }
    _chart->setTitle("Wykres");
    _chart->createDefaultAxes();

    QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));

    xAxis->setTitleText("Numer pomiaru");
    yAxis->setTitleText("Stężenie wskaźnika, μS");

    xAxisMin = xAxis->min();
    xAxisMax = xAxis->max();
    yAxisMin = yAxis->min();
    yAxisMax = yAxis->max();

    qDebug() << xAxisMin << xAxisMax << yAxisMin << yAxisMax;

    ui->doubleSpinBoxXAxisMin->setMinimum(xAxisMin);
    ui->doubleSpinBoxXAxisMax->setMaximum(xAxisMax);
    ui->doubleSpinBoxYAxisMin->setMinimum(yAxisMin);
    ui->doubleSpinBoxYAxisMax->setMaximum(yAxisMax);


    ui->doubleSpinBoxXAxisMin->setValue(xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(yAxisMax);


    ui->chartView->setChart(_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

QVector<QLineSeries *> chartDialog::lineSeries() const
{
    return _lineSeries;
}

void chartDialog::setLineSeries(const QVector<QLineSeries *> &newLineSeries)
{
    qDeleteAll(_lineSeries);
    _lineSeries.clear();

    for (QLineSeries *series : newLineSeries) {
            QLineSeries *newSeries = new QLineSeries();
            for (int i = 0; i < series->count(); ++i) {
                newSeries->append(series->at(i));
            }
            _lineSeries.append(newSeries);
        }
}

QVector<QLineSeries *> chartDialog::untouchedLineSeries() const
{
    return _untouchedLineSeries;
}

void chartDialog::setUntouchedLineSeries(const QVector<QLineSeries *> &newUntouchedLineSeries)
{   
    qDeleteAll(_untouchedLineSeries);
    _untouchedLineSeries.clear();

    for (QLineSeries *series : newUntouchedLineSeries) {
            QLineSeries *newSeries = new QLineSeries();
            for (int i = 0; i < series->count(); ++i) {
                newSeries->append(series->at(i));
            }
            _untouchedLineSeries.append(newSeries);
        }

}

void chartDialog::setSeriesVisible(int seriesNumber, bool visible)
{
    QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(seriesNumber));
    if (series)
        series->setVisible(visible);
}

bool chartDialog::checkStandardDeviationForFirstPoint()
{
    for (int i = 0; i < 6; ++i)
    {
        QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
        if(series)
        {
            if(series->count() >= 6)
            {
                QPointF potentialError = series->points().first();
                double sum = 0;
                double mean;
                double squerdiff;

                for (int i = 1; i < 6; ++i)
                {
                    double value = series->points().at(i).y();
                    sum += value;
                }
                mean = sum/5;

                for (int i = 1; i < 6; ++i)
                {
                    double diff = series->points().at(i).y() - mean;
                    squerdiff += diff*diff;
                }

                double variance = squerdiff/5;
                double deviation = sqrt(variance);

                if(potentialError.y() < (mean - 2*deviation) || potentialError.y() > (mean + 2*deviation))
                    return true;
            }
        }
    }
    return false;
}

void chartDialog::unifySeriesVisibility()
{
    setSeriesVisible(0,ui->checkBoxSeries1->isChecked());
    setSeriesVisible(1,ui->checkBoxSeries2->isChecked());
    setSeriesVisible(2,ui->checkBoxSeries3->isChecked());
    setSeriesVisible(3,ui->checkBoxSeries4->isChecked());
    setSeriesVisible(4,ui->checkBoxSeries5->isChecked());
    setSeriesVisible(5,ui->checkBoxSeries6->isChecked());
}

void chartDialog::setVisibleDimensionlessTime(bool visible)
{
    ui->labelLiquidFlow->setVisible(visible);
    ui->doubleSpinBoxLiquidFlow->setVisible(visible);
    ui->labelTundishCapacity->setVisible(visible);
    ui->doubleSpinBoxTundishCapacity->setVisible(visible);
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
    bool emptyValues = true;
    for (int i = 0; i < 6; ++i)
    {
        QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
        if(series && series->isVisible())
        {


            foreach (const QPointF &point, series->points())
            {
                if(emptyValues)
                {
                    minXValue = point.x();
                    minYValue = point.y();
                    maxXValue = point.x();
                    minYValue = point.y();
                    emptyValues = false;
                }

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

void chartDialog::on_checkBoxReadValueError_stateChanged(int arg1)
{
    if(arg1)
    {
        if(checkStandardDeviationForFirstPoint())
        {
            for (int i = 0; i < 6; ++i)
            {
                QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
                series->removePoints(0,2);
                ui->chartView->repaint();
                ui->chartView->update();
            }
            on_pushButtonAutoAdjustment_clicked();
        }
    }
    else
    {
        setLineSeries(untouchedLineSeries());
        prepareChart();
        unifySeriesVisibility();
    }
}




void chartDialog::on_checkBoxDimensionlessTime_stateChanged(int arg1)
{
    setVisibleDimensionlessTime(arg1);
}

