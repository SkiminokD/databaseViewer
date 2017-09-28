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
};

#endif // CHANNELTABLEMODEL_H
