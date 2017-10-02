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

void DatabaseTable::setPrimaryKey(const QString &fieldName)
{
    m_primaryKey = {m_columns.indexOf(fieldName), fieldName};
    if(m_primaryKey.first == -1)
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

void DatabaseTable::addColumn(const QString &column)
{
    m_columns.push_back(column);
}

void DatabaseTable::setColumns(const QVector<QString> &columns)
{
    m_columns = columns;
}

void DatabaseTable::updateColumnsName()
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_db);
    if(!query.exec(QString("SELECT * FROM \"%1\" LIMIT 1").arg(m_tableName)))
    {
        PRINT_CRITICAL(query.lastError().text());
        return;
    }
    if(query.first())
    {
        QSqlRecord rec = query.record();
        for(int i=0 ; i<rec.count(); ++i)
            m_columns.push_back(rec.fieldName(i));
    }
}

void DatabaseTable::clearColumns()
{
    m_columns.clear();
}

int DatabaseTable::columnsCount() const
{
    return m_columns.count();
}

QString DatabaseTable::column(int index) const
{
    return m_columns[index];
}

int DatabaseTable::columnIndex(const QString &column) const
{
    return m_columns.indexOf(column);
}

QString DatabaseTable::columnsToString()
{
    QString columns;
    for(auto column: m_columns)
        columns+="\""+column+"\", ";
    columns.chop(2);
    return columns;
}
