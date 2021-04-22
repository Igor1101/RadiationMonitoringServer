#include "db_mng.h"
#include <QObject>
#include <QSqlQuery>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "defs.h"

db_mng::db_mng(const QString & file)
{

   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(file);
   if (!db.open()) {
      QINFO << U("db: не можу під'єднатися");
      exit(-1);
   }
   else {
      QINFO << U("db: приєднання");
      cr_table();
   }
}

bool db_mng::cr_table()
{


    QSqlQuery q;
    q.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tb_name));
    if(q.next()) {
        if(q.value(0).toInt()==0) {
            // there is no such table
            QINFO << "table " << tb_name << " not found, creating new one";

            QString s = 	"CREATE TABLE %1 ("
                "timestamp integer PRIMARY KEY NOT NULL, "
                "json VARCHAR(511), "
                "dev_id	  VARCHAR(32) PRIMARY KEY NOT NULL,"
                "GPS	  VARCHAR(256),"
                "NanoSv   integer PRIMARY KEY NOT NULL,"
                "uptime_s integer"
                ");" ;
            QString sn = s.arg(tb_name);
            return q.exec(sn);
        } else {
            // table exists
            QINFO << "table " << tb_name << " found";
        }
    }
    return true;
}

bool db_mng::insert(QJsonDocument jdoc)
{
    QJsonObject jobj = jdoc.object();
    QString timestamp = jobj.value("timestamp").toString();
    QString dev_id = jobj.value("dev_id").toString();
    QString GPS = jobj.value("GPS").toString();
    QString NanoSv = jobj.value("NanoSv").toString();
    QString uptime_s = jobj.value("uptime_s").toString();
    QString s = "INSERT INTO %1(timestamp, json, dev_id, GPS, NanoSv, uptime_s) "
            "VALUES (%2, '%3', %4, %5, %6, %7);";
    QString sins = s.arg(tb_name).arg(timestamp).arg("JSON").arg(dev_id).arg(GPS).arg(NanoSv).arg(uptime_s);
    QSqlQuery q;
    return q.exec(sins);
}
