#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QDir>

class FileManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = nullptr);
    QString loadDataFromFile();
    QStringList dataList() const;

private:
    QFile _csvFile;
    QStringList _dataList;

signals:

};

#endif // FILEMANAGER_H
