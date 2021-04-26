#ifndef SERVER_H
#define SERVER_H
#include "defs.h"
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include "db_mng.h"

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
    void set_logfile(QString file)
    {
        this->LOG_file = file;
    }
    void set_dbfile(QString file)
    {
        this->db_file = file;
    }

signals:
public slots:
    void newConnection();
    void readClient();
private:
    void client_savedata(QJsonDocument data);
    void client_data_file_save(QJsonDocument jdoc);
    QJsonDocument client_data_file_read();
    QJsonDocument client_data_addtimestamp(QJsonDocument jdoc, qint64* timestamp);
    quint16 port = PORT_NUM_DEF;
    QString LOG_file = LOG_FILE_DEF;
    QString db_file = DB_FILE_DEF;
    bool status;
    QTcpServer*qtcp_serv;
    QMap<int,QTcpSocket *> clients;
    // BD
    db_mng*db;
};

#endif // SERVER_H
