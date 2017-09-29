#ifndef CHANNELTABLEMODEL_H
#define CHANNELTABLEMODEL_H

#include <QtSql>

class ChannelTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit ChannelTableModel(QObject *parent = nullptr);

public slots:
    void insertDefaultRow();
    void removeRowImmediately(int index);

private:
    const QMap<const char*, QString> m_fields
    {
        {"id",           tr("ID Канала")},
        {"name",         tr("Имя канала")},
        {"address",      tr("Aдрес")},
        {"port",         tr("Порт")},
        {"protocol",     tr("Протокол")},
        {"login",        tr("Логин")},
        {"password",     tr("Пароль")},
        {"lastStartTime",tr("Последнее время запуска")},
    };
    /* FIXME: Добавить тест для проверки соответствия количества столбцов в m_fields
     *        количеству столбцов в текущей таблице */
};

#endif // CHANNELTABLEMODEL_H
