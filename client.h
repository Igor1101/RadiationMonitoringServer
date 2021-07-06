#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void serv_connect();

signals:
    // signals
public slots:
    void set_port(quint16 port)
    {
        this->port = port;
    }
    void set_host(QString host)
    {
        this->host = host;
    }
    void set_username(QString username)
    {
        this->username = username;
    }
    void init();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
private:
    QString username;
    QString host;
    QString data_snd = "";
    quint16 port;
    QTcpSocket *socket;
};

#endif // CLIENT_H
