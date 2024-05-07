#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filemanager.h"
#include "QVector"
#include "QPair"
#include "QtCharts"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    FileManager _fileManager;
    int _tableColumnNumber;
    int _tableRowNumber;
    //TO DO - Pack this to QVector<QVector<QPoint>>
    QVector<QPointF> _series1;
    QVector<QPointF> _series2;
    QVector<QPointF> _series3;
    QVector<QPointF> _series4;
    QVector<QPointF> _series5;
    QVector<QPointF> _series6;

    QChart *_chart;
    QVector<QLineSeries*> _lineSeries;

    void createChart();
    void loadFileDataToTable();
    int calculateColumnNumber();
};
#endif // MAINWINDOW_H
