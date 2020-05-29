#include "headers/backend.h"
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>


BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    QNetworkAccessManager networkManager;

    QUrl url("https://api.ipify.org");
    //the query used to add the parameter "format=json" to the request
    QUrlQuery query;
    query.addQueryItem("format", "json");
    //set the query on the url
    url.setQuery(query);

    //make a *get* request using the above url
    QNetworkReply* reply = networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [&](){
        if(reply->error() != QNetworkReply::NoError) {
            //failure
            qDebug() << "error: " << reply->error();
        } else { //success
            //parse the json reply to extract the IP address
            QJsonObject jsonObject= QJsonDocument::fromJson(reply->readAll()).object();
            QHostAddress ip(jsonObject["ip"].toString());
            //do whatever you want with the ip
            qDebug() << "external ip: " << ip;
        }
        //delete reply later to prevent memory leak
        reply->deleteLater();
    });
}

void BackEnd::dataReceived(QString portSelf, QString portOut, QString ip, QString filePath)
{
    qDebug() << "portSelf: "<< portSelf;
    qDebug() << "portOut:  "<< portOut;
    qDebug() << "ip:       "<< ip;
    qDebug() << "filePath: "<< filePath;
}
