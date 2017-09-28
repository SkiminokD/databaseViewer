#ifndef CHANNELTABLEVIEW_H
#define CHANNELTABLEVIEW_H

#include <QWidget>
#include <QAbstractItemView>

namespace Ui {
class ChannelTableView;
}

class ChannelTableView : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelTableView(QWidget *parent = 0);
    ~ChannelTableView();

    void setModel(QAbstractItemModel *model, int idColumn);
private:
    Ui::ChannelTableView *ui;
public slots:
    void handleRemoveButtonClick();
signals:
    void addDefaultRow();
    void removeRow(int index);
};

#endif // CHANNELTABLEVIEW_H
