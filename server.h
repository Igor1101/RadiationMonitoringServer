#ifndef SERVER_H
#define SERVER_H
#include "defs.h"
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    void init();
    bool status_working()
    {
        return status;
    }
    void set_port(quint16 port)
    {
        this->port = port;
    }
    void set_jsonfile(QString file)
    {
        this->JSON_file = file;
    }

signals:
public slots:
    void newConnection();
    void readClient();
    void connectionClosed();
private:
    void client_savedata(QJsonDocument data);
    void client_data_save(QJsonDocument jdoc);
    QJsonDocument client_data_read();
    QJsonDocument client_data_addtimestamp(QJsonDocument jdoc, qint64* timestamp);
    quint16 port = PORT_NUM_DEF;
    QString JSON_file = JSON_FILE_DEF;
    bool status;
    QTcpServer*qtcp_serv;
    QMap<int,QTcpSocket *> clients;
};

#endif // SERVER_H
