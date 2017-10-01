#ifndef PROXYFETCHMODEL_H
#define PROXYFETCHMODEL_H

#include <QAbstractTableModel>
#include <QtSql>
#include <cache.h>

class ProxyFetchModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProxyFetchModel(QObject *parent = nullptr);
    virtual ~ProxyFetchModel();

    virtual void setTable(const QString &tableName, const QString &primaryKeyField);
    QString tableName() const;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    int fieldIndex(const QString &fieldName) const;
    QString primaryKeyField() const;
    int     primaryKeyFieldIndex() const;

    enum class Column : quint32
    {
        ID = 0,
        NAME,
        ADDRESS,
        PORT,
        PROTOCOL,
        LOGIN,
        PASSWORD,
        LAST_START_TIME
    };
    Q_ENUM(Column)
private:
    const QMap<Column, QString> m_headers =
        {
            {Column::ID,           tr("ID Канала")},
            {Column::NAME,         tr("Имя канала")},
            {Column::ADDRESS,      tr("Aдрес")},
            {Column::PORT,         tr("Порт")},
            {Column::PROTOCOL,     tr("Протокол")},
            {Column::LOGIN,        tr("Логин")},
            {Column::PASSWORD,     tr("Пароль")},
            {Column::LAST_START_TIME,tr("Последнее время запуска")}
        };
    QVector<QPair<QString,QString>> m_columns;
    qint64 m_rowCount;
    QSqlDatabase m_db;
    QString m_tableName;
    QPair<int,QString> m_primaryKey;    //index + fieldName;

    using RowQuery = QPair<QSqlQuery,int>;
    mutable Cache<QSqlQuery> m_cache;

    bool createCursor();
    bool closeCursor();
protected:
    bool select();
    void updateColumnsName();
};

#endif // PROXYFETCHMODEL_H
