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
        while(stream.atEnd() == false)
        {
            QString data = stream.readLine();
            qDebug() << data;
        }
    }
    else
    {
        qDebug() << _csvFile.errorString();
    }
    return filePath;
}
