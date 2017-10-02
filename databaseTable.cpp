#include "databaseTable.h"
#include "debug.h"

DatabaseTable::DatabaseTable(QObject *parent) :
    QObject(parent),
    m_db(QSqlDatabase::database("inobitec"))
{

}

DatabaseTable::~DatabaseTable()
{

}

QSqlDatabase DatabaseTable::database() const
{
    return m_db;
}

void DatabaseTable::setTable(const QString &tableName)
{
    m_tableName = tableName;
}

QString DatabaseTable::tableName() const
{
    return m_tableName;
}

void DatabaseTable::setPrimaryKey(const int &index, const QString &fieldName)
{
    m_primaryKey = {index, fieldName};
    if(index == -1)
    {
        PRINT_CRITICAL("Primary key not found!");
        Q_ASSERT_X(0, "pkey", "primary key not found");
    }
}

QString DatabaseTable::primaryKeyField() const
{
    return m_primaryKey.second;
}

int DatabaseTable::primaryKeyFieldIndex() const
{
    return m_primaryKey.first;
}
