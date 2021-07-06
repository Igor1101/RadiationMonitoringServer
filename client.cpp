#include <QString>
#include <QTcpSocket>
#include <QDebug>
#include <thread>
#include <iostream>
#include <chrono>
#include <QJsonDocument>
#include <QJsonObject>
#include "client.h"
#include "defs.h"

Client::Client(QObject *parent) : QObject(parent)
{
}

void Client::serv_connect()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    socket->connectToHost(host, port);
    QINFO << U("порт:") << port << U(" ,адреса:") << host << U(" ,приєднуємось...");
}

void Client::connected()
{
    QINFO << U("приєднано до сервера");
    // setup this connection+write name
}

void Client::disconnected()
{
    qWarning() << QString::fromUtf8("від'єднався від сервера");
}

void Client::readyRead()
{
    // force read
    //QDEB << "READ:";
    QINFO << socket->readAll();
}

void Client::bytesWritten(qint64 bytes)
{
    //QDEB << "Written " << bytes << " bytes";
}

void Client::init()
{
    // test server here
    // 
    // for i in loop of 1000
	// 60 ms pause
    for(int i=0; i<1000; i++) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(6000));
        serv_connect();
        // send some data
    	QJsonObject jobj;
    	jobj.insert("dev_id", "id:" + QString::number(i));
    	jobj.insert("client", "sensor");
    	jobj.insert("DBsave", "true");
    	jobj.insert("GPS", "");
    	jobj.insert("NanoSv", "0");
    	jobj.insert("uptime_s", "1");
	QJsonDocument jdoc(jobj);
	QString jdoc_str = jdoc.toJson();
        socket->write(jdoc_str.toLocal8Bit()); 
	socket->waitForBytesWritten(1000);
        socket->disconnect();
	socket->waitForDisconnected(1000);

    }
}
