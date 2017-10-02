#include "proxyFetchModel.h"
#include "debug.h"
#include <algorithm>

ProxyFetchModel::ProxyFetchModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_cache.setMaxSize(50);
    m_table = new DatabaseTable(this);
}

ProxyFetchModel::~ProxyFetchModel()
{
    delete m_table;
}

void ProxyFetchModel::setTable(const QString &tableName, const QString &primaryKeyField)
{
    m_table->setTable(tableName);
    if(!m_table->columnsCount())
        m_table->updateColumnsName();
    m_table->setPrimaryKey(primaryKeyField);
    m_rowCount = m_table->selectRowCount();
    m_table->createCursor();
}

QString ProxyFetchModel::tableName() const
{
    return m_table->tableName();
}

void ProxyFetchModel::setColumns(const QVector<QPair<QString, QString> > &columns)
{
    m_table->clearColumns();
    m_headers.clear();
    for(auto column: columns)
    {
        m_table->addColumn(column.first);
        m_headers.push_back(column.second);
    }
}

QVariant ProxyFetchModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            return QVariant(m_headers[section]);
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

    return m_table->columnsCount();
}

QVariant ProxyFetchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(!m_cache.contains(index.row()))
        {
            QVariantVector vec;
            if(!m_table->selectRow(index.row()+1, vec))
                return QVariant();
            m_cache.append(index.row(), QVariantVector());
            m_cache[index.row()].swap(vec);
        }
        return QVariant(m_cache[index.row()][index.column()]);
    }

    return QVariant();
}

bool ProxyFetchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        int pkey = m_cache[index.row()].value(m_table->primaryKeyFieldIndex()).toInt();
        if(!m_table->updateRow(pkey, index.column(), value))
            return false;
        m_cache.remove(index.row());
        m_table->closeCursor();
        m_table->createCursor();
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
    if(row!=0 && row !=rowCount(parent))
        return false;
    beginInsertRows(parent, row, row + count - 1);

    if(!m_table->insertRows(count))
    {
        endInsertRows();
        return false;
    }
    m_rowCount+=count;
    m_table->closeCursor();
    m_table->createCursor();

    endInsertRows();
    return true;
}

bool ProxyFetchModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_ASSERT_X(!m_table->tableName().isEmpty(), "tableName", "tableName is empty");
    beginRemoveRows(parent, row, row + count - 1);
    QSqlQuery query(m_table->database());
    QString request = QString("DELETE FROM \"%1\" WHERE \"%2\" = :id")
                                                    .arg(m_table->tableName())
                                                    .arg(m_table->primaryKeyField());
    for(int i=0; i<count; ++i)
    {
        int removableId = m_cache[row+i].value(m_table->primaryKeyFieldIndex())
                                        .toInt();
        query.prepare(request);
        query.bindValue(":id", removableId);
        if(!query.exec())
        {
            PRINT_CRITICAL(query.lastError().text());
        }
    }
    m_cache.removeAt(row);
    m_rowCount-=count;
    m_table->closeCursor();
    m_table->createCursor();
    endRemoveRows();
    return true;
}

int ProxyFetchModel::fieldIndex(const QString &fieldName) const
{
    return m_table->columnIndex(fieldName);
}

QString ProxyFetchModel::primaryKeyField() const
{
    return m_table->primaryKeyField();
}

int ProxyFetchModel::primaryKeyFieldIndex() const
{
    return m_table->primaryKeyFieldIndex();
}

void ProxyFetchModel::setCacheSize(const int &value)
{
    m_cache.setMaxSize(value);
}
