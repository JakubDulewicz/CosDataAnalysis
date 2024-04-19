#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _fileManager.setParent(this);
    _lineSeries1 = new QLineSeries();
    _chart = new QChart();
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

}

void MainWindow::createChart()
{

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
           if((currentRow >= 1) && (currentColumn >= 2))
           {
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

           }
           ui->tableData->setItem(currentRow,currentColumn,item);
           currentColumn++;
       }
       currentRow++;
    }
    qDebug() << _series1;

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


