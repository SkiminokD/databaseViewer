#include "channelTableView.h"
#include "ui_channelTableView.h"
#include <QSqlRelationalDelegate>
#include <QTableView>

ChannelTableView::ChannelTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelTableView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->btnAdd,&QPushButton::clicked,
            this,&ChannelTableView::addDefaultRow);
    connect(ui->btnRemove,&QPushButton::clicked,
            this,&ChannelTableView::handleRemoveButtonClick);
}

ChannelTableView::~ChannelTableView()
{
    delete ui;
}

//!
//! \brief ChannelTableView::setModel
//! Устанавливает модель для вида
//!
//! \param model - собственно модель
//! \param idColumn - номер столбца идентификаторов для скрытия
//!
void ChannelTableView::setModel(QAbstractItemModel *model, int idColumn)
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(idColumn, true);

    connect(model,&QAbstractItemModel::rowsInserted,
            ui->tableView, &QTableView::scrollToBottom);
}

void ChannelTableView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    emit viewResized(ui->tableView->height() / ui->tableView->rowHeight(0));
}

void ChannelTableView::handleRemoveButtonClick()
{
    if(ui->tableView->currentIndex().isValid())
        emit removeRow(ui->tableView->currentIndex().row());
}
