#include "filemanager.h"
#include <QDebug>
#include <QCoreApplication>
FileManager::FileManager(QWidget *parent)
    : QWidget{parent}
{

}

QString FileManager::loadDataFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Wybierz plik",QDir::currentPath(),"Pliki CSV (*.csv);;Wszystkie pliki (*)");
    _csvFile.setFileName(filePath);
    if(_csvFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream (&_csvFile);
        _dataList.clear();
        while(stream.atEnd() == false)
        {
            QString data = stream.readLine();
            qDebug() << data;
            _dataList.append(data);
        }
    }
    else
    {
        qDebug() << _csvFile.errorString();
    }
    return filePath;
}

QStringList FileManager::dataList() const
{
    return _dataList;
}
