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
    setWindowTitle("Aplikacja do analizy danych procesu COS");


    initLineSeries();
    ui->tableData->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLineSeries()
{
    _lineSeries.clear();
    for (int i = 0; i < 6; ++i)
        _lineSeries.append(new QLineSeries());
}

void MainWindow::clearData()
{
    for (int row = 0; row < _tableRowNumber; ++row)
    {
        for (int col = 0; col < _tableColumnNumber; ++col)
        {
            QTableWidgetItem* item = ui->tableData->takeItem(row, col);
            delete item;
        }
    }
    ui->tableData->setRowCount(0);
    ui->tableData->setColumnCount(0);

    _series1.clear();
    _series2.clear();
    _series3.clear();
    _series4.clear();
    _series5.clear();
    _series6.clear();
    initLineSeries();
}


void MainWindow::on_pushButton_clicked()
{
    QString path = _fileManager.loadDataFromFile();

    if(!path.isEmpty())
    {
        ui->filePath->setText(path);
        clearData();
        loadFileDataToTable();
        createLineSeries();
    }

    ui->pushButton_2->setEnabled(!ui->filePath->text().isEmpty());
}

void MainWindow::on_pushButton_2_clicked()
{
    chartDialog dialog;
    dialog.setWindowFlags(Qt::Window);
    dialog.setLineSeries(_lineSeries);
    dialog.setUntouchedLineSeries(_lineSeries);
    dialog.convertToTimeStep();
    dialog.prepareChart();
    dialog.setVisibleEmptySeriesCheckBoxes();
    dialog.setVisibleInvalidSeries();
    dialog.setMinMaxAxisValues();
    dialog.on_spinBoxFontSize_valueChanged(10);
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

            qDebug() << currentColumn << "CURRENT COLUMN";
            qDebug() << currentRow << "CURRENT ROW";

            QString adjustedValue = value;
            adjustedValue = adjustedValue.replace(',','.');
            double numericValue = adjustedValue.toDouble();
            qDebug() << recordSplit.at(1);
            QDateTime dateTime = QDateTime::fromString(recordSplit.at(1), "yyyy-MM-dd hh:mm:ss,z");
            if(dateTime.isValid())
            {
                QPointF point(dateTime.toSecsSinceEpoch(),numericValue);
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


