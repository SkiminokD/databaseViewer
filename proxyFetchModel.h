#ifndef PROXYFETCHMODEL_H
#define PROXYFETCHMODEL_H

#include <QAbstractTableModel>
#include <cache.h>
#include <databaseTable.h>

/*!
 * \brief The ProxyFetchModel class
 *
 * The ProxyFetchModel class provides an editable data model for a single database table
 * with table content caching.
 */
class ProxyFetchModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProxyFetchModel(QObject *parent = nullptr);
    virtual ~ProxyFetchModel();

    virtual void setTable(const QString &tableName, const QString &primaryKeyField);
    QString tableName() const;
    void    setColumns(const QVector<QPair<QString,QString>>& columns);

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

    void setCacheSize(const int& value);

private:
    DatabaseTable   *m_table;
    QVector<QString> m_headers;
    qint64 m_rowCount;

    mutable Cache<QVariantVector> m_cache;
};

#endif // PROXYFETCHMODEL_H
