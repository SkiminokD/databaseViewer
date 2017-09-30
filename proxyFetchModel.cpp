#include "proxyFetchModel.h"
#include "debug.h"
#include <algorithm>

ProxyFetchModel::ProxyFetchModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_db(QSqlDatabase::database("inobitec"))
{
    select();
    m_cache.setMaxSize(50);
}

ProxyFetchModel::~ProxyFetchModel()
{
    QSqlQuery query(m_db);
    if(!query.exec("CLOSE chcursor"))
    {
        PRINT_CRITICAL(query.lastError().text());
    }
    if(!query.exec("COMMIT WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
    }
}

bool ProxyFetchModel::select()
{
    QSqlQuery query(m_db);
    if(!query.exec("SELECT count(*) FROM channels"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    if(query.first())
    {
        m_rowCount = query.value(0).toLongLong();
    }
    if(!query.exec("BEGIN WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
    }
    if(!query.exec("DECLARE chcursor SCROLL CURSOR FOR "
                   "SELECT * FROM channels ORDER BY id"))
    {
        PRINT_CRITICAL(query.lastError().text());
    }
    return true;
}

QVariant ProxyFetchModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            return QVariant(m_headers.value(static_cast<Column>(section),""));
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

    return m_headers.size();
}

QVariant ProxyFetchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        auto rowQuery = std::find_if(m_cache.begin(), m_cache.end(),
                        [index](auto object) { return object.second == index.row(); } );

        if(rowQuery == m_cache.end())
        {
            RowQuery query(QSqlQuery(m_db),index.row());
            QString request = "FETCH ABSOLUTE %0 FROM chcursor";
            if(!query.first.exec(request.arg(index.row()+1)) || !query.first.first())
            {
                PRINT_CRITICAL(query.first.lastError().text());
                PRINT_CRITICAL(query.first.executedQuery());
                return QVariant();
            }
            m_cache.append(query);
            rowQuery = m_cache.end();
            --rowQuery;
        }
        return QVariant(rowQuery->first.value(index.column()));
    }

    return QVariant();
}

int ProxyFetchModel::fieldIndex(const QString &fieldName) const
{
    return static_cast<int>(m_headers.key(fieldName));
}
