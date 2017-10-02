#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QtSql>
#include <QObject>

class DatabaseTable : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseTable(QObject *parent = nullptr);
    ~DatabaseTable();

    QSqlDatabase database() const;

    void    setTable(const QString &tableName);
    QString tableName() const;

    void    setPrimaryKey(const QString& fieldName);
    QString primaryKeyField() const;
    int     primaryKeyFieldIndex() const;

    void    addColumn(const QString& column);
    void    setColumns(const QVector<QString>& columns);
    void    updateColumnsName();
    void    clearColumns();
    int     columnsCount() const;
    QString column(int index) const;
    int     columnIndex(const QString& column) const;
    QString columnsToString();
protected:
    QSqlDatabase m_db;
    QString m_tableName;
    QPair<int,QString> m_primaryKey;    //index + fieldName;
    QVector<QString> m_columns;
};

#endif // DATABASETABLE_H
