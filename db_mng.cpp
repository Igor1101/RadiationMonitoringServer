#include "db_mng.h"
#include <QObject>
#include <QtSql>
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
      if(!cr_table()) {
          QINFO << U("db: таблиця не може бути створена");
      }
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
                "dev_id	  VARCHAR(32) NOT NULL,"
                "GPS	  VARCHAR(256),"
                "NanoSv   integer NOT NULL,"
                "uptime_s integer"
                ");" ;
            QString sn = s.arg(tb_name);
            bool res = q.exec(sn);
            if(!res) {
                QINFO << "cr_table:" << q.lastError().databaseText() << " " << q.lastError().driverText();
            }
            return res;
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
            "VALUES (\"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\""");";
    QString sins = s.arg(tb_name, timestamp, "JSON", dev_id, GPS, NanoSv, uptime_s);
    QSqlQuery q;
    bool res = q.exec(sins);
    if(!res)
        QINFO << "cr_table:" << q.lastError().databaseText() << " " << q.lastError().driverText();
    return res;
}

QJsonDocument db_mng::get_all()
{
    QString s = "select * from %1;";
    QString sins = s.arg(tb_name);
    QSqlQuery q;
    if(!q.exec(sins)) {
        QINFO << "db: select* not working";
        QJsonDocument jdoc;
        return jdoc;
    }
    QSqlRecord rec = q.record();
    QJsonArray jarr;
    while(q.next()) {
        QJsonObject jobj_inst;
        QString timestamp = q.value(rec.indexOf("timestamp")).toString();
        QString dev_id = q.value(rec.indexOf("dev_id")).toString();
        QString GPS = q.value(rec.indexOf("GPS")).toString();
        QString NanoSv = q.value(rec.indexOf("NanoSv")).toString();
        QString uptime_s = q.value(rec.indexOf("uptime_s")).toString();
        jobj_inst.insert("timestamp", QJsonValue::fromVariant(timestamp));
        jobj_inst.insert("dev_id", QJsonValue::fromVariant(dev_id));
        jobj_inst.insert("GPS", QJsonValue::fromVariant(GPS));
        jobj_inst.insert("NanoSv", QJsonValue::fromVariant(NanoSv));
        jobj_inst.insert("uptime_s", QJsonValue::fromVariant(uptime_s));
        jarr.push_back(jobj_inst);
    }
    QJsonObject jobj;
    jobj.insert("recorded", jarr);
    QJsonDocument jdoc(jobj);
    return jdoc;
}
