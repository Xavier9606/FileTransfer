#include "headers/backend.h"
#include <QDebug>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
}

void BackEnd::dataReceived(QString ip, QString port, QString filePath)
{
    qDebug() << ip;
    qDebug() << port;
    qDebug() << filePath;
}
