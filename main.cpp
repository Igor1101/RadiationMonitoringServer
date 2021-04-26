#include <QCoreApplication>
#include <QtNetwork>
#include <QString>
#include <QDebug>
#include "server.h"
#include "defs.h"

int server_port;
QString server_LOG_file;
QString server_SQL_file;
QString server_DB_file;
class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0) : QObject(parent) {}
    QList<int> lwords;
    double average;
public slots:
    void run();

signals:
    void finished();
};

void Task::run()
{
    static server serv;
    serv.set_logfile(server_LOG_file);
    serv.set_port(server_port);
    serv.set_dbfile(server_DB_file);
    serv.init();
}

#include "main.moc"
int main(int argc, char *argv[])
{
    #ifdef __linux__

#elif _WIN32
    // force output utf8
    system("chcp 65001");
#else

#endif
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QStringList arguments;
    foreach (auto arg, a.arguments()) {
        if (arg.startsWith('/')) {
            arg.replace(0,1,"--");
        }
        arguments.append(arg);
    }
    QCommandLineParser parser;
    parser.setApplicationDescription(APP_NAME " це сервер, що є частиною системи радіаційного моніторингу");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
                          {{ "p", "port"}, "порт", "значення" },
                          {{ "l", "logfile"}, "файл log", "значення" },
                          {{ "d", "sqlbd"}, "файл bd", "значення" }
                      }
                );
    parser.process(arguments);
    QString port_s = parser.value("p");
    int port = port_s.toInt();
    if(port == 0) {
        port = PORT_NUM_DEF;
    }
    server_port = port;
    server_LOG_file = parser.value("l");
    if(server_LOG_file.size() == 0) {
        server_LOG_file = LOG_FILE_DEF;
    }
    // BD
    server_DB_file = parser.value("d");
    if(server_DB_file.size() == 0) {
        server_DB_file = DB_FILE_DEF;
    }

    // creating task
    Task *task = new Task(&a);

    // connect signals to app
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // create task
    QTimer::singleShot(0, task, SLOT(run()));
    return a.exec();
}
