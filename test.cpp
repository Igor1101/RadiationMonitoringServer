#include <QCoreApplication>
#include <QtNetwork>
#include <QString>
#include <QDebug>
#include "client.h"
#include "defs.h"

int server_port;
QString server_addr;
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
    static Client c;
    c.set_port(server_port);
    c.set_host(server_addr);
    c.init();
    exit(0);
}

#include "test.moc"
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
    parser.setApplicationDescription(APP_NAME " це клієнт тестування, що є частиною системи радіаційного моніторингу");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
                          {{ "p", "port"}, "порт", "значення" },
                          {{ "a", "addr"}, "адреса", "значення" },
                      }
                );
    parser.process(arguments);
    QString port_s = parser.value("p");
    int port = port_s.toInt();
    if(port == 0) {
        port = PORT_NUM_DEF;
    }
    server_port = port;
    server_addr = parser.value("d");
    if(server_addr.size() == 0) {
        server_addr = "127.0.0.1";
    }

    // creating task
    Task *task = new Task(&a);

    // connect signals to app
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // create task
    QTimer::singleShot(0, task, SLOT(run()));
    return a.exec();
}
