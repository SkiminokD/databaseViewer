#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

/* TODO: Добавить функции:
 * - create - создание базы данных и таблиц
 * - check - проверки базы данных (возможно проверка версии)
 * - fill - начальное наполнение базы данных (возможно из файла) */

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

    QSqlError init(QString hostname = defaultHostname,
                   int port = defaultPort,
                   QString username = defaultUsername,
                   QString password = defaultPassword);
public:
    static const char defaultHostname[];
    static const char defaultDatabaseName[];
    static const int  defaultPort;
    static const char defaultUsername[];
    static const char defaultPassword[];
};

#endif // DATABASE_H
