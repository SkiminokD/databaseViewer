#include "mainwindow.h"
#include <QApplication>
#include "authentication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Authentication *auth = new Authentication();
    bool result = (auth->exec() != QDialog::Accepted);
    delete auth;
    if(result)
    {
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
