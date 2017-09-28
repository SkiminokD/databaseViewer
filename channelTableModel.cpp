#include "channelTableModel.h"

ChannelTableModel::ChannelTableModel(QObject *parent):
    QSqlTableModel(parent)
{

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
    this->select(); //Обновить кэш
}
