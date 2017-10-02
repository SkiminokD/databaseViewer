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
    closeCursor();
    delete m_table;
}

void ProxyFetchModel::setTable(const QString &tableName, const QString &primaryKeyField)
{
    m_table->setTable(tableName);
    if(!m_table->columnsCount())
        m_table->updateColumnsName();
    setPrimaryKey(primaryKeyField);
    select();
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

bool ProxyFetchModel::select()
{
    Q_ASSERT_X(!m_table->tableName().isEmpty(), "tableName", "tableName is empty");
    QSqlQuery query(m_table->database());
    if(!query.exec(QString("SELECT count(*) FROM \"%1\"").arg(m_table->tableName())))
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

void ProxyFetchModel::setPrimaryKey(const QString &primaryKeyField)
{
    m_table->setPrimaryKey(primaryKeyField);
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
            QSqlQuery query = QSqlQuery(m_table->database());
            QString request = "FETCH ABSOLUTE %0 FROM chcursor";
            if(!query.exec(request.arg(index.row()+1)) || !query.first())
            {
                PRINT_CRITICAL(query.lastError().text());
                PRINT_CRITICAL(query.executedQuery());
                return QVariant();
            }
            QVariantVector vec(m_table->columnsCount());
            for(int i=0; i<m_table->columnsCount(); ++i)
                vec[i] = query.value(i);
            m_cache.append(index.row(), QVariantVector());
            m_cache[index.row()].swap(vec);
        }
        return QVariant(m_cache[index.row()][index.column()]);
    }

    return QVariant();
}

bool ProxyFetchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT_X(!m_table->tableName().isEmpty(), "tableName", "tableName is empty");
    if (data(index, role) != value) {
        QSqlQuery query(m_table->database());
        query.prepare(QString("UPDATE \"%1\" SET \"%2\" = :value WHERE \"%3\" = :id ")
                                        .arg(m_table->tableName())
                                        .arg(m_table->column(index.column()))
                                        .arg(m_table->primaryKeyField()));
        query.bindValue(":value",value);
        query.bindValue(":id",m_cache[index.row()].value(
                                                    m_table->primaryKeyFieldIndex()));
        if(!query.exec())
        {
            PRINT_CRITICAL(query.lastError().text());
            PRINT_CRITICAL(query.executedQuery());
            return false;
        }
        m_cache.remove(index.row());
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
    Q_ASSERT_X(!m_table->tableName().isEmpty(), "tableName", "tableName is empty");
    if(row!=0 && row !=rowCount(parent))
        return false;
    beginInsertRows(parent, row, row + count - 1);
    QSqlQuery query(m_table->database());
    for(int i=0; i<count; ++i)
    {
        QString request = QString("INSERT INTO \"%1\" DEFAULT VALUES")
                                    .arg(m_table->tableName());
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
    closeCursor();
    createCursor();
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

/*!
 * \brief ProxyFetchModel::createCursor
 *
 * Starts transaction and create a new cursor to select item.
 *
 * \return true if success.
 */
bool ProxyFetchModel::createCursor()
{
    Q_ASSERT_X(!m_table->tableName().isEmpty(), "tableName", "tableName is empty");
    Q_ASSERT_X(m_table->columnsCount(), "columns", "columns is empty");

    QSqlQuery query(m_table->database());
    if(!query.exec("BEGIN WORK"))
    {
        PRINT_CRITICAL(query.lastError().text());
        return false;
    }
    if(!query.exec(QString("DECLARE chcursor SCROLL CURSOR FOR "
                           "SELECT %1 FROM \"%2\" ORDER BY \"%3\"")
                           .arg(m_table->columnsToString())
                           .arg(m_table->tableName())
                           .arg(m_table->primaryKeyField())))
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
bool ProxyFetchModel::closeCursor()
{
    QSqlQuery query(m_table->database());
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
