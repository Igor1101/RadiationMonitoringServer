#ifndef DB_MNG_H
#define DB_MNG_H
#include <QObject>
#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QSqlDatabase>
#include "defs.h"


class db_mng
{
public:
    db_mng(const QString & file);
    bool insert(QJsonDocument jdoc);
private:
    const QString tb_name = "dosimeter_table";
    bool cr_table();
    QSqlDatabase db;
};


#endif // DB_MNG_H
