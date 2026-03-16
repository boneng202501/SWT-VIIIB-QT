#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("高压开关下位机模拟器 v1.0");

    // 初始化模拟器
    m_mockDevice = new MockDevice(this);
    connect(m_mockDevice, &MockDevice::logMessage, this, &MainWindow::appendLog);

    // 初始状态
    ui->btnStop->setEnabled(false);
    ui->plainLog->setReadOnly(true);
    ui->plainLog->setFont(QFont("Consolas", 10));
    appendLog("模拟器就绪，请设置端口并启动");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendLog(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    ui->plainLog->appendPlainText(QString("[%1] %2").arg(time).arg(msg));

    // 自动滚到底部
    ui->plainLog->ensureCursorVisible();
}

void MainWindow::on_btnStart_clicked()
{
    bool ok;
    quint16 port = ui->lePort->text().toUShort(&ok);
    if (!ok || port == 0) {
        QMessageBox::warning(this, "错误", "端口号必须是 1~65535 的有效数字");
        return;
    }

    if (m_mockDevice->start(port)) {
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        ui->lePort->setEnabled(false);
        statusBar()->showMessage(QString("正在监听端口 %1...").arg(port));
    }
}

void MainWindow::on_btnStop_clicked()
{
    m_mockDevice->stop();
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->lePort->setEnabled(true);
    statusBar()->showMessage("已停止监听");
}

void MainWindow::on_btnTripComplete_clicked()
{
    //m_mockDevice->simulateTripComplete();
}

void MainWindow::on_btnCloseComplete_clicked()
{
   // m_mockDevice->simulateCloseComplete();
}

void MainWindow::on_btnWaveReady_clicked()
{
  //  m_mockDevice->simulateWaveformReady();
}

void MainWindow::on_btnFault_clicked()
{
  //  m_mockDevice->simulateFault();
}

void MainWindow::on_btnClearLog_clicked()
{
    ui->plainLog->clear();
}

void MainWindow::on_btnSaveLog_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存日志",
                                                    QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt",
                                                    "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->plainLog->toPlainText();
        file.close();
        QMessageBox::information(this, "成功", "日志已保存");
    } else {
        QMessageBox::warning(this, "失败", "保存失败：" + file.errorString());
    }
}
