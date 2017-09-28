#include "channelTableModel.h"

ChannelTableModel::ChannelTableModel(QObject *parent):
    QSqlTableModel(parent)
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setTable("channels");
    for(auto field: m_fields.keys())
    {
        setHeaderData(fieldIndex(field), Qt::Horizontal, m_fields.value(field));
    }
    setSort(fieldIndex("id"),Qt::AscendingOrder);
    select();
}

void ChannelTableModel::insertDefaultRow()
{
    emit beginInsertRows(QModelIndex(), rowCount(), rowCount());
    QSqlQuery query(database());
    QString request = "INSERT INTO \"channels\" "
            "(id, name, address, protocol, port, login, password, \"lastStartTime\") "
            "VALUES (default,default,default,default,default,default,default,default)";
    query.prepare(request);
    if(!query.exec())
    {
        qDebug()<<"Insert error"<<query.lastError().text();
    }
    emit endInsertRows();
    select(); //Обновить кэш
}

void ChannelTableModel::removeRowImmediately(int index)
{
    if(!removeRow(index))
        qDebug()<<"Error while removing"<<lastError();
    submitAll();
}
