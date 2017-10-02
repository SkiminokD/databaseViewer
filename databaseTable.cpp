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

int DatabaseTable::selectRowCount()
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_db);
    if(!query.exec(QString("SELECT count(*) FROM \"%1\"").arg(m_tableName)))
    {
        PRINT_CRITICAL(query.lastError().text());
        return 0;
    }
    if(query.first())
    {
        return query.value(0).toLongLong();
    }
    return 0;
}

bool DatabaseTable::selectRow(const int &index, QVariantVector &result)
{
    QSqlQuery query = QSqlQuery(m_db);
    QString request = "FETCH ABSOLUTE %0 FROM chcursor";
    if(!query.exec(request.arg(index)) || !query.first())
    {
        PRINT_CRITICAL(query.lastError().text());
        PRINT_CRITICAL(query.executedQuery());
        return false;
    }
    result.resize(m_columns.count());
    for(int i=0; i<m_columns.count(); ++i)
        result[i] = query.value(i);
    return true;
}

bool DatabaseTable::updateRow(const int &index,
                              const int &fieldIndex,
                              const QVariant &value)
{
    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE \"%1\" SET \"%2\" = :value WHERE \"%3\" = :id ")
                                    .arg(m_tableName)
                                    .arg(m_columns[fieldIndex])
                                    .arg(m_primaryKey.second));
    query.bindValue(":value",value);
    query.bindValue(":id",   index);
    if(!query.exec())
    {
        PRINT_CRITICAL(query.lastError().text());
        PRINT_CRITICAL(query.executedQuery());
        return false;
    }
    return true;
}

bool DatabaseTable::insertRows(const int& count)
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_db);
    for(int i=0; i<count; ++i)
    {
        QString request = QString("INSERT INTO \"%1\" DEFAULT VALUES").arg(m_tableName);
        query.prepare(request);
        if(!query.exec())
        {
            PRINT_CRITICAL("Insert error:"+query.lastError().text());
            return false;
        }
    }
    return true;
}
