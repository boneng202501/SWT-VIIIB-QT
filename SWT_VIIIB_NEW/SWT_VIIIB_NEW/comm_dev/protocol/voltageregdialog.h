#ifndef VOLTAGEREGDIALOG_H
#define VOLTAGEREGDIALOG_H

#include <QDialog>

namespace Ui {
class voltageregdialog;
}

class voltageregdialog : public QDialog
{
    Q_OBJECT

public:
    explicit voltageregdialog(QWidget *parent = nullptr);
    ~voltageregdialog();

private:
    Ui::voltageregdialog *ui;
};

#endif // VOLTAGEREGDIALOG_H
