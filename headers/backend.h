#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include "headers/backmain.h"

class BackEnd : public QObject
{
    Q_OBJECT

public:

    explicit BackEnd(QObject *parent = nullptr);
    Q_INVOKABLE void dataReceived(QString portSelf, QString portOut, QString ip, QString filePath);
    Q_INVOKABLE void setFileSavePath(QString saveFileSavePath){
        Transferer.DESTPATH = saveFileSavePath.toStdString();
        std::cout<<"Save path changed to "<<saveFileSavePath.toStdString()<<std::endl;
    }
    Q_INVOKABLE void init(QString portSelf){
        Transferer.init(portSelf.toInt());
    };
    Q_INVOKABLE void connect(QString portOut, QString ip){
        Transferer.connect(portOut.toInt(),ip.toStdString());
    };
    Q_INVOKABLE void send(QString filePath){
        Transferer.send(filePath.toStdString());
    };
signals:

private:
    BackMain Transferer;
    qint16 m_portOut;
    QString m_portSelf;
    QString m_ip;
    QString m_filePath;
};

#endif // BACKEND_H
