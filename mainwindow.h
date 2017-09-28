#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "channelTableModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Database m_db;
    ChannelTableModel *model;

protected slots:
    void delRow(int index);
};

#endif // MAINWINDOW_H
