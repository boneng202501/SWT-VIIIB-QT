#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MockDevice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnTripComplete_clicked();
    void on_btnCloseComplete_clicked();
    void on_btnWaveReady_clicked();
    void on_btnFault_clicked();
    void on_btnClearLog_clicked();
    void on_btnSaveLog_clicked();

    void appendLog(const QString &msg);

private:
    Ui::MainWindow *ui;
    MockDevice *m_mockDevice = nullptr;
};

#endif // MAINWINDOW_H
