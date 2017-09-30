#include "proxyFetchModel.h"
#include "debug.h"

ProxyFetchModel::ProxyFetchModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_db = QSqlDatabase::database("inobitec");
    select();
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

    // FIXME: Implement me!
    return QVariant();
}

int ProxyFetchModel::fieldIndex(const QString &fieldName) const
{
    return static_cast<int>(m_headers.key(fieldName));
}
