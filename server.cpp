#include <QObject>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "server.h"
#include "defs.h"
#include "db_mng.h"

server::server(QObject *parent) : QObject(parent)
{
    qtcp_serv = new QTcpServer(this);
    // signal
    connect(qtcp_serv, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void server::init()
{
    status = qtcp_serv->listen(QHostAddress::Any, port);
    if(status) {
        QINFO << U("Сервер чекає на клієнтів, порт:") <<  port;
    } else {
        QCRI << U("Помилка запуска сервера, не може чекати клієнтів");
        QCRI << qtcp_serv->serverError() << qtcp_serv->errorString();
        exit(-1);
    }
    // create or open DB
    db = new db_mng(db_file);
}

void server::newConnection()
{
    QINFO << U("нове з'єднання");
    QTcpSocket * socket = qtcp_serv->nextPendingConnection();
    int idusersocs=socket->socketDescriptor();
    clients[idusersocs]=socket;
    // append signal from this user
    connect(clients[idusersocs],SIGNAL(readyRead()),this, SLOT(readClient()));
}

void server::readClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QByteArray arr = clientSocket->readAll();
     // client added some data
    QJsonDocument jdoc = QJsonDocument::fromJson(arr);
    qint64 timestamp;
    jdoc = client_data_addtimestamp(jdoc, &timestamp);
    // save to the file
    client_data_file_save(jdoc);
    // findout what is it
    QJsonObject jobj = jdoc.object();
    // client can be "app" or "sensor"
    QString client = jobj.value("client").toString();
    if(client == "app") {
        // manage app tasks
        // parse app specific funcs
        // attr "mode" has following values:
        // "get_data_all", "get_data_since_time" , "set_settings", ""
        QString mode = jobj.value("mode").toString();
        if(mode == "get_data_all") {
            QJsonDocument jall = db->get_all();
            clientSocket->write(jall.toJson());
        } else if(mode == "get_data_since_time") {
            // since_time attr is used
            qint64 since_time = jobj.value("since_time").toInt();
            QJsonDocument jfound = db->get("timestamp > " + QString::number(since_time));
            clientSocket->write(jfound.toJson());
        }
    } else if(client == "sensor") {
        // do sensor specific staff
    }
    // save to DB needed?
    QString DBsave = jobj.value("DBsave").toString();
    if(DBsave == "true" && client == "sensor") {
        // save to DB
        db->insert(jdoc);
    }
    QJsonDocument jall = db->get_all();
    //QString res = QString(arr);
    QDEB << "recv:id:" + QString::number(idusersocs) + "data:" + QString(jall.toJson());
}

QJsonDocument server::client_data_addtimestamp(QJsonDocument jdoc, qint64* timestamp)
{
    qint64 time_st = QDateTime::currentSecsSinceEpoch();
    if(timestamp != NULL)
        *timestamp = time_st;
    QJsonObject jobj = jdoc.object();
    jobj.insert("timestamp", time_st);
    QJsonDocument jdoc_new(jobj);
    return jdoc_new;
}

void server::client_data_file_save(QJsonDocument jdoc)
{
    QFile fobj(LOG_file);
    if(!fobj.open(QFile::WriteOnly| QFile::Text | QFile::Append)){
        QINFO <<"Failed to open "<<LOG_file;
        exit(1);
    }
    fobj.write(jdoc.toJson());
    fobj.close();
}

QJsonDocument server::client_data_file_read()
{
    QFile fobj(LOG_file);
    if(!fobj.open(QFile::ReadOnly| QFile::Text)){
        QINFO <<"Failed to open "<<LOG_file;
        exit(1);
    }
    QString data = fobj.readAll();
    fobj.close();
    return QJsonDocument::fromJson(data.toUtf8());
}
