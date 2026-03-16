#include "voltageregdialog.h"
#include "ui_voltageregdialog.h"

voltageregdialog::voltageregdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::voltageregdialog)
{
    ui->setupUi(this);
}

voltageregdialog::~voltageregdialog()
{
    delete ui;
}
