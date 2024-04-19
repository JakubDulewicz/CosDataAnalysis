#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QDir>

class FileManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = nullptr);
    QString loadDataFromFile();
private:
    QFile _csvFile;

signals:

};

#endif // FILEMANAGER_H
