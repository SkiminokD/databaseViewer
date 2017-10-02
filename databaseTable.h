#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QtSql>
#include <QObject>

class DatabaseTable : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseTable(QObject *parent = nullptr);
    ~DatabaseTable();

    QSqlDatabase database() const;

    void    setTable(const QString &tableName);
    QString tableName() const;

    void    setPrimaryKey(const int& index, const QString& fieldName);
    QString primaryKeyField() const;
    int     primaryKeyFieldIndex() const;
protected:
    QSqlDatabase m_db;
    QString m_tableName;
    QPair<int,QString> m_primaryKey;    //index + fieldName;
};

#endif // DATABASETABLE_H
