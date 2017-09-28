#include "channelTableModel.h"
#include "debug.h"

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
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    QSqlQuery query(database());
    QString request = "INSERT INTO channels DEFAULT VALUES";
    query.prepare(request);
    if(!query.exec())
    {
        PRINT_CRITICAL("Insert error:"+query.lastError().text());
    }
    endInsertRows();
    //Обновить кэш
    select(); //WARN: Снижение производительности
}

void ChannelTableModel::removeRowImmediately(int index)
{
    if(!removeRow(index))
        PRINT_CRITICAL("Error while removing: "+lastError().text());
    submitAll();
    //Обновить кэш
    select(); //WARN: Снижение производительности
}
