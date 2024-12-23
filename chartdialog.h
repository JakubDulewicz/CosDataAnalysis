#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include "QtCharts"

namespace Ui {
class chartDialog;
}

class chartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chartDialog(QWidget *parent = nullptr);
    ~chartDialog();

    void prepareChart();
    void convertToTimeStep();
    void convertToDimensionlessTracerConcentration();
    void convertToDimensionlessTime();
    void adjustChartScale();
    bool checkStandardDeviationForFirstPoint();
    void unifySeriesVisibility();
    void removeInvalidFirstPoints();
    double calculateStandardDeviation(QLineSeries *series);
    void reloadChartParameters();

    void setMinMaxAxisValues();
    void setSeriesVisible(int seriesNumber, bool visible);
    void setVisibleDimensionlessTime(bool visible);
    void setVisibleEmptySeriesCheckBoxes();
    void setVisibleInvalidSeries();

    QVector<QLineSeries *> lineSeries() const;
    void setLineSeries(const QVector<QLineSeries *> &newLineSeries);

    QVector<QLineSeries *> untouchedLineSeries() const;
    void setUntouchedLineSeries(const QVector<QLineSeries *> &newUntouchedLineSeries);

private slots:
    void on_checkBoxSeries1_stateChanged(int arg);

    void on_checkBoxSeries2_stateChanged(int arg1);

    void on_checkBoxSeries3_stateChanged(int arg1);

    void on_checkBoxSeries4_stateChanged(int arg1);

    void on_checkBoxSeries5_stateChanged(int arg1);

    void on_checkBoxSeries6_stateChanged(int arg1);

    void on_doubleSpinBoxXAxisMin_valueChanged(double arg1);

    void on_doubleSpinBoxXAxisMax_valueChanged(double arg1);

    void on_doubleSpinBoxYAxisMin_valueChanged(double arg1);

    void on_doubleSpinBoxYAxisMax_valueChanged(double arg1);

    void on_pushButtonAutoAdjustment_clicked();

    void on_checkBoxReadValueError_stateChanged(int arg1);

    void on_checkBoxDimensionlessTime_stateChanged(int arg1);

    void on_checkBoxDimensionlessTracerConcentration_stateChanged(int arg1);

    void on_pushButtonSaveChart_clicked();

    void on_pushButtonCalculateTime_clicked();

    void on_lineEditSeries1_textChanged(const QString &arg1);

    void on_lineEditSeries2_textChanged(const QString &arg1);

    void on_lineEditSeries3_textChanged(const QString &arg1);

    void on_lineEditSeries4_textChanged(const QString &arg1);

    void on_lineEditSeries5_textChanged(const QString &arg1);

    void on_lineEditSeries6_textChanged(const QString &arg1);

    void on_lineEditChartTitle_textChanged(const QString &arg1);

public slots:
    void on_spinBoxFontSize_valueChanged(int arg1);

private:
    Ui::chartDialog *ui;
    QVector<QLineSeries*> _lineSeries;
    QVector<QLineSeries*> _untouchedLineSeries;
    QChart *_chart;

    double _xAxisMin;
    double _xAxisMax;
    double _yAxisMin;
    double _yAxisMax;
};

#endif // CHARTDIALOG_H
