#include "databaseTable.h"
#include "debug.h"

DatabaseTable::DatabaseTable(QObject *parent) :
    QObject(parent),
    m_db(QSqlDatabase::database("inobitec"))
{

}

DatabaseTable::~DatabaseTable()
{
    closeCursor();
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

/*!
 * \brief ProxyFetchModel::createCursor
 *
 * Starts transaction and create a new cursor to select item.
 *
 * \return true if success.
 */
bool DatabaseTable::createCursor()
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    Q_ASSERT_X(columnsCount(), "columns", "columns is empty");

    QSqlQuery query(m_db);
    if(!query.exec("BEGIN WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    QString columns;
    for(auto column: m_columns)
        columns+="\""+column+"\", ";
    columns.chop(2);
    if(!query.exec(QString("DECLARE chcursor SCROLL CURSOR FOR "
                           "SELECT %1 FROM \"%2\" ORDER BY \"%3\"")
                           .arg(columns)
                           .arg(m_tableName)
                           .arg(m_primaryKey.second)))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    return true;
}

/*!
 * \brief ProxyFetchModel::closeCursor
 *
 * Closes cursor and commit transaction
 *
 * \return true if success.
 */
bool DatabaseTable::closeCursor()
{
    QSqlQuery query(m_db);
    if(!query.exec("CLOSE chcursor"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    if(!query.exec("COMMIT WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    return true;
}
