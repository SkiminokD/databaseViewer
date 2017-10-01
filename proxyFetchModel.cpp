#include "proxyFetchModel.h"
#include "debug.h"
#include <algorithm>

ProxyFetchModel::ProxyFetchModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_db(QSqlDatabase::database("inobitec"))
{
    m_cache.setMaxSize(50);
}

ProxyFetchModel::~ProxyFetchModel()
{
    closeCursor();
}

void ProxyFetchModel::setTable(const QString &tableName, const QString &primaryKeyField)
{
    m_tableName = tableName;
    m_primaryKey.second = primaryKeyField;
    select();
    if(m_columns.isEmpty())
        updateColumnsName();
}

QString ProxyFetchModel::tableName() const
{
    return m_tableName;
}

bool ProxyFetchModel::select()
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_db);
    if(!query.exec(QString("SELECT count(*) FROM \"%1\"").arg(m_tableName)))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    if(query.first())
    {
        m_rowCount = query.value(0).toLongLong();
    }
    createCursor();
    return true;
}

void ProxyFetchModel::updateColumnsName()
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
        {
            m_columns.append({rec.fieldName(i),rec.fieldName(i)});
            if(rec.fieldName(i) == m_primaryKey.second)
                m_primaryKey.first = i;
        }
    }
}

QVariant ProxyFetchModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            return QVariant(m_columns[section].second);
        if(orientation == Qt::Vertical)
            return QVariant(section+1);
    }

    return QVariant();
}

int ProxyFetchModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_rowCount;
}

int ProxyFetchModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_columns.size();
}

QVariant ProxyFetchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QSqlQuery query;
        if(m_cache.contains(index.row()))
            query = m_cache.value(index.row());
        else
        {
            query = QSqlQuery(m_db);
            QString request = "FETCH ABSOLUTE %0 FROM chcursor";
            if(!query.exec(request.arg(index.row()+1)) || !query.first())
            {
                PRINT_CRITICAL(query.lastError().text());
                PRINT_CRITICAL(query.executedQuery());
                return QVariant();
            }
            m_cache.append(index.row(), query);
        }
        return QVariant(query.value(index.column()));
    }

    return QVariant();
}

bool ProxyFetchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    if (data(index, role) != value) {
        QSqlRecord rec = m_cache[index.row()].record();
        m_cache.remove(index.row());

        QSqlQuery query(m_db);
        query.prepare(QString("UPDATE \"%1\" SET \"%2\" = :value WHERE \"%3\" = :id ")
                                        .arg(m_tableName)
                                        .arg(rec.fieldName(index.column()))
                                        .arg(m_primaryKey.second));
        query.bindValue(":value",value);
        query.bindValue(":id",rec.value(m_primaryKey.first));
        if(!query.exec())
        {
            PRINT_CRITICAL(query.lastError().text());
            PRINT_CRITICAL(query.executedQuery());
            return false;
        }
        closeCursor();
        createCursor();
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ProxyFetchModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool ProxyFetchModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    if(row!=0 && row !=rowCount(parent))
        return false;
    beginInsertRows(parent, row, row + count - 1);
    QSqlQuery query(m_db);
    for(int i=0; i<count; ++i)
    {
        QString request = QString("INSERT INTO \"%1\" DEFAULT VALUES").arg(m_tableName);
        query.prepare(request);
        if(!query.exec())
        {
            PRINT_CRITICAL("Insert error:"+query.lastError().text());
        }
    }
    m_rowCount+=count;
    closeCursor();
    createCursor();
    endInsertRows();
    return true;
}

bool ProxyFetchModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    beginRemoveRows(parent, row, row + count - 1);
    QSqlQuery query(m_db);
    QString request = QString("DELETE FROM \"%1\" WHERE \"%2\" = :id")
                                                    .arg(m_tableName)
                                                    .arg(m_primaryKey.second);
    for(int i=0; i<count; ++i)
    {
        int removableId = m_cache[row+i].value(m_primaryKey.first).toInt();
        m_cache.remove(row+i);
        query.prepare(request);
        query.bindValue(":id", removableId);
        if(!query.exec())
        {
            PRINT_CRITICAL(query.lastError().text());
        }
    }
    m_rowCount-=count;
    closeCursor();
    createCursor();
    endRemoveRows();
    return true;
}

int ProxyFetchModel::fieldIndex(const QString &fieldName) const
{
    for(int i=0; i<m_columns.count(); ++i)
        if(m_columns[i].first == fieldName)
            return i;
    return -1;
}

bool ProxyFetchModel::createCursor()
{
    Q_ASSERT_X(!m_tableName.isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_db);
    if(!query.exec("BEGIN WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    if(!query.exec(QString("DECLARE chcursor SCROLL CURSOR FOR "
                           "SELECT * FROM \"%1\" ORDER BY \"%2\"")
                           .arg(m_tableName)
                           .arg(m_primaryKey.second)))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    return true;
}

bool ProxyFetchModel::closeCursor()
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
