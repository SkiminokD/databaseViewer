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

    QSqlError init();
protected:
    const QString m_hostname {"localhost"};
    const QString m_dbname   {"inobitec"};
    const int     m_port     {5432};
    const QString m_username {"postgres"};
};

#endif // DATABASE_H
