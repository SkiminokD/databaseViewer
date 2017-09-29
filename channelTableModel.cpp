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
    /* INFO: Для увеличения производительности возможен возврат к штатным средствам
     *       QSqlTableModel по вставке новой записи. Но в таком случае возможно
     *       рассогласование defaults в коде с defaults в реальной таблице.
     *       Дополнительно могут возникнуть проблемы при добавлении в таблицу новых
     *       NOT NULL параметров. */
}

void ChannelTableModel::removeRowImmediately(int index)
{
    if(!removeRow(index))
        PRINT_CRITICAL("Error while removing: "+lastError().text());
    submitAll();
    //Обновить кэш
    select(); //WARN: Снижение производительности
    /* INFO: Для увеличения производительности возможно осуществление дополнительной
     *       обработки представлением событий удаления столбца с целью принудительного
     *       сокрытия знака "!" в заголовке строки после удаления. Или же при развитии
     *       проекта возможна замена представления на более соответствующее будущим
     *       задачам. */
}
