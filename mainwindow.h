#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "proxyFetchModel.h"

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
    ProxyFetchModel *model;

    const QVector<QPair<QString, QString>> m_headers =
        {
            {"id",           tr("ID Канала")},
            {"name",         tr("Имя канала")},
            {"address",      tr("Aдрес")},
            {"protocol",     tr("Протокол")},
            {"port",         tr("Порт")},
            {"login",        tr("Логин")},
            {"password",     tr("Пароль")},
            {"lastStartTime",tr("Последнее время запуска")}
        };
};

#endif // MAINWINDOW_H
