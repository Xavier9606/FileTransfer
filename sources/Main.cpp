// Main.cpp : Defines the entry point for the application.

#include "headers/Main.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "headers/backend.h"


#define DEBUG

int main(int argc, char* argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");


    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();;
}


//const int CHUNKSIZE = 1024 * 3; //bytes
//
//const std::string str1("C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.rar"); //Source file
//
//const std::string str2("C:\\Users\\Administrator\\Desktop\\Received\\ChunkedRecv "); //Save path
//const std::string TEMP_EXT = ".inprogress"; //temp file extension until file is not downloaded
//
//
//const std::string mapFilePath("C:\\Users\\Administrator\\Desktop\\Received\\"); //temp map file path
//const std::string MAP_EXT = ".chunkmap"; //map file extention


/*
192.168.88.254
C:\\Users\\Administrator\\Desktop\\FileTranserTest\\Source.jpg
C:\\Users\\Administrator\\Desktop\\Received\\
*/
