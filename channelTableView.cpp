#include "channelTableView.h"
#include "ui_channelTableView.h"
#include <QSqlRelationalDelegate>

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

void ChannelTableView::setModel(QAbstractItemModel *model, int idColumn)
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(idColumn, true);
}

void ChannelTableView::handleRemoveButtonClick()
{
    if(ui->tableView->currentIndex().isValid())
        emit removeRow(ui->tableView->currentIndex().row());
}
