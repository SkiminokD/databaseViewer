#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new ChannelTableModel(this);
    ui->view->setModel(model, model->fieldIndex("id"));

    /* TODO: в случае более жесткой привязки ChannelTableView к интерфейсам моделей типа
     *       ChannelTableModel, переместить код connect в ChannelTableView::setModel */
    connect(ui->view,&ChannelTableView::addDefaultRow,
            model,&ChannelTableModel::insertDefaultRow);
    connect(ui->view,&ChannelTableView::removeRow,
            model,&ChannelTableModel::removeRowImmediately);
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}
