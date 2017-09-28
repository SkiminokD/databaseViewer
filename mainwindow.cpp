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
