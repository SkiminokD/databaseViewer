#include "database.h"

const char Database::defaultHostname[] = "localhost";
const char Database::defaultDatabaseName[] = "inobitec";
const int  Database::defaultPort = 5432;
const char Database::defaultUsername[] = "postgres";
const char Database::defaultPassword[] = "";

Database::Database(QObject *parent) : QObject(parent)
{

}

QSqlError Database::init(QString hostname, int port, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(defaultDatabaseName);
    db.setUserName(username);
    db.setPassword(password);
    db.open();
    return db.lastError();
}
