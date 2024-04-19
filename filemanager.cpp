#include "filemanager.h"
#include "QDebug"
#include "QCoreApplication"
FileManager::FileManager(QObject *parent)
    : QObject{parent}
{

}

void FileManager::loadDataFromFile()
{
    qDebug() << "POCZATEK";
    QString filePath = QCoreApplication::applicationDirPath() + "/krzywaF.csv";
    qDebug() << filePath;
    _csvFile.setFileName(filePath);
    if(_csvFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "SRODEK";
        QTextStream stream(&_csvFile);
        while(stream.atEnd() == false)
        {
            QString data = stream.readLine();
            qDebug() << data;
        }
    }
    else
    {
        qDebug() << "Błąd podczas otwierania pliku:" << _csvFile.errorString();
    }

}
