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

    QVector<QLineSeries *> lineSeries() const;
    void setLineSeries(QVector<QLineSeries *> &newLineSeries);

    void setSeriesVisible(int seriesNumber, bool visible);

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

private:
    Ui::chartDialog *ui;
    QVector<QLineSeries*> _lineSeries;
    QChart *_chart;

    double xAxisMin;
    double xAxisMax;
    double yAxisMin;
    double yAxisMax;
};

#endif // CHARTDIALOG_H
