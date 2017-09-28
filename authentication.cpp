#include "authentication.h"
#include "ui_authentication.h"
#include "database.h"
#include <QDialogButtonBox>

Authentication::Authentication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authentication)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Authentication::ok);
}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::ok()
{
    Database db(this);
    QSqlError error = db.init(ui->edtHost->text(),
                              ui->spbPort->value(),
                              ui->edtUsername->text(),
                              ui->edtPassword->text());
    switch(error.type())
    {
    case QSqlError::ConnectionError:
        ui->lblStatus->setText(tr("Ошибка подключения. "
                                  "Пожалуйста проверьте введенные данные."));
        qDebug()<<"Connection error:"<<error.text();
        break;
    case QSqlError::NoError:
        done(DialogCode::Accepted);
        break;
    default:
        ui->lblStatus->setText(tr("Неизвестная ошибка"));
        qDebug()<<"Unknown error:"<<error.text();
    }
}
