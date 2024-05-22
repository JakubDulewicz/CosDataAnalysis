#include "chartdialog.h"
#include "ui_chartdialog.h"
#include "QString"
#include "stdio.h"

chartDialog::chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chartDialog)
{
    ui->setupUi(this);
    setWindowTitle("Wykres");
    this->showMaximized();
    _chart = new QChart();
    setVisibleDimensionlessTime(false);

    //ui->verticalLayoutRange->setAlignment(ui->pushButtonAutoAdjustment,Qt::AlignCenter);
    //ui->pushButtonAutoAdjustment->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

}

chartDialog::~chartDialog()
{
    for (QLineSeries* series : _lineSeries)
        delete series;
    for (QLineSeries* series : _untouchedLineSeries)
        delete series;
    _untouchedLineSeries.clear();
    _lineSeries.clear();
    ui->chartView->deleteLater();
    on_spinBoxFontSize_valueChanged(15);
    delete _chart;
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

    xAxis->setTitleText("Czas pomiaru, s");
    yAxis->setTitleText("Przewodność elektryczna, μS");

    _xAxisMin = xAxis->min();
    _xAxisMax = xAxis->max();
    _yAxisMin = yAxis->min();
    _yAxisMax = yAxis->max();

    _xAxisMin = 0;
    _yAxisMin = 0;

    qDebug() << _xAxisMin << _xAxisMax << _yAxisMin << _yAxisMax;

    ui->doubleSpinBoxXAxisMin->setMinimum(-3);
    ui->doubleSpinBoxXAxisMax->setMaximum(_xAxisMax);
    ui->doubleSpinBoxYAxisMin->setMinimum(_yAxisMin);
    ui->doubleSpinBoxYAxisMax->setMaximum(_yAxisMax);


    ui->doubleSpinBoxXAxisMin->setValue(_xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(_xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(_yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(_yAxisMax);


    ui->chartView->setChart(_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void chartDialog::convertToTimeStep()
{
    foreach (QLineSeries *series, _lineSeries)
    {
        if(series && (series->count() != 0))
        {
            double startTime = series->at(1).x();
            for (int i = 0; i < series->count(); ++i)
            {
                if(i > 1)
                {
                    double xValue = series->at(i).x();
                    qDebug() << xValue-startTime;
                    series->replace(i,xValue-startTime,series->at(i).y());
                }
            }
            series->replace(0,-3,series->at(0).y());
            series->replace(1,0,series->at(1).y());
        }
    }
}

void chartDialog::convertToDimensionlessTracerConcentration()
{
    foreach (QLineSeries *series, _lineSeries)
    {
        if(series && (series->count() != 0))
        {
            double startTracerConcentrationValue = series->at(1).y();
            double endTracerConcentrationValue = series->at(series->count() - 1).y();
            for (int i = 0; i < series->count(); ++i)
            {
                double currentYValue = series->at(i).y();
                double dimensionlessTracerValue = (currentYValue - startTracerConcentrationValue)/(endTracerConcentrationValue - startTracerConcentrationValue);
                series->replace(i,series->at(i).x(),dimensionlessTracerValue);
            }
        }
    }
}

void chartDialog::convertToDimensionlessTime()
{
    double tundishCapacity = ui->doubleSpinBoxTundishCapacity->value();
    double liquidFlow = ui->doubleSpinBoxLiquidFlow->value();
    double theoreticalTime = tundishCapacity/liquidFlow;

    foreach (QLineSeries *series, _lineSeries)
    {
        if(series && (series->count() != 0))
        {
            for (int i = 0; i < series->count(); ++i)
            {
                qDebug() << "Point Before: " << series->at(i);
                double currentXValue = series->at(i).x();
                double dimensionlessTime = (currentXValue/theoreticalTime);
                series->replace(i,dimensionlessTime,series->at(i).y());
                qDebug() << "Point After: " << series->at(i);
                _chart->update();
            }
        }
    }
}

void chartDialog::adjustChartScale()
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
    minXValue = 0;
    minYValue = 0;

    QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));
    xAxis->setRange(minXValue,maxXValue);
    yAxis->setRange(minYValue,maxYValue);

    _xAxisMin = minXValue;
    _xAxisMax = maxXValue;
    _yAxisMin = minYValue;
    _yAxisMax = maxYValue;

    ui->doubleSpinBoxXAxisMin->setValue(_xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(_xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(_yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(_yAxisMax);
}

QVector<QLineSeries *> chartDialog::lineSeries() const
{
    return _lineSeries;
}

void chartDialog::setLineSeries(const QVector<QLineSeries *> &newLineSeries)
{
    qDeleteAll(_lineSeries);
    _lineSeries.clear();

    for (QLineSeries *series : newLineSeries)
    {
            QLineSeries *newSeries = new QLineSeries();
            for (int i = 0; i < series->count(); ++i)
            {
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
                double squerdiff = 0;
                double mean;


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

void chartDialog::removeInvalidFirstPoints()
{
    if(checkStandardDeviationForFirstPoint())
    {
        for (int i = 0; i < 6; ++i)
        {
            QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
            if(series->count() > 0)
            {
                series->removePoints(0,2);
                ui->chartView->repaint();
                ui->chartView->update();
            }
        }
        on_pushButtonAutoAdjustment_clicked();
    }
}

double chartDialog::calculateStandardDeviation(QLineSeries *series)
{
    if(!series || series->count() == 0)
        return -1;

    int pointsAmount = series->count();
    QVector<QPointF> points = series->pointsVector();
    double sum = 0;
    double mean;
    double sumOfSquares;

    for ( const QPointF &point : points)
        sum += point.y();
    mean = sum/pointsAmount;

    for (const QPointF &point : points)
    {
        double diff = point.y() - mean;
        sumOfSquares += diff * diff;
    }

    double variance = sumOfSquares / pointsAmount;
    double standardDeviation = std::sqrt(variance);

    return standardDeviation;
}

void chartDialog::reloadChartParameters()
{
    on_spinBoxFontSize_valueChanged(ui->spinBoxFontSize->value());
    on_lineEditChartTitle_textChanged(ui->lineEditChartTitle->text());
    on_lineEditSeries1_textChanged(ui->lineEditSeries1->text());
    on_lineEditSeries2_textChanged(ui->lineEditSeries2->text());
    on_lineEditSeries3_textChanged(ui->lineEditSeries3->text());
    on_lineEditSeries4_textChanged(ui->lineEditSeries4->text());
    on_lineEditSeries5_textChanged(ui->lineEditSeries5->text());
    on_lineEditSeries6_textChanged(ui->lineEditSeries6->text());
    on_lineEditChartTitle_textChanged(ui->lineEditChartTitle->text());
}

void chartDialog::setMinMaxAxisValues()
{
    double minXValue = 0;
    double maxXValue = 0;
    double minYValue = 0;
    double maxYValue = 0;
    bool emptyValues = true;

    foreach (QLineSeries *series, _lineSeries)
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
    minXValue = 0;
    minYValue = 0;

    QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));
    xAxis->setRange(minXValue,maxXValue);
    yAxis->setRange(minYValue,maxYValue);

    _xAxisMin = minXValue;
    _xAxisMax = maxXValue;
    _yAxisMin = minYValue;
    _yAxisMax = maxYValue;

    // _xAxisMin = 0;
    // _yAxisMin = 0;

    ui->doubleSpinBoxXAxisMin->setMinimum(-3);
    ui->doubleSpinBoxXAxisMax->setMaximum(_xAxisMax);
    ui->doubleSpinBoxYAxisMin->setMinimum(_yAxisMin);
    ui->doubleSpinBoxYAxisMax->setMaximum(_yAxisMax);

    ui->doubleSpinBoxXAxisMin->setValue(_xAxisMin);
    ui->doubleSpinBoxXAxisMax->setValue(_xAxisMax);
    ui->doubleSpinBoxYAxisMin->setValue(_yAxisMin);
    ui->doubleSpinBoxYAxisMax->setValue(_yAxisMax);
}

void chartDialog::setVisibleDimensionlessTime(bool visible)
{
    ui->labelLiquidFlow->setVisible(visible);
    ui->doubleSpinBoxLiquidFlow->setVisible(visible);
    ui->labelTundishCapacity->setVisible(visible);
    ui->doubleSpinBoxTundishCapacity->setVisible(visible);
    ui->pushButtonCalculateTime->setVisible(visible);
}

void chartDialog::setVisibleEmptySeriesCheckBoxes()
{
    if(_lineSeries.at(0)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(0))[0]->setVisible(false);
        ui->checkBoxSeries1->setChecked(false);
        ui->checkBoxSeries1->setVisible(false);
        ui->lineEditSeries1->setVisible(false);
    }
    if(_lineSeries.at(1)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(1))[0]->setVisible(false);
        ui->checkBoxSeries2->setChecked(false);
        ui->checkBoxSeries2->setVisible(false);
        ui->lineEditSeries2->setVisible(false);
    }
    if(_lineSeries.at(2)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(2))[0]->setVisible(false);
        ui->checkBoxSeries3->setChecked(false);
        ui->checkBoxSeries3->setVisible(false);
        ui->lineEditSeries3->setVisible(false);
    }
    if(_lineSeries.at(3)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(3))[0]->setVisible(false);
        ui->checkBoxSeries4->setChecked(false);
        ui->checkBoxSeries4->setVisible(false);
        ui->lineEditSeries4->setVisible(false);
    }
    if(_lineSeries.at(4)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(4))[0]->setVisible(false);
        ui->checkBoxSeries5->setEnabled(false);
        ui->checkBoxSeries5->setVisible(false);
        ui->lineEditSeries5->setVisible(false);
    }
    if(_lineSeries.at(5)->count() == 0)
    {
        _chart->legend()->markers(_lineSeries.at(5))[0]->setVisible(false);
        ui->checkBoxSeries6->setEnabled(false);
        ui->checkBoxSeries6->setVisible(false);
        ui->lineEditSeries6->setVisible(false);
    }
}

void chartDialog::setVisibleInvalidSeries()
{
    for (int i = 0; i < 6; ++i)
    {
        qDebug() << "Value i" << i;
        QLineSeries *series = qobject_cast<QLineSeries*>(_chart->series().at(i));
        if(series)
        {
            double standardDeviation = calculateStandardDeviation(series);
            // Value 5 is arbitrarily assumed value based on obesrvation of invalidly working sensors
            if(standardDeviation < 5)
            {
                qDebug() << "Works for: " << i;
                switch (i) {
                case 0: ui->checkBoxSeries1->setChecked(false); break;
                case 1: ui->checkBoxSeries2->setChecked(false); break;
                case 2: ui->checkBoxSeries3->setChecked(false); break;
                case 3: ui->checkBoxSeries4->setChecked(false); break;
                case 4: ui->checkBoxSeries5->setChecked(false); break;
                case 5: ui->checkBoxSeries6->setChecked(false); break;
                default:
                    break;
                }
            }
        }

    }
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
    _xAxisMin = arg1;
    _chart->axisX()->setMin(_xAxisMin);
}


void chartDialog::on_doubleSpinBoxXAxisMax_valueChanged(double arg1)
{
    _xAxisMax = arg1;
    _chart->axisX()->setMax(_xAxisMax);
}


void chartDialog::on_doubleSpinBoxYAxisMin_valueChanged(double arg1)
{
    _yAxisMin = arg1;
    _chart->axisY()->setMin(_yAxisMin);
}


void chartDialog::on_doubleSpinBoxYAxisMax_valueChanged(double arg1)
{
    _yAxisMax = arg1;
    _chart->axisY()->setMax(_yAxisMax);
}


void chartDialog::on_pushButtonAutoAdjustment_clicked()
{
    adjustChartScale();
}

void chartDialog::on_checkBoxReadValueError_stateChanged(int arg1)
{
    if(arg1)
    {
        removeInvalidFirstPoints();
    }
    else
    {
        setLineSeries(untouchedLineSeries());
        convertToTimeStep();
        prepareChart();
        setVisibleEmptySeriesCheckBoxes();
        unifySeriesVisibility();
        if(ui->checkBoxDimensionlessTracerConcentration->isChecked())
        {
            convertToDimensionlessTracerConcentration();
            QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));
            yAxis->setTitleText("Bezwymiarowe stężenie wskaźnika, - ");
        }
        adjustChartScale();
        reloadChartParameters();
    }
}

void chartDialog::on_checkBoxDimensionlessTime_stateChanged(int arg1)
{
    setVisibleDimensionlessTime(arg1);
    if(arg1)
    {

    }
    else
    {
        setLineSeries(untouchedLineSeries());
        convertToTimeStep();
        prepareChart();
        setVisibleEmptySeriesCheckBoxes();
        unifySeriesVisibility();
        if(ui->checkBoxReadValueError->isChecked())
            removeInvalidFirstPoints();
        if(ui->checkBoxDimensionlessTracerConcentration->isChecked())
            on_checkBoxDimensionlessTime_stateChanged(1);

        QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
        xAxis->setTitleText("Czas pomiaru, s");
        reloadChartParameters();
    }
}


void chartDialog::on_checkBoxDimensionlessTracerConcentration_stateChanged(int arg1)
{
    if(arg1)
    {
        convertToDimensionlessTracerConcentration();
        adjustChartScale();
        QValueAxis *yAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Vertical).at(0));
        yAxis->setTitleText("Bezwymiarowe stężenie wskaźnika, - ");
    }
    else
    {
        setLineSeries(untouchedLineSeries());
        convertToTimeStep();
        prepareChart();
        setVisibleEmptySeriesCheckBoxes();
        unifySeriesVisibility();
        if(ui->checkBoxReadValueError->isChecked())
            removeInvalidFirstPoints();
        adjustChartScale();
        reloadChartParameters();
    }
}


void chartDialog::on_pushButtonSaveChart_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr,"Zapisz wykres","","PNG Files (*.png);;JPEG Files (*.jpg)");
    if(!filePath.isEmpty())
        ui->chartView->grab().save(filePath);
}

void chartDialog::on_pushButtonCalculateTime_clicked()
{
    convertToDimensionlessTime();
    setMinMaxAxisValues();
    QValueAxis *xAxis = qobject_cast<QValueAxis *>(_chart->axes(Qt::Horizontal).at(0));
    xAxis->setTitleText("Czas bezwymiarowy, -");
}

void chartDialog::on_lineEditSeries1_textChanged(const QString &arg1)
{
    _lineSeries.at(0)->setName(arg1);
}

void chartDialog::on_lineEditSeries2_textChanged(const QString &arg1)
{
    _lineSeries.at(1)->setName(arg1);
}

void chartDialog::on_lineEditSeries3_textChanged(const QString &arg1)
{
    _lineSeries.at(2)->setName(arg1);
}


void chartDialog::on_lineEditSeries4_textChanged(const QString &arg1)
{
    _lineSeries.at(3)->setName(arg1);
}


void chartDialog::on_lineEditSeries5_textChanged(const QString &arg1)
{
    _lineSeries.at(4)->setName(arg1);
}


void chartDialog::on_lineEditSeries6_textChanged(const QString &arg1)
{
    _lineSeries.at(5)->setName(arg1);
}


void chartDialog::on_lineEditChartTitle_textChanged(const QString &arg1)
{
    _chart->setTitle(arg1);
}

void chartDialog::on_spinBoxFontSize_valueChanged(int arg1)
{
    int fontSize = arg1;
    QFont dataFont("Arial", fontSize);
    QFont titleFont("Arial", fontSize, QFont::Bold);
    _chart->legend()->setFont(dataFont);
    _chart->setTitleFont(titleFont);
    _chart->axisX()->setLabelsFont(dataFont);
    _chart->axisY()->setLabelsFont(dataFont);
    _chart->axisX()->setTitleFont(dataFont);
    _chart->axisY()->setTitleFont(dataFont);
}

