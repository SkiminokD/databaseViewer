#include "channelTableModel.h"

ChannelTableModel::ChannelTableModel(QObject *parent):
    QSqlTableModel(parent)
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setTable("channels");
    setHeaderData(fieldIndex("id"),       Qt::Horizontal, tr("ID Канала"));
    setHeaderData(fieldIndex("name"),     Qt::Horizontal, tr("Имя канала"));
    setHeaderData(fieldIndex("address"),  Qt::Horizontal, tr("Aдрес"));
    setHeaderData(fieldIndex("port"),     Qt::Horizontal, tr("Порт"));
    setHeaderData(fieldIndex("protocol"), Qt::Horizontal, tr("Протокол"));
    setHeaderData(fieldIndex("login"),    Qt::Horizontal, tr("Логин"));
    setHeaderData(fieldIndex("password"), Qt::Horizontal, tr("Пароль"));
    setHeaderData(fieldIndex("lastStartTime"), Qt::Horizontal,
                                               tr("Последнее время запуска"));
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
