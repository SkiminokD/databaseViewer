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

    model = new ChannelTableModel(this);
    ui->view->setModel(model, model->fieldIndex("id"));

    connect(ui->view,&ChannelTableView::addDefaultRow,
            model,&ChannelTableModel::insertDefaultRow);
    connect(ui->view,&ChannelTableView::removeRow,
            this,&MainWindow::delRow);
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

void MainWindow::delRow(int index)
{
    if(!model->removeRow(index))
        qDebug()<<"Error while removing"<<model->lastError();
    model->submitAll();
}
