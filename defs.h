#ifndef DEFS_H
#define DEFS_H
#include <QDebug>
#define PORT_NUM_DEF 19999
#define ADDR_DEFAULT "127.0.0.1"
#define U(STR)	QStringLiteral(STR)
#define QINFO 	qInfo().noquote()
#define QWARN 	qWarning().noquote()
#define QDEB	qDebug().noquote()
#define QERR	qError().noquote()
#define QCRI	qCritical().noquote()
#define NAME_SZ 64
#define APP_VERSION "0.0"
#define APP_NAME "RadiationMonitoringServer"
#define LOG_FILE_DEF APP_NAME "_" "LOG" ".log"
#define DB_FILE_DEF APP_NAME ".db"
struct client_info_t {
    char usrname[NAME_SZ];
};

// transmission
// username "broadcast" is reserved
struct client_transmission_t {
    char usrname_transfer[NAME_SZ];
};

#endif // DEFS_H
