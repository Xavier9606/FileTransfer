#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class BackEnd : public QObject
{
    Q_OBJECT

public:
    explicit BackEnd(QObject *parent = nullptr);
    Q_INVOKABLE void dataReceived(QString ip, QString port, QString filePath);

signals:

private:
    QString m_ip;
    qint16 m_port;
    QString m_filePath;
};

#endif // BACKEND_H
