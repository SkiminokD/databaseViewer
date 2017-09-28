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

    model = new QSqlTableModel(ui->tableView);
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

    connect(ui->btnAdd,&QPushButton::clicked,this,&MainWindow::addRow);
    connect(ui->btnRemove,&QPushButton::clicked,this,&MainWindow::delRow);
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

void MainWindow::addRow()
{
    int index = model->rowCount()+1;
    /* FIXME: Стандартные средства insertRow и insertRecord не в состоянии заполнить
    /* таблицу дефолтными значениями */
    QSqlRecord record = model->record();
    record.setValue("id",index);
    record.setValue("name", "Channel "+QString::number(index));
    record.setValue("protocol", "RTSP");
    record.setValue("port",80);
    record.setValue("lastStartTime", QDateTime::currentDateTime());

    if(!model->insertRecord(-1,record))
        qDebug()<<"Error while insert"<<model->lastError();
    model->submitAll();
}

void MainWindow::delRow()
{
    if(!model->removeRow(ui->tableView->currentIndex().row()))
        qDebug()<<"Error while removing"<<model->lastError();
    model->submitAll();
}
