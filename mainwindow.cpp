#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _fileManager.setParent(this);
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

void MainWindow::loadFileDataToTable()
{
    ui->tableData->setRowCount(_fileManager.dataList().size());
    ui->tableData->setColumnCount(calculateColumnNumber());
    _tableColumnNumber = ui->tableData->colorCount();
    _tableRowNumber = ui->tableData->rowCount();

    int currentRow = 0;
    int currentColumn = 0;

    foreach (const QString &record, _fileManager.dataList()) {
       QStringList recordSplit = record.split(";");
       currentRow++;
       currentColumn = 0;
       foreach (const QString &value, recordSplit) {
           QTableWidgetItem *item = new QTableWidgetItem(value);
           ui->tableData->setItem(currentRow,currentColumn,item);
           currentColumn++;
       }
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


