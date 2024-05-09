#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartdialog.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);

    for (int i = 0; i < 6; ++i)
        _lineSeries.append(new QLineSeries());

    ui->tableData->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
   QString path = _fileManager.loadDataFromFile();
   ui->filePath->setText(path);
   loadFileDataToTable();
   if(!ui->filePath->text().isEmpty())
       ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    createLineSeries();
    chartDialog dialog;
    dialog.setWindowFlags(Qt::Window);
    dialog.setLineSeries(_lineSeries);
    dialog.setUntouchedLineSeries(_lineSeries);
    dialog.prepareChart();
    dialog.setVisibleEmptySeriesCheckBoxes();
    dialog.exec();
}


void MainWindow::createLineSeries()
{
    foreach(const QPointF &point, _series1) {
        qDebug() << point;
        _lineSeries.at(0)->append(point);
    }
    foreach(const QPointF &point, _series2) {
        qDebug() << point;
        _lineSeries.at(1)->append(point);
    }
    foreach(const QPointF &point, _series3) {
        qDebug() << point;
        _lineSeries.at(2)->append(point);
    }
    foreach(const QPointF &point, _series4) {
        qDebug() << point;
        _lineSeries.at(3)->append(point);
    }
    foreach(const QPointF &point, _series5) {
        qDebug() << point;
        _lineSeries.at(4)->append(point);
    }
    foreach(const QPointF &point, _series6) {
        qDebug() << point;
        _lineSeries.at(5)->append(point);
    }

}
void MainWindow::loadFileDataToTable()
{
    ui->tableData->setRowCount(_fileManager.dataList().size());
    ui->tableData->setColumnCount(calculateColumnNumber());
    _tableColumnNumber = ui->tableData->colorCount();
    _tableRowNumber = ui->tableData->rowCount();

    int currentRow = 0;
    int currentColumn = 0;

    foreach (const QString &record, _fileManager.dataList())
    {
       QStringList recordSplit = record.split(";");
       currentColumn = 0;


       foreach (const QString &value, recordSplit)
       {
           QTableWidgetItem *item = new QTableWidgetItem(value);
           //if((currentRow >= 2) && (currentColumn >= 2))
           //{
               qDebug() << currentColumn << "CURRENT COLUMN";
               qDebug() << currentRow << "CURRENT ROW";

               QString adjustedValue = value;
               adjustedValue = adjustedValue.replace(',','.');
               double numericValue = adjustedValue.toDouble();

               //It look like a piece of shit and indead it's a shit
               QPointF point(currentRow,numericValue);
               switch (currentColumn) {
               case 2: _series1.append(point); break;
               case 3: _series2.append(point); break;
               case 4: _series3.append(point); break;
               case 5: _series4.append(point); break;
               case 6: _series5.append(point); break;
               case 7: _series6.append(point); break;
               default:
                   break;
               }

           //}
           ui->tableData->setItem(currentRow,currentColumn,item);
           currentColumn++;
       }
       currentRow++;
    }
}


int MainWindow::calculateColumnNumber()
{
    int column = 0;
    if(_fileManager.dataList().size() > 0)
    {
       QString row = _fileManager.dataList().first();
       QStringList rowCount = row.split(";");
       column = rowCount.size();
    }
    return column;
}


