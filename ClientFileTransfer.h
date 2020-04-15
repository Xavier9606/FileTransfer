// ClientFileTransfer.h : Include file for standard system include files,
// or project specific include files.
#pragma once

//Qt
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>

//Standart/Windows
#include <winsock2.h>
#include "stdio.h"
#include <iostream>
#include <charconv>
#include <thread>
#include <filesystem>


//My API's & etc
#include "SocketsAPI.h"
#include "MyFileAPI.h"


//Misc
int numberOfDigits(int numToCheck) {
	for (int i = 1; i < 20; i++) {
		numToCheck /= 10;
		if (numToCheck <= 0) {
			return i;
		}
	}
}
// TODO: Reference additional headers your program requires here.
