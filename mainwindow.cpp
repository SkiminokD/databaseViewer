#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlError err = m_db.init();
    if(err.type() != QSqlError::NoError)
    {
        qDebug()<<"Error"<<err;
        return;
    }

    model = new ChannelTableModel(ui->tableView);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setTable("channels");
    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID Канала"));
    model->setHeaderData(model->fieldIndex("name"), Qt::Horizontal, tr("Имя канала"));
    model->setHeaderData(model->fieldIndex("address"), Qt::Horizontal, tr("Aдрес"));
    model->setHeaderData(model->fieldIndex("port"), Qt::Horizontal, tr("Порт"));
    model->setHeaderData(model->fieldIndex("protocol"), Qt::Horizontal, tr("Протокол"));
    model->setHeaderData(model->fieldIndex("login"), Qt::Horizontal, tr("Логин"));
    model->setHeaderData(model->fieldIndex("password"), Qt::Horizontal, tr("Пароль"));
    model->setHeaderData(model->fieldIndex("lastStartTime"), Qt::Horizontal, tr("Последнее время запуска"));
    model->setSort(model->fieldIndex("id"),Qt::AscendingOrder);
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(model->fieldIndex("id"), true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->btnAdd,&QPushButton::clicked,model,&ChannelTableModel::insertDefaultRow);
    connect(ui->btnRemove,&QPushButton::clicked,this,&MainWindow::delRow);
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

void MainWindow::delRow()
{
    if(!model->removeRow(ui->tableView->currentIndex().row()))
        qDebug()<<"Error while removing"<<model->lastError();
    model->submitAll();
}
