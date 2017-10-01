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
    void resizeEvent(QResizeEvent *event) override;
protected slots:
    void handleRemoveButtonClick();
public:
signals:
    void addDefaultRow();
    void removeRow(int index);
    void viewResized(int rowShowed);
};

#endif // CHANNELTABLEVIEW_H
