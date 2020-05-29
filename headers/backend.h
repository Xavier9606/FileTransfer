#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class BackEnd : public QObject
{
    Q_OBJECT

public:
    explicit BackEnd(QObject *parent = nullptr);
    Q_INVOKABLE void dataReceived(QString portSelf, QString portOut, QString ip, QString filePath);

signals:

private:
    qint16 m_portOut;
    QString m_portSelf;
    QString m_ip;
    QString m_filePath;
};

#endif // BACKEND_H
