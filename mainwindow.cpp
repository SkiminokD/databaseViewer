#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new ProxyFetchModel(this);
    model->setTable("channels");
    model->setTable("channels", "id");
    ui->view->setModel(model, model->primaryKeyFieldIndex());

    /* TODO: в случае более жесткой привязки ChannelTableView к интерфейсам моделей типа
     *       ChannelTableModel, переместить код connect в ChannelTableView::setModel */
    connect(ui->view,&ChannelTableView::addDefaultRow,
            [&] () { model->insertRow(0); });
    connect(ui->view,&ChannelTableView::removeRow,
            [&] (int index) { model->removeRow(index); });
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}
