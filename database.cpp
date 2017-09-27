#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{

}

QSqlError Database::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(m_hostname);
    db.setPort(m_port);
    db.setDatabaseName(m_dbname);
    db.setUserName(m_username);
    db.open();
    return db.lastError();
}
