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

private:
    Ui::chartDialog *ui;
    QVector<QLineSeries*> _lineSeries;
    QChart *_chart;
};

#endif // CHARTDIALOG_H
