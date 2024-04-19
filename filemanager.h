#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDir>
class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);
    void loadDataFromFile();

private:
    QFile _csvFile;


signals:

};

#endif // FILEMANAGER_H
